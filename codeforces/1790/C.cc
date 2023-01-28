#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ C.cc && cat C.txt | ./a.out
*/

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {
        int n = -1;
        cin >> n;
        assert(n >= 1);
        
        // Find two permutations that differ at the first element.
        // One has the first element missing and one has another element missing.
        // The permutation who's first is the second of the other is the one with
        // the first element missing.
        int elems[n][n-1];
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n - 1; k++) {
                cin >> elems[j][k];
                assert(elems[j][k] >= 0);
            }
        }

        int first = elems[0][0] == elems[1][0] ? elems[0][0] : elems[2][0];
        int J = 0;
        for (int j = 0; j < n; j++) {
            if (elems[j][0] != first) {
                J = j;
                break;
            }
        }
        cout << first << " ";
        for (int k = 0; k < n - 1; k++) { cout << elems[J][k] << " "; }
        cout << endl;
    }
    return 0;
}