#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ 1772e.cc && cat 1772e.txt | ./a.out
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
        assert(n >= 2);

        int p[n + 1];
        for (int j = 1; j < n + 1; j++) {
            cin >> p[j];
            assert(1 <= p[j] && p[j] <= n);
        }

        // Keep track of what needs to be fixed
        unordered_set<int> first_only;
        unordered_set<int> second_only;
        unordered_set<int> overlap;
        for (int j = 1; j < n + 1; j++) {
            if (p[j] != j && p[j] != n - j + 1)      {
                // cout << "p[" << j << "] = " << p[j] << endl; // XXX
                overlap.insert(j);
            }
            else if (p[j] != j)                      first_only.insert(j);
            else if (p[j] != n - j + 1)              second_only.insert(j);
        }

        // // XXX
        // cout << "First only size: " << first_only.size() << endl;
        // cout << "Second only size: " << second_only.size() << endl;
        // cout << "Overlap size: " << overlap.size() << endl;
        // // XXX

        if (first_only.size() + overlap.size() <= second_only.size()) { cout << "First" << endl; }
        else if (second_only.size() + overlap.size() < first_only.size()) { cout << "Second" << endl; }
        else {  cout << "Tie" << endl; }
    }
}