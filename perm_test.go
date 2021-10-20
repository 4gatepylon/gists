package permutation

import (
	"testing"
	// "fmt"
)

func slice_eq(a []int, b[]int) bool {
	if len(a) != len(b) {
		return false
	}
	for idx, v := range a {
		if b[idx] != v {
			return false
		}
	}
	return true
}

func Test(t *testing.T) {
	// pairs of lists of permutations in the order they should appear starting on the first one
	pairs := [][][]int{
		// Check that the lower numbers work OK
		[][]int{
			// 1, 2
			[]int{1, 2, 3, 4, 5},
			[]int{1, 2, 3, 5, 4},
			[]int{1, 2, 4, 3, 5},
			[]int{1, 2, 4, 5, 3},
			[]int{1, 2, 5, 3, 4},
			[]int{1, 2, 5, 4, 3},
			// 1, 3
			[]int{1, 3, 2, 4, 5},
			[]int{1, 3, 2, 5, 4},
			[]int{1, 3, 4, 2, 5},
			[]int{1, 3, 4, 5, 2},
			[]int{1, 3, 5, 2, 4},
			[]int{1, 3, 5, 4, 2},
			// 1, 4
			[]int{1, 4, 2, 3, 5},
		},
		// Check that we can go up on the second highest number
		[][]int{
			[]int{1, 5, 4, 2, 3},
			[]int{1, 5, 4, 3, 2},
			[]int{2, 1, 3, 4, 5},
		},
		// Check that we can go up on the highest number
		[][]int{
			[]int{2, 5, 3, 4, 1},
			[]int{2, 5, 4, 1, 3},
			[]int{2, 5, 4, 3, 1},
			[]int{3, 1, 2, 4, 5},
			[]int{3, 1, 2, 5, 4},
			[]int{3, 1, 4, 2, 5},
			[]int{3, 1, 4, 5, 2},
		},
		// Check that we go in a loop
		[][]int{
			[]int{5, 4, 3, 2, 1},
			[]int{1, 2, 3, 4, 5},
		},
	}

	for _, permutation_list := range pairs {
		input := permutation_list[0]
		for permutation_idx := 1; permutation_idx < len(permutation_list); permutation_idx++ {
			expected := permutation_list[permutation_idx]
			gotten := next_permutation(input)
			if !slice_eq(expected, gotten) {
				t.Fatalf("Expected permutation `%v`, but got `%v` from input `%v`", expected, gotten, input)
			}
			input = expected
		}
	}
}

// A O(n) next_permutation algorithm: finds the next (higher) permutation, lexicographically
// copies: not in place; expects UNIQUE numbers but they can be anything (though I only test 1, 2, 3..., n)
// goes around and cycles back to lowest if at highest...

// Strategy:
// 1A. find the last index (left to right) such that there exists a number larger to that idx's value to the right
//   2. pick the smallest such value (larger than the one at this idx and to the right) and swap it with this
//   3. sort the remainder
// 1B. if the last index was -1 (i.e. no such) index just reverse the array

func next_permutation(p []int) []int {
	if len(p) <= 0 {
		return make([]int, 0)
	} else if len(p) == 1 {
		return []int{p[0]}
	}
	next := make([]int, 0, len(p))
	right_largest_idx := len(p) - 1
	last_idx := len(p) - 2
	for last_idx >= 0 {
		if p[last_idx] < p[right_largest_idx] {
			break
		}
		if p[last_idx] > p[right_largest_idx] {
			right_largest_idx = last_idx
		}
		last_idx --
	}
	// if there was nothing loop around
	if last_idx < 0 {
		for idx := len(p) - 1; idx >= 0; idx -- {
			next = append(next, p[idx])
		}
		return next
	}
	// else first copy the prefix of the array
	for idx := 0; idx < last_idx; idx ++ {
		next = append(next, p[idx])
	}
	// then we need to find the smallest number to the right of last_idx that is larger
	// than its value and swap it with this
	right_smallest_larger_idx := right_largest_idx
	for idx := last_idx + 1; idx < len(p); idx ++ {
		if p[idx] > p[last_idx] && p[idx] < p[right_smallest_larger_idx] {
			right_smallest_larger_idx = idx
		}
	}
	next = append(next, p[right_smallest_larger_idx])
	// now, noting that the rest of the array must be sorted in descending order
	// (because if it weren't, there would be a last_idx further to the right) we can just
	// go from right to left (ascending) until we hit something larger than p[last_idx] then
	// we add that number there instead, then we continue until and we skip right_smallest_larger_idx
	idx := len(p) - 1
	added_last_idx := false
	for idx > last_idx {
		if p[idx] > p[last_idx] && !added_last_idx {
			added_last_idx = true
			next = append(next, p[last_idx])
			// skip this idx (necessary larger by construction)
			if idx == right_smallest_larger_idx {
				idx --
			}
		} else if idx != right_smallest_larger_idx{
			next = append(next, p[idx])
			idx --
		}
	}
	return next
}