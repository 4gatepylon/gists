#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ 1774c.cc && cat 1774c.txt | ./a.out
*/

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n >= 0);

        int m = -1;
        cin >> m;
        assert(m >= 0);

        int k = -1;
        cin >> k;
        assert(k >= 0);

        cout << "hi\n" << endl;
    }
}