#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using namespace std;
// Compile with
/*
g++ G.cc && cat G.txt | ./a.out
*/

string readline() {
    string s;
    getline(cin, s);
    return s;
}

// For any assignment I can grow any of the intervals to the left or right so long as those elements to the left or right
// are red and NOT white (AND the no other interval starts and ends at the exact same point)
// 
// If within any interval of size x whites there are yl reds on the left and yr reds on the right then we have available
// yl * yr total intervals that are not the same and that have x whites inside.
//
// Idea to find this:
// XXX The best possible answer of this type (since every critic needs to try at least n wines) is to look at each starting point
// XXX and look n ahead, then find the product of the number of reds on the left and on the right, which we can find using basically
// XXX a stack-like data structure beforehand in O(n) time for each side (making this whole thing O(n)).
// Correction: this can be improved, since you could potentially skip over some reds and whites to get to a longer interval.
//
// Assume we've compressed the data such that we keep track simply of run lengths.
// We use DP to find the longest run of reds AFTER this index because longest_runs[i] = longest_runs[i+1] if
// it is white and otherwise longest of(this, longest_runs[i+1]) if it is a red (where this is the length of this run:
// remember we've compressed it).
// After this, we can update the original array so that every real index knows its longest run to the right. We can do
// the same backwards to know the longest run to the left (going left). Note that we may want tot store a boolean array for
// each run to know if it, itself was the longest run, and we may want to store also the longest run to the right (that is disjoint
// from this run... damn its getting complicated!)
//
// Then, we can iterate for the array for all O(n) length n segments (we use a sliding window basically, and we just index
// it using the starting location) and we look at the longest run to the right and left of our window bounds and that helps us
// find the optimal windowing strategy that contains this window. If any of these windows work we have a solution.
//
// Question now is, is there always such a solution or do we sometimes need to scan (i.e. this just grows, but sometimes we may
// need windows that overlap and move around etc...)?

// Answer: this is not all encompasing, for example RWRWRWRWRW => you can achieve size 4 with a sliding window for each four
// RWRWRW______
// _WRWRWR_____
// __RWRWRW____
// ___WRWRWR___
// ____RWRWRW__
// ...
// But it is impossible with a growing window because 
// R, RW, WR, WRW => done for any given center!
// By symmetry same for larger centers
// So our idea has only taken us so far...

// Imagine we knew how many whites we wanted specifically (i.e. we knew x)... how could we check if x was valid?
// ... Imagine we just start picking arbitrary disjoint subsegments. At some point we need to start picking segments
// that overlap.
int main() {
    // Load the number of tests from the input stream
    int n = -1;
    cin >> n;
    assert(n >= 0);

    string wines = readline();
    assert(wins.size() == n);
    
    // O(n) is good enough for our purposes given the size of the input, but quadratic should not be good enough

    return 0;
}