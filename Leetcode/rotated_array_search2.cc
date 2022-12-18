#include <vector>

using namespace std;

// Return whether an element exists in a range or not
static inline bool binary_search(vector<int>& A, int i, int j, int T) {
    assert(0 <= i && i < j && j <= A.size());
    while (j - i > 1) {
        int m = i + (j - i) / 2;
        if (A[m] == T)     return true;
        else if (A[m] < T) i = m + 1;
        else               j = m;
    }
    if (i < A.size()) return A[i] == T;
    return false;
}

// In upper vs. in lower
// Returns whether we can definately know (if we can't know, we can't know and that's that)
static inline bool in_U(vector<int>& A,  int i, int j, int m) { return A[i] < A[m] || A[i] == A[m] && A[j-1] < A[i]; }
static inline bool in_L(vector<int>& A,  int i, int j, int m) { return A[i] > A[m] || A[i] == A[m] && A[i] < A[i+1]; }

static inline bool rotated_search(vector<int>& A, int T) {
    int i = 0, j = A.size();
    while (j - i > 1) {
        // If we've fallen into one of the two halves just terminate with a BS
        if (A[i] < A[j-1]) return binary_search(A, i, j, T);

        int m = i + (j - i) / 2;
        assert(0 <= i && i < m && m < j && j <= A.size());

        bool m_in_u = in_U(A, i, j, m);
        bool m_in_l = in_L(A, i, j, m);
        if (m_in_u && (A[i] <= T && T <= A[m]))        return binary_search(A, i, m + 1, T);
        else if (m_in_l && (A[m] <= T && T <= A[j-1])) return binary_search(A, m, j, T);
        else if (m_in_u)                               i = m + 1;
        else if (m_in_l)                               j = m;
        else {
            assert(!m_in_u && !m_in_l);
            assert(A[i] == A[m]);

            if (T == A[i]) return true;
            else if (T == A[j-1]) return true;
            else {
                i ++;
                j --;
            }   
        }
    }
    if (i < A.size()) return A[i] == T;
    return false;
}

class Solution {
public:
    bool search(vector<int>& nums, int target) {
        if (nums.size() == 0) return false;
        if (nums.size() == 1) return nums[0] == target;
        return rotated_search(nums, target);
    }
};