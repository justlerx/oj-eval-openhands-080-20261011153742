#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<bool> in_odd_cycle;
vector<int> color, parent;
vector<vector<int>> adj_global;

void dfs_find_odd_cycles(int u, int p) {
    parent[u] = p;
    color[u] = 1;  // visiting
    
    for (int v : adj_global[u]) {
        if (color[v] == 0) {
            dfs_find_odd_cycles(v, u);
        } else if (color[v] == 1 && v != p) {
            // Found a cycle
            vector<int> cycle;
            int current = u;
            cycle.push_back(v);
            while (current != v && current != -1) {
                cycle.push_back(current);
                current = parent[current];
            }
            
            if (cycle.size() >= 3 && cycle.size() % 2 == 1) {
                // Odd cycle found
                for (int vertex : cycle) {
                    in_odd_cycle[vertex] = true;
                }
            }
        }
    }
    
    color[u] = 2;  // visited
}

bool is_bipartite_component(int start, vector<int>& component_color) {
    queue<int> q;
    q.push(start);
    component_color[start] = 1;
    
    vector<int> component_vertices;
    bool is_bipartite = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        component_vertices.push_back(u);
        
        for (int v : adj_global[u]) {
            if (component_color[v] == 0) {
                component_color[v] = -component_color[u];
                q.push(v);
            } else if (component_color[v] == component_color[u]) {
                is_bipartite = false;
            }
        }
    }
    
    if (!is_bipartite) {
        // For non-bipartite components, do a more precise check
        // Reset color array for DFS
        color.assign(component_color.size(), 0);
        parent.assign(component_color.size(), -1);
        
        for (int v : component_vertices) {
            if (color[v] == 0) {
                dfs_find_odd_cycles(v, -1);
            }
        }
    }
    
    return is_bipartite;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    // Build adjacency list
    adj_global.resize(n + 1);
    
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj_global[x].push_back(y);
        adj_global[y].push_back(x);
    }
    
    // Initialize arrays
    in_odd_cycle.resize(n + 1, false);
    color.resize(n + 1, 0);
    parent.resize(n + 1, -1);
    
    // Process each connected component
    for (int i = 1; i <= n; i++) {
        if (color[i] == 0) {
            vector<int> component_color(n + 1, 0);
            is_bipartite_component(i, component_color);
        }
    }
    
    // Count ministers who cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!in_odd_cycle[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}