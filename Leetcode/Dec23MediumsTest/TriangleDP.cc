#include <iostream>
#include <math.h>
#include <assert.h>
#include <vector>
using namespace std;
// From problem
// https://leetcode.com/problems/triangle/description/

class Solution {
public:
    // O(n^2) = O(N) where N is the number of elements (n is the height/width of the triangle)
    int minimumTotal(vector<vector<int>>& triangle) {
        vector<vector<int>> memo;
        for (int row = 0; row < triangle.size() + 1; row++) memo.push_back(vector<int>(row + 1, 0));
        for (int row = triangle.size() - 1; row >= 0; row --) {
            assert(triangle[row].size() == 1 + row);
            assert(triangle[row].size() == memo[row].size());
            assert(memo[row+1].size() == memo[row].size() + 1);

            for (int col = 0; col < triangle[row].size(); col++){
                memo[row][col] = triangle[row][col] + min(memo[row+1][col], memo[row+1][col+1]);
            }
        }
        assert(memo[0].size() == 1);
        return memo[0][0];
    }
};