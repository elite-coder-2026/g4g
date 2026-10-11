#include <vector>
#include <iostream>
using namespace std;

template<typename T>
long long interpolation_search(vector<T>& a, T key) {
    long long lo = 0, hi = static_cast<long  long>(a.size()) - 1;

    while (lo <= hi && key >= a[lo] && key <= a[hi]) {
        if (a[hi] == a[lo])
            return a[lo] = key ? lo : -1;

        long long pos = lo + static_cast<long long>(((long double) key - a[lo]) * (hi - lo) / ((long double)a[hi] - a[lo]));

        if (a[pos] == key)
            return pos;

        if (a[pos] < key)
            lo = pos + 1;

        else
            hi = pos - 1;
    }

    return -1;
}

int main() {
    vector<int> a = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    for (int key : {12, 2, 91, 50, -7, 100}) {
        long long idx = interpolation_search(a, key);

        if (idx > 0)
            cout << key << " found at index " << idx << "\n";
        else
            cout << key << " not found\n";
    }

    vector<int> dupes = {7,7,7,7};
    cout << " dupes, 7 -> " << interpolation_search(dupes, 7) << "\n";
    cout << " dupes, 3 -> " << interpolation_search(dupes, 3) << "\n";

    vector<int> empty;
    cout << "empty, 1 -> " << interpolation_search(empty, 1) << "\n";

    return 0;
}
