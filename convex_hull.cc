#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <math.h>

// Compile and run with
// rm -rf && clang++ convex_hull.cc -std=c++11 -stdlib=libc++ && ./a.out
//
// add `-Weverything` as in `clang++ convex_hull.cc -std=c++11 -stdlib=libc++ -Weverything`
// to make this give you helpful warnings (though a lot of them are not helpful)
using namespace std;

// We will be doing an in-place merge ala Merge-Sort
// Here is an example run-through
// The idea only differs for our case insofar that when
// you merge sometimes you might skip points and be happy to
// overwrite them.
// The idea is that the write array is the lower array in memory,
// if it wins it just wins, if it loses then we swap with B and increment A's pointer
// that way, we build the join sorted area in the beginning without allocating space.
// If A is out of space we can just copy B where the point points to because
// what is left in A is not important or we are in B and that is the beginning of B
// which we already displaced.
//
// 1 2 3 4
// 2 4 5
// 
//
// 1 2 >3 4
// 2 4 5
// 1 2 2 >4
// 3 4 5
// 1 2 2 3
// 4 4 5
// 1 2 2 3 4 4 5

// For numerical issues and testing
struct Float {
    double v;
    Float(double u) { v = u; }
    Float(const Float& u) { v = u.v; }

    #define EPS 0.000000000001
    bool operator ==(const Float& u) const { return abs(v - u.v) < EPS; }
    bool operator !=(const Float& u) const { return !(*this == u); }
    Float operator +(const Float& u) const{ return Float(v + u.v); }
    Float operator -(const Float& u) const{ return Float(v - u.v); }
    Float operator *(const Float& u) const{ return Float(v * u.v); }
    Float operator /(const Float& u) const{ return Float(v / u.v); }
    bool operator >(const Float& u) const { return v > u.v; }
    bool operator >=(const Float& u) const { return v >= u.v; }
    bool operator <(const Float& u) const { return v < u.v; }
    bool operator <=(const Float& u) const { return v <= u.v; }
    bool operator =(const Float& u) { return v = u.v; }
    
};

struct Point_t {
    Float x;
    Float y;
    const bool operator ==(const struct Point_t& p) const { return x == p.x && y == p.y; }
};

struct Vec_t {
    Float x;
    Float y;
    const bool operator ==(const struct Vec_t& v) const { return x == v.x && y == v.y; }
};

typedef struct Point_t Point;
typedef struct Vec_t Vec;

typedef int Quadrant;
#define I 1
#define II 2
#define III 3
#define IV 4


// Identifier for quadrants. Importantly, everything on a vertical or horizontal slope
// gets assigned to the quadrant that it ENDS because that way we just want the slope
// to always get more positive.
// RETURN
//  - A quadrant that is numerated in a mod-4 fashion such that
//  - until you loop there is an ordering of the quadrants where
//  - if you go to the lower quadrant above you, you are rotating less
static inline Quadrant quadrant(Vec v) {
    if (v.x >= 0 && v.y > 0) return I;
    if (v.x < 0 && v.y >= 0) return II;
    if (v.x <= 0 && v.y < 0) return III;
    return IV;
}

// Get an ordering of the changes in quadrants from less costly to more costly
// RETURN
//  - The difference in quadrants (counting in a circle) in number of steps
static inline int quadrant_diff(Quadrant q1, Quadrant q2) {
    int d = q2 - q1;
    if (d < 0) d += 4;
    return d;
}

// RETURN
//  - Vector from diff of two points
static inline Vec point_diff(Point p1, Point p2) {
    return Vec{p1.x - p2.x, p1.y - p2.y};
}

// RETURN
//  - The dot product of two vectors
static inline Float dot(Vec v, Vec u) {
    return v.x * u.x + v.y * u.y;
}
// RETURN
//  - The squared norm (euclidean) of a vector
static inline Float norm2(Vec v) {
    return v.x * v.x + v.y * v.y;
}

// RETURN
//  - Whether two vectors have the same heading
// (assuming they are in the same quadrant)
static inline bool same_direction(Vec v, Vec u) {
    Float d = dot(v, u);
    // In the case of d = 0 they can only be in the same direction if it is
    // the zero vector. This is an edge case which won't occur, but it will pass.
    return d * d == norm2(v) * norm2(u) && d >= 0;
}


