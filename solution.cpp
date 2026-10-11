#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

vector<int> discovery_time, low_time, parent;
vector<bool> visited, in_odd_cycle;
int time_counter;

void dfs(int u, const vector<vector<int>>& adj) {
    visited[u] = true;
    discovery_time[u] = low_time[u] = time_counter++;
    
    for (int v : adj[u]) {
        if (!visited[v]) {
            parent[v] = u;
            dfs(v, adj);
            low_time[u] = min(low_time[u], low_time[v]);
        } else if (v != parent[u]) {
            // Back edge
            low_time[u] = min(low_time[u], discovery_time[v]);
            
            // Check if this back edge forms an odd cycle
            int cycle_length = discovery_time[u] - discovery_time[v] + 1;
            if (cycle_length % 2 == 1) {
                // Mark all vertices in this odd cycle
                int current = u;
                while (current != v && current != -1) {
                    in_odd_cycle[current] = true;
                    current = parent[current];
                }
                in_odd_cycle[v] = true;
            }
        }
    }
}

bool is_bipartite_component(int start, const vector<vector<int>>& adj, vector<bool>& component_vertices) {
    vector<int> color(adj.size(), 0);
    queue<int> q;
    q.push(start);
    color[start] = 1;
    
    vector<int> component;
    bool is_bipartite = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        component.push_back(u);
        component_vertices[u] = true;
        
        for (int v : adj[u]) {
            if (color[v] == 0) {
                color[v] = -color[u];
                q.push(v);
            } else if (color[v] == color[u]) {
                is_bipartite = false;
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
    vector<vector<int>> adj(n + 1);
    vector<int> degree(n + 1, 0);
    
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
        degree[x]++;
        degree[y]++;
    }
    
    // First pass: find all vertices in non-bipartite components
    vector<bool> in_non_bipartite(n + 1, false);
    vector<bool> visited_bfs(n + 1, false);
    
    for (int i = 1; i <= n; i++) {
        if (!visited_bfs[i]) {
            vector<bool> component_vertices(n + 1, false);
            bool is_bipartite = is_bipartite_component(i, adj, component_vertices);
            
            if (!is_bipartite) {
                for (int j = 1; j <= n; j++) {
                    if (component_vertices[j]) {
                        in_non_bipartite[j] = true;
                    }
                }
            }
            
            for (int j = 1; j <= n; j++) {
                if (component_vertices[j]) {
                    visited_bfs[j] = true;
                }
            }
        }
    }
    
    // Second pass: for vertices in non-bipartite components, 
    // check if they're actually part of an odd cycle
    in_odd_cycle.resize(n + 1, false);
    visited.resize(n + 1, false);
    discovery_time.resize(n + 1, 0);
    low_time.resize(n + 1, 0);
    parent.resize(n + 1, -1);
    time_counter = 0;
    
    for (int i = 1; i <= n; i++) {
        if (in_non_bipartite[i] && !visited[i]) {
            dfs(i, adj);
        }
    }
    
    // Count ministers who cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        // Cannot invite if: not in odd cycle OR degree < 2
        if (!in_odd_cycle[i] || degree[i] < 2) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}