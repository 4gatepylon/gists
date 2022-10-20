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
// THe runtime is O(nm) (it sees each index at most twice I think).

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

func propagateWave(matrix [][]int, row int, currHasZeros bool, prevHadZeros bool) {
	for i := 0; i < len(matrix[row]); i++ {
		if prevHadZeros == (matrix[row-1][i] != 0) {
			// Propagate the wave
			if currHasZeros == prevHadZeros {
				// If they are both zeros or one there is no need to flip
				matrix[row][i] = matrix[row-1][i]
			} else if currHasZeros {
				// Flip from zero to one
				matrix[row][i] = 1
			} else {
				// Flip from one to zero
				matrix[row][i] = 0
			}
			// Flip the previous entry
			if prevHadZeros {
				matrix[row-1][i] = 0
			} else {
				matrix[row-1][i] = 1
			}
		}
	}
}

func updateBodyAndReturnHasZeros(matrix [][]int, body int, prevHadZeros bool) bool {
	currHasZeros := rowHasZeros(matrix, body)
	if currHasZeros {
		zeroFlipRow(matrix, body)
	}
	propagateWave(matrix, body, currHasZeros, prevHadZeros)
	return currHasZeros
}

func setZeros(matrix [][]int)  {
	// Set up invariances
	prevHadZeros := rowHasZeros(matrix, /* row = */ 0)
	for i := 0; i < len(matrix[0]); i++ {
		if prevHadZeros {
			if matrix[0][i] == 0 {
				matrix[0][i] = 1;
			} else {
				matrix[0][i] = 0;
			}
		}
	}

	// Loop
	for wavebody := 1; wavebody < len(matrix); wavebody ++ {
		prevHadZeros = updateBodyAndReturnHasZeros(matrix, wavebody, prevHadZeros)
	}

	// Handle last section
	// (if the last one was zeros then we need to flip the nonzero entries
	// otherwise, we should be OK, because the incoming zeros were from waves)
	if prevHadZeros {
		for i := 0; i < len(matrix[len(matrix) - 1]); i++ {
			if matrix[len(matrix) - 1][i] != 0 {
				matrix[len(matrix) - 1][i] = 0
			}
		}
	}
}

func main() {
	fmt.Println("Hi")
}