#include <iostream>
#include <math.h>
#include <assert.h>
#include <vector>
using namespace std;
// From https://leetcode.com/problems/rectangle-area/description/

typedef struct {
    // Bottom left corner
    int cx1;
    int cy1;
    // Top right corner
    int cx2;
    int cy2;
    // Whether or not it intersects at all
    bool intersects;
} Intersection;

class Solution {
private:
    static inline int rectArea(int ax1, int ay1, int ax2, int ay2) {
        assert(ax1 <= ax2);
        assert(ay1 <= ay2);
        return (ax2 - ax1) * (ay2 - ay1);
    }
    static inline Intersection intersects(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        Intersection intersect;
        intersect.intersects = false;
        if (
            (ax1 <= bx1 && bx1 <= ax2 || bx1 <= ax1 && ax1 <= bx2) && 
            (ay1 <= by1 && by1 <= ay2 || by1 <= ay1 && ay1 <= by2)
        ) {
            intersect.intersects = true;
            intersect.cx1 = max(ax1, bx1);
            intersect.cy1 = max(ay1, by1);
            intersect.cx2 = min(ax2, bx2);
            intersect.cy2 = min(ay2, by2);
            assert(intersect.cx1 <= intersect.cx2);
            assert(intersect.cy1 <= intersect.cy2);
        }

        return intersect;
    }
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        int aArea = rectArea(ax1, ay1, ax2, ay2);
        assert(aArea >= 0);// cout << aArea << "\n";

        int bArea = rectArea(bx1, by1, bx2, by2);
        assert(bArea >= 0);// cout << bArea << "\n";

        Intersection intersect = intersects(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);

        int totalArea = aArea + bArea;
        if (intersect.intersects) {
            int cArea = rectArea(intersect.cx1, intersect.cy1, intersect.cx2, intersect.cy2);
            assert(cArea >= 0);// cout << cArea << "\n";
            totalArea -= cArea;
        }
        assert(totalArea >= 0);
        return totalArea;
    }
};