

#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ B.cc && cat B.txt | ./a.out
*/

// Written after the fact

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {
        int n = -1, full_sum = - 1, partial_sum = -1;
        cin >> n >> full_sum >> partial_sum;
        assert(n >= 1 && full_sum >= 1 && partial_sum >= 1);

        int max_die = full_sum - partial_sum;

        // cout << "number of dice is " << n << " full sum is " << full_sum << " max die is is " << max_die << endl;

        // Create counts histogram
        int counts[max_die + 1];
        for (int i = 0; i < max_die + 1; i++) { counts[i] = 0; }

        // Fill with as many die as possible favoring bigger ones
        int curr_die = max_die ;
        while (full_sum > 0) {
            assert(max_die >= curr_die && curr_die >= 1);
            if (full_sum >= curr_die && full_sum - curr_die >= n - 1) {
                // If we can take of size curr_die we should greedily
                // We can take of size curr_die when we wouldn't have too few numbers left
                // to fill up with 1's
                counts[curr_die] ++;
                full_sum -= curr_die;
                n --;
            } else {
                // If taking with curr_die would make us go negative or stop us from filling up with 1's then we should not take curr_die
                curr_die --;
            }
        }
        assert(counts[max_die] > 0);
        assert(curr_die >= 1);
        assert(n == 0);
        assert(full_sum == 0);
        for (int c = max_die; c > 0; c--) {
            for (int i = 0; i < counts[c]; i++) {
                cout << c << " ";
            }
        }
        cout << endl;
    }
}

