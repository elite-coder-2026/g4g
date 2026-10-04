#include <iostream>
#include <vector>

using namespace std;

int find_min_idx(vector<int>& arr) {
    int low = 0, high = arr.size() - 1;
    int min_idx = -1;

    while (low < high) {
        int a = low + (high - low) / 3;
        int b = high - (high - low) / 3;

        if (arr[a] == arr[b]) {
            low = a + 1;
            high = b - 1;
            min_idx = a;
        } else if (arr[a] > arr[b]) {
            high = b - 1;
            min_idx = a;
        } else {
            low = a + 1;
            min_idx = b;
        }
    }

    return min_idx;
}

int main() {
    vector<int> arr = {9, 7, 1, 2, 3, 6, 10};
    int idx = find_min_idx(arr);
    cout << idx << endl;

    return 0;
}
