#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

int parent[100005];

void make_set(int v) {
  parent[v] = v;
}

int find_set(int v) {
  if (v == parent[v])
    return v;
  return find_set(parent[v]);
}

void union_sets(int a, int b) {
  a = find_set(a);
  b = find_set(b);
  if (a != b)
    parent[b] = a;
}

void solve() {
  int n;
  cin >> n;
}

int main() {
  cpu();
  int t;
  t = 1;
  // cin >> t;
  while (t--)
    solve();
  return 0;
}
