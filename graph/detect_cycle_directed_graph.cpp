#include <iostream>
#include <vector>
using namespace std;
bool dfs(vector<vector<int>>& adj, int u, vector<bool>& visited, vector<bool>& rec_stack) {
    if (rec_stack[u]) {
        return true;
    }

    if (visited[u]) {
        return false;
    }

    visited[u] = true;
    rec_stack[u] = true;

    for (int v : adj[u]) {
        if (dfs(adj, v, visited, rec_stack))
            return true;
    }

    rec_stack[u] = false;
    return false;
}

bool isCyclic(int v, vector<vector<int>>& edges) {
    vector<vector<int>> adj(v);

    for (auto& edge : edges) {
        adj[edge[0]].push_back(edge[1]);
    }

    vector<bool> visited(v, false);
    vector<bool> rec_stack(v, false);

    for (int i = 1; i < v; i++) {
        if (!visited[i] && dfs(adj, i, visited, rec_stack))
            return true;
    }

    return false;
}

int main() {
    int v = 4;

    vector<vector<int>> edges = {
        {0, 1},
        {1, 2},
        {2, 0},
        {2, 3}
    };

    cout << (isCyclic(v, edges) ? "true" : "false") << endl;
    return 0;
}
