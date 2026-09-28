#include <iostream>
#include <vector>
using namespace std;

int count_increasing(vector<int>& arr) {
    int n = arr.size();
    int count = 0;
    int len = 1;

    for (int i = 1; i < n; i++) {
        if (arr[i] > arr[i - 1]) {
            len++;
        } else {
            count += (len * (len - 1)) / 2;
            len = 1;
        }
    }

    count += (len * (len - 1)) / 2;
    return count;
}

int main() {
    vector<int> arr = { 1, 4, 5, 3, 7, 9};
    cout << count_increasing(arr) << endl;

    return 0;
}
