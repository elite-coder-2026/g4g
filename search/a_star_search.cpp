#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <set>
#include <stack>
#include <utility>

using namespace std;

#define ROW 9
#define COL 10

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;

struct cell {
    int i;
    int k;
    double f, g, h;
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

double calcHValue(int row, int col, Pair dest) {
    double dr = (double) (row - dest.first);
    double dc = (double) (col - dest.second);
    return sqrt(dr * dr + dc * dc);
}

void tracePath(cell cd[][COL], Pair dest) {
    printf("\nThe path is ");

    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cd[row][col].i == row && cd[row][col].k == col)) {
        Path.push(make_pair(row, col));

        int tr = cd[row][col].i;
        int tc = cd[row][col].k;

        row = tr;
        col = tc;
    }

    Path.push(make_pair(row, col));

    while (!Path.empty()) {
        Pair p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }

    printf("\n");
}

// Pushes (fn, cell) onto the open list, removing any stale entry for the same cell first.
void pushOpen(set<pPair> &open, cell cd[][COL], int row, int col, double fn) {
    if (cd[row][col].f != DBL_MAX) {
        open.erase(make_pair(cd[row][col].f, make_pair(row, col)));
    }

    open.insert(make_pair(fn, make_pair(row, col)));
}

void search(int grid[][COL], Pair src, Pair dest) {
    if (is_valid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }

    if (is_valid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }

    if (isUnblocked(grid, src.first, src.second) == false ||
        isUnblocked(grid, dest.first, dest.second) == false) {
        printf("Source or the destination is blocked\n");
        return;
    }

    if (isDest(src.first, src.second, dest) == true) {
        printf("We are already at the destination\n");
        return;
    }

    bool closed[ROW][COL];
    memset(closed, false, sizeof(closed));

    cell cd[ROW][COL];
    int i, k;

    for (i = 0; i < ROW; i++) {
        for (k = 0; k < COL; k++) {
            cd[i][k].f = DBL_MAX;
            cd[i][k].g = DBL_MAX;
            cd[i][k].h = DBL_MAX;
            cd[i][k].i = -1;
            cd[i][k].k = -1;
        }
    }

    i = src.first;
    k = src.second;

    cd[i][k].f = 0.0;
    cd[i][k].g = 0.0;
    cd[i][k].h = 0.0;
    cd[i][k].i = i;
    cd[i][k].k = k;

    set<pPair> open;
    open.insert(make_pair(0.0, make_pair(i, k)));

    bool found = false;

    const double STRAIGHT = 1.0;
    const double DIAGONAL = sqrt(2.0);

    while (!open.empty()) {
        pPair p = *open.begin();
        open.erase(open.begin());

        i = p.second.first;
        k = p.second.second;
        closed[i][k] = true;

        double gn, hn, fn;

        // ---------- north (i - 1, k) ----------
        if (is_valid(i - 1, k) == true) {
            if (isDest(i - 1, k, dest) == true) {
                cd[i - 1][k].i = i;
                cd[i - 1][k].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i - 1][k] == false && isUnblocked(grid, i - 1, k) == true) {
                gn = cd[i][k].g + STRAIGHT;
                hn = calcHValue(i - 1, k, dest);
                fn = gn + hn;

                if (cd[i - 1][k].f == DBL_MAX || cd[i - 1][k].f > fn) {
                    pushOpen(open, cd, i - 1, k, fn);

                    cd[i - 1][k].f = fn;
                    cd[i - 1][k].g = gn;
                    cd[i - 1][k].h = hn;
                    cd[i - 1][k].i = i;
                    cd[i - 1][k].k = k;
                }
            }
        }

        // ---------- south (i + 1, k) ----------
        if (is_valid(i + 1, k) == true) {
            if (isDest(i + 1, k, dest) == true) {
                cd[i + 1][k].i = i;
                cd[i + 1][k].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i + 1][k] == false && isUnblocked(grid, i + 1, k) == true) {
                gn = cd[i][k].g + STRAIGHT;
                hn = calcHValue(i + 1, k, dest);
                fn = gn + hn;

                if (cd[i + 1][k].f == DBL_MAX || cd[i + 1][k].f > fn) {
                    pushOpen(open, cd, i + 1, k, fn);

                    cd[i + 1][k].f = fn;
                    cd[i + 1][k].g = gn;
                    cd[i + 1][k].h = hn;
                    cd[i + 1][k].i = i;
                    cd[i + 1][k].k = k;
                }
            }
        }

        // ---------- east (i, k + 1) ----------
        if (is_valid(i, k + 1) == true) {
            if (isDest(i, k + 1, dest) == true) {
                cd[i][k + 1].i = i;
                cd[i][k + 1].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i][k + 1] == false && isUnblocked(grid, i, k + 1) == true) {
                gn = cd[i][k].g + STRAIGHT;
                hn = calcHValue(i, k + 1, dest);
                fn = gn + hn;

                if (cd[i][k + 1].f == DBL_MAX || cd[i][k + 1].f > fn) {
                    pushOpen(open, cd, i, k + 1, fn);

                    cd[i][k + 1].f = fn;
                    cd[i][k + 1].g = gn;
                    cd[i][k + 1].h = hn;
                    cd[i][k + 1].i = i;
                    cd[i][k + 1].k = k;
                }
            }
        }

        // ---------- west (i, k - 1) ----------
        if (is_valid(i, k - 1) == true) {
            if (isDest(i, k - 1, dest) == true) {
                cd[i][k - 1].i = i;
                cd[i][k - 1].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i][k - 1] == false && isUnblocked(grid, i, k - 1) == true) {
                gn = cd[i][k].g + STRAIGHT;
                hn = calcHValue(i, k - 1, dest);
                fn = gn + hn;

                if (cd[i][k - 1].f == DBL_MAX || cd[i][k - 1].f > fn) {
                    pushOpen(open, cd, i, k - 1, fn);

                    cd[i][k - 1].f = fn;
                    cd[i][k - 1].g = gn;
                    cd[i][k - 1].h = hn;
                    cd[i][k - 1].i = i;
                    cd[i][k - 1].k = k;
                }
            }
        }

        // ---------- north-east (i - 1, k + 1) ----------
        if (is_valid(i - 1, k + 1) == true) {
            if (isDest(i - 1, k + 1, dest) == true) {
                cd[i - 1][k + 1].i = i;
                cd[i - 1][k + 1].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i - 1][k + 1] == false && isUnblocked(grid, i - 1, k + 1) == true) {
                gn = cd[i][k].g + DIAGONAL;
                hn = calcHValue(i - 1, k + 1, dest);
                fn = gn + hn;

                if (cd[i - 1][k + 1].f == DBL_MAX || cd[i - 1][k + 1].f > fn) {
                    pushOpen(open, cd, i - 1, k + 1, fn);

                    cd[i - 1][k + 1].f = fn;
                    cd[i - 1][k + 1].g = gn;
                    cd[i - 1][k + 1].h = hn;
                    cd[i - 1][k + 1].i = i;
                    cd[i - 1][k + 1].k = k;
                }
            }
        }

        // ---------- north-west (i - 1, k - 1) ----------
        if (is_valid(i - 1, k - 1) == true) {
            if (isDest(i - 1, k - 1, dest) == true) {
                cd[i - 1][k - 1].i = i;
                cd[i - 1][k - 1].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i - 1][k - 1] == false && isUnblocked(grid, i - 1, k - 1) == true) {
                gn = cd[i][k].g + DIAGONAL;
                hn = calcHValue(i - 1, k - 1, dest);
                fn = gn + hn;

                if (cd[i - 1][k - 1].f == DBL_MAX || cd[i - 1][k - 1].f > fn) {
                    pushOpen(open, cd, i - 1, k - 1, fn);

                    cd[i - 1][k - 1].f = fn;
                    cd[i - 1][k - 1].g = gn;
                    cd[i - 1][k - 1].h = hn;
                    cd[i - 1][k - 1].i = i;
                    cd[i - 1][k - 1].k = k;
                }
            }
        }

        // ---------- south-east (i + 1, k + 1) ----------
        if (is_valid(i + 1, k + 1) == true) {
            if (isDest(i + 1, k + 1, dest) == true) {
                cd[i + 1][k + 1].i = i;
                cd[i + 1][k + 1].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i + 1][k + 1] == false && isUnblocked(grid, i + 1, k + 1) == true) {
                gn = cd[i][k].g + DIAGONAL;
                hn = calcHValue(i + 1, k + 1, dest);
                fn = gn + hn;

                if (cd[i + 1][k + 1].f == DBL_MAX || cd[i + 1][k + 1].f > fn) {
                    pushOpen(open, cd, i + 1, k + 1, fn);

                    cd[i + 1][k + 1].f = fn;
                    cd[i + 1][k + 1].g = gn;
                    cd[i + 1][k + 1].h = hn;
                    cd[i + 1][k + 1].i = i;
                    cd[i + 1][k + 1].k = k;
                }
            }
        }

        // ---------- south-west (i + 1, k - 1) ----------
        if (is_valid(i + 1, k - 1) == true) {
            if (isDest(i + 1, k - 1, dest) == true) {
                cd[i + 1][k - 1].i = i;
                cd[i + 1][k - 1].k = k;

                printf("The destination cell is found\n");
                tracePath(cd, dest);
                found = true;
                return;
            } else if (closed[i + 1][k - 1] == false && isUnblocked(grid, i + 1, k - 1) == true) {
                gn = cd[i][k].g + DIAGONAL;
                hn = calcHValue(i + 1, k - 1, dest);
                fn = gn + hn;

                if (cd[i + 1][k - 1].f == DBL_MAX || cd[i + 1][k - 1].f > fn) {
                    pushOpen(open, cd, i + 1, k - 1, fn);

                    cd[i + 1][k - 1].f = fn;
                    cd[i + 1][k - 1].g = gn;
                    cd[i + 1][k - 1].h = hn;
                    cd[i + 1][k - 1].i = i;
                    cd[i + 1][k - 1].k = k;
                }
            }
        }
    }

    if (found == false) {
        printf("Failed to find the destination cell\n");
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

    search(grid, src, dest);

    return 0;
}
