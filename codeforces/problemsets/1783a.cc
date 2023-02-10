#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
#include "math.h"
#include "assert.h"

using namespace std;

// Compile with
/*
g++ 1783a.cc && cat 1783a.txt | ./a.out
*/

// Note that they'll be sorted
static inline pair<bool, unordered_set<int> > knapsack(long* a, long size, long val) {
    // DP[i][val] = DP[i+1][val - a[i]] or DP[i+1][val]
    bool dp[size][val + 1];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= val; j++) {
            if (j == 0)         dp[i][j] = true;  // Fix in zero steps (already fixed)
            else if (a[i] == j) dp[i][j] = true;  // Fix in one step
            else                dp[i][j] = false; // Haven't fixed yet
        }
    }

    // Maybe you can make this linear but eh
    for (int i = 1; i < size; i++) {
        for (int j = 1; j <= val; j++) {
            dp[i][j] = dp[i - 1][j]; // Try to solve for same value
            if (j - a[i - 1] >= 0) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - a[i - 1]]; // Try to solve for value - a[i]
            }
        }
    }

    // XXX
    cout << "DP FOR VAL " << val << " WITH PREFIX of length " << size << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= val; j++) {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }
    cout << "******************\n";
    // XXX


    unordered_set<int> indices;
    if (dp[size - 1][val]) {
        int running_val = val;
        for (int i = size; i >= 0; i++) {
            assert(dp[i][running_val]);

            // We've collected all the indices we need
            if (running_val == 0 || running_val == a[i]) {
                if (running_val == a[i]) indices.insert(i);
                break;
            }

            assert(running_val > a[i]);
            assert(i > 0);

            // Otherwise greedily don't collect indices
            if (!dp[i - 1][running_val]) {
                assert(dp[i - 1][running_val - a[i]]);
                indices.insert(i);
            }
        }

        int sum = 0;
        for (auto it = indices.begin(); it != indices.end(); it++) { sum += a[*it]; }
        assert (sum == val);

        return make_pair(true, indices);
    }
    return make_pair(false, indices);
}

int main() {
    // Load the number of tests from the input stream
    long num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (long i = 0; i < num_tests; i++) {
        long n = -1;
        cin >> n;

        long a[n];
        for (long j = 0; j < n; j++) { cin >> a[j]; }
        sort(a, a + n);

        bool succ = false;
        unordered_set<int> indices;
        for (int j = 1; j < n; j ++) {
            pair<bool, unordered_set<int> > res = knapsack(a, n, 0);
            if (res.first) {
                indices = res.second;
                succ = true;
                break;
            }
        }
        cout << (succ ? "YES" : "NO") << endl;
        if (succ) {
            // Really shit but I guess it should work?
            int sum = 0;
            for (auto it = indices.begin(); it != indices.end(); it++) {
                cout << a[*it] << " ";
                sum += a[*it];
            }
            cout << sum;

            if (n > indices.size() + 1) {
                cout << " ";
                bool skipped_sum = false;
                for (int j = 0; j < n; j++) {
                    // Only collect what we haven't already collected
                    if (indices.find(j) == indices.end()) {
                        // Print out
                        if (a[j] == sum) {
                            if (skipped_sum) {
                                cout << a[j];
                            }
                            skipped_sum = true;
                        } else {
                            cout << a[j];
                        }
                        
                        // Make sure to add a space if we are't at the end
                        if (j != n - 1) {
                            cout << " ";
                        }
                    }
                }
            }
            cout << endl;
        }
    }
}