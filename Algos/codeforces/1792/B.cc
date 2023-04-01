#include <iostream>
#include <algorithm>
#include <cassert>
#include <math.h>

using namespace std;

// Compile with
/*
g++ B.cc && cat B.txt | ./a.out
*/


// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them longo longs returning zero if not an long
// (after which it might return some trash like -1)
int main() {
    // Load the number of tests from the input stream
    long num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    for (long i = 0; i < num_tests; i++) {
        long a1 = -1, a2 = -1, a3 = -1, a4 = -1;
        cin >> a1 >> a2 >> a3 >> a4;
        assert(a1 >= 0 && a2 >= 0 && a3 >= 0 && a4 >= 0);
        
        if (a1 > 0) {
            long jokes = 0;
            jokes += a1;
            
            // After using this their health is a1
            jokes += 2 * min(a2, a3); // alternate between types 2 and 3, so 2 per cycle
            // After using this, their health is still a1

            // lower the health to zero then to a negative number for either one
            jokes += a1;
            jokes += 1;

            // make sure that we used a number of jokes that is valid
            jokes = min(jokes, a1 + a2 + a3 + a4);
            cout << jokes << "\n";
        } else {
            assert(a1 + a2 + a3 + a4 > 0);
            cout << "1\n";
        }
        
    }
    return 0;
}