#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ B.cc && cat B.txt | ./a.out
*/

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

        assert(n > 1);
        if (n < 4) { cout << n << endl; } else {
            // This is ok if you look at the size of the problem
            // This is slippery... I think it works because we shrink k
            // (it is impossible to find a non-prime number that divides because we will hit
            // its prime factors first...)
            vector<int> sums;
            for (int k = 2; n > 1 && k * k <= n; k ++) {
                if (n % k == 0) {
                    int idx = 0;
                    while (n % k == 0) {
                        if (idx >= sums.size()) { sums.push_back(k); }
                        else { sums[idx] *= k; }
                        idx ++;
                        n /= k;
                    }
                }
            }
            // Can only get here if we have a big prime number left (if it's two or more numbers, then
            // k * k must be smaller than it because the number must both be greater than k)
            if (n != 1) {
                if (sums.size() == 0) { sums.push_back(n); }
                else { sums[0] *= n; }
            }

            int sum = 0;
            for (int s : sums) { sum += s; }
            cout << sum << endl;
        }
    }
    return 0;
}