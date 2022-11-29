class Solution {
private:
    // Return index of first element whose value comes after the value
    // (can return something whose value is == the value IFF can_eq)
    int first_after(vector<int> a, int val, bool can_eq) {
        if (a.size() == 0) return 1;
        // Inclusive of lo and exclusive of hi
        // Find last element equal to or below
        int lo = 0;
        int hi = a.size();
        while (hi > lo + 1) {
            int mid = lo + (hi - lo) / 2;
            // If we know that this is too big then
            // we should consider only up until this element
            if (a[mid] > val) hi = mid;
            else if (a[mid] < val) lo = mid;
            // If == value then we should consider this IFF can_eq
            else if (can_eq) hi = mid;
            else hi = mid;
        }
        if (a[lo] > val || (a[lo] == val && can_eq)) return lo;
        return lo + 1;
    }
public:
    // O(nlogn)
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.size() == 0) return vector<vector<int>>(0);

        // First sory by starting point and they may overlap in the beginning
        // Copy it to avoid breaking anything outside
        vector<vector<int>> ints_overlapping_at_points(intervals.begin(), intervals.end());
        sort(
            ints_overlapping_at_points.begin(),
            ints_overlapping_at_points.end(),
            [](vector<int>& a, vector<int>& b){
                return a[0] < b[0];
            });

        // Next, remove elements that are overlapping by not adding them...
        vector<vector<int>> ints;
        ints.push_back(ints_overlapping_at_points[0]);
        for (int i = 1; i < ints_overlapping_at_points.size(); i++) {
            vector<int> candidate = ints_overlapping_at_points[i];
            int curr_end = ints[ints.size() - 1][1];
            int cand_start = candidate[0];
            int cand_end = candidate[1];
            if (cand_start <= curr_end) ints[ints.size() - 1][1] = max(curr_end, cand_end);

            else ints.push_back(candidate);
        }
        return ints;
    }
};