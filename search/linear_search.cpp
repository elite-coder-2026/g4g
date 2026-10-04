#include <vector>
#include <iostream>
using namespace std;

int search (vector<int>& arr, int x) {
    for (int i = 0; i < arr.size(); i++)
        if (arr[i] == x)
            return i;

    return -1;
}

int main() {
    vector<int> arr = {2, 3, 5, 10, 40};
    int x = 10;
    int res = search(arr, x);

    if (res == -1)
        cout << "element is not present in the array" << endl;
    else
        cout << "element is present at index " << res;

    return 0;
}
