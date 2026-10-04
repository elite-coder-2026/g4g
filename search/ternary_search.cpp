#include <iostream>
#include <vector>

using namespace std;

bool ternarySearch(vector<int>& arr, int x) {
    int left = 0, right = arr.size() - 1;

    while (left <= right) {
        int a = left + (right - left) / 3;
        int b = right - (right - left) / 3;

        if (arr[a] == x || arr[b] == x) {
            return true;
        }

        if (x < arr[a]) {
            right = a - 1;
        } else if (x > arr[b]) {
            left = b + 1;
        } else {
            left = a + 1;
            right = b - 1;
        }
    }

    return false;
}

int main() {
    vector<int> arr = {1, 2, 3, 4, 6};
    int x = 6;
    cout << (ternarySearch(arr, x) ? "true" : "false") << endl;

    arr = {1, 3, 4, 5, 6};
    x = 2;
    cout << (ternarySearch(arr, x) ? "true" : "false") << endl;

    return 0;
}
