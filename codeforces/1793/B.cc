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
        long x = -1000000000;
        cin >> x;
        assert(x != -1000000000);
        long y = 1000000000;
        cin >> y;
        assert(y != 1000000000);

        assert(x - y >= 1);
        long n = 2 + 2 * (x - y - 1);
        assert (n >= 2 && n == 2 * (x - y));
        // idt there is anything you can ever do here, the sum of the number is always x - y...
        // You can always shrink the size of the peaks but you can't shrink the length of this array
        cout << n << endl;
        for (long j = x; j > y; j--) {
            cout << j << " ";
            n --;
        }
        for (long j = y; j < x - 1; j++) {
            cout << j << " ";
            n --;
        }
        cout << x - 1 << endl;
        n --;
        assert(n == 0);
    }
    return 0;
}