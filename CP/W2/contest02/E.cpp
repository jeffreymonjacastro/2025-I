#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve() {
  string s, animal;
  vector<string> animals;
  getline(cin, s);

  while (true) {
    getline(cin, animal);

    string word;
    vector<string> info;
    stringstream ss(animal);

    while (ss >> word) {
      info.push_back(word);
    }

    if (info[0] == "what")
      break;

    animals.push_back(info[2]);
  }

  string word;
  vector<string> sounds;
  stringstream ss(s);

  while (ss >> word) {
    sounds.push_back(word);
  }

  for (string sound : sounds) {
    auto it = find(animals.begin(), animals.end(), sound);
    if (it == animals.end()) {
      cout << sound << " ";
    }
  }
}

int main() {
  cpu();
  int t = 1;
  cin >> t;
  cin.ignore();
  while (t--)
    solve();
  return 0;
}
