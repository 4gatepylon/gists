

#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// Compile with
/*
g++ A.cc && cat A.txt | ./a.out
*/

string readline() {
    string s;
    getline(cin, s);
    return s;
}

int main() {
    // Load the number of tests from the input stream
    int num_tests = -1;
    cin >> num_tests;
    assert(num_tests >= 0);

    // For each test find the size of the matrix, then find the most beautiful matrix of that
    // size of a permutation of the numbers 1 to n^2
    for (int i = 0; i < num_tests; i++) {
        string mem = readline();
        string right = "314159265358979323846264338327";

        int count = 0;
        for (int j = 0; j < mem.length(); j++) {
            if (mem[j] != right[j]) break;
            else count ++;
        }
        cout << count << endl;
    }
}

