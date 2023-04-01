// Another better solution might be to
// multiply everything on the left by everything on the right and vice versa
// then recurse... but also O(nlogn)?

// O(log(n)) extra space solution, can probably be made to use O(1)
// Its runtime, I think, is O(nlogn)
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        // Our solver is recursive
        pes(nums, 0, nums.size());
        return nums;
    }
    // Rot is a utility function we use when solving as you will see below
    void rot(vector<int>& nums, int i, int j) {
        int last = nums[j];
        for (int k = 1; k < j; k++) {
            nums[k] = nums[k-1];
        }
        nums[i] = last;
    }
    // Make sure that the elements between i and j are all the product of all the elements except
    // themselves.
    void pes(vector<int>& a, int i, int j) {
        if (j <= i) {
            return;
        }
        // This base-case is OK because we will have gotten that value in
        // the caller.
        if (j == i + 1) {
            a[i] = 1;
            return;
        }
        // We deal with this case because it does not fit nicely with our search for m
        // (we assume later that there is an element at index m) and also we don't want
        // to use the base case from before
        if (j == i + 2) {
            int tmp = a[i];
            a[i] = a[i+1];
            a[i+1] = tmp;
        }

        // m will be a power of 2 plus one representing the index to which we can use our
        // "duplicating" strategy, also called the loop strategy. That is because basically
        // we will just grow the number of elements to the left of our element exponentially,
        // so we will get 2^alpha for some alpha elements and we have one more element for the
        // one we start from
        int m = 2;
        int s = 0;
        while (i + (2*m-1) < j) {
            m *= 2;
            m --;

            // s is the logarithm of (m - 1)
            s ++;
        }

        // First, solve to the right and then insert the joint multiple of all the
        // elements into all the elements on the left using the properties of the solution
        int aim = a[i + m];
        pes(a, i + m, j);
        for (int k = i; k < i + m; k++) {
            a[k] *= aim;
        }

        // We can then finish by simply solving the left using our loop (because
        // the structure of the problem is such that if we multiply everything on one
        // side by everything on the other, solving it is basically the same, since
        // we wanted to include everything anyways)
        pesloop(a, i, i + m, s);
    }
    // This is O(nlogn)... maybe there's a better solution?
    void pesloop(vector<int>& a, int i, int j, int s) {
        // s is the power of 2 

        // The way we solve is
        // 1. Make every element the element to its left
        // 2. Every element is multipled by the element to its left
        // 3. Every element is multiplied by the element twice to its left
        // 4. four times to its left and so on
        // ...
        // Up until 2^s times
        rot(a, i, j);
        int shift = 1;
        for (int k = 0; k < s; k++) {
            for (int x = i; x < j; x ++) {
                int y = x - shift;
                if (y < 0) {
                    y = j + y;
                }
                a[x] *= a[y];
            }
            shift *= 2;
        }
    }
};