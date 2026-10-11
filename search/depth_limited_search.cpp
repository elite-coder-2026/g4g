#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>

using namespace std;

#define ROW 9
#define COL 10

typedef pair<int, int> Pair;

// Result of a depth-limited search.
//   FOUND   - destination reached within the limit
//   CUTOFF  - search hit the depth limit somewhere; a deeper limit might succeed
//   FAILURE - entire reachable space exhausted within the limit; no path exists
enum DLSResult {
    FOUND,
    CUTOFF,
    FAILURE
};

// 8-way movement: N, S, E, W, NE, NW, SE, SW
static const int dRow[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
static const int dCol[8] = {0, 0, 1, -1, 1, -1, 1, -1};
static const char *dName[8] = {"N", "S", "E", "W", "NE", "NW", "SE", "SW"};

struct SearchStats {
    long nodesExpanded;
    long cutoffs;
    int maxDepthReached;

    SearchStats() : nodesExpanded(0), cutoffs(0), maxDepthReached(0) {}
};

bool is_valid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool isUnblocked(int grid[][COL], int row, int col) {
    return grid[row][col] == 1;
}

bool isDest(int row, int col, Pair dest) {
    return row == dest.first && col == dest.second;
}

// Recursive depth-limited DFS.
//
// onPath[][] marks cells on the current recursion path only, so the search
// avoids cycles without the incompleteness a global visited set would cause
// under a depth limit (a cell first reached by a long branch must still be
// reachable later by a shorter one).
DLSResult dlsRecursive(int grid[][COL],
                       int row, int col,
                       Pair dest,
                       int depth, int limit,
                       bool onPath[][COL],
                       vector<Pair> &path,
                       SearchStats &stats) {
    stats.nodesExpanded++;

    if (depth > stats.maxDepthReached) {
        stats.maxDepthReached = depth;
    }

    path.push_back(make_pair(row, col));

    if (isDest(row, col, dest)) {
        return FOUND;
    }

    if (depth == limit) {
        stats.cutoffs++;
        path.pop_back();
        return CUTOFF;
    }

    onPath[row][col] = true;

    bool cutoffOccurred = false;

    for (int d = 0; d < 8; d++) {
        int nr = row + dRow[d];
        int nc = col + dCol[d];

        if (!is_valid(nr, nc)) {
            continue;
        }

        if (!isUnblocked(grid, nr, nc)) {
            continue;
        }

        if (onPath[nr][nc]) {
            continue;
        }

        DLSResult r = dlsRecursive(grid, nr, nc, dest, depth + 1, limit, onPath, path, stats);

        if (r == FOUND) {
            onPath[row][col] = false;
            return FOUND;
        }

        if (r == CUTOFF) {
            cutoffOccurred = true;
        }
    }

    onPath[row][col] = false;
    path.pop_back();

    return cutoffOccurred ? CUTOFF : FAILURE;
}

// Validates input, then runs DLS from src to dest with the given depth limit.
DLSResult depthLimitedSearch(int grid[][COL],
                             Pair src, Pair dest,
                             int limit,
                             vector<Pair> &path,
                             SearchStats &stats) {
    path.clear();

    if (!is_valid(src.first, src.second)) {
        printf("Source is invalid\n");
        return FAILURE;
    }

    if (!is_valid(dest.first, dest.second)) {
        printf("Destination is invalid\n");
        return FAILURE;
    }

    if (!isUnblocked(grid, src.first, src.second) ||
        !isUnblocked(grid, dest.first, dest.second)) {
        printf("Source or the destination is blocked\n");
        return FAILURE;
    }

    if (limit < 0) {
        printf("Depth limit must be non-negative\n");
        return FAILURE;
    }

    bool onPath[ROW][COL];
    memset(onPath, false, sizeof(onPath));

    return dlsRecursive(grid, src.first, src.second, dest, 0, limit, onPath, path, stats);
}

const char *directionBetween(Pair a, Pair b) {
    int dr = b.first - a.first;
    int dc = b.second - a.second;

    for (int d = 0; d < 8; d++) {
        if (dRow[d] == dr && dCol[d] == dc) {
            return dName[d];
        }
    }

    return "?";
}

void printPath(const vector<Pair> &path) {
    printf("Path (%d moves): ", (int) path.size() - 1);

    for (size_t n = 0; n < path.size(); n++) {
        printf("(%d,%d)", path[n].first, path[n].second);

        if (n + 1 < path.size()) {
            printf(" -%s-> ", directionBetween(path[n], path[n + 1]));
        }
    }

    printf("\n");
}

void printGridWithPath(int grid[][COL], const vector<Pair> &path, Pair src, Pair dest) {
    char view[ROW][COL];

    for (int r = 0; r < ROW; r++) {
        for (int c = 0; c < COL; c++) {
            view[r][c] = grid[r][c] == 1 ? '.' : '#';
        }
    }

    for (size_t n = 0; n < path.size(); n++) {
        view[path[n].first][path[n].second] = '*';
    }

    view[src.first][src.second] = 'S';
    view[dest.first][dest.second] = 'D';

    for (int r = 0; r < ROW; r++) {
        printf("    ");
        for (int c = 0; c < COL; c++) {
            printf("%c ", view[r][c]);
        }
        printf("\n");
    }
}

const char *resultName(DLSResult r) {
    switch (r) {
        case FOUND:   return "FOUND";
        case CUTOFF:  return "CUTOFF";
        case FAILURE: return "FAILURE";
    }
    return "?";
}

void runAndReport(int grid[][COL], Pair src, Pair dest, int limit) {
    vector<Pair> path;
    SearchStats stats;

    DLSResult r = depthLimitedSearch(grid, src, dest, limit, path, stats);

    printf("Limit %2d: %-7s  expanded=%ld  cutoffs=%ld  maxDepth=%d\n",
           limit, resultName(r), stats.nodesExpanded, stats.cutoffs, stats.maxDepthReached);

    if (r == FOUND) {
        printPath(path);
        printGridWithPath(grid, path, src, dest);
    }
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

    Pair src = make_pair(8, 0);
    Pair dest = make_pair(0, 0);

    int limit = 8;

    runAndReport(grid, src, dest, limit);

    return 0;
}