// RETURN
//  - Whether the beater_traj has more negative slope or not than loser_traj
// NOTE: that is not vector, it is SLOPE (so it is assumed that you are in the same quadrant)
static inline bool beats_more_neg_slope(Vec beater_traj, Vec loser_traj){
    if (same_direction(beater_traj, loser_traj)) {
        // If one dimension is zero, then we want to pick the other dimension that is bigger
        if (beater_traj.x == 0) {
            if (beater_traj.y < 0) return beater_traj.y <= loser_traj.y;
            else                   return beater_traj.y >= loser_traj.y;
        } else if (beater_traj.y == 0) {
            if (beater_traj.x < 0) return beater_traj.x <= loser_traj.x;
            else                   return beater_traj.x >= loser_traj.x;
        }
    }
    return beater_traj.y * loser_traj.x <= beater_traj.x * loser_traj.y;
}

// Assuming that two vectors are in the same quadrant, decide
// which one has the least change in the right hand rule direction
// RETURN
//  - The trajectory that wins and true if it was beater
static inline bool beats_in_quad(
    Quadrant quad, 
    Vec beater_traj,
    Vec loser_traj) {

    // dx / dy > dx' / dy' => dy'*dx > dy * dx'
    // if each is negative then it should still work...
    assert(beater_traj.x != 0 || beater_traj.y != 0);
    assert(loser_traj.x != 0 || loser_traj.y != 0);
    
    assert(quadrant(beater_traj) == quad);
    assert(quadrant(loser_traj) == quad);
    // Turns out, you always want the more negative slope, because on teh right hand of y you get
    // more positive and you want to slow that, while on the left hand of y you also get more positive
    // (and also want to slow that), it's just that at y you jump from +infty to -infty
    if (beats_more_neg_slope(beater_traj, loser_traj)) {
        return true;
    }
    return false;
}

// Get which next point beats the other (i.e. we should take to keep
// the hull convex)
// RETURN
//  - The heading that we should take to stay convex
//  - Whether the beater won (if so, increment beater, else increment loser)
static inline pair<Quadrant, bool> beats(
    // Current heading quadrant
    Quadrant curr_quad,
    // Current point (could be in another quadrant)
    Point curr_pnt,
    // Next options
    Point cand_beater,
    Point cand_loser) {
    Vec beater_traj = {
        cand_beater.x - curr_pnt.x,
        cand_beater.y - curr_pnt.y
    };
    Vec loser_traj = {
        cand_loser.x - curr_pnt.x,
        cand_loser.y - curr_pnt.y
    };

    // Quadrant pointing of that vector
    Quadrant beater_quad = quadrant(beater_traj);
    Quadrant loser_quad = quadrant(loser_traj);

    int beater_quad_diff = quadrant_diff(curr_quad, beater_quad);
    int loser_quad_diff = quadrant_diff(curr_quad, loser_quad);
    // If there is a change in quadrants that is larger for one vector than the other
    // just return the smaller change
    if (beater_quad_diff < loser_quad_diff) {
        return pair<Quadrant, bool>(beater_quad, true);
    } else if (beater_quad_diff > loser_quad_diff) {
        return pair<Quadrant, bool>(loser_quad, false);
    }
    // If the changes are both in the same quadrant, return the one that leads to the less
    // difference
    assert(beater_quad == loser_quad);
    return pair<Quadrant, bool>(beater_quad, beats_in_quad(beater_quad, beater_traj, loser_traj));
}

static inline pair<Quadrant, Point> beating_point(
    // Current heading quadrant
    Quadrant curr_quad, 
    // Current point
    Point curr_pnt, 
    vector<Point>& pnts) {
    Point beater = pnts[0];
    if (beater == curr_pnt) {
        assert(pnts.size() > 1);
        beater = pnts[1];
    }
    Quadrant beater_quad = quadrant(point_diff(beater, curr_pnt));
    for (int i = 1; i < pnts.size(); i++) {
        if (pnts[i] == curr_pnt) continue;

        pair<Quadrant, bool> beater_beats = beats(curr_quad, curr_pnt, beater, pnts[i]);
        if (!beater_beats.second) {
            beater = pnts[i];
            beater_quad = beater_beats.first;
        }
    }
    return pair<Quadrant, Point>(beater_quad, beater);
}

