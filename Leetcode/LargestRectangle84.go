package main

// Tackle this problem:
// https://leetcode.com/problems/largest-rectangle-in-histogram/
// Heights are all positive

import "fmt"

// Strategy is to
// 1. For each element, find the index of the leftmost element to the right smaller than it
// 2. For each element, find the index of the rightmost element to the left smaller than it
// 3. For each element, compute it's maximal rectangle (assuming that it is the decider for
//   the height, i.e. it's the shortest element in that rectangle) by finding
//   H(element) * (Index(right) - Index(left) - 1) since that is its maximal rectangle
//   (assuming it is the height-decider).
//
// This works because every rectangle can be defined in terms of its bottleneck height
// and the indices that induces.
// Runtime is O(n).
//
// If this were production code we'd want it a little more modular...
func largestRectangleArea(heights []int) int {
	// Store the index of the first element to the right (of each element, denoted by
	// its index) such that the height of that element to the right is smaller than
	// this element's height.
    rightIdxs := make([]int, len(heights), len(heights))
	// Store the index of the first element to the left of each element that is
	// smaller (analogous to rightIdxs)
	leftIdxs := make([]int, len(heights), len(heights))
	
	// A LIFO stack that stores a sequence that is decreasing from right to left.
	// It is used as we traverse the array to remember the possible first elements
	// smaller (than the current element) to either the right or left.
	//
	// VALUES:    indices of candidate elements
	// INVARIANT 1: stack[stackHead] is the first element smaller than our current element
	//   in some direction.
	// INVARIANT 2: stack[idx] <= stack[idx + 1] forall idx <= stackHead
	// INVARIANT 3: stack[0] = -1 if going right and len(heigths) if going left
	stack := make([]int, len(heights) + 1, len(heights) + 1)
	stackHead := 0

	// Calculate the largest index smaller to the left (moving right using DP)
	// By convention the first elements gets -1
	stack[stackHead] = -1
	for i := 0; i < len(heights); i++ {
		// Solve for this index by popping all invalid options
		// and then getting the stackHead
		for ; stackHead > 0 && heights[stack[stackHead]] >= heights[i]; stackHead -- {}
		if (stack[stackHead] != -1 && heights[stack[stackHead]] >= heights[i]) {
			panic("Invariant broken")
		}
		leftIdxs[i] = stack[stackHead]

		// Keep the invariants
		stackHead ++
		stack[stackHead] = i
	}

	// Calculate the smallest index larger to the right (moveing left using DP)
	// By convention the last elements gets len(heights)
	stackHead = 0
	stack[stackHead] = len(heights)
	for i := len(heights) - 1; i >= 0; i-- {
		// Like before pop out invalid stack elements and then get the
		// proper element for this one
		for ; stackHead > 0 && heights[stack[stackHead]] >= heights[i]; stackHead -- {}
		if (stack[stackHead] != len(heights) && heights[stack[stackHead]] >= heights[i]) {
			panic("Invariant broken")
		}
		rightIdxs[i] = stack[stackHead]

		// Keep the invariants
		stackHead ++
		stack[stackHead] = i
	}

	// Find the max area
	maxArea := heights[0]
	for bottleneck := 0; bottleneck < len(heights); bottleneck ++ {
		bottleneckHeight := heights[bottleneck]
		bottleneckWidth := rightIdxs[bottleneck] - leftIdxs[bottleneck] - 1;
		bottleneckArea := bottleneckHeight * bottleneckWidth;
		if bottleneckArea > maxArea {
			maxArea = bottleneckArea
		}
	}
	return maxArea;
}

// Our algorithm will operate as follows

// Build and run this by doing
// 1. `go build LargestRectangle84.go`
// 2. `go run LargestRectangle84`
func main() {
	fmt.Println("Trying out problem 84 of Leetcode")
	exampleArr := []int{2, 1, 5, 6, 2, 3}
	exampleArr2 := []int{2, 4}

	max := largestRectangleArea(exampleArr)
	fmt.Printf("The max size of exampleArr is %d\n", max)
	max = largestRectangleArea(exampleArr2)
	fmt.Printf("The max size of exampleArr2 is %d\n", max)
}