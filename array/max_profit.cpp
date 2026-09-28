#include "../lib/base.hpp"

using namespace std;

int maxProfit(vector<int>& prices) {
    int _min = prices[0], res = 0;

    for (int i = 1; i < prices.size(); i++) {
        _min = min(_min, prices[i]);
        res = max(res, prices[i] - _min);
    }

    return res;
}

int main() {
    vector<int> prices = {7, 10, 1, 3, 6, 9, 2};
    cout << maxProfit(prices) << endl;
    return 0;
}
