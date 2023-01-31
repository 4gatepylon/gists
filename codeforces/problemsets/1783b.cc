#include <iostream>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ 1783b.cc && cat 1783b.txt | ./a.out
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

        // We want as many differences as possible, there are n^2 - 1 differences to capture across n^2 entries
        // (each number 1 to n^2 appears exactly once)
        int mat[n][n];

        // Put zeros so we can later check if we fked it up
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mat[i][j] = 0;
            }
        }

        // Greedily fill up the matrix in a snake pattern from top left right right then snaking around
        // Draw an arrow from previous placement to next placement, for each arrow there is a new difference
        // added to the matrix, thus we'll have n^2 - 1 arrows (n * (n - 1) + n - 1 = (n + 1) * (n - 1)  = n^2 - 1)
        // and we'll therefore get all the the differences.
        // 
        // Each new arrow adds a new differnce because either the lowest gets higher by 1 or the highest gets lower by 1
        // (and induction).
        int highest = n * n;
        int lowest = 1;
        int use_lowest = true;
        for (int i = 0; i < n; i++) {
            int start = (i % 2 == 0 ? 0 : n - 1);
            int end = (i % 2 == 0 ? n : -1);
            int step = (i % 2 == 0 ? 1 : -1);
            for (int j = start; j != end; j += step) {
                if (use_lowest) {
                    mat[i][j] = lowest;
                    lowest ++;
                } else {
                    mat[i][j] = highest;
                    highest --;
                }
                use_lowest = !use_lowest;
            }
        }

        // Assert that we have properly filled up the matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                assert(mat[i][j] != 0);
            }
        }
        // Make sure that they crossed...
        assert(highest == lowest - 1);

        // Print out the matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }
}