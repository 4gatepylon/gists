package main

import "fmt"

// Do two rows at a time, the front is called the
// wavefront and the back is called the body. The element before the body is called
// the residual.
// 1. The body is filled in with zeros if necessary and a "zero" flag is set correspondingly.
//    All original zero locations are flipped to non-zero.
// 2. When we reach the wavefront it becomes the body, the next unseen row becomes the wavefront,
//    and the (original) body becomes the residual. 
//    2.1 If the zero flag was set:
//          If the current row should be zeroed:
//            Zero out the previous row's non-zero elements, zero out this row
//            (if necessary, keeping the zero elements as non-zero), propgate the
//            zeroed indices from the residue onto this row setting them to 1's.
//            Set the zero flag.
//          If the current row should not be zeroed:
//            Zero out the previous row's non-zero elements, propgate those as
//            as 0's.
//        If the zero flag was not set:
//          If the current row should be zeroed:
//            Zero it out, propagate zeros from above into non-zero elements here,
//            turn zero elements here into non-zero (only originals). Set zero flag.
//          If the current row should not be zeroed:
//            Make sure the zero flag is unset. Propgate any zeros from the
//            body onto the wavefront.
//
// The idea is that rows are represented by just zeroing, while
// columns are represented by oscillatory waves of 0's and 1's (non-zeros).
// The runtime is O(nm). An important change I made to allow the columns to include
// elements above their first zero in the original matrix is to
// bounce them back up at the very end.

func rowHasZeros(matrix [][]int, row int) bool {
	for i := 0; i < len(matrix[row]); i++ {
		if matrix[row][i] == 0 {
			return true
		}
	}
	return false
}

func zeroFlipRow(matrix [][]int, row int) {
	for i := 0; i < len(matrix[row]); i++ {
		if matrix[row][i] == 0 {
			matrix[row][i] = 1
		} else {
			matrix[row][i] = 0
		}
	}
}

func propagateWave(matrix [][]int, row int, up bool, currHasZeros bool, prevHadZeros bool) {
	residual := row - 1
	if up {
		residual = row + 1
	}
	for i := 0; i < len(matrix[row]); i++ {
		if prevHadZeros == (matrix[residual][i] != 0) {
			// Propagate the wave
			if currHasZeros == prevHadZeros {
				// If they are both zeros or one there is no need to flip
				matrix[row][i] = matrix[residual][i]
			} else if currHasZeros {
				// Flip from zero to one
				matrix[row][i] = 1
			} else {
				// Flip from one to zero
				matrix[row][i] = 0
			}
			// Flip the previous entry
			if prevHadZeros {
				matrix[residual][i] = 0
			}
		}
	}
}

func updateBodyAndReturnHasZeros(matrix [][]int, body int, up bool, prevHadZeros bool) bool {
	currHasZeros := rowHasZeros(matrix, body)
	if currHasZeros {
		zeroFlipRow(matrix, body)
	}
	propagateWave(matrix, body, up, currHasZeros, prevHadZeros)
	return currHasZeros
}

func setZeroesRowsDown(matrix [][]int) bool {
	// Precondition: length is at least 2
	// Initialize 
	prevHadZeros := rowHasZeros(matrix, /* row = */ 0)
	if prevHadZeros {
		zeroFlipRow(matrix, 0)
	}

	// Loop
	for wavebody := 1; wavebody < len(matrix); wavebody ++ {
		prevHadZeros = updateBodyAndReturnHasZeros(matrix, wavebody, /*up = */ false, prevHadZeros)
	}

	// This is needed to handle edge cases and fix the last row if it
	// was a zero row
	return prevHadZeros
}

// Set all entires in a column to zero above a certain point
func zeroColumnAbove(matrix [][]int, col int, row int) {
	for i := 0; i < row; i++ {
		matrix[i][col] = 0
	}
}

func setZeroes(matrix [][]int) {
	// Avoid these nasty cases
	if len(matrix) < 1 {
		return
	} else if len(matrix) == 1 && rowHasZeros(matrix, 0) {
		for i := 0; i < len(matrix[0]); i++ {
			matrix[0][i] = 0
		}
		return
	}

	// Set zeros for all rows and downwards
	lastRowWasZero := setZeroesRowsDown(matrix)
	lastRow := len(matrix) - 1

	// Set zeros for columns (including above first index, which we missed)
	for col := 0; col < len(matrix[lastRow]); col ++ {
		if (matrix[lastRow][col] == 0 != lastRowWasZero) {
			zeroColumnAbove(matrix, col, lastRow)
		}
	}
	// The last row could still be zero and need to be zeroed out
	if lastRowWasZero {
		for col := 0; col < len(matrix[lastRow]); col ++ {
			matrix[lastRow][col] = 0;
		}
	}
}

func main() {
	// Create a matrix that looks like this and should transition into the following
	// 1 1 1      1 0 1
	// 1 0 1  ->  0 0 0
	// 1 1 1      1 0 1
	mat1 := make([][]int, 3)
	mat2 := make([][]int, 3)
	mat3 := make([][]int, 4)

	// [[1,2,3,4],[5,0,7,8],[0,10,11,12],[13,14,15,0]]
	mat4 := [][]int {[]int {1,2,3,4}, []int {5,0,7,8}, []int {0,10,11,12}, []int {13,14,15,0}}

	// Create rows
	for row := 0; row < 3; row ++ {
		mat1[row] = make([]int, 3)
		mat2[row] = make([]int, 3)
		mat3[row] = make([]int, 4)
	}
	mat3[3] = make([]int, 4)
	// Populate non-zeros
	for row := 0; row < 3; row ++ {
		for col := 0; col < 3; col ++ {
			mat1[row][col] = 1
			mat2[row][col] = 1
			mat3[row][col] = 1
			mat3[3][col] = 1
		}
		mat3[row][3] = 1
	}

	// Populate zeros
	mat1[1][1] = 0
	mat2[2][1] = 0
	mat3[1][2] = 0
	mat3[2][1] = 0
	// This one was already ignored, but that's OK
	mat3[3][3] = 0

	// Mat 1
	fmt.Println("Mat1 Before")
	fmt.Println(mat1)
	setZeroes(mat1)
	fmt.Println("\nAfter")
	fmt.Println(mat1)

	// Mat2
	fmt.Println("\nMat2 Before")
	fmt.Println(mat2)
	setZeroes(mat2)
	fmt.Println("\nAfter")
	fmt.Println(mat2)

	// Mat3
	fmt.Println("\nMat3 Before")
	fmt.Println(mat3)
	setZeroes(mat3)
	fmt.Println("\nAfter")
	fmt.Println(mat3)

	// Mat4
	fmt.Println("\nMat4 Before")
	fmt.Println(mat4)
	setZeroes(mat4)
	fmt.Println("\nAfter")
	fmt.Println(mat4)
}