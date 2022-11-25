// Runtime O(nlog(n)) without using division
// Space O(n)
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> output(nums.size(), 1);
        pes(nums, output, 0, nums.size());
        return output;
    }
    void pes(vector<int>& in, vector<int>& out, int i, int j) {
        if (j == i + 1) {
            return;
        }
        else {
            // Pick a pivot
            int m = (j + i) / 2;
            // Left and right products (to the left and right of the pivot)
            int lp = 1;
            int rp = 1;
            for (int k = i; k < m; k++) { lp *= in[k]; }
            for (int k = m; k < j; k++) { rp *= in[k]; }

            // Left must be given right product and vice versa
            for (int k = i; k < m; k++) { out[k] *= rp; }
            for (int k = m; k < j; k++) { out[k] *= lp; }

            // Recurse on left and right half
            pes(in, out, i, m);
            pes(in, out, m, j);
        }
    }
};