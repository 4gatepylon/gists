class Solution {
private:
    int memo[20]; 
    int nt(int n) {
        assert(0 <= n && n <= 19);
        if (n == 0) return 1;
        if (memo[n] != -1) return memo[n];
        // For each center, the number of unique binary search trees on the prefix and then the
        // number of unique binary trees on the suffix
        int sum = 0;
        for (int left = 0; left < n; left++) {
            sum += nt(left)*nt(n - left - 1);
        }
        memo[n] = sum;
        assert(memo[n] != -1);
        return sum;
    }
public:
    int numTrees(int n) {
        for (int i = 0; i < 20; i++) memo[i] = -1;
        return nt(n);
    }
};