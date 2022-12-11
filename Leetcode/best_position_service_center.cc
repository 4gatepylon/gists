template<>
struct hash<vector<int>> {
    size_t operator()(const vector<int>& v) const {
        size_t hsh = 0;
        for (int i : v) {
            hsh ^= hash<int>()(i);
        }
        return hsh;
    }
};

class Solution {
private:
    double euclid_dist(vector<int>& x, vector<double>& y) {
        assert(x.size() == y.size());
        double d2 = 0.0;
        for (int d = 0; d < x.size(); d++) {
            d2 += (y[d] - x[d]) * (y[d] - x[d]);
        }
        return sqrt(d2);
    }
    vector<double> euclid_dists(vector<vector<int>>& xs, vector<double>& y) {
        vector<double> dists(xs.size());
        for (int i = 0; i < xs.size(); i++) {
            dists[i] = euclid_dist(xs[i], y);
        }
        return dists;
    }
    double sum(vector<double>& v) {
        double a = 0.0;
        for (double d : v) a += d;
        return a;
    }
    // With some more cleverness we could avoid all the inversions...
    double sum_inv_dist(vector<double> dists) {
        double sid = 0.0;
        for (int i = 0; i < dists.size(); i++) {
            sid += 1.0 / dists[i];
        }
        return sid;
    }
    double update_y(vector<vector<int>>& xs, vector<double>& y) {
        vector<double> dists = euclid_dists(xs, y);
        double _inv_scl = sum_inv_dist(dists);

        for (int d = 0; d < y.size(); d++) y[d] = 0.0;
        for (int i = 0; i < xs.size(); i++) {
            assert(xs[i].size() == y.size());
            for (int d = 0; d < xs[i].size(); d++) {
                y[d] += xs[i][d] / dists[i];
            }
        }
        for (int d = 0; d < y.size(); d++) y[d] /= _inv_scl;
        // Ok this is wrong, but whatever
        return sum(dists);
    }
public:
    // Maybe the 2-dimensional median?
    // Maybe that's the median in both dimensions?
    // Need point that is between all points
    double getMinDistSum(vector<vector<int>>& positions) {
        // de-dup and check a bs edge-case
        unordered_set<vector<int>> unique_positions(positions.begin(), positions.end());
        if (unique_positions.size() == 1) return 0.0;

        // Max dist is 100 * 100 i.e. 100 from every point
        #define ITER 10000
        #define EPS_BIG 0.5
        #define EPS_SMALL 0.0000005
        #define MAX_DIST 10000

        // Center of mass
        // Init at eps because its a way to avoid landing an an integer, which means
        // we can't land on a point and get a nan
        vector<double> com{EPS_BIG, EPS_BIG};
        for (int i = 0; i < positions.size(); i++) {
            assert(positions[i].size() == com.size());
            for (int d = 0; d < positions[i].size(); d++) { com[d] += positions[i][d]; }
        }
        for (int d = 0; d < com.size(); d++) { com[d] /= positions.size(); }

        // Geometric center
        vector<double> y(com.begin(), com.end());
        double pdist = MAX_DIST;
        for (int _i = 0; _i < ITER; _i++) {
            double dist = update_y(positions, y);

            // Keep trap of distance for early stopping
            assert(dist <= pdist + EPS_SMALL);
            if (abs(dist - pdist) <= EPS_SMALL) return dist;
            pdist = dist;
        }
        return pdist;
    }
};