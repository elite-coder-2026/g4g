// Prefix sums: O(n) preprocessing, O(1) per range-sum query.
// Runs with built-in test data; no input needed.

#include <cstdio>
#include <vector>

using namespace std;

// prefix[i] = a[0] + a[1] + ... + a[i-1], prefix[0] = 0
vector<long long> buildPrefix(const vector<long long> &a) {
    vector<long long> prefix(a.size() + 1, 0);

    for (size_t i = 0; i < a.size(); i++) {
        prefix[i + 1] = prefix[i] + a[i];
    }

    return prefix;
}

// Sum of a[l..r], 1-indexed, inclusive.
long long rangeSum(const vector<long long> &prefix, int l, int r) {
    return prefix[r] - prefix[l - 1];
}

int main() {
    long long values[] = {3, 1, 4, 1, 5, 9, 2, 6};
    vector<long long> a(values, values + sizeof(values) / sizeof(values[0]));

    vector<long long> prefix = buildPrefix(a);

    printf("array:  ");
    for (size_t i = 0; i < a.size(); i++) {
        printf("%lld ", a[i]);
    }
    printf("\nprefix: ");
    for (size_t i = 0; i < prefix.size(); i++) {
        printf("%lld ", prefix[i]);
    }
    printf("\n\n");

    int queries[][2] = {{1, 8}, {2, 4}, {5, 5}, {3, 7}};
    int q = sizeof(queries) / sizeof(queries[0]);

    for (int k = 0; k < q; k++) {
        int l = queries[k][0];
        int r = queries[k][1];
        printf("sum(%d..%d) = %lld\n", l, r, rangeSum(prefix, l, r));
    }

    return 0;
}
