#include <iostream>
#include <math.h>
#include <assert.h>
#include <vector>
using namespace std;
// From problem https://leetcode.com/problems/interleaving-string/description/

class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        // O(S_1*S_2) with 2D dynamic programming. We don't need to keep track of the index, k, because
        // we know that the sizes force us to pick certain combinations of letters
        bool interleaving[s1.size()+1][s2.size()+1];    
        // If you have any strings left over obviously it is not possible
        for (int i = 0; i <= s1.size(); i++) {
            for (int j = 0; j <= s2.size(); j++) {
                interleaving[i][j] = false;
            }
        }
        // If you deplete the strings then it is possible
        interleaving[s1.size()][s2.size()] = true;

        // Otherwise iterate backwards
        for (int i = s1.size(); i >= 0; i--) {
            for (int j = s2.size(); j >= 0; j--) {
                // You could either take s1 or take s2
                int k = i + j;
                if (i < s1.size() && k < s3.size() && s1[i] == s3[k]) interleaving[i][j] = interleaving[i+1][j];
                if (j < s2.size() && k < s3.size() && s2[j] == s3[k]) {
                    interleaving[i][j] = interleaving[i][j] || interleaving[i][j+1];
                }
            }
        }
        return interleaving[0][0];
    }
};