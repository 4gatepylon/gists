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
        assert(n >= 0);

        // 2 evens 1 odd, or 3 odds
        int odd1 = -1, odd2 = -1, odd3 = -1;
        int even1 = -1, even2 = -1;
        for (int j = 0; j < n; j++) {
            int a = -1;
            cin >> a;
            assert(a >= 1);

            // Fill in the first three odds and/or first two evens
            if (a % 2 == 1) {
                if (odd1 == -1) odd1 = j + 1;
                else if (odd2 == -1) odd2 = j + 1;
                else if (odd3 == -1) odd3 = j + 1;
            } else {
                if (even1 == -1) even1 = j + 1;
                else if (even2 == -1) even2 = j + 1;
            }
        }
        // If we found a combination output it else don't
        if (odd1 != -1 && odd2 != -1 && odd3 != -1) {
            cout << "YES\n" << odd1 << " " << odd2 << " " << odd3 << "\n";
        } else if (even1 != -1 && even2 != -1 && odd1 != -1) {
            cout << "YES\n" << even1 << " " << even2 << " " << odd1 << "\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}