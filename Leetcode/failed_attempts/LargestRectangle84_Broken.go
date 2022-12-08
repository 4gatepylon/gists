package main

// Tackle this problem:
// https://leetcode.com/problems/largest-rectangle-in-histogram/
// Heights are all positive

import "fmt"

// Queue of increasing subsequence of elements in a consecutive section of
// an array. This is a subset of a sliding window.
// The last element of the queue is the minimum.
// - On enqueue
//   1. For all elements larger than the current incoming element, pop them off
//      the right and then append our element to the right.
//   2. For all elements of an index lesser than the starting index, pop them.
// - To Find Min
//   1. Just check the last element (remember, this is an increasing subsequence)
// - There is no dequeue
// Should allocate it to have size n since at most n elements are enqueued and
// some might be dequeued.
type QueueElement struct {
	idx int
	val int
}

type Queue struct {
	queue []QueueElement
	idxLeft int
	idxRight int
	arrWindowLeftIdx int
	arrWindowRightIdx int
}

func queueMin(q *Queue) int {
	if len(q.queue) == 0 {
		return -1
	}
	return q.queue[len(q.queue) - 1].val
}

// Helper to pushQueue
func pushQueueOnlyPush(q *Queue, nextVal int, nextIdx int) {
	// Pop all larger elements to maintain increasing subsequence invariant
	for q.idxRight >= q.idxLeft && q.queue[q.idxRight].val >= nextVal {
		q.idxRight --
	}

	// Push element on the queue
	q.idxRight ++
	q.queue[q.idxLeft] = QueueElement{/* idx = */ nextIdx, /* val = */ nextVal}
}

// Push a value onto the queue after popping all that are on the right
// and larger to it. Then increment the arrWindowRightIdx and the
// arrWindowLeftIdx to match with the new location of the window.
// Then pop the last element (or any last elements)
// on the left that are to the left of the arrWindowLeftIdx
func pushQueue(q *Queue, nextVal int) {
	// Increment the window
	q.arrWindowLeftIdx ++
	q.arrWindowRightIdx ++
	
	// Pop all larger elements to maintain increasing subsequence invariant
	pushQueueOnlyPush(q, nextVal, q.arrWindowRightIdx)

	// Pop out everything on the left that is too old
	for q.idxLeft <= q.idxRight && q.queue[q.idxLeft].idx < q.arrWindowRightIdx {
		q.idxLeft ++
	}
}

// Initialize a queue on the values in an array by taking in the first
// windowSize elements and setting up the invariant.
func initQueue(arr []int, windowSize int) *Queue {
	if len(arr) == 0 {
		return nil
	}

	// Create the queue
	q := new(Queue)
	q.queue = make([]QueueElement, len(arr))

	// Populate the queue by enqueing, except
	// we do not modify the arrWindow indices
	q.idxLeft = 0
	q.idxRight = 0
	q.queue[0] = QueueElement{/* idx = */ 0, /* val = */ arr[0]}
	for i := 1; i < windowSize; i++ {
		// fmt.Printf("Step %d/%d of push\n", i, windowSize) // XXX
		pushQueueOnlyPush(q, arr[i], i)
	}

	// Update the sliding window elements
	q.arrWindowLeftIdx = 0
	q.arrWindowRightIdx = windowSize - 1

	return q
}

// Fixing a size of sliding windows, find the largest area possible with that window size.
// O(n)
func maxWithSlidingWindowSize(arr []int, windowSize int) int {
	// fmt.Println("Initializing queue") // XXX
	q := initQueue(arr, windowSize)
	maxHeight := queueMin(q)
	// fmt.Println("Pushing onto the queue a lot") // XXX
	for i := windowSize; i < len(arr) - windowSize + 1; i++ {
		pushQueue(q, arr[i])
		candidateHeight := queueMin(q)
		if candidateHeight > maxHeight {
			maxHeight = candidateHeight
		}
	}
	return maxHeight * windowSize
}

// For an array, do binary search on the sizes of possible windows (n possible sizes)
// to find a rectangle of maximal size. O(nlogn)
func maxTotal(arr []int) int {
	maxArea := arr[0]
	// Perform binary search on the delta areas
	// We want to find the first element s.t.
	// the area after it is smaller. TODO we need
	// to prove that this actually correct, but there
	// is an intuition that the shape is parabolic
	// for the areas.
	left := 0
	right := len(arr) 
	for left < right - 1 {
		mid := left + (right - left) / 2
		// fmt.Printf("Trying [%d > %d < %d] \n", left, mid, right) // XXX
		midArea := maxWithSlidingWindowSize(arr, mid)
		midNextArea := maxWithSlidingWindowSize(arr, mid + 1)
		if midNextArea - midArea < 0 {
			// Could be this one but can't be to the right of mid + 1
			// because this one would come before it
			maxArea = midArea
			right = mid + 1
		} else {
			// Has to be the next one or after because
			// this one is increasing
			maxArea = midNextArea
			left = mid + 1
		}
	}

	return maxArea
}

// Our algorithm will operate as follows

// Build and run this by doing
// 1. `go build LargestRectangle84.go`
// 2. `go run LargestRectangle84`
func main() {
	fmt.Println("Trying out problem 84 of Leetcode")
	exampleArr := make([]int, 6)
	exampleArr[0] = 2
	exampleArr[1] = 1
	exampleArr[2] = 5
	exampleArr[3] = 6
	exampleArr[4] = 2
	exampleArr[5] = 3

	max := maxTotal(exampleArr)
	fmt.Printf("The max size is %d\n", max)
}