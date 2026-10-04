#include <iostream>
#include <vector>

using namespace std;

bool bfs(int start, vector<vector<int>>& adj, vector<bool>& visited) {
    queue<pair<int, int>> q;
    q.push({start, -1});
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front().first;
        int parent = q.front().second;
        q.pop();

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push({neighbor, node});
            } else if (neighbor != parent) {
                return true;
            }
        }
    }

    return false;
}

bool isCycle(vector<vector<int>>& adj) {
    int v = adj.size();
    vector<bool> visited(v, false);

    for (int i = 0; i < v; i++) {
        if (!visited[i]) {
            if (bfs(i, adj, visited)) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    vector<vector<int>> adj = {
        {1, 2},
        {0, 2},
        {0, 1, 3},
        {2}
    };

    isCycle(adj) ? cout << "true" : cout << "false";
}
