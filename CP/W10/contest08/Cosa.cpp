#include <bits/stdc++.h>

using namespace std;

vector<int> dijkstra(vector<vector<pair<int, int>>>& g, int start) {
    vector<int> d(26, 1e9);
    priority_queue<pair<int, int>> pq;
    
    d[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        int dist = -pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        if (dist > d[u]) continue;
        
        for (auto [v, w] : g[u]) {
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                pq.push({-d[v], v});
            }
        }
    }
    
    return d;
}

void solve() {
    string s, t;
    cin >> s >> t;
    
    if (s.size() != t.size()) {
        cout << -1 << "\n";
        return;
    }
    
    int n;
    cin >> n;
    
    vector<vector<pair<int, int>>> graf(26);
    
    for (int i = 0; i < n; i++) {
        char a, b;
        int w;
        cin >> a >> b >> w;
        graf[a - 'a'].push_back({b - 'a', w});
    }
    
    // floyd warshall para encontrar todos los caminos minimos
    vector<vector<int>> dist(26, vector<int>(26, 1e9));
    
    for (int i = 0; i < 26; i++) {
        dist[i][i] = 0;
        for (auto [v, w] : graf[i]) {
            dist[i][v] = min(dist[i][v], w);
        }
    }
    
    for (int k = 0; k < 26; k++) {
        for (int i = 0; i < 26; i++) {
            for (int j = 0; j < 26; j++) {
                if (dist[i][k] != 1e9 && dist[k][j] != 1e9) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    ll costo = 0;
    string resultado = s;
    
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == t[i]) continue;
        
        int a = s[i] - 'a';
        int b = t[i] - 'a';
        
        int mejor = 1e9;
        char mejorChar = 'a';
        
        // probar todas las opciones
        for (int c = 0; c < 26; c++) {
            int costoTotal = dist[a][c] + dist[b][c];
            if (costoTotal < mejor) {
                mejor = costoTotal;
                mejorChar = 'a' + c;
            }
        }
        
        if (mejor == 1e9) {
            cout << -1 << "\n";
            return;
        }
        
        costo += mejor;
        resultado[i] = mejorChar;
    }
    
    cout << costo << "\n";
    cout << resultado << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    
    solve();
}