// Basically a state machine... could be improved...
//
// One way to solve this would be to know the next index to the right
// and left s.t. that index is the last one in the decreasing 
class Solution {
private:
    tuple<int, int, int> next_minima(vector<int>& ratings, int i, int min_max) {
        // Assume that
        // 1. min_max is the minimum the max can have
        // 2. The next minima is to the right of i inclusive
        // Return
        // 1. The amount assigned assuming the next_minima starts at 1
        //    then backwards is 2, 3, and so on until the next element of ours
        //    (not that this is done here and not in the outside function because
        //     we want to greedily drop to 1 as fast as possible and that may require
        //     keeping track of when elements are equal). NOTE this assigns weight
        //     to EVERYTHING FROM I TO THE NEXT MINIMA INCLUSIVE OF BOTH.
        // 2. The index of the next element to see (one after the next minima)
        // 3. The value of the next minima + 1 (i.e. min_max afterwards)
        int next_min_i = i;
        // This finds a strictly decreasing subsequence
        for (int j = i; j < ratings.size(); j++) {
            if (j == ratings.size() - 1 || ratings[j + 1] >= ratings[j]) {
                next_min_i = j;
                break;
            } 
        }
        // Either we are at this element then we need to increase
        int next_min_max = min_max + 1;
        // Except if the next element is equal in which case it can be anything
        if (next_min_i == ratings.size() - 1 || ratings[next_min_i + 1] == ratings[next_min_i]) next_min_max = 1;
        // But if we are not at this element then we don't wnat to increase
        else if (next_min_i != i) next_min_max = 2;

        int d = next_min_i - i + 1;
        int assignment = d * (d + 1) / 2;
        if (d < min_max) assignment += (min_max - d);
        // cout << "FROM " << i << " TO " << next_min_i << " INCLUSIVE: " << assignment << "\n";
        return make_tuple(assignment, next_min_i + 1, next_min_max);
    }
public:
    int candy(vector<int>& ratings) {
        int total = 0;
        int min_max = 1;
        int i = 0;
        while (i < ratings.size()) {
            // Invariant is that ratings[i] is not yet visited
            // and it is assumed to be the beginning of a strictly
            // decreasing sequence of length at least 1
            tuple<int, int, int> nm = next_minima(ratings, i, min_max);
            total += get<0>(nm);
            i = get<1>(nm);
            min_max = get<2>(nm);
        }
        return total;
    }
};