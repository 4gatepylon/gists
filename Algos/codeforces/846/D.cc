#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;
// Compile with
/*
g++ D.cc && cat D.txt | ./a.out
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
        // TODO get standard in and return the answer
    }
    return 0;
}