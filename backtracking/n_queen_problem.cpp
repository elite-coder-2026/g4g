#include <ios>
#include <iostream>
#include <vector>

using namespace std;

void placeQueens(int i, vector<int>& cols, vector<int>& leftDiag, vector<int>& rightDiag, vector<int>& curr, vector<vector<int>>& result) {
    int n = cols.size();

    if (i == n) {
        result.push_back(curr);
        return;
    }

    for (int k = 0; k < n; k++) {
        if (cols[k] || rightDiag[i + k] || leftDiag[i - k + n - 1])
            continue;

        cols[k] = 1;
        rightDiag[i + k] = 1;
        leftDiag[i - k + n - 1] = 1;
        curr.push_back(k + 1);

        placeQueens(i + 1, cols, leftDiag, rightDiag, curr, result);

        curr.pop_back();
        cols[k] = 0;
        rightDiag[i + k] = 0;
        leftDiag[i - k + n - 1] = 0;
    }
}

vector<vector<int>> nQueen(int n) {
    vector<int> cols(n, 0);
    vector<int> leftDiag(n * 2, 0);
    vector<int> rightDiag(n * 2, 0);
    vector<int> curr;
    vector<vector<int>> result;

    placeQueens(0, cols, leftDiag, rightDiag, curr, result);

    return result;
}

int main() {
    int n = 4;
    vector<vector<int>> ans = nQueen(n);

    for (auto &a: ans) {
        for(auto i : a) {
            cout << i << " ";
        }

        cout << endl;
    }

    return 0;
}
