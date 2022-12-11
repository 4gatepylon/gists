\static inline double d2(double x1, double y1, double x2, double y2) { return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 -y2); }

class Solution {
public:
    // You know two points are always on the side because otherwise contradiction
    int numPoints(vector<vector<int>>& darts, int r) {
        // O(n^3)
        // For every pair of points that has a distance less than r between them
        // We can inscribe a circle on either side and see which is best
        // EPS cuz was getting floating point issues lmao
        #define EPS 0.141
        int R = r * r; // Rad sq
        int D = 4 * R; // Diam sq
        int max_cnt = 1;
        for (int i = 0; i < darts.size(); i++) {
            for (int j = i + 1; j < darts.size(); j++) {
                int ds = d2(darts[i][0], darts[i][1], darts[j][0], darts[j][1]);
                if (ds <= D + EPS) {
                    double midpointx, midpointy;
                    midpointx = darts[i][0] + (darts[j][0] - darts[i][0]) / 2.0;
                    midpointy = darts[i][1] + (darts[j][1] - darts[i][1]) / 2.0;

                    #define NOT_THERE 10001
                    double centerx1 = NOT_THERE, centery1 = NOT_THERE;
                    double centerx2 = NOT_THERE, centery2 = NOT_THERE;
                    if (ds == R) {
                        centerx1 = midpointx;
                        centery1 = midpointy;
                        centerx2 = midpointx;
                        centery2 = midpointy;
                    } else {
                        // Try putting it to one side
                        // Then try putting it to the other side
                        // Find the two points s.t. they are r away from both of the points we have

                        // distpx and distpy is the vector from one to the other
                        // (diffpx or diffpy may be better name)
                        double distpx = (darts[i][0] - darts[j][0]) / 2.0;
                        double distpy = (darts[i][1] - darts[j][1]) / 2.0;

                        // Non-normal vector pointing from midpoint to our centers
                        double ortho_distpx = -distpy;
                        double ortho_distpy = distpx;
                        double ortho_d = sqrt(d2(0.0, 0.0, ortho_distpx, ortho_distpy));

                        // Vector Normal vector pointing from midpoint to center
                        double ortho_distpx_norm = ortho_distpx / ortho_d;
                        double ortho_distpy_norm = ortho_distpy / ortho_d;

                        // length to up-scale vector normal from midpoint to center
                        // r^2 + (|diff|/2)^2 = dist to point we care about
                        int d = sqrt(R - ds/4.0);
                        
                        // center1 is midpoint plus d * normal
                        // center2 is midpoint - d * normal
                        centerx1 = midpointx + d * ortho_distpx_norm;
                        centery1 = midpointy + d * ortho_distpy_norm;
                        centerx2 = midpointx - d * ortho_distpx_norm;
                        centery2 = midpointy - d * ortho_distpy_norm;
                    }
                    int cnt1 = 0;
                    int cnt2 = 0;
                    for (int k = 0; k < darts.size(); k++) {
                        if (d2(centerx1, centery1, darts[k][0], darts[k][1]) <= R + EPS) {
                            cnt1++;
                        }
                        if (d2(centerx2, centery2, darts[k][0], darts[k][1]) <= R + EPS) {
                            cnt2++;
                        }
                    }
                    max_cnt = max(max_cnt, max(cnt1, cnt2));
                }
            }
        }
        return max_cnt;
    }
};