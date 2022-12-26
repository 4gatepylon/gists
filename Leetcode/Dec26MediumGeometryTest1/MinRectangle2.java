import java.util.Optional;
// From This: https://leetcode.com/problems/minimum-area-rectangle-ii/

class Vector {
    int dx, dy;
    Vector (int x, int y) {
        this.dx = x;
        this.dy = y;
    }
    Vector (Vector a, Vector b) {
        this.dx = b.dx - a.dx;
        this.dy = b.dy - a.dy;
    }
    Vector(int x0, int y0, int x1, int y1) {
        this.dx = x0 - x1;
        this.dy = y0 - y1;
    }
    // Compare vectors and get info about them
    boolean ortho(Vector o) { return o.dx * this.dx + o.dy * this.dy == 0; }
    double norm() { return Math.sqrt(dx * dx + dy * dy); }

    // Common utility
    @Override
    public boolean equals(Object other) {
        if (other instanceof Vector) return ((Vector)other).dx == dx && ((Vector)other).dy == dy;
        return false;
    } 
    @Override
    public int hashCode() { return Integer.hashCode(dx) ^ Integer.hashCode(dy); }
    @Override
    public String toString() { return "(" + Integer.toString(this.dx) + ", " + Integer.toString(this.dy) + ")"; }

    // Distance between points and vector manipulation
    static int dist2(int x0, int y0, int x1, int y1) { return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1); }
    static Vector add(Vector a, Vector b) { return new Vector(a.dx + b.dx, a.dy + b.dy); }
    static Vector add(Vector ...vs) {
        Vector acc = new Vector(0, 0);
        for (Vector v : vs) {
            acc.dx += v.dx;
            acc.dy += v.dy;
        }
        return acc;
    }
    static Vector neg(Vector a) { return new Vector(-a.dx, -a.dy); }

    // Fill up a HashSet of points
    static HashSet<Vector> HashFromPointsArray(int[][] points) {
        HashSet<Vector> pnts = new HashSet<Vector>();
        for (int i = 0; i < points.length; i++) {
            int x = points[i][0], y = points[i][1];
            Vector r = new Vector(x, y);
            pnts.add(r);
        }
        return pnts;
    }
}

class Solution {
    // Would be O(n^4) with brute force search, though O(n^3log(n)) 
    // is possible using a 2D binary search tree
    // (just mean sorted on each axis, which one being the major axis)
    // If we know that each element is an int (which in this case we know) then
    // it should be possible to use a HashSet for O(n^3). We use the hash set so
    // for us it is 
    // O(n^3).
    // Can be sped up by
    // 1. Avoiding quadruple counting rectangles
    // 2. Avoidings so much malloc using vectors like that
    // 3. Keeping the squared area and then taking the root only once at the very end
    // 4. Taking less norms (or squared areas) and using more jumps instead
    // and maybe some other things idk...
    public double minAreaFreeRect(int[][] points) {
        // For every triple of points
        // see whether two vectors are perpendicular to each other
        // (can only exist up to two) where each vector is the difference between
        // the points. If it is true search for the third point
        Optional<Double> minArea = Optional.empty();
        HashSet<Vector> pnts = Vector.HashFromPointsArray(points);

        // This will loop four times over (4 choose 3) but it should not matter asymptotically
        for (int i = 0; i < points.length; i++) {
            for (int j = i + 1; j < points.length; j++) {
                for (int k = j + 1; k < points.length; k++) {
                    Vector r0 = new Vector(points[i][0], points[i][1]);
                    Vector r1 = new Vector(points[j][0], points[j][1]);
                    Vector r2 = new Vector(points[k][0], points[k][1]);
                    Vector v_01 = new Vector(r0, r1);
                    Vector v_02 = new Vector(r0, r2);
                    Vector v_12 = new Vector(r1, r2);

                    Optional<Vector> _r3 = Optional.empty();
                    int center = 0;
                    if (v_01.ortho(v_02)) {
                        // 0 is the center
                        _r3 = Optional.ofNullable(Vector.add(r0, v_01, v_02));
                    } else if (v_01.ortho(v_12)) {
                        // 1 is the center
                        _r3 = Optional.ofNullable(Vector.add(r1, Vector.neg(v_01), v_12));
                        center = 1;
                    } else if (v_02.ortho(v_12)) {
                        // 2 is the center
                        _r3 = Optional.ofNullable(Vector.add(r2, Vector.neg(v_02), Vector.neg(v_12)));
                        center = 2;
                    }
 
                    if (_r3.isPresent() && pnts.contains(_r3.get())) {
                        double area                = v_01.norm();
                        if (center == 0)      area *= v_02.norm();
                        else if (center == 1) area *= v_12.norm();
                        else                  area = v_02.norm() * v_12.norm();

                        if (!minArea.isPresent() || minArea.get() > area) minArea = Optional.ofNullable(new Double(area));
                    }
                }
            }
        }
        return minArea.isPresent() ? minArea.get() : 0;
    }
}