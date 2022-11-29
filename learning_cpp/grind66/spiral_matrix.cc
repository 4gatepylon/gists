class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> elements;
        int barrier_right = matrix[0].size();
        int barrier_left = -1;
        int barrier_bot = matrix.size();
        int barrier_top = -1;
        int dx = 1, dy = 0;
        int x = 0, y = 0;
        while (barrier_left < x && x < barrier_right && barrier_top < y && y < barrier_bot) {
            elements.push_back(matrix[y][x]);
            // If we cannot move, rotate
            if (x + dx <= barrier_left) {
                dx = 0;
                dy = -1;
                barrier_bot --;
            } else if (x + dx >= barrier_right) {
                dx = 0;
                dy = 1;
                barrier_top ++;
            } else if (y + dy <= barrier_top) {
                dx = 1;
                dy = 0;
                barrier_left ++;
            } else if (y + dy >= barrier_bot) {
                dx = -1;
                dy = 0;
                barrier_right --;
            }
            x += dx;
            y += dy;
        }
        return elements;
    }
};