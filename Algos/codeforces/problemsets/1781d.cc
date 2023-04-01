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
g++ 1781d.cc && cat 1781d.txt | ./a.out
*/

static inline bool sq(long x) {
    long l = 1;
    long r = x / 2 + 1;
    while (l < r - 1) {
        // cout << "l: " << l << " r: " << r << " x: " << x << endl;
        long m = l + (r - l) / 2;
        long m2 = m * m;
        // cout << "m: " << m << " m2: " << m2 << endl;
        if (m2 == x) {
            return true;
        } else if (m2 < x) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    return l * l == x;
}

#define MAX 1000000000000000000l
// #define MAX 10000l


// a + x perfect sq
// b + x perfect sq
// If it's not the nearest perfect square for the larger number, then it's probably not for the smaller number?
static inline pair<long, bool> shared(long aa, long bb) {
    long a = aa < bb ? aa : bb;
    long b = aa < bb ? bb : aa;
    for (int k = 1; k < MAX; k++) { // Technically wrong bound?
        long k2 = k * k;
        long x = k2 - a;
        if (MAX >= x && x >= 0 && sq(b + x)) {
            return make_pair(x, true);
        }
    }
    return make_pair(0, false);
}

// If a given x works for a set of a's, then no other x will work for all those a's.
// For every a we can find an x that works for it, as well as an x that works for each
// other specific a. The maximal x has to be one of these because an x that works for a larger
// superset of a's must work for these two, and we know this is the only x that works for
// these two. This means we have O(n^2) x's to pick from and we only need to do the long search
// once... There is always at least one

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

        // We want as many differences as possible, there are n^2 - 1 differences to capture across n^2 entries
        // (each number 1 to n^2 appears exactly once)
        long a[n];
        for (long j = 0; j < n; j++) { cin >> a[j]; }

        // For each candidate keep track of which a's it's able to square
        long max_count = 1;
        for (long j = 0; j < n; j++) {
            for (long k = j + 1; k < n; k++) {
                // For each pair of as (2500)
                // See if there is an x that works for both, then try and see how many other places it works for
                long count = 0;
                auto xok = shared(a[j], a[k]);
                if (xok.second) {
                    for (int w = 0; w < n; w ++) {
                        if (sq(a[w] + xok.first)) {
                            count ++;
                        }
                    }
                }
                max_count = max_count > count ? max_count : count;
            }
        }
        cout << max_count << endl;
    }
}