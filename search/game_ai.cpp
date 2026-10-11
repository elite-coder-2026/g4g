// D* Lite (Koenig & Likhachev, 2002) - incremental replanning on an 8-connected grid.
//
// The robot starts with NO knowledge of obstacles (free-space assumption). It plans
// from the goal backward, moves one cell at a time, senses obstacles within a radius,
// and repairs only the affected part of the search tree instead of replanning from
// scratch. The output is the robot's actual traversal, including every replan event.

#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <set>
#include <utility>
#include <vector>

using namespace std;

#define ROW 9
#define COL 10
#define SENSOR_RADIUS 1

typedef pair<int, int> Pair;
typedef pair<double, double> Key;
typedef pair<Key, Pair> QEntry;

static const double INF = DBL_MAX;
static const double SQRT2 = 1.4142135623730951;

static const int dRow[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
static const int dCol[8] = {0, 0, 1, -1, 1, -1, 1, -1};
static const char *dName[8] = {"N", "S", "E", "W", "NE", "NW", "SE", "SW"};

struct Stats {
    long expansions;
    long vertexUpdates;
    long replans;
    long obstaclesDiscovered;

    Stats() : expansions(0), vertexUpdates(0), replans(0), obstaclesDiscovered(0) {}
};

class DStarLite {
public:
    DStarLite(int trueGrid[][COL], Pair start, Pair goal)
        : start_(start), goal_(goal), last_(start), km_(0.0), stats_(0) {
        memcpy(truth_, trueGrid, sizeof(truth_));

        for (int r = 0; r < ROW; r++) {
            for (int c = 0; c < COL; c++) {
                known_[r][c] = 1;          // free-space assumption
                g_[r][c] = INF;
                rhs_[r][c] = INF;
                inQueue_[r][c] = false;
            }
        }

        rhs_[goal_.first][goal_.second] = 0.0;
        insert(goal_, calcKey(goal_));
    }

    // Runs the full navigate-sense-replan loop. Returns true if the goal is reached.
    bool run(vector<Pair> &traversed, Stats &stats) {
        stats_ = &stats;
        traversed.clear();
        traversed.push_back(start_);

        sense();
        long before = stats_->expansions;
        computeShortestPath();
        printf("[initial plan] expansions=%ld  planned cost=%s\n",
               stats_->expansions - before, costStr(g_[start_.first][start_.second]));

        int step = 0;

        while (start_ != goal_) {
            if (g_[start_.first][start_.second] == INF) {
                printf("[step %d] no path exists from (%d,%d) given known obstacles\n",
                       step, start_.first, start_.second);
                return false;
            }

            int bestDir = -1;
            double best = INF;

            for (int d = 0; d < 8; d++) {
                int nr = start_.first + dRow[d];
                int nc = start_.second + dCol[d];

                if (!inBounds(nr, nc)) {
                    continue;
                }

                double v = add(cost(start_.first, start_.second, nr, nc), g_[nr][nc]);

                if (v < best) {
                    best = v;
                    bestDir = d;
                }
            }

            if (bestDir < 0) {
                printf("[step %d] stuck at (%d,%d)\n", step, start_.first, start_.second);
                return false;
            }

            Pair prev = start_;
            start_ = make_pair(start_.first + dRow[bestDir], start_.second + dCol[bestDir]);

            if (truth_[start_.first][start_.second] == 0) {
                // Cannot happen: sensing guarantees adjacent cells are known before moving.
                printf("[step %d] collision at (%d,%d)\n", step, start_.first, start_.second);
                return false;
            }

            step++;
            traversed.push_back(start_);
            printf("[step %2d] (%d,%d) -%s-> (%d,%d)\n",
                   step, prev.first, prev.second, dName[bestDir], start_.first, start_.second);

            vector<Pair> found = sense();

            if (!found.empty()) {
                km_ += heuristic(last_, start_);
                last_ = start_;

                for (size_t n = 0; n < found.size(); n++) {
                    int r = found[n].first;
                    int c = found[n].second;

                    updateVertex(found[n]);

                    for (int d = 0; d < 8; d++) {
                        int pr = r + dRow[d];
                        int pc = c + dCol[d];

                        if (inBounds(pr, pc)) {
                            updateVertex(make_pair(pr, pc));
                        }
                    }
                }

                stats_->replans++;
                before = stats_->expansions;
                computeShortestPath();

                printf("          sensed %d new obstacle(s):", (int) found.size());
                for (size_t n = 0; n < found.size(); n++) {
                    printf(" (%d,%d)", found[n].first, found[n].second);
                }
                printf("\n          replan #%ld  km=%.3f  expansions=%ld  new cost-to-goal=%s\n",
                       stats_->replans, km_, stats_->expansions - before,
                       costStr(g_[start_.first][start_.second]));
            }
        }

        return true;
    }

    // ---------------- chase API (moving goal) ----------------

    void attachStats(Stats &stats) {
        stats_ = &stats;
    }

    Pair position() const {
        return start_;
    }

    Pair goal() const {
        return goal_;
    }

    double costToGoal() const {
        return g_[start_.first][start_.second];
    }

    // Points the planner at a new goal. Sensed obstacles are kept; the search tree is
    // rebuilt because D* Lite's backward search is rooted at the goal.
    void retarget(Pair goal) {
        goal_ = goal;
        last_ = start_;
        km_ = 0.0;
        open_.clear();

        for (int r = 0; r < ROW; r++) {
            for (int c = 0; c < COL; c++) {
                g_[r][c] = INF;
                rhs_[r][c] = INF;
                inQueue_[r][c] = false;
            }
        }

        rhs_[goal_.first][goal_.second] = 0.0;
        insert(goal_, calcKey(goal_));
    }

    // Runs/repairs the search. Returns the number of expansions it took.
    long plan() {
        long before = stats_->expansions;
        computeShortestPath();
        return stats_->expansions - before;
    }

    // Senses around the robot; if anything new is found, repairs the plan in place.
    vector<Pair> senseAndRepair(long &expansions) {
        expansions = 0;
        vector<Pair> found = sense();

        if (found.empty()) {
            return found;
        }

        km_ += heuristic(last_, start_);
        last_ = start_;

        for (size_t n = 0; n < found.size(); n++) {
            updateVertex(found[n]);
            updatePredecessors(found[n]);
        }

        stats_->replans++;
        expansions = plan();
        return found;
    }

    // Takes one step along the current plan. Returns the direction index, or -1 if no path.
    int advance() {
        if (start_ == goal_ || g_[start_.first][start_.second] == INF) {
            return -1;
        }

        int bestDir = -1;
        double best = INF;

        for (int d = 0; d < 8; d++) {
            int nr = start_.first + dRow[d];
            int nc = start_.second + dCol[d];

            if (!inBounds(nr, nc)) {
                continue;
            }

            double v = add(cost(start_.first, start_.second, nr, nc), g_[nr][nc]);

            if (v < best) {
                best = v;
                bestDir = d;
            }
        }

        if (bestDir >= 0) {
            start_ = make_pair(start_.first + dRow[bestDir], start_.second + dCol[bestDir]);
        }

        return bestDir;
    }

    void printChaseMap(const vector<Pair> &monsterTrail, const vector<Pair> &playerTrail) const {
        char view[ROW][COL];

        for (int r = 0; r < ROW; r++) {
            for (int c = 0; c < COL; c++) {
                if (truth_[r][c] == 0) {
                    view[r][c] = known_[r][c] == 0 ? '#' : '?';
                } else {
                    view[r][c] = '.';
                }
            }
        }

        for (size_t n = 0; n < playerTrail.size(); n++) {
            view[playerTrail[n].first][playerTrail[n].second] = 'o';
        }

        for (size_t n = 0; n < monsterTrail.size(); n++) {
            view[monsterTrail[n].first][monsterTrail[n].second] = '*';
        }

        Pair m = monsterTrail.back();
        Pair p = playerTrail.back();
        view[p.first][p.second] = 'P';
        view[m.first][m.second] = 'M';

        printf("\n  legend: M monster  P player  * monster trail  o player trail\n");
        printf("          # obstacle sensed by monster  ? obstacle never sensed\n");
        for (int r = 0; r < ROW; r++) {
            printf("    ");
            for (int c = 0; c < COL; c++) {
                printf("%c ", view[r][c]);
            }
            printf("\n");
        }
    }

    void printMap(const vector<Pair> &traversed, Pair origin) const {
        char view[ROW][COL];

        for (int r = 0; r < ROW; r++) {
            for (int c = 0; c < COL; c++) {
                if (truth_[r][c] == 0) {
                    view[r][c] = known_[r][c] == 0 ? '#' : '?';   // '?' = obstacle never sensed
                } else {
                    view[r][c] = '.';
                }
            }
        }

        for (size_t n = 0; n < traversed.size(); n++) {
            view[traversed[n].first][traversed[n].second] = '*';
        }

        view[origin.first][origin.second] = 'S';
        view[goal_.first][goal_.second] = 'G';

        printf("\n  legend: S start  G goal  * traversed  # sensed obstacle  ? unsensed obstacle\n");
        for (int r = 0; r < ROW; r++) {
            printf("    ");
            for (int c = 0; c < COL; c++) {
                printf("%c ", view[r][c]);
            }
            printf("\n");
        }
    }

private:
    int truth_[ROW][COL];
    int known_[ROW][COL];
    double g_[ROW][COL];
    double rhs_[ROW][COL];
    bool inQueue_[ROW][COL];
    Key queuedKey_[ROW][COL];

    set<QEntry> open_;

    Pair start_;
    Pair goal_;
    Pair last_;
    double km_;
    Stats *stats_;

    static bool inBounds(int r, int c) {
        return r >= 0 && r < ROW && c >= 0 && c < COL;
    }

    static double add(double a, double b) {
        return (a == INF || b == INF) ? INF : a + b;
    }

    static const char *costStr(double v) {
        static char buf[32];
        if (v == INF) {
            return "INF";
        }
        snprintf(buf, sizeof(buf), "%.3f", v);
        return buf;
    }

    // Octile distance: admissible and consistent for 8-way movement with sqrt(2) diagonals.
    static double heuristic(Pair a, Pair b) {
        double dr = fabs((double) (a.first - b.first));
        double dc = fabs((double) (a.second - b.second));
        double lo = dr < dc ? dr : dc;
        double hi = dr < dc ? dc : dr;
        return (hi - lo) + SQRT2 * lo;
    }

    double cost(int r1, int c1, int r2, int c2) const {
        if (known_[r1][c1] == 0 || known_[r2][c2] == 0) {
            return INF;
        }
        return (r1 != r2 && c1 != c2) ? SQRT2 : 1.0;
    }

    Key calcKey(Pair s) const {
        double m = g_[s.first][s.second] < rhs_[s.first][s.second]
                       ? g_[s.first][s.second]
                       : rhs_[s.first][s.second];
        return make_pair(add(add(m, heuristic(start_, s)), km_), m);
    }

    void insert(Pair s, Key k) {
        open_.insert(make_pair(k, s));
        inQueue_[s.first][s.second] = true;
        queuedKey_[s.first][s.second] = k;
    }

    void remove(Pair s) {
        if (inQueue_[s.first][s.second]) {
            open_.erase(make_pair(queuedKey_[s.first][s.second], s));
            inQueue_[s.first][s.second] = false;
        }
    }

    void updateVertex(Pair u) {
        stats_->vertexUpdates++;

        if (u != goal_) {
            double best = INF;

            for (int d = 0; d < 8; d++) {
                int nr = u.first + dRow[d];
                int nc = u.second + dCol[d];

                if (!inBounds(nr, nc)) {
                    continue;
                }

                double v = add(cost(u.first, u.second, nr, nc), g_[nr][nc]);
                if (v < best) {
                    best = v;
                }
            }

            rhs_[u.first][u.second] = best;
        }

        remove(u);

        if (g_[u.first][u.second] != rhs_[u.first][u.second]) {
            insert(u, calcKey(u));
        }
    }

    void updatePredecessors(Pair u) {
        for (int d = 0; d < 8; d++) {
            int pr = u.first + dRow[d];
            int pc = u.second + dCol[d];

            if (inBounds(pr, pc)) {
                updateVertex(make_pair(pr, pc));
            }
        }
    }

    void computeShortestPath() {
        while (!open_.empty() &&
               (open_.begin()->first < calcKey(start_) ||
                rhs_[start_.first][start_.second] != g_[start_.first][start_.second])) {
            QEntry top = *open_.begin();
            Key kOld = top.first;
            Pair u = top.second;
            Key kNew = calcKey(u);

            stats_->expansions++;

            if (kOld < kNew) {
                remove(u);
                insert(u, kNew);
            } else if (g_[u.first][u.second] > rhs_[u.first][u.second]) {
                g_[u.first][u.second] = rhs_[u.first][u.second];
                remove(u);
                updatePredecessors(u);
            } else {
                g_[u.first][u.second] = INF;
                updateVertex(u);
                updatePredecessors(u);
            }
        }
    }

    // Reveals true obstacles within SENSOR_RADIUS of the robot. Returns newly found ones.
    vector<Pair> sense() {
        vector<Pair> found;

        for (int dr = -SENSOR_RADIUS; dr <= SENSOR_RADIUS; dr++) {
            for (int dc = -SENSOR_RADIUS; dc <= SENSOR_RADIUS; dc++) {
                int r = start_.first + dr;
                int c = start_.second + dc;

                if (!inBounds(r, c)) {
                    continue;
                }

                if (truth_[r][c] == 0 && known_[r][c] == 1) {
                    known_[r][c] = 0;
                    found.push_back(make_pair(r, c));
                    if (stats_) {
                        stats_->obstaclesDiscovered++;
                    }
                }
            }
        }

        return found;
    }
};

// ---------------- chase driver ----------------

#define MONSTER_RANGE 1        // monster catches the player within this many cells
#define PLAYER_MOVE_EVERY 2    // player moves once every N turns (monster moves every turn)
#define RETARGET_DISTANCE 2.0  // monster replans when player drifts this far from its goal
#define MAX_TURNS 80

static double octile(Pair a, Pair b) {
    double dr = fabs((double) (a.first - b.first));
    double dc = fabs((double) (a.second - b.second));
    double lo = dr < dc ? dr : dc;
    double hi = dr < dc ? dc : dr;
    return (hi - lo) + SQRT2 * lo;
}

static bool caught(Pair monster, Pair player) {
    int dr = monster.first - player.first;
    int dc = monster.second - player.second;
    if (dr < 0) dr = -dr;
    if (dc < 0) dc = -dc;
    return dr <= MONSTER_RANGE && dc <= MONSTER_RANGE;
}

// Walking distance (8-way steps) from the monster to every cell on the true map.
static void distanceField(int grid[][COL], Pair from, int dist[][COL]) {
    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
            dist[r][c] = -1;
        }
    }

    vector<Pair> frontier(1, from);
    dist[from.first][from.second] = 0;

    for (size_t head = 0; head < frontier.size(); head++) {
        Pair u = frontier[head];

        for (int d = 0; d < 8; d++) {
            int nr = u.first + dRow[d];
            int nc = u.second + dCol[d];

            if (nr < 0 || nr >= ROW || nc < 0 || nc >= COL) continue;
            if (grid[nr][nc] == 0 || dist[nr][nc] >= 0) continue;

            dist[nr][nc] = dist[u.first][u.second] + 1;
            frontier.push_back(make_pair(nr, nc));
        }
    }
}

