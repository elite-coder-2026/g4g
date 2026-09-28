#include <iostream>
#include <vector>

using namespace std;

/**
 * the idea is to check if the last two elements are in order, then recursively
 * check the rest of the array. the base case is when the array has zero or one
 * element, which is always considered sorted
 */
bool isSortedUtil(vector<int>& arr, int n) {
    if (n == 0 || n == 1)
        return true;

    return arr[n - 1] >= arr[n - 2] && isSortedUtil(arr, n - 1);
}

bool isSorted(vector<int>& arr) {
    return isSortedUtil(arr, arr.size());
}

int main() {
    vector<int> arr = {10, 20, 30, 40, 50};
    cout << (isSorted(arr) ? "true\n" : "false\n");

    return 0;
}
