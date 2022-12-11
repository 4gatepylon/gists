class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        // If the first integer of the row is larger than our target, then it is impossible
        // for the target to be below. No guarantees w.r.t. last integer
        int row_lo = 0;
        int row_hi = matrix.size();
        while (row_hi - row_lo > 1) {
            int row_mid = row_lo + (row_hi - row_lo) / 2;
            if (matrix[row_mid][0] > target) row_hi = row_mid;
            else                             row_lo = row_mid;
        }
        // We can still find it below because the sequences could grow at differing
        // rates
        int lowest_possible_row = 0;
        int highest_possible_row = row_lo;
        for (int row = lowest_possible_row; row <= highest_possible_row; row++) {
            int col_lo = 0;
            int col_hi = matrix[row].size();
            while (col_hi - col_lo > 1) {
                int col_mid = col_lo + (col_hi - col_lo) / 2;
                if (matrix[row][col_mid] > target) col_hi = col_mid;
                else                               col_lo = col_mid;
            }
            if (matrix[row][col_lo] == target) return true;
        }
        return false;
    }
};