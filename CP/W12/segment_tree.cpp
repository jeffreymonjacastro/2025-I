#include <bits/stdc++.h>

using namespace std;
#define cpu()                  \
  ios::sync_with_stdio(false); \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

const int MAXN = 1e5 + 5;

int t[4 * MAXN];

// Segment Tree with a size of four times the input size n
int left(int v) {
  return v * 2;
}

int right(int v) {
  return v * 2 + 1;
}

// Builds the segment tree from the input array a
// a: input array
// v: current node index
// tl: left index
// tr: right index
void build(int a[], int v, int tl, int tr) {
  if (tl == tr) {
    t[v] = a[tl];
  } else {
    int tm = (tl + tr) / 2;
    build(a, left(v), tl, tm);
    build(a, right(v), tm + 1, tr);
    t[v] = t[left(v)] + t[right(v)];
  }
}

// Queries the sum in the range [l, r]
// v: current node index
// tl: left index
// tr: right index
// l: query left index
// r: query right index
int sum(int v, int tl, int tr, int l, int r) {
  if (l > r)
    return 0;
  if (l == tl && r == tr) {
    return t[v];
  }
  int tm = (tl + tr) / 2;
  return sum(left(v), tl, tm, l, min(r, tm)) + sum(right(v), tm + 1, tr, max(l, tm + 1), r);
}

// Updates the value at position pos to new_val
// v: current node index
// tl: left index
// tr: right index
// pos: position to update
// new_val: new value
void update(int v, int tl, int tr, int pos, int new_val) {
  if (tl == tr) {
    t[v] = new_val;
  } else {
    int tm = (tl + tr) / 2;
    if (pos <= tm)
      update(left(v), tl, tm, pos, new_val);
    else
      update(right(v), tm + 1, tr, pos, new_val);
    t[v] = t[left(v)] + t[right(v)];
  }
}

// Helper function to print detailed node information recursively
void printNodeDetails(int v, int tl, int tr, int depth) {
  // Print indentation based on depth
  for (int i = 0; i < depth; i++) {
    cout << "  ";
  }

  cout << "Node " << v << ": " << t[v] << " [" << tl << "," << tr << "]";

  if (tl == tr) {
    cout << " (leaf)";
  }
  cout << endl;

  // Recursively print children if not a leaf
  if (tl != tr) {
    int tm = (tl + tr) / 2;
    printNodeDetails(left(v), tl, tm, depth + 1);
    printNodeDetails(right(v), tm + 1, tr, depth + 1);
  }
}

// Prints the segment tree in a pretty format
void printTree(int n) {
  cout << "\n=== Segment Tree ===" << endl;
  cout << "Array size: " << n << endl;

  // Calculate the height of the tree
  int height = ceil(log2(n)) + 1;

  cout << "\nTree structure (node: value [range]):" << endl;

  // Print level by level
  for (int level = 0; level < height; level++) {
    cout << "Level " << level << ": ";

    // Calculate range of nodes in this level
    int start = (1 << level);                     // 2^level
    int end = min((1 << (level + 1)) - 1, 4 * n); // 2^(level+1) - 1

    // Add spacing for better visualization
    for (int i = 0; i < (1 << (height - level - 1)) - 1; i++) {
      cout << "  ";
    }

    bool first = true;
    for (int v = start; v <= end; v++) {
      // Check if this node exists in our tree
      if (v < 4 * n && t[v] != 0) {
        if (!first) {
          // Add spacing between nodes
          for (int i = 0; i < (1 << (height - level)) - 1; i++) {
            cout << "  ";
          }
        }
        cout << t[v];
        first = false;
      }
    }
    cout << endl;
  }

  cout << "\nDetailed node information:" << endl;
  printNodeDetails(1, 0, n - 1, 0);
  cout << "===================" << endl;
}

void solve() {
  int n;
  cin >> n;

  int a[n];
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  // Starts with v = 1, tl = 0, tr = n - 1
  build(a, 1, 0, n - 1);

  cout << "\nInitial array: ";
  for (int i = 0; i < n; i++) {
    cout << a[i] << " ";
  }
  cout << endl;

  printTree(n);

  cout << "\n=== Example Operations ===" << endl;

  // Query sum of range [2, 4]
  if (n > 2) {
    cout << "Sum of range [2, 4]: " << sum(1, 0, n - 1, 2, 4) << endl;
  }

  // Update position 1 to value 10
  if (n > 1) {
    cout << "Updating position 1 to value 10..." << endl;
    update(1, 0, n - 1, 1, 10);

    // Print tree after update
    cout << "\nTree after update:" << endl;
    printTree(n);

    // Query again
    if (n > 2) {
      cout << "Sum of range [0, 2] after update: " << sum(1, 0, n - 1, 0, 2) << endl;
    }
  }
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