static inline vector<Point> convex_hull_slow(vector<Point>& points) {
    assert(points.size() > 0);

    // Get the lowest point (must be in the convex hull)
    int m = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i].x < points[m].x || points[i].x == points[m].x && points[i].y < points[m].y) m = i;
    }

    vector<Point> hull = vector<Point>(1, points[m]);
    if (points.size() == 1) return hull;

    // Current quadrant tells us where our heading's quadrant is
    // We start at II because we choose the minimum x point to be the
    // starting point, meaning that anyone pointing to us must be pointing
    // towards II or III. The only case in which they point to III is when
    // there are multiple on the same X, in which case we pick the lowest one,
    // but in that case it doesn't matter since the up vector <1, 0> is in
    // quadrant I and so those will be ignored anyways.
    Quadrant curr_quad = II;

    bool next_point_in_hull = false;
    while (!next_point_in_hull) {
        pair<Quadrant, Point> beater = beating_point(curr_quad, hull[hull.size() - 1], points);
        next_point_in_hull = beater.second == hull[0];

        if (!next_point_in_hull) {
            curr_quad = beater.first;
            hull.push_back(beater.second);
        }
        if (hull.size() > points.size()) {
            throw new runtime_error("Ran forever :(");
        }
    }
    return hull;
}

// Functions to transform for Leetcode's format to/from my own
static inline vector<int> point2vec(Point& p) {
    assert(p.x.v == (int)p.x.v && p.y.v == (int)p.y.v);
    return vector<int>{(int)p.x.v, (int)p.y.v};
}
static inline Point vec2point(vector<int>& p) { return Point{p[0], p[1]}; }
static inline vector<Point> vecs2points(vector<vector<int>>& vs) {
    vector<Point> ps;
    for (auto& v : vs) ps.push_back(vec2point(v));
    return ps;
}
static inline vector<vector<int>> points2vecs(vector<Point>& ps) {
    vector<vector<int>> vs;
    for (auto& p : ps) vs.push_back(point2vec(p));
    return vs;
}

/** TODO FIX BELOW (RECURSIVE NLOGN IMPLEMENTATION)
 * 
// Return the last index that is relevant + 1 (i.e. exclusive)
static int fill_hull(vector<Point>& hull, int i, int j) {
    // Should have at least two points
    assert(i + 2 <= j);
    if (i + 3 >= j) {
        // With 3 points (triangle) we have some cases
        // 1. Both All three points on a flat line: default OK
        // 2. Two points on a flat line and a third point above: default OK
        // 3. Three points on seperate flat lines (the other two above this one)
        //    3.1 Both on quadrant I or both on quadrant II:
        //        The one with the most negative slope first
        //    3.3 Quadrants I and II:
        //        The one in quadrant I then the one in quadrant II
        if (i + 3 == j) {
            // If beater ever wins we do nothing (it's the thing immediately after)
            Vec beater_traj = diff(hull[i], hull[i+1]);
            Vec loser_traj = diff(hull[i], hull[i+2]);
            Quadrant beater_quad = quadrant(beater_traj);
            Quadrant loser_quad = quadrant(loser_traj);
            assert(beater_quad == I || beater_quad == II);
            assert(loser_quad == I || loser_quad == II);
            if (
                beater_quad == loser_quad && beats_more_neg_slope(beater_traj, loser_traj) ||
                beater_quad != loser_quad && beater_quad == II
                ) {
                // If they are on the same quadrant, we need to pass by the one with the more negative slope
                // before the other
                // If they are on seperate quadrants, we need to pass by I before II
                Point tmp_pnt = hull[i+1];
                hull[i+1] = hull[i+2];
                hull[i+2] = tmp_pnt;
            }
        }
        // With 2 points its already sorted in the right order
        cout << "done " << i << " to " << j << "\n"; // XXX
        return j;
    }
    int m = i + (j - i) / 2;
    int ki = fill_hull(hull, i, m);
    int kj = fill_hull(hull, m, j);
    for (int z = i; z < ki; z++) { // XXX
        Point p = hull[z];
        cout << "Point in A" << p.x << " " << p.y << "\n";
    }
    for (int z = m; z < kj; z++) { // XXX
        Point p = hull[z];
        cout << "Point in B" << p.x << " " << p.y << "\n";
    }
    assert(i < ki && ki <= m);
    assert(m < kj && kj <= j);

    // Note A occurs before B
    // A will have elements that are below and to the left of those in B
    // We can expect the first element of A to be inside the convex hull because there is no element
    // both lower and more to the left
    int A = i;
    int B = m;
    int k = i;

    // We need to have a cycle of length at least two
    // To get vectors
    assert(A < ki - 1);
    assert(B < kj - 1);
    cout << "ki is " << ki << " and kj is " << kj << "\n";
    Point curr_pnt = hull[A];
    Quadrant curr_quad = quadrant(diff(curr_pnt, hull[ki - 1]));
    A++;
    while (A < ki && B < kj) {
        cout << "Current point is " << curr_pnt.x << ", " << curr_pnt.y << "\n"; // XXX
        cout << "A is index " << A << " with value " << hull[A].x << ", " << hull[A].y << "\n"; // XXX
        cout << "B is index " << B << " with value " << hull[B].x << ", " << hull[B].y << "\n"; // XXX
        pair<Quadrant, bool> beat_info = beats(curr_quad, curr_pnt, hull[A], hull[B]);
        bool A_beats = beat_info.second;
        curr_quad = beat_info.first;
        curr_pnt = A_beats ? hull[A] : hull[B];
        
        if (A_beats) {
            cout << "\t A won\n"; // XXX
            // One point should always beat so eventually we get here
            // Because it's in-place do nothing
            A++;
        } else {
            cout << "\t B won\n"; // XXX
            Point tmp_pnt = hull[A];
            hull[A] = hull[B];
            hull[B] = tmp_pnt;
        }
        k++;
        cout << "--\n"; // XXX
    }
    while (A < ki) { hull[k++] = hull[B++]; cout << "out of B, using up A\n"; }
    while (B < kj) { hull[k++] = hull[B++]; cout << "out of A, using up B\n"; }
    return k;
}

// Return a out of page RHR - following cycle of points that form the convex hull
// of the points set given to us.
// Points should be unique
// Next time use absl span https://abseil.io/tips/93
static vector<Point> convex_hull(vector<Point>& points) {
    if (points.size() == 1) return points;

    // Malloc ONCE, sort it, only merge consecutive locations into the start
    // of the first location (overflowing if necessary)
    vector<Point> hull(points.size());
    for (int i = 0; i < points.size(); i++) {
        hull[i] = points[i];
    }
    sort(hull.begin(), hull.end(), [](const Point& a, const Point& b) -> bool {
        return a.y < b.y ? true : (a.y == b.y && a.x <= b.x ? true : false);
    });
    int last_index = fill_hull(hull, 0, points.size());
    while(hull.size() > last_index + 1) {
        hull.pop_back();
    }
    return hull;
}

* TODO FIX ABOVE
**/

