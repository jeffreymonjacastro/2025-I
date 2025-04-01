#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define cpu()                                                                  \
  ios::sync_with_stdio(false);                                                 \
  cin.tie(nullptr);
#define ll long long
#define lld long double
const int mod = 1e9 + 7;

void solve(int n) {
  vector<string> v;
  string reg = "????????????????????????????????";
  string s;

  for (int i = 0; i < n; i++) {
    getline(cin, s);
    v.push_back(s);
  }

  for (int i = 0; i < n; i++) {
    vector<string> words;
    stringstream ss(v[i]);
    string word;

    while (ss >> word) {
      words.push_back(word);
    }

    if (words[0] == "CLEAR") {
      reg[stoi(words[1])] = '0';
    } else if (words[0] == "SET") {
      reg[stoi(words[1])] = '1';
    } else if (words[0] == "AND") {
      int i = stoi(words[1]);
      int j = stoi(words[2]);
      if (reg[i] == '0' || reg[j] == '0') {
        reg[i] = '0';
      } else if ((reg[i] == '1' && reg[j] == '?') ||
                 (reg[i] == '?' && reg[j] == '1')) {
        reg[i] = '?';
      } else {
        reg[i] = ((reg[i] - '0') & (reg[j] - '0')) + '0';
      }
    } else if (words[0] == "OR") {
      int i = stoi(words[1]);
      int j = stoi(words[2]);
      if (reg[i] == '1' || reg[j] == '1') {
        reg[i] = '1';
      } else if ((reg[i] == '0' && reg[j] == '?') ||
                 (reg[i] == '?' && reg[j] == '0')) {
        reg[i] = '?';
      } else {
        reg[i] = ((reg[i] - '0') | (reg[j] - '0')) + '0';
      }
    }
  }

  reverse(reg.begin(), reg.end());
  cout << reg << endl;
}

int main() {
  cpu();
  int t;
  cin >> t;
  cin.ignore();

  while (t != 0) {
    solve(t);
    cin >> t;
    cin.ignore();
  }
  return 0;
}
