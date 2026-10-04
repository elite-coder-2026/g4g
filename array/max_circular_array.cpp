#include <iostream>
#include <vector>

using namespace std;

int maxCircularSum(vector<int>& arr) {
    int n = arr.size();
    int suffix = arr[n - 1];

    vector<int> max_suffix(n + 1, 0);
    max_suffix[n - 1] = arr[n - 1];

    for (int i = n - 2; i>=0; --i) {
        suffix = suffix + arr[i];
        max_suffix[i] = max(max_suffix[i + 1], suffix);
    }

    int circular_sum = arr[0];
    int normalSum = arr[0];
    int curr = 0;
    int prefix = 0;

    for (int i = 0; i < n; i++) {
        curr = max(curr + arr[i], arr[i]);
        normalSum = max(normalSum, curr);
        prefix = prefix + arr[i];
        circular_sum = max(circular_sum, prefix + max_suffix[i + 1]);
    }

    return max(circular_sum, normalSum);
}

int main() {
    vector<int> arr = {8, -8, 9, -9, 10, -11, 12};
    cout << maxCircularSum(arr);
}
