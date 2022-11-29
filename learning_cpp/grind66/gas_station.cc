class Solution {
public:
    // You have to be unable to go from the node before the solution to
    // the solution since otherwise that would also be a solution
    // (and we heard that it's unique)
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        if (gas.size() < 1) return -1;
        if (gas.size() == 1) return cost[0] > gas[0] ? -1 : 0;
        int start = 0;
        int end = start + 1;
        int g = gas[start] - cost[start];
        bool looped = false;
        while (end != start) {
            // cout << start << " " << end << " " << g << " " << looped << "\n";
            if (g < 0) {
                start = end;
                // If we looped back around that means that we found no solution because of
                // the property elucidated above, so we cannot have anything
                if (looped) return -1;

                g = gas[start] - cost[start];
            } else {
                g += gas[end] - cost[end];
            }

            end ++;
            if (end == gas.size()) end = 0;
            if (end == 0) looped = true;
        }
        return g >= 0 ? start : -1;
    }
};