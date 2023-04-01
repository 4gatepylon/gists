#include <iostream>
#include <algorithm>
#include <cassert>
#include <math.h>

using namespace std;

// Compile with
/*
g++ B.cc && cat B.txt | ./a.out
*/


// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them longo longs returning zero if not an long
// (after which it might return some trash like -1)
int main() {
    // Load the number of tests
    long num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (long i = 0; i < num_tests; i++) {
        // Load the array
        int n = -1;
        cin >> n;
        assert(n >= 0);

        int as[n];
        for (int j = 0; j < n / 2; j++) { cin >> as[j]; }
        int moves = 0;
        for (int j = 0; j < n; j++) {
            if (as[j] > j + 1) moves ++;
            else break;
        }
        cout << moves << "\n";
        
    }
    return 0;
}