class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int height = matrix.size();
        int width = matrix[0].size();
        vector<vector<int>> dp(height + 1, vector<int>(width, 0));
        for (int row = height - 1; row >= 0; row--) {
            dp[row][0] = matrix[row][0] + dp[row + 1][0];
            dp[row][width - 1] = matrix[row][width - 1] + dp[row + 1][width - 1];
            if (width > 1) {
                dp[row][0] = min(dp[row][0], matrix[row][0] + dp[row + 1][1]);
                dp[row][width - 1] = min(dp[row][width - 1],  matrix[row][width - 1] + dp[row + 1][width - 2]);

            }
            for (int col = 1; col < width - 1; col++) {
                dp[row][col] = matrix[row][col] + min(min(dp[row + 1][col - 1], dp[row + 1][col + 1]), dp[row + 1][col]);
            }
        }
        return *min_element(dp[0].begin(), dp[0].end());
    }
};