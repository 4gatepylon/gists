class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        #define NUM_NUMS 9
        #define NUM_ROWS 9
        #define NUM_COLS 9
        bool present[NUM_NUMS + 1];
        
        // vertical, horizontal, square
        int x_start_stride[] = {1, 9, 3};
        int y_start_stride[] = {9, 1, 3};
        int x_bound[] = {1, 9, 3};
        int y_bound[] = {9, 1, 3};
        
        // Fill the frequency map
        for (int i = 0; i < NUM_NUMS + 1; i++) { present[i] = false; }
        // You know how it is
        for (int k = 0; k < 3; k++) {
            for (int x_start = 0; x_start < NUM_ROWS; x_start += x_start_stride[k]) {
                for (int y_start = 0; y_start < NUM_COLS; y_start += y_start_stride[k]) {
                    // For this start, calculate the inclusion in the box
                    for (int x = x_start; x < x_start + x_bound[k]; x++) {
                        for (int y = y_start; y < y_start + y_bound[k]; y++) {
                            if ('0' <= board[y][x] && board[y][x] <= '9') {
                                int c = board[y][x] - '0';
                                if (present[c]) return false;
                                else                  present[c] = true;
                            }
                        }
                    }
                    // Empty the frequency map
                    for (int i = 0; i < NUM_NUMS + 1; i++) { present[i] = false; }
                }
            }
        }
        return true;
    }
};