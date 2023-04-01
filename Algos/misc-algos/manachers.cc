#include <vector>
#include <iostream>

using namespace std;

// For each index, add the stop locations of ALL the palindromes that start at it
// in linear time. Run manacher to find the centers in O(n) and then go through the centers
// to fill in the start/stops.
vector<int> manacher(string _input) {
    vector<char> input(2*_input.size() + 1);
    // We use '\0' as a special character to make sure that all centers are
    // singletons and not pairs.
    #define SPECIAL '_' //'\0'
    input[0] = SPECIAL;
    for (int i = 1; i < input.size(); i += 2) {
        input[i] = _input[i/2];
        input[i+1] = SPECIAL;
    }
    // Rads will store the radii sizes of the longest palindrome centered
    // at each index. NOTE, when reading this, it tells you how many indices to
    // go to either side to reach an index that is NOT in the palindrome
    vector<int> rads(input.size(), 1);

    // Build both using Manacher's
    for (int center = 0; center < input.size();) {
        // Start off from wherever we left off
        int R = rads[center];
        // Expand the radius and update it
        while (0 <= center - R && center + R < input.size() && input[center - R] == input[center + R]) { R ++; }
        rads[center] = R;

        // Clone DP of mirrored points and if necessary terminate early by
        // re-commencing at the place we weren't able to finish from
        // NOTE: if we are able to cleanly clone everything, there is no need
        // to go to any element under the `wing` let's call it of this palindrome
        int next_center = center + R;
        // p_r is the displacement to the right inside this palindrome
        for (int p_r = 1; p_r < R; p_r ++) {
            // p is the thing we are not pointing to/focusing on (going to the right) and it will
            // copy from the left using the property of palindromes
            int p = center + p_r;
            // m is he `mirror` of p and it is at the same distance from the center, but below
            int m = 2 * center - p;
            // m_r is the radius of the largest palindrome at m
            int m_r = rads[m];
            // In the case of equality you can show that it would be contradictory for 
            if (m_r <= R - p_r) {
                rads[p] = m_r;
            } else {
                // If the radius is bigger we set it and want to recommence from that location
                rads[p] = R - p_r;
                next_center = p;
                // Break out of the for loop inside
                break;
            }
        }
        center = next_center;
    }
    // It's kinda fucked because its on the wrong string
    return rads;
}

int main() {
    manacher("abcdaba");
    return 0;
}