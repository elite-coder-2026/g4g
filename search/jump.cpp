#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;
int jump_search(int a[], int x, int n) {
    int step = sqrt(n);
    int prev = 0;

    while (a[min(step, n) -1] < x) {
        prev = step;
        step += sqrt(n);

        if (prev >= n) {
            return -1;
        }
    }

    while (a[prev] < x) {
        prev++;

        if (prev == min(step, n))
            return -1;
    }

    if (a[prev] == x)
        return prev;

    return -1;
}

int main() {
    int a[] = {0, 1, 2, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 277, 610};
    int x = 55;
    int n = sizeof(a) / sizeof(a[0]);

    int idx = jump_search(a, x, n);

    cout << "\nNumber " << x << " is at index " << idx;

    return 0;
}
