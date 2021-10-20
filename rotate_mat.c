// The matrix is guaranteed to be size n x n so that
// there are n columns each with n elements in `matrix`
void rot(int** matrix, int n){
    int margin = 0;
    while (n - (margin << 1) >= 2) {
        // Rotate the `outer` rows and columns where `first` and `last` mean
        // first and last INSIDE THE SLICE that excludes the `margin` outer
        // rows and columns

        // firstRow = matrix[0][_]
        // lastRow = matrix[n - 1][_]
        // firstCol = matrix[_][0]
        // lastCol = matrix[_][n - 1]
        int swp;
        for (int k = margin; k < n - 1 - margin; k++) { // TODO modify k
            swp = matrix[k][n - 1 - margin];
            // Numbers describe if you go counterclockwise what the rotation would hit in order.
            // To use only one number to swap with, we instead do a different order where the
            // cols/rows fill in where there is redundancy until at last `swp` is used.

            // lastCol <= firstRow (1)
            matrix[k][n - 1 - margin] = matrix[margin][k];
            // firstRow <= firstCol (3)
            matrix[margin][k] = matrix[n - 1 - k][margin];
            // firstCol <= lastRow (2)
            matrix[n - 1 - k][margin] = matrix[n - 1 - margin][n - 1 - k];
            // lastRow <= lastCol (4)
            matrix[n - 1 - margin][n - 1 - k] = swp;
        }
        margin ++;
    }
}
// Would be a cool exercise to try and make this faster with SIMD and vectorization by uint64_t
// Another question is why is this so slow relative to other submissions? How can I use extra memory
// to be faster? Right now this is literally O(n^2) w/ ~n^2 operations...