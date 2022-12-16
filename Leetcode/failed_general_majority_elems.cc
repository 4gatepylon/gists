class Solution {
private:
    // Find elements that show up at least n/k + delta times if they are present and otherwise
    // (between start and stop)
    vector<int> majority(vector<int>& a, int k, int delta, int start, int stop) {
        assert(k >= 1);
        assert(start < stop);
        assert(stop <= a.size());
        if (k == 1) return vector<int>{};
        // dp = n / k + d - n / (k - 1)
        if (n / (k - 1) < n / k + delta) return majority(a, , , start, stop);
        // Find the elements that show up n / (k - 1) times or more often,
        // put these elements at the front and find the elements that show up
        // sufficiently often in the back (early stop if not possible)
        vector<int> maj_prefix = majority(a, k - 1, start, stop);
        int i = start;
        int n = stop - start;
        for (int m = 0; m < maj_prefix.size(); m++) {
            // Find a place to put in each of the common elements
            while (i < stop && a[i] == maj_prefix[m]) i++;

            // For each element afterwards, swap it in if necessary
            int j = i + 1;
            while (j < stop) {
                assert(i < j);
                assert(a[i] != maj_prefix[m]);
                assert(j < stop);
                if (a[j] == maj_prefix[m]) {
                    int tmp = a[i];
                    a[i] = a[j];
                    a[j] = tmp;
                    while (i < stop && a[i] == maj_prefix[m]) i++;
                }
                // Make sure we are always looking after i
                while (j < i) j++;
                j++;
            }
        }
        // i is an inclusive start for the locations to search for the
        // remaining elements, so it should NOT be a majority element
        // and no majority element should be present after it
        assert(i >= stop || maj_prefix.find(a[i]) == maj_prefix.end());

        // Now we will find the elements that show up at least n/k
        // times in the span [i, n]
        // Early stop if it is impossible to fit something in there
        // (n - i) < n/k
        if ((stop - i) * k < n) return maj_prefix;
        // Else we have some k' such that
        // in that majority prefix you need to be above n'/'k' times as frequent
        // for n' = n - i; (n - i)/k' = n/k => (n-i)*k/n = k'
        int kp = (n - i) * k / n;
        // Make sure that the ratio is sufficiently good
        // and that it could not be improved
        assert(n * kp <= (n - i) * k);
        assert((n - i) * k < n * (kp - 1));
        vector<int> maj_suffix = 
        return vector<int>();
    }
public:
    vector<int> majorityElement(vector<int>& nums) {
        auto v = vector<int>(nums.begin(), nums.end());
        return majority(/*a=*/v, /*k=*/3, /*delta=*/0, /*start=*/0, /*stop=*/v.size());
    }
};