int main() {
    // Make sure quadrant works ok on normal cases
    assert(quadrant(Vec{1, 1}) == I);
    assert(quadrant(Vec{-1, 1}) == II);
    assert(quadrant(Vec{-1, -1}) == III);
    assert(quadrant(Vec{1, -1}) == IV);
    cout << "Quadrant (Common Case) OK!\n";
    // Make sure that the difference between two quadrants is increasing always as you spin RHR
    assert(quadrant_diff(I, II) <= quadrant_diff(I, III));
    assert(quadrant_diff(I, III) <= quadrant_diff(I, IV));
    assert(quadrant_diff(II, III) <= quadrant_diff(II, IV));
    assert(quadrant_diff(II, IV) <= quadrant_diff(II, I));
    assert(quadrant_diff(III, IV) <= quadrant_diff(III, I));
    assert(quadrant_diff(III, I) <= quadrant_diff(III, II));
    assert(quadrant_diff(IV, I) <= quadrant_diff(IV, II));
    assert(quadrant_diff(IV, II) <= quadrant_diff(IV, III));
    assert(quadrant_diff(I, I) == 0 && quadrant_diff(II, II) == 0 && quadrant_diff(III, III) == 0 && quadrant_diff(IV, IV) == 0);
    cout << "Quadrant Difference OK!\n";
    // Make sure that regular vector functions are ok
    assert(point_diff(Point{1, 1}, Point{1, 1}) == (Vec{0, 0}));
    assert(point_diff(Point{1, 3}, Point{3, 1}) == (Vec{-2, 2}));
    assert(point_diff(Point{1, 1}, Point{0, 0}) == (Vec{1, 1}));
    assert(point_diff(Point{1, 3}, Point{1, 3.1}) == (Vec{0, -0.1}));
    assert(point_diff(Point{-1, -1}, Point{-1, -1}) == (Vec{0, 0}));
    assert(point_diff(Point{-1, 3}, Point{3, -1}) == (Vec{-4, 4}));
    assert(dot(Vec{1, 1}, Vec{1, 0}) == 1);
    assert(dot(Vec{1, 1}, Vec{0, 0}) == 0);
    assert(dot(Vec{1, 1}, Vec{-1, 1}) == 0);
    assert(dot(Vec{1, 1}, Vec{-2, 3.3}) == 1.3);
    assert(norm2(Vec{1, 1}) == 2);
    assert(norm2(Vec{-1, 1}) == 2);
    assert(norm2(Vec{-1, -1}) == 2);
    assert(norm2(Vec{1, 2}) == 5);
    assert(norm2(Vec{1, sqrt(3)}) == 4);
    cout << "Difference, dot, and norm2 OK!\n";
    // Make sure we can tell when two vectors are in the same direction
    assert(same_direction(Vec{1, 1}, Vec{1, 1}));
    assert(same_direction(Vec{0, 1}, Vec{0, 1}));
    assert(same_direction(Vec{1, 0}, Vec{1, 0}));
    assert(same_direction(Vec{0, 0}, Vec{0, 0}));
    assert(same_direction(Vec{2, 2}, Vec{2, 2}));
    assert(same_direction(Vec{3.3, 2.5}, Vec{3.3, 2.5}));
    assert(!same_direction(Vec{-1, -1}, Vec{1, 1}));
    assert(!same_direction(Vec{-1, 2}, Vec{3, 1}));
    assert(!same_direction(Vec{-1, 5}, Vec{1, 5}));
    cout << "Same Direction OK!\n";
    // Make sure that we can tell what slope is more negative between two vectors
    // (by slope more negative we mean it comes at a higher angle using RHR
    // when we start at 0 and go to 360)
    assert(beats_more_neg_slope(Vec{1, 1}, Vec{2, 2}));
    assert(beats_more_neg_slope(Vec{1, 1}, Vec{0, 1}));
    assert(!beats_more_neg_slope(Vec{0, 1}, Vec{2, 2}));
    assert(!beats_more_neg_slope(Vec{0, 1}, Vec{0.0001, 1}));
    assert(!beats_more_neg_slope(Vec{1, 1}, Vec{2, 1}));
    assert(beats_more_neg_slope(Vec{2, 2}, Vec{0, 2}));
    assert(beats_more_neg_slope(Vec{-2, 3}, Vec{-2, 1}));
    assert(!beats_more_neg_slope(Vec{-10.322, 1.1}, Vec{-100.23, 20032.2}));
    assert(beats_more_neg_slope(Vec{-2, 3}, Vec{-2, -2}));
    assert(!beats_more_neg_slope(Vec{-2, -3}, Vec{-2, 1}));
    assert(!beats_more_neg_slope(Vec{-2, -3}, Vec{-2, -1}));
    assert(beats_more_neg_slope(Vec{-2, -3}, Vec{-2, -100}));
    assert(beats_more_neg_slope(Vec{-2, -100}, Vec{0, -2}));
    assert(!beats_more_neg_slope(Vec{1, 1}, Vec{1, 0}));
    assert(!beats_more_neg_slope(Vec{1, 1}, Vec{0, -2}));
    assert(beats_more_neg_slope(Vec{-100, -100}, Vec{1, 1}));
    cout << "Beats more negative slope OK\n";
    // Make sure that we can see, when two vectors are in the same quadrant,
    // which one we want to take assuming we want to take the one at the next RHR assuming that
    // we start at 0,0 and go up and to the right
    assert(beats_in_quad(I, Vec{1, 1}, Vec{1, 2}));
    assert(beats_in_quad(I, Vec{1, 1}, Vec{1, 2}));
    assert(beats_in_quad(I, Vec{1, 1000}, Vec{0, 1}));
    assert(!beats_in_quad(I, Vec{0, 1}, Vec{0.000001, 1}));
    assert(beats_in_quad(II, Vec{-1, 2}, Vec{-1, 1}));
    assert(beats_in_quad(II, Vec{-1, 1}, Vec{-1, 0.5}));
    assert(beats_in_quad(II, Vec{-1, 1}, Vec{-1, 0}));
    assert(beats_in_quad(II, Vec{-100, 1}, Vec{-1, 0}));
    assert(beats_in_quad(III, Vec{-1, -1}, Vec{-1, -2}));
    assert(!beats_in_quad(III, Vec{-1, -1}, Vec{-1, -0.5}));
    assert(!beats_in_quad(III, Vec{-1, -2}, Vec{-1, -1}));
    assert(beats_in_quad(III, Vec{-1, -2}, Vec{0, -1}));
    assert(!beats_in_quad(IV, Vec{1, -1}, Vec{1, -2}));
    assert(!beats_in_quad(IV, Vec{1, -0.5}, Vec{1, -1}));
    assert(!beats_in_quad(IV, Vec{1, 0}, Vec{1, -1}));
    cout << "Beats in Quad OK!\n";
    assert(!beats(I, Point{1, 0.5}, Point{1, 0}, Point{1, 1}).second);
    assert(beats(I, Point{1, 0.5}, Point{1, 1}, Point{1, 0}).second);
    assert(beats(IV, Point{0, 0}, Point{1, 1}, Point{1, 2}).second);
    assert(!beats(IV, Point{0, 0}, Point{1, 1}, Point{1, -1}).second);
    assert(beats(IV, Point{0, 0}, Point{1, -1}, Point{1, 1}).second);
    assert(beats(IV, Point{0, 0}, Point{-1, 1}, Point{-1, 0.5}).second);
    assert(beats(II, Point{0, 0}, Point{1, -1}, Point{1, 1}).second);
    assert(beats(II, Point{0, 0}, Point{-1, -1}, Point{1, -1}).second);
    assert(beats(II, Point{0, 0}, Point{-1, 0}, Point{-1, -1}).second);
    assert(beats(III, Point{0, 0}, Point{-1, -1}, Point{-1, -2}).second);
    assert(beats(III, Point{0, 0}, Point{0, -1}, Point{1, -1}).second);
    assert(beats(III, Point{0, 0}, Point{1, -1}, Point{1, 0}).second);
    cout << "Beats Ok!\n";
    // Make sure that we are able to, for a sequence of points, find the next point
    // to go to
    vector<Point> tbp{
        Point{1, 2},
        Point{2, 5},
        Point{0, 0},
        Point{1, 1},
    };
    assert(beating_point(I, tbp[0], tbp).second == tbp[1]);
    assert(beating_point(I, tbp[1], tbp).second == tbp[2]);
    assert(beating_point(I, tbp[2], tbp).second == tbp[3]);
    assert(beating_point(I, tbp[3], tbp).second == tbp[1]);
    assert(beating_point(II, tbp[0], tbp).second == tbp[2]);
    assert(beating_point(II, tbp[1], tbp).second == tbp[2]);
    assert(beating_point(II, tbp[2], tbp).second == tbp[3]);
    assert(beating_point(II, tbp[3], tbp).second == tbp[2]);
    assert(beating_point(III, tbp[0], tbp).second == tbp[2]);
    assert(beating_point(III, tbp[1], tbp).second == tbp[2]);
    assert(beating_point(III, tbp[2], tbp).second == tbp[3]);
    assert(beating_point(III, tbp[3], tbp).second == tbp[2]);
    assert(beating_point(IV, tbp[0], tbp).second == tbp[1]);
    assert(beating_point(IV, tbp[1], tbp).second == tbp[2]);
    assert(beating_point(IV, tbp[2], tbp).second == tbp[3]);
    assert(beating_point(IV, tbp[3], tbp).second == tbp[1]);
    cout << "Finding the beater OK!\n";
    // Test convex hull for some bigger problems
    vector<pair<vector<Point>, vector<Point>>> tests {
        // First vector is points and second vector is expected hull
        {
            tbp, 
            vector<Point>{Point{2, 5}, Point{0, 0}, Point{1, 1}}
        },
        {
            vector<Point>{Point{1, 1},Point{2, 2},Point{2, 0},Point{2, 4},Point{3, 3},Point{4, 2}},
            vector<Point>{Point{1, 1},Point{3, 3},Point{2, 0},Point{4, 2},Point{2, 4}}
        },
        {
            vector<Point>{Point{1, 2},Point{2, 2},Point{4, 2}},
            vector<Point>{Point{4, 2},Point{2, 2},Point{1, 2}}
        },
    };
    int testi = 1;
    for (auto& p : tests) {
        auto& points = p.first;
        auto& exp_hull = p.second;
        auto hull = convex_hull_slow(points);

        // Make sure that the two hulls are equal
        // (we don't guarantee order and I'm tool lazy to create templating for this shit's hashing)
        cout << "Trying test " << testi << "\n";
        assert(hull.size() == exp_hull.size());
        for (auto& exp : exp_hull) {
            bool found = false;
            for (auto& p : hull) {
                if (p == exp) {
                    found = true;
                    break;
                }
            }
            assert(found);
        }
        testi ++;
    }
    cout << "Convex hull (slow) passes simple test cases!\n";
}
