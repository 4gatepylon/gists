#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>

// Compile with
// clang++ convex_hull.cc -std=c++11 -stdlib=libc++ -Weverything
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

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int x;
    int y;
} Vec;

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
    if (v.x > 0 && v.y >= 0) return I;
    if (v.x <= 0 && v.y > 0) return II;
    if (v.x < 0 && v.y <= 0) return III;
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
static inline Vec diff(Point p1, Point p2) {
    return Vec{p2.x - p1.x, p2.y - p1.y};
}

// RETURN
//  - The dot product of two vectors
static inline int dot(Vec v, Vec u) {
    return v.x * u.x + v.y * u.y;
}
// RETURN
//  - The squared norm (euclidean) of a vector
static inline int norm2(Vec v) {
    return v.x * v.x + v.y * v.y;
}

// RETURN
//  - Whether two vectors have the same heading
// (assuming they are in the same quadrant)
static inline bool same_direction(Vec v, Vec u) {
    // (v.u)^2 = size(v)size(u)
    int d = dot(v, u);
    return d * d == norm2(v) * norm2(u);
}

// RETURN
//  - Whether the beater_traj has more negative slope or not than loser_traj
// NOTE: that is not vector, it is SLOPE
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
    return beater_traj.y * loser_traj.x > beater_traj.x * loser_traj.y;
}

// Assuming that two vectors are in the same quadrant, decide
// which one has the least change in the right hand rule direction
// RETURN
//  - The trajectory that wins and true if it was beater
static inline pair<Quadrant, bool> beats_in_quad(
    Quadrant quad, 
    Vec beater_traj,
    Vec loser_traj) {

    // dx / dy > dx' / dy' => dy'*dx > dy * dx'
    // if each is negative then it should still work...
    assert(beater_traj.x != 0 || beater_traj.y != 0);
    assert(loser_traj.x != 0 || loser_traj.y != 0);
    // Turns out, you always want the more negative slope, because on teh right hand of y you get
    // more positive and you want to slow that, while on the left hand of y you also get more positive
    // (and also want to slow that), it's just that at y you jump from +infty to -infty
    if (beats_more_neg_slope(beater_traj, loser_traj)) {
        return pair<Quadrant, bool>(quad, true);
    }
    return pair<Quadrant, bool>(quad, false);
}

// Get which next point beats the other (i.e. we should take to keep
// the hull convex)
// RETURN
//  - The heading that we should take to stay convex
//  - Whether the beater won (if so, increment beater, else increment loser)
static inline pair<Quadrant, bool> beats(
    // Current heading quadrant
    Quadrant curr_quad,
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
    if (beater_quad_diff > loser_quad_diff) {
        return pair<Quadrant, bool>(beater_quad, true);
    } else if (beater_quad_diff < loser_quad_diff) {
        return pair<Quadrant, bool>(loser_quad, false);
    }
    // If the changes are both in the same quadrant, return the one that leads to the less
    // difference
    assert(beater_quad == loser_quad);
    return beats_in_quad(beater_quad, beater_traj, loser_traj);
}

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
        return j;
    }
    int m = i + (j - i) / 2;
    int ki = fill_hull(hull, i, m);
    int kj = fill_hull(hull, m, j);
    assert(i < ki && ki <= m);
    assert(m < kj && kj <= j);

    // Note A occurs before B
    // A will have elements that are below and to the left of those in B
    // We can expect the first element of A to be inside the convex hull because there is no element
    // both lower and more to the left
    int A = i;
    int B = j;
    int k = i;
    Point curr_pnt = hull[A];
    Quadrant curr_quad = quadrant(diff(curr_pnt, hull[ki - 1])); // TODO deal with singletons
    A++;
    while (A < ki && B < kj) {
        pair<Quadrant, bool> beat_info = beats(curr_quad, curr_pnt, hull[A], hull[B]);
        bool A_beats = beat_info.second;
        curr_quad = beat_info.first;
        curr_pnt = A_beats ? hull[A] : hull[B];
        
        if (A_beats) {
            // One point should always beat so eventually we get here
            // Because it's in-place do nothing
            A++;
        } else {
            Point tmp_pnt = hull[A];
            hull[A] = hull[B];
            hull[B] = tmp_pnt;
        }
    }
    while (A < ki) { hull[k++] = hull[B++]; }
    while (B < kj) { hull[k++] = hull[B++]; }
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
        return a.y > b.y ? true : (a.y == b.y && a.x >= b.x ? true : false);
    });
    int last_index = fill_hull(hull, 0, points.size());
    while(hull.size() > last_index + 1) {
        hull.pop_back();
    }
    return hull;
}

int main() {
    cout << "Testing shit!\n";

    vector<Point> points{
        Point{1, 2},
        Point{2, 5},
        Point{0, 0}
    };
    vector<Point> hull = convex_hull(points);
    for (Point& p : hull) {
        cout << "Point " << p.x << " " << p.y << "\n";
    }
}
