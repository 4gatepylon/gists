#include <math.h>

class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        // Assume begins with no overlapping intervals
        // Four cases
        // 1. It does not overlap at all
        // 2. It overlaps in start only
        // 3. It overlaps in end only
        // 4. It overlaps in start and end

        // Different approaches for different cases
        //   Find last interval whose starting point is before or equal to our starting point
        //   Call that X
        //   1: insert at next and bubble up
        //   2: replace end of X
        //   3: find Y, the last element whose start occurs before the end
        //      and for all elements between succ(X) and Y, replace them with this start and that end
        //      (and then bubble down the rest of the list)
        //   4: replace the end of X, then while the next start is less than the end of X
        //      update the end of X to be the max of its end and the the next's end; then bubble
        //      down all the subsequence elements

        // Operationalizing
        // Input I
        // . Go Until X
        // If Overlaps X (X's end after I's start)
        //   replace X's end
        //   P = X
        //   L = succ(X)
        //   CONTINUE
        // Else
        //  Look at Y = succ(X)
        //    If Overlaps Y (Y's start before I's end)
        //      update Y's start to min(I's start, Y's start)
        //      update Y's end to max(I's end, Y's end)
        //      P = Y
        //      L = succ(Y)
        //      CONTINUE
        //    Else
        //      bubble
        //      RETURN
        // While P's end < L's start and L < last index
        //   P's end = max(P's end, L's end)
        //   L ++
        // P = succ(P)
        // While L < last index
        //   P = L
        //   P = succ(P)
        //   L = succ(L)
        // Pop everything after P
        vector<vector<int>> insertion;
        if (intervals.size() == 0) {
            insertion.push_back(newInterval);
            return insertion;
        }

        int start = newInterval[0];
        int end = newInterval[1];

        int x = 1;
        // Copy over the vector until we might have to modify anything
        while (x < intervals.size() && intervals[x][0] < start) {
            insertion.push_back(intervals[x-1]);
            x ++;
        }
        // Make sure we're pointing to the last element that starts before `start`
        x --;
        // Need to check that we did find such an index because it's possible that
        // x = 0 and the interval should have gone at the very front
        int Xstart = intervals[x][0];
        int Xend = intervals[x][1];
        bool found_insertion = Xstart < start;
        if (
            found_insertion &&
            (Xend >= start)
        ) {
            vector<int> newXVec{Xstart, max(Xend, end)};
            insertion.push_back(newXVec);
            // NOW X IS POINTING AT THE NEXT LOCATION
            x++;
        } else {
            // Make sure to add X since it was not overlapped
            if (found_insertion) {
                insertion.push_back(intervals[x]);
            }
            // Go to the next element
            x = found_insertion ? x + 1 : 0;
            // If there is next element we are done
            if (x >= intervals.size()) {
                insertion.push_back(newInterval);
                return insertion;
            }
            // Else we have to do the whole thing
            if (end >= intervals[x][0]) {
                int Ystart = intervals[x][0];
                int Yend = intervals[x][1];
                vector<int> newYVec{min(Ystart, start), max(Yend, end)};
                insertion.push_back(newYVec);
                // NOW X IS POINTING AT THE NEXT LOCATION
                x++;
            } else {
                insertion.push_back(newInterval);
                while (x < intervals.size()) {
                    insertion.push_back(intervals[x]);
                    x ++;
                }
                return insertion;
            }
        }
        // Eat as much as possible into P
        int Pend = insertion[insertion.size() - 1][1];
        while (x < intervals.size() && Pend >= intervals[x][0]) {
            Pend = max(Pend, intervals[x][1]);
            insertion[insertion.size() - 1][1] = Pend;
            x ++;
        }
        while (x < intervals.size()) {
            insertion.push_back(intervals[x]);
            x ++;
        }
        return insertion;
    }
};