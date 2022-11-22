// To make this faster you could use some sort of dp to get the best array from the middle
typedef struct {
    int i;
    int j;
    int s;
} RangeStruct;

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        // Max Subarray is such that adding any elements to the left or right
        // would make it worse. That means the suffix and prefix sums to the right and left
        // are negative
        
        // One way in nlog(n) would be to find the max subarray of the left and right halves of
        // the vector. Then to find the max subarray that includes the middle (which is naturally the maxsubarray
        // ending at the middle on the left appending the max subarray starting from the middle on the right,
        // such that first we find the case where neither is empty, but we may drop one of the two halves).
        RangeStruct range = mse(nums, 0, nums.size());
        return range.s;
    }

    RangeStruct mse(vector<int>& a, int i, int j) {
        // Base case when the size of the array is one
        if (i == j - 1) { 
            return RangeStruct{
                i: i,
                j: j,
                s: a[i]
            };
        }
        // Get the middle index

        int m = (i + j) >> 1;
        // Get the best possible subarray on the left and on the right
        RangeStruct mse_left = mse(a, i, m);
        RangeStruct mse_right = mse(a, m, j);

        // Find the best subarrays that go up to or down from the middle so that later
        // we can find the best subarray including it
        RangeStruct mse_left_end_mid = mse_left.j == m ? mse_left : mse_to(a, i, m);
        RangeStruct mse_right_start_mid = mse_right.i == m ? mse_right : mse_from(a, m, j);

        // Note this is all by value... basically get the best possible subarray
        // including the middle
        RangeStruct mse_mid = mse_left_end_mid;
        if (mse_left_end_mid.s < 0 && mse_right_start_mid.s > mse_left_end_mid.s) {
            mse_mid = mse_right_start_mid;
        } else if (mse_right_start_mid.s >= 0) {
            mse_mid.j = mse_right_start_mid.j;
            mse_mid.s += mse_right_start_mid.s;
        }
        
        // Return the max mse
        if (mse_mid.s >= mse_left.s && mse_mid.s >= mse_right.s) {
            return mse_mid;
        } else if (mse_left.s > mse_right.s) {
            return mse_left;
        } else {
            return mse_right;
        }
    }
    RangeStruct mse_to(vector<int>& a, int i, int j) {
        RangeStruct r = RangeStruct{
            i: j - 1,
            j: j,
            s: a[j - 1]
        };
        int s = 0;
        for (int k = j - 1; k >= i; k--) {
            s += a[k];
            if (s >= r.s) {
                r.i = k;
                r.s = s;
            }
        }
        return r;
    }
    RangeStruct  mse_from(vector<int>& a, int i, int j) {
        RangeStruct r = RangeStruct{
            i: i,
            j: i + 1,
            s: a[i]
        };
        int s = 0;
        for (int k = i; k < j; k++) {
            s += a[k];
            if (s >= r.s) {
                r.j = k + 1;
                r.s = s;
            }
        }
        return r;
    }
};