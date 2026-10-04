#include <algorithm>
#include <climits>
#include <vector>
#include <iostream>

using namespace std;

int tsp(vector<vector<int>>& cost) {
    int num_nodes = cost.size();
    vector<int> nodes;

    for (int i = 0; i < num_nodes; i++) {
        nodes.push_back(i);
    }

    int min_cost = INT_MAX;

    do {
        int curr_cost = 0;
        int curr_node = 0;

        for (int i = 0; i < nodes.size(); i++) {
            curr_cost += cost[curr_node][nodes[i]];
            curr_node = nodes[i];
        }

        curr_cost += cost[curr_node][0];
        min_cost = min(min_cost, curr_cost);
    } while (next_permutation(nodes.begin(), nodes.end()));

    return min_cost;
}

int main() {
    vector<vector<int>> cost = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int res = tsp(cost);
    cout << res << endl;

    return 0;
}
