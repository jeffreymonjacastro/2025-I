#include <bits/stdc++.h>
using namespace std;

template <typename A, typename B> ostream &operator<<(ostream &os, const pair<A, B> &p) {
  return os << '(' << p.first << ", " << p.second << ')';
}
template <typename T_container, typename T = typename enable_if<
                                    !is_same<T_container, string>::value,
                                    typename T_container::value_type>::type>
ostream &operator<<(ostream &os, const T_container &v) {
  os << '{';
  string sep;
  for (const T &x : v)
    os << sep << x, sep = ", ";
  return os << '}';
}
void dbg_out() { cerr << endl; }
template <typename Head, typename... Tail> void dbg_out(Head H, Tail... T) {
  cerr << ' ' << H;
  dbg_out(T...);
}
#ifdef LOCAL
#define dbg(...) cerr << "(" << #__VA_ARGS__ << "):", dbg_out(__VA_ARGS__)
#else
#define dbg(...)
#endif

#define ar array
#define ll long long
#define ld long double
#define sza(x) ((int)x.size())
#define all(a) (a).begin(), (a).end()

const int MAX_N = 1e5 + 5;
const ll MOD = 10000007;
const ll INF = 1e9;
const ld EPS = 1e-9;

auto binpow = [](ll a, ll b, ll m) {
  ll res = 1;
  while (b > 0) {
    if (b & 1)
      res = res * a;
    a = a * a;
    b >>= 1;
  }
  return res;
};

void solve() {
    int n;
    cin >> n;
    
    vector<vector<ll>> mat(n, vector<ll>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    
    vector<int> orden(n);
    for (int i = 0; i < n; i++) {
        cin >> orden[i];
        orden[i]--;
    }
    
    vector<bool> usado(n, false);
    vector<ll> respuestas;
    
    // procesar en orden inverso
    for (int paso = n - 1; paso >= 0; paso--) {
        int k = orden[paso];
        usado[k] = true;
        
        // actualizar floyd warshall con el nuevo vertice k
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mat[i][j] = min(mat[i][j], mat[i][k] + mat[k][j]);
            }
        }
        
        // calcular suma de caminos entre vertices activos
        ll suma = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (usado[i] && usado[j]) {
                    suma += mat[i][j];
                }
            }
        }
        
        respuestas.push_back(suma);
    }
    
    // imprimir en orden correcto
    reverse(all(respuestas));
    for (int i = 0; i < n; i++) {
        cout << respuestas[i];
        if (i < n - 1) cout << " ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    
    solve();
}