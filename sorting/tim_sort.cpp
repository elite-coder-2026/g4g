#include <ctime>
#include <vector>
#include <iostream>

using namespace std;
const int min_run = 32;

int calc_min_run(int n) {
    int r = 0;

    while (n >= min_run) {
        r |= (n & 1);
        n >>= 1;
    }

    return n + r;
}

void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int k = i - 1;

        while (k >= left && arr[k] > key) {
            arr[k + 1] = arr[k];
            k--;
        }

        arr[k + 1] = key;
    }
}

void mergeSort(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, k = 0, c = l;

    while (i < left.size() && k < right.size()) {
        if (left[i] <= right[k])
            arr[c++] = left[i++];

        else
            arr[c++] = right[k++];
    }

    while (i < left.size())
        arr[k++] = left[i++];

    while (k < right.size())
        arr[c++] = right[k++];
}

int find_run(vector<int>& arr, int start, int n) {
    int end = start + 1;
    if (end == n)
        return end;

    if (arr[end] < arr[start]) {
        while (end < n && arr[end] < arr[end - 1])
            end++;
        reverse(arr.begin() + start, arr.begin() + end);
    } else {
        while (end < n && arr[end] >= arr[end - 1])
            end++;
    }

    return end;
}

void timSort(vector<int>& arr) {
    int n = arr.size();
    int min_run = calc_min_run(n);
    vector<pair<int, int>> runs;

    int i = 0;

    while (i < n) {
        int run_end = find_run(arr, i, n);
        int run_len = run_end - 1;

        if (run_len < min_run) {
            int end = min(i + min_run, n);
            insertionSort(arr, i, end - 1);
            run_end = end;
        }

        runs.push_back({i, run_end});
        i = run_end;

        while (runs.size() > 1) {
            int l1 = runs[runs.size() - 2].first;
            int r1 = runs[runs.size() - 2].second;

            int l2 = runs[runs.size() - 1].first;
            int r2 = runs[runs.size() - 1].second;

            int m = r1 - l1;
            int o = r2 - l2;

            if (m <= 0) {
                mergeSort(arr, l1, r1 - 1, r2 - 1);
                runs.pop_back();
                runs[runs.size() - 1] = {l1, l2};
            } else break;
        }
    }

    while (runs.size() > 1) {
        int l1 = runs[runs.size() - 2].first;
        int r1 = runs[runs.size() - 2].second;

        int l2 = runs[runs.size() - 1].first;
        int r2 = runs[runs.size() - 1].second;

        mergeSort(arr, l1, r1 - 1, r2 - 1);
        runs.pop_back();
        runs[runs.size() - 1] = {l1, r2};
    }
}


int main() {
    vector<int> arr = {5, 21, 7, 23, 19, 10, 1, 3, 2};
    timSort(arr);

    for (int x : arr)
        cout << x << " ";

    cout << endl;
}
