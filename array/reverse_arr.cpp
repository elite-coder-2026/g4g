#include "../lib/base.hpp"
#include <vector>

using namespace std;

void reverseArr(vector<int>& arr) {
    int left = 0, right = arr.size() - 1;

    while (left < right) {
        swap(arr[left], arr[right]);
        left++;
        right--;
    }
}

int main() {
    vector<int> arr { 1, 4, 3, 2, 6, 5 };
    reverseArr(arr);
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }

    return 0;
}
