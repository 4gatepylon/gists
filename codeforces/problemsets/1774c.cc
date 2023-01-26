#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ 1774c.cc && cat 1774c.txt | ./a.out
*/

string readline() {
    string s;
    getline(cin, s);
    return s;
}

int readint() {
    string s = readline();
    return stoi(s);
}

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    int num_tests = readint();
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = readint();
        assert(n >= 2);

        string bs = readline();
        assert(bs.length() == n - 1);
        int curr = 1;
        cout << curr << " ";
        for (int j = 1; j < n - 1; j++) {
            // index is length - 1, we have length + 1 players, one player cannot win, but
            // otherwise either all or the previous set can win
            curr = bs[j] == bs[j - 1] ? curr : j + 1;
            cout << curr << (j == n - 1 ? "" : " ");
        }
        cout << endl;
    }
}