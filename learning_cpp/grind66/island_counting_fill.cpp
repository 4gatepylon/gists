// Cooler with some sort of edge detection :/
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.size() <= 0) {
            return 0;
        }
        int count = 0;
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                if (grid[i][j] == '1') {
                    fill(grid, i, j);
                    count ++;
                }
            }
        }
        return count;
    }
    void fill(vector<vector<char>>& grid, int i, int j) {
        if (grid[i][j] == '2') {
            return;
        }
        grid[i][j] = '2';
        for (int d = 0; d < 2; d++) {
            for (int k = -1; k < 2; k += 2) {
                int ip = d == 0 ? i + k : i;
                int jp = d == 1 ? j + k  : j;
                if (0 <= ip && ip < grid.size() && 0 <= jp && jp < grid[0].size() && grid[ip][jp] == '1') {
                    fill(grid, ip, jp);
                }
            }
        }
    }
};