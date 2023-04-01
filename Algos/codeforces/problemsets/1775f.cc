#include <iostream>
#include <string>
#include <algorithm>
#include "assert.h"

using namespace std;

// NOT SURE IF THIS IS RIGHT LOL

// Compile with
/*
g++ 1775f.cc && cat 1775f_1.txt | ./a.out && echo "\n" && cat 1775f_2.txt | ./a.out
*/

// wrong answer on test 2 kms
// something something dp? https://zhuanlan.zhihu.com/p/598748332
// could I be missing optimal solutions?

pair<int, int> max_sq_below(int n) {
    assert(n >= 1);

    int lo = 1;
    int hi = n / 2 + 1;
    while (lo < hi - 1) {
        int mid = lo + (hi - lo) / 2;
        if (mid * mid <= n) lo = mid;
        else                hi = mid;
    }
    assert(lo * lo <= n && n < (lo + 1) * (lo + 1));
    return pair<int, int>(lo, n - lo * lo);
}

// Greedily create a maximal configuration by appending to a rectangle in two slabs
void cout_maximal(int n) {
    // print out the number of rows and columns then the rows and columns with
    // `#` where there is a house and `.` where there is not
    pair<int, int> side_rem = max_sq_below(n);
    int side = side_rem.first;
    int rem = side_rem.second;
    assert(0 <= side);
    assert(0 <= rem && rem <= 2 * side);
    
    int add1 = min(rem, side);
    int add2 = 0;
    if (rem > side) { add2 = rem - add1; }

    assert(0 <= add1 && add1 <= side);
    assert(0 <= add2 && add2 <= side);

    int rows = side;
    if (add1 > 0) rows ++;
    if (add2 > 0) {
        assert(add1 > 0 && add1 == side);
        rows ++;
    }
    int cols = side;
    cout << rows << " " << cols << endl;

    for (int r = 0; r < side; r++) {
        for (int c = 0; c < side; c++) {
            cout << "#";
        }
        cout << endl;
    }
    if (add1 > 0) {
        #define num_startends 4
        int starts[num_startends] = {0, add1, 0, add2};
        int ends[num_startends] = {add1, side, add2, side};
        bool finished = false;
        for (int i = 0; i < num_startends; i++) {
            int start = starts[i];
            int end = ends[i];

            // Head is #, then tail is .
            bool head = (start == 0);
            // If the row is headless then there is no row
            finished = (finished || (head && start == end));

            // Only print out the row if it exists
            if (!finished) {
                for (int c = start; c < end; c++) {
                    cout << (head ? "#" : ".");
                }
                if (!head) { cout << endl; }
            }
        }
    }
}


int maximal_count(int n, int mod) {
    int count = 0;

    pair<int, int> siderem = max_sq_below(n);
    int side = siderem.first; 
    int rem = siderem.second;
    assert(rem >= 0);

    // Squares have only one way to be oriented
    if (rem == 0) {
        return 1;
    } else if (rem < side) {
        // If rem <= side then we need to place the slab on one of the four sides to be optimal
        return (4 * (side - rem + 1)) % mod;
    } else if (rem == side) {
        // As before, but this case is more unique (here it is a rectangle, and this is its optimizing
        // rectangle) becuase of symmetry
        return 2;
    } else {
        assert(rem > 1);
        assert(rem > side);
        assert(rem <= 2 * side);

        // Stack: necessarily two slab additions to the same side, both must be positive, and this should
        // only be counted in the case where we are forced to stack (since we prefer to add only slabs
        for (int bottom_stack = rem / 2 + (rem % 2 == 1 ? 1 : 0); bottom_stack <= side; bottom_stack++) {
            int top_stack = rem - bottom_stack;

            assert(bottom_stack >= 1);
            assert(top_stack >= 1);
            assert(top_stack + bottom_stack == rem && top_stack <= bottom_stack && bottom_stack <= side);

            // By translational symmetry there are only two axes for this case
            if (top_stack == bottom_stack && bottom_stack == side) { count = (count + 2) % mod; }

            // Otherwise all four sides are open (and will yield to a different shape); we simply need to decide
            // how to place the bottom and for each of those how to place the top
            else {
                int bottom_placements = (side - bottom_stack + 1);
                int top_placements = (bottom_stack - top_stack + 1);
                count = (count + 4 * bottom_placements * top_placements) % mod;
            }
        }
        // cout << "count after stack: " << count << endl; // XXX

        // Pair: necessarily two slab additions to different sides and neither can be empty
        for (int top = side; rem - top <= top; top--) {
            int right = rem - top;
            // cout << "top " << top << " and " << right << endl; // XXX

            assert(top >= 1);
            assert(right <= top);

            // In this case avoid counting opposite axes (stackings) as well as opposite orderings (symmetric)
            if (right == top && top == side) { count = (count + 4) % mod; } 
            else {
                int top_placements = (side - top + 1);
                int right_placements = (side - right + 1);

                // If right is equal to side we must avoid being counted as a stack
                if (top == side) { count = (count + 4 * 2 * right_placements) % mod; }
                else { count = (count + 4 * 3 * top_placements * right_placements) % mod; }

                // There is a really annoying symmetry case here where you'll get a tooth basically
                // For every side we added this twice, so for each one we must remove it once
                if (top == side && side % 2 == 0 && right % 2 == 1) { count = (count - 4 * 1) % mod; }
                
                // In this case we must avoid double-counting symmetric cases where along
                // the same axis the two moved in unison
                if (right == top) {
                    // There are only 2 cases but we counted four (one out of each four above)
                    count = (count - 2 * top_placements) % mod;
                } 
            }
        }

        // cout << "count after pair: " << count << endl; // XXX

        // Corner: necessarily one corner slab addition to one corner, both must be positive
        // we must account for flips as well as all four corners with the exception of special cases
        // like when the corner is identical (we actually don't need to deal with flips cuz we just iterate more)
        if (rem >= 3) {
            // NOTE: TOP WILL BE OVERHANGING and there is no reason to do corner with under three rem
            // (or under side, though that should already be guaranteed), so think of this like top is placed and
            // shifted left, then left is placed as far up as possible sliding into the hole (under the overhang)

            // NOTE: you could do this in O(1) but ehhhhh
            assert(rem > side);
            for (int top = 2; top < side + 1; top++) {
                int left = rem - top;
                assert(left >= 1);

                // Special case where the corner looks like the square so the two diagonal axes are all that matters
                if (top - 1 == left && top == side) { count = (count + 2) % mod; }
                // In this other case you can just rotate for each corner and do the same thing
                else { count = (count + 4) % mod; }
            }
        }

        // cout << "count after corner: " << count << endl; // XXX
    }

    return count;
}

// It seems like cin ignores `\n` and tokenizes inputs and then just transforms them into ints returning zero if not an int
// (after which it might return some trash like -1)
int main() {
    int num_tests = -1, type= -1;
    cin >> num_tests;
    cin >> type;
    assert(num_tests >= 0);
    assert(type == 1 || type == 2);

    if (type == 1) {
        for (int i = 0; i < num_tests; i++) {
            int n = -1;
            cin >> n;
            cout_maximal(n);
        }
    } else {
        int mod = -1;
        cin >> mod;
        assert(mod >= -1);
        for (int i = 0; i < num_tests; i++) {
            int n = -1;
            cin >> n;
            cout << maximal_count(n, mod) << endl;
        }
    }
}