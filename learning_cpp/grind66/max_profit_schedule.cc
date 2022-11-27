class Segment {
public:
    Segment(int start, int end, int profit) { this->start = start; this->end = end; this->profit = profit; }
    int start;
    int end;
    int profit;
};
class Solution {
private:
    int first_element_starting_after_end(vector<Segment*>& segs, int idx) {
        if (idx >= segs.size() - 1 || segs[segs.size() - 1]->start < segs[idx]->end) return segs.size();
        int end = segs[idx]->end;
        // Binary search exclusive of left and inclusive of right
        int left = idx;
        int right = segs.size() - 1;
        while (right < segs.size() && right - left > 1) {
            int mid = right - (right - left) / 2;
            if (segs[mid]->start < end) left = mid;
            else right = mid;
        }
        return right;
    }
    // Find the best solution only using segments to the right of idx
    int dp(unordered_map<int,int>& sols, vector<Segment*>& segs, int idx) {
        // This base case is OK because we are guaranteed at least 1 start/end time w/ profit
        if (idx >= segs.size()) return 0;
        else if (sols.find(idx) != sols.end()) return sols[idx];
        else {
            // If we don't take
            int sol = dp(sols, segs, idx + 1);
            // If we take (using binary search)
            int next_idx = first_element_starting_after_end(segs, idx);
            // Some debug...
            assert(next_idx > idx);
            assert(next_idx >= segs.size() || segs[next_idx]->start >= segs[idx]->end);
            sol = max(sol, segs[idx]->profit + dp(sols, segs, next_idx));
            // Cache
            sols[idx] = sol;
            return sol;
        }        
    }
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        // Each segment is either taken or not
        // Max profit at a point is max profit if you take it or not to the right
        // You can compute max profit if you take in terms of the point after the end
        // You can compute max profit if you don't take in terms of the next point
        vector<Segment*> segs(startTime.size(), nullptr);
        for (int i = 0; i < startTime.size(); i++) { segs[i] = new Segment(startTime[i], endTime[i], profit[i]); }
        sort(segs.begin(), segs.end(), [](Segment* a, Segment* b) { return a->start < b->start; });
        // Map idx -> max profit to the right of it
        unordered_map<int, int> sols;
        dp(sols, segs, 0);
        return sols[0];
    }
};