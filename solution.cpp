#include <iostream>
#include <vector>
#include <queue>

using namespace std;

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
    
    // Color array: 0 = unvisited, 1 = color A, -1 = color B
    vector<int> color(n + 1, 0);
    vector<bool> can_invite(n + 1, false);
    
    for (int i = 1; i <= n; i++) {
        if (color[i] == 0) {
            // BFS to check if this component is bipartite
            queue<int> q;
            q.push(i);
            color[i] = 1;
            
            vector<int> component_vertices;
            bool is_bipartite = true;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                component_vertices.push_back(u);
                
                for (int v : adj[u]) {
                    if (color[v] == 0) {
                        color[v] = -color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // Found odd cycle, component is not bipartite
                        is_bipartite = false;
                    }
                }
            }
            
            // If component is not bipartite, check which vertices can be in odd cycles
            if (!is_bipartite) {
                // In a non-bipartite component, a vertex can be in an odd cycle
                // if and only if it's not a leaf (degree >= 2 in the component)
                for (int v : component_vertices) {
                    // Count neighbors within this component
                    int component_degree = 0;
                    for (int neighbor : adj[v]) {
                        if (color[neighbor] != 0) {  // neighbor is in this component
                            component_degree++;
                        }
                    }
                    
                    if (component_degree >= 2) {
                        can_invite[v] = true;
                    }
                }
            }
        }
    }
    
    // Count ministers who cannot be invited
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!can_invite[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}