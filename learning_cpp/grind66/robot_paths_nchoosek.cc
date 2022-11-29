class Solution {
private:
    int choose(int m, int n) {
        if (n == 0) return 1;
        if (m == 0 && n > 0) return 0;
        if (n > m) return 0;
        
        // n is the bigger half
        if (n < m / 2) n = m - n;
        int acc = 1;
        int k = m;
        int d = 2;
        while (k > n) {
            acc *= k;
            while (d <= m - n && acc % d == 0) {
                acc /= d;
                d ++;
            }
            k --;
        }
        return acc;
    }
public:
    int uniquePaths(int m, int n) {
        m --;
        n --;
        return choose(m + n, m);
    }
};