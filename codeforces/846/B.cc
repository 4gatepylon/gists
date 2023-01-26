#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;
// Compile with
/*
g++ B.cc && cat B.txt | ./a.out
*/


// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
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
        assert(n >= 0);

        // Up until and including the element at that index
        long as[n];
        long prefixes[n];
        long suffixes[n];
        for (long j = 0; j < n; j++) { cin >> as[j]; }
        
        // I think... it's best to always have only two numbers because for any greater number of elements you can
        // improve it, but I did not prove...
        prefixes[0] = as[0];
        suffixes[n - 1] = as[n - 1];
        for (long j = 1; j < n; j++) { prefixes[j] = prefixes[j - 1] + as[j]; }
        for (long j = n - 2; j >= 0; j--) { suffixes[j] = suffixes[j + 1] + as[j]; }

        long max_gcd = 1;
        for (long prefix_end = 0; prefix_end < n - 1; prefix_end ++) {
            assert (prefix_end >= 0 && prefix_end + 1 < n);
            long prefix = prefixes[prefix_end];
            long suffix = suffixes[prefix_end + 1];
            // cout << "with prefix up to, including, index " << prefix_end << "/" << n-1 << ", prefix: " << prefix << " suffix: " << suffix << endl;
            max_gcd = max(max_gcd, __gcd(prefix, suffix));
        }
        cout << max_gcd << endl;
    }
    return 0;
}