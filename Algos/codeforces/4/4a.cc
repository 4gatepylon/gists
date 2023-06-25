#include  <iostream>

using namespace std;

int main() {
  int x;
  x = -1;
  cin >> x;
  cout << ((x % 2 == 0 && x >= 4) ? "YES" : "NO") << "\n";
}
