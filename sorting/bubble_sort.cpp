#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int k = 0; k < n - i - 1; k++) {
            if (arr[k] > arr[k + 1]) {
                swap(arr[k], arr[k + 1]);
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void print_vector(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }

    cout << endl;
}

int main() {
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    bubbleSort(arr);
    cout << "sorted array: \n";
    print_vector(arr);

    return 0;
}
