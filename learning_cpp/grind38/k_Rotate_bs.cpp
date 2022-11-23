class Solution {
public:
    int search(vector<int>& nums, int target) {
        // Binary search for m where m is the index of nums[0]
        // We can do that by basically binary searching for the first point
        // i AFTER nums[0] such that nums[i] < nums[0] (since they are all like
        // that after)
        int m = 0;
        int m_hi = nums.size() - 1;
        int m_lo = -1;
        while (m_hi > m_lo + 1) {
            int m_mid = m_lo + (m_hi - m_lo) / 2;
            if (nums[m_mid] >= nums[0]) {
                m_lo = m_mid;
            } else {
                m_hi = m_mid;
            }
        }
        m = m_hi;

        int idx;
        if (m_hi == nums.size() -1 && nums.size() > 1 && nums[m_hi - 1] <= nums[m_hi]) {
            // This occurs when k = 0
            idx = bs(nums, 0, nums.size(), target);
        } else {
            // This occurs when k != 0
            idx = target >= nums[0] ? bs(nums, 0, m, target) : bs(nums, m, nums.size(), target);
        }
        return nums[idx] == target ? idx : -1;
    }

    int bs(vector<int>& a, int i, int j, int t) {
        while (j > i + 1) {
            int m = i + (j - i) / 2;
            if (a[m] <= t) {
                i = m;
            } else {
                j = m;
            }
        }
        return i;
    }
};