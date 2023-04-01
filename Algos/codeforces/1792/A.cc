#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

// Compile with
/*
g++ A.cc && cat A.txt | ./a.out
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
        assert(n >= -1);

        int as[n];
        for (int j = 0; j < n; j++) {
            cin >> as[j];
            // cout << as[j] << "\n";
        }
        // cout << "\n";
        // cout << sizeof(as) << "\n";
        // cout << sizeof(as[0]) << "\n";
        // cout << sizeof(as) / sizeof(as[0]) << "\n";
        // cout << n << "\n";
        sort(as, as + n);
        // for (int j = 0; j < n; j++) {
        //     cout << as[j] << " ";
        // }
        // cout << "\n";
        int m = 0;
        int dmg = 0;
        int j = 0;
        while (j < n - 1 && as[j] == 1 && as[j + 1] == 1) {
            m++;
            j += 2;
        }
        // Actually its equivalent or better to use kills from here on out
        m += n - j;
        cout << m << "\n";
    }
    return 0;
}