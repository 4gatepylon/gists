#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ L.cc && cat L.txt | ./a.out
*/

string readline() {
    string s;
    getline(cin, s);
    return s;
}

pair<long, long> parse_pair(string s) {
    long a = 0;
    long b = 0;
    long i = 0;
    // parse the first number
    while (s[i] != ' ') {
        a = a * 10 + (s[i] - '0');
        i++;
    }
    // Skip the space
    i++;
    // Go until eos parsing second number
    while (i < s.size()) {
        b = b * 10 + (s[i] - '0');
        i++;
    }
    return make_pair(a, b);
}

int main() {
    // Load the number of tests from the input stream
    long n = stoi(readline());
    assert(n >= 0);
    // cout << "n: " << n << endl;

    long num_1s = 0l;
    long num_0s = -1l;
    long num_tot = 0l;
    string line = readline();
    // cout << "line is " << line << endl;
    for (long j = 0l; j < n; j++) {
        char c = line[j];
        assert(c == '+' || c == '-');
        if (c == '+') { num_1s ++; }
        num_tot ++;

    }
    num_0s = num_tot - num_1s; // minus 1s, yea it's misleading
    assert(num_0s >= 0l);
    // cout << "num_1s: " << num_1s << endl;
    // cout << "num_0s: " << num_0s << endl;
    // cout << "num_tot: " << num_tot << endl;

    long q = stoi(readline());
    // cout << "q: " << q << endl;
    assert(q >= 0l);
    for (long j = 0l; j < q; j++) {
        string _ab_str = readline();
        pair<long, long> _ab = parse_pair(_ab_str);
        long _a = _ab.first; long _b = _ab.second;
        assert(_a >= 0l && _b >= 0l);
        long a = max(_a, _b);
        long b = min(_a, _b);
        // cout << "a is " << a << " and cb is " << b << endl;
        long default_sum = a * num_1s - b * num_0s;
        // assert(default_sum == max(a * num_1s + b * num_0s, b * num_1s + a * num_0s));
        if (default_sum < 0) {
            // cout << "default sum was negative: " << default_sum << endl;
            // If the maximal ones are not enough to change the sign ever then we cannot win
            cout << "NO" << endl;
        } else {
            if (a == b) {
                // cout << "a == b\n";
                if (num_0s == num_1s) {
                    cout << "YES" << endl;
                } else {
                    cout << "NO" << endl;
                }
            } else {
                assert(a > b);
                // cout << "default_sum is " << default_sum << " and it is divisibly by " << a + b << " : " << (default_sum % (a + b) == 0) << endl; 
                if (
                    default_sum == 0l || 
                    (
                        (default_sum % (a - b) == 0l)
                    )
                ) {
                    // Swap until you can't and need to do a single change
                    int changes_needed = default_sum / (a - b);
                    int swaps_needed = changes_needed / 2;
                    int moves_needed = changes_needed % 2 == 0 ? 0 : 1;
                    // By symmetry assume that shared_per is all of +'s and that the leftovers are all -'s
                    int shared_per = min(num_1s, num_0s);
                    int leftover_tot = num_tot - 2 * shared_per;
                    // Assume that we are moving out of shared_per, we need to be able to swap swaps_needed
                    // and to move moves_needed (so we need swaps_needed + moves_needed <= shared_per)
                    // ... also we need swaps_needed <= leftover_tot + shared_per (but that is implied)
                    // cout << "changes_needed: " << changes_needed << endl;
                    // cout << "swaps_needed: " << swaps_needed << endl;
                    // cout << "moves_needed: " << moves_needed << endl;
                    // cout << "shared_per: " << shared_per << endl;
                    // cout << "leftover_tot: " << leftover_tot << endl;
                    // cout << "a is " << a << " and b is " << b << " and a - b = " << a - b << endl;
                    // cout << "default sum is " << num_1s << " * " << a << " - " << num_0s << " * " << b << " = " << default_sum << endl;

                    if (swaps_needed + moves_needed <= shared_per) {
                        cout << "YES" << endl;
                    } else {
                        cout << "NO" << endl;
                    }
                } else {
                    cout << "NO" << endl;
                }   
            }
        }
        // cout << "*********************\n";
    }
    return 0;
}