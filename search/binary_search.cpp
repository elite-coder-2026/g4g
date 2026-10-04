#include <iostream>
#include <vector>

using namespace std;

int binary_search(vector<int>& arr, int low, int high, int x) {
    if (high >= low) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == x)
            return mid;

        if (arr[mid] > x)
            return binary_search(arr, low, mid - 1, x);
        return binary_search(arr, mid + 1, high, x);
    }

    return -1;
}

int main() {
    vector<int> arr = {2, 3, 4, 10, 20, 40};
    int query = 40;
    int n = arr.size();
    int res = binary_search(arr, 0, n - 1, query);

    if (res == -1)
        cout << "Element is not present in array";

    else
        cout << "element is present at index " << res;

    return 0;
}
