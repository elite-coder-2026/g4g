#include <iostream>
#include <vector>

using namespace std;

bool isSafe(vector<vector<int>>& mat, int row, int col, int num) {
    for (int x = 0; x <= 8; x++) {
        if (mat[row][x] == num)
            return false;
    }

    for (int x = 0; x < 8; x++) {
        if (mat[x][col] == num)
            return false;
    }

    int start_row = row - (row % 3), start_col = col - (col % 3);

    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            if (mat[i + start_row][k + start_col] == num)
                return false;
        }
    }

    return true;
}

bool solver_util(vector<vector<int>>& mat, int row, int col) {
    int n = mat.size();

    if (row == n - 1 && col == n)
        return true;

    if (col == n) {
        row++;
        col = 0;
    }

    for (int num = 1; num <= n; num++) {
        if (isSafe(mat, row, col, num)) {
            mat[row][col] = num;

            if (solver_util(mat, row, col + 1))
                return true;

            mat[row][col] = 0;
        }
    }

    return false;
}

void solve(vector<vector<int>>& mat) {
    solver_util(mat, 0, 0);
}

int main() {
    vector<vector<int>> mat = {
        {3, 0, 6, 5, 0, 8, 4, 0, 0},
       	{5, 2, 0, 0, 0, 0, 0, 0, 0},
       	{0, 8, 7, 0, 0, 0, 0, 3, 1},
        {0, 0, 3, 0, 1, 0, 0, 8, 0},
       	{9, 0, 0, 8, 6, 3, 0, 0, 5},
       	{0, 5, 0, 0, 9, 0, 6, 0, 0},
        {1, 3, 0, 0, 0, 0, 2, 5, 0},
       	{0, 0, 0, 0, 0, 0, 0, 7, 4},
       	{0, 0, 5, 2, 0, 6, 3, 0, 0}
    };

    solve(mat);

    for (int i = 0; i < mat.size(); i++) {
        for (int k = 0; k < mat.size(); k++) {
            cout << mat[i][k] << " ";
        }

        cout << endl;
    }

    return 0;
}
