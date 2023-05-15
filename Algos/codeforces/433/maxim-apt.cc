#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;
// Compile with
/*
g++ maxim-apt.cc && cat maxim-apt.txt | ./a.out
*/


// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
   int n, k;
    cin >> n >> k;
    int min = k >= n || k <= 0 ? 0 : 1;
    int max = n - k >= 2 * k ? 2 * k : n - k;
    cout << min << " " << max << endl;
}