#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;
// Compile with
/*
g++ A.cc && cat A.txt | ./a.out
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
        assert(n != -1);
        if (n % 2 == 0) { cout << 1 << " " << n / 2 << endl; }
        else { cout << -1 << endl; }
    }
    return 0;
}