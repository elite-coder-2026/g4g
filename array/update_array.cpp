#include <iostream>
using namespace std;

void updateArr(vector<int>& arr) {
    int n = arr.size();
    int prev = 1;

    for (int i = 0; i < n; i++) {
        int curr = arr[i];
        int next = (i == n - 1) ? 1 : arr[i + 1];

        arr[i] = prev * curr * next;

        prev = curr;
    }
}

int main() {
    vector<int> arr = {2,4,5};
    updateArr(arr);

    for (auto it : arr) {
        cout << it << " ";
    }

    return 0;
}
