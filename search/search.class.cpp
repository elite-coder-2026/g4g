#include "search.class.hpp"

int SearchAlgorithms::expontential(vector<int>& arr, int l, int r, int x) {
    int n = arr.size();

    if (arr[0] == x)
        return 0;

    int i = 1;

    while (i < n && arr[i] < x) {
        i = i * 2;
    }

    return this->binary(arr, i / 2, min(i, n - 1), x)

}

int SearchAlgorithms::binary(vector<int>& arr, int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x)
            return mid;

        if (arr[mid] > x) {
            return this->binary(arr, l, mid - 1, x);
        }

        return this->binary(arr, mid + 1, r, x);
    }
    return -1;
}

int SearchAlgorithms::linear(vector<int>& arr, int x) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == x) {
            return i;
        }
    }

    return -1;
}

bool SearchAlgorithms::ternary(vector<int>& arr, int x) {
    int l = 0, r = arr.size() - 1;

    while (l <= r) {
        int a = l + (r - l) / 3;
        int b = r - (r - l) / 3;

        if (arr[a] == x || arr[b] == x) {
            return true;
        }

        if (x < arr[a]) {
            r = a - 1;
        } else if (x > arr[b]) {
            l = b + 1;
        } else {
            l = a + 1l;
            r = b - 1;
        }
    }

    return false;
}
