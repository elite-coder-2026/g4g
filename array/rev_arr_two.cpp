#include "../lib/base.hpp"

using namespace std;

void reverseArr(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        swap(arr[i], arr[n - i - 1]);
    }
}

int main() {
    vector<int> arr = { 1, 4, 3, 2, 6, 5 };

    reverseArr(arr);

    for(int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
    return 0;
}
