#include <cmath>

typedef int Quadrant;
#define I 1
#define II 2
#define III 3
#define IV 4
static inline Quadrant quadrant(int x, int y) {
    if (x >= 0 && y >= 0) return I;
    if (x < 0 && y > 0) return II;
    if (x <= 0 && y <= 0) return III;
    return IV;
}

class Solution {
public:
    int visiblePoints(vector<vector<int>>& points, int angle_deg, vector<int>& location) {
        if (points.size() == 0) return 0;
        // Due to the way that atan works in C++ (between neg pi/2 and pos pi/2, we need to
        // add pi or 2pi sometimes)
        vector<vector<int>> shifted_points;
        for (vector<int>& point : points) {
            if (point[0] != location[0] || point[1] != location[1]) {
                shifted_points.push_back(vector<int>{point[0] - location[0], point[1] - location[1]});
            }
        }

        int N = shifted_points.size(); 
        int max_cnt = 0;
        int superposition_cnt = points.size() - shifted_points.size();
        double angle = M_PI * angle_deg / 180.0;

        vector<double> angles(N);
        for (int i = 0; i < N; i++) {
            // Everyone is as if the guy was at 0,0 
            int pointy = shifted_points[i][1];
            int pointx = shifted_points[i][0];
            // Vertical bar gets positive angle because tangent would yield positive ratio
            // (and then quadrants will find how to shift)
            angles[i] = pointx == 0 ? M_PI_2 : atan(((double)(pointy))/pointx);
            Quadrant q = quadrant(pointx, pointy);
            if (angles[i] >= 0) {
                assert(q == I || q == III);
                if (q == III) {
                    angles[i] += M_PI;
                }
            } else {
                cout << pointx << " " << pointy << "\n";
                assert(q == II || q == IV);
                if (q == II) {
                    angles[i] += M_PI;
                } else {
                    angles[i] += 2 * M_PI;
                }
            }
        }
        sort(angles.begin(), angles.end());
        for (int i = 0; i < N; i++) { angles.push_back(angles[i] + 2 * M_PI); }

        int hi = 1;
        for (int lo = 0; lo < N; lo++) {
            while (hi <= lo || (hi < N + lo && angles[hi] <= angles[lo] + angle)) { hi++; }
            max_cnt = max(max_cnt, hi - lo);
        }
        return max_cnt + superposition_cnt;
    }
};