// Player knows the map and steps to the open neighbor with the greatest walking
// distance from the monster. Holds position when no move improves on staying.
static Pair flee(int grid[][COL], Pair player, Pair monster) {
    int dist[ROW][COL];
    distanceField(grid, monster, dist);

    Pair best = player;
    int bestDist = dist[player.first][player.second];

    for (int d = 0; d < 8; d++) {
        int nr = player.first + dRow[d];
        int nc = player.second + dCol[d];

        if (nr < 0 || nr >= ROW || nc < 0 || nc >= COL || grid[nr][nc] == 0) {
            continue;
        }

        if (dist[nr][nc] > bestDist) {
            bestDist = dist[nr][nc];
            best = make_pair(nr, nc);
        }
    }

    return best;
}

int main() {
    int grid[ROW][COL] = {
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
        {1, 0, 1, 1, 1, 1, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 0, 0, 1}
    };

    Pair monsterStart = make_pair(8, 9);
    Pair player = make_pair(5, 3);

    if (grid[monsterStart.first][monsterStart.second] == 0 || grid[player.first][player.second] == 0) {
        printf("Monster or player starts on a blocked cell\n");
        return 1;
    }

    printf("D* Lite chase: monster (%d,%d) hunts player (%d,%d)\n",
           monsterStart.first, monsterStart.second, player.first, player.second);
    printf("monster: unknown map, sensor radius %d, moves every turn, retargets at %.1f cells\n",
           SENSOR_RADIUS, RETARGET_DISTANCE);
    printf("player:  knows map, flees, moves every %d turns\n\n", PLAYER_MOVE_EVERY);

    DStarLite monster(grid, monsterStart, player);
    Stats stats;
    monster.attachStats(stats);

    vector<Pair> monsterTrail(1, monsterStart);
    vector<Pair> playerTrail(1, player);

    long exp = 0;
    monster.senseAndRepair(exp);
    exp += monster.plan();
    long retargets = 0;

    printf("[turn  0] initial plan: expansions=%ld  cost-to-player=%.3f\n", exp, monster.costToGoal());

    bool wasCaught = false;
    int turn;

    for (turn = 1; turn <= MAX_TURNS; turn++) {
        // Retarget when the player has drifted from where the monster is heading,
        // or when the monster has reached a stale goal.
        if (octile(player, monster.goal()) > RETARGET_DISTANCE || monster.position() == monster.goal()) {
            Pair oldGoal = monster.goal();
            monster.retarget(player);
            exp = monster.plan();
            retargets++;
            printf("[turn %2d] RETARGET (%d,%d) -> (%d,%d)  expansions=%ld  cost-to-player=%.3f\n",
                   turn, oldGoal.first, oldGoal.second, player.first, player.second,
                   exp, monster.costToGoal());
        }

        Pair from = monster.position();
        int dir = monster.advance();

        if (dir < 0) {
            printf("[turn %2d] monster has no path to the player\n", turn);
            break;
        }

        monsterTrail.push_back(monster.position());
        printf("[turn %2d] monster (%d,%d) -%s-> (%d,%d)\n", turn,
               from.first, from.second, dName[dir],
               monster.position().first, monster.position().second);

        vector<Pair> found = monster.senseAndRepair(exp);

        if (!found.empty()) {
            printf("          sensed:");
            for (size_t n = 0; n < found.size(); n++) {
                printf(" (%d,%d)", found[n].first, found[n].second);
            }
            printf("  -> repair expansions=%ld  cost-to-player=%.3f\n", exp, monster.costToGoal());
        }

        if (caught(monster.position(), player)) {
            wasCaught = true;
            break;
        }

        if (turn % PLAYER_MOVE_EVERY == 0) {
            Pair pFrom = player;
            player = flee(grid, player, monster.position());
            playerTrail.push_back(player);

            if (player == pFrom) {
                printf("          player holds at (%d,%d) (cornered)\n", player.first, player.second);
            } else {
                printf("          player (%d,%d) -> (%d,%d)\n", pFrom.first, pFrom.second, player.first, player.second);
            }

            if (caught(monster.position(), player)) {
                wasCaught = true;
                break;
            }
        }
    }

    if (wasCaught) {
        printf("\nPLAYER CAUGHT on turn %d at (%d,%d)  monster at (%d,%d)\n",
               turn, player.first, player.second, monster.position().first, monster.position().second);
    } else {
        printf("\nPLAYER ESCAPED after %d turns\n", MAX_TURNS);
    }

    printf("retargets=%ld  repairs=%ld  expansions=%ld  vertex-updates=%ld  obstacles-sensed=%ld\n",
           retargets, stats.replans, stats.expansions, stats.vertexUpdates, stats.obstaclesDiscovered);

    monster.printChaseMap(monsterTrail, playerTrail);

    return 0;
}
