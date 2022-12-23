import java.util.Collection;
// https://leetcode.com/problems/minimum-area-rectangle/description/

class Point {
    int x;
    int y;
    Point(int[] xy) { x = xy[0]; y = xy[1]; }
    Point(int x, int y) { this.x = x; this.y = y; }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Point) return ((Point)other).x == x && ((Point)other).y == y;
        return false;
    } 
    
    @Override
    public int hashCode() {
        return Integer.hashCode(x) ^ Integer.hashCode(y);
    }

    static Collection<Point> fromPoints(int[][] points) {
        ArrayList<Point> ps = new ArrayList<Point>(points.length);
        for (int i = 0; i < points.length; i++) { ps.add(new Point(points[i])); }
        return ps;
    }
}

class Solution {
    // O(n^2) by noting that every rectangle is defined by a point and an offset vector
    // or in our case two points (which induce the offset vector)
    // This will double-count by a factor of 2 but that's OK
    public int minAreaRect(int[][] points) {
        Set<Point> ps = new HashSet<Point>(Point.fromPoints(points));
        int minArea = Integer.MAX_VALUE;
        for (int i = 0; i < points.length; i ++) {
            for (int j = i + 1; j < points.length; j ++) {
                // This is bottom left and top right
                int x0 = points[i][0];
                int x1 = points[j][0];
                int y0 = points[i][1];
                int y1 = points[j][1];
                if (x0 != x1 && y0 != y1) {
                    // Might not actually be top left and bot right, so a little misleading
                    Point topleft = new Point(x0, y1);
                    Point botright = new Point(x1, y0);
                    if (ps.contains(topleft) && ps.contains(botright)) {
                        int area = Math.abs((x1 - x0) * (y1 - y0));
                        minArea = Math.min(area, minArea);
                    }
                }   
            }
        }
        if (minArea == Integer.MAX_VALUE) minArea = 0;
        return minArea;
    }
}