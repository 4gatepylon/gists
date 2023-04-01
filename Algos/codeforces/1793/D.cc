// O(n^2f) is just to find the MEX for each pair of indices
// f can be O(n) since we can find the MEX by having an array
// of pointers to a linkedlist. This puts our runtime at O(n^3)
// which is not ideal...


// An idea that does not require is to calculate all this BS:
// - All singletons work except where the 1's are unless they are aligned
// - All doubles work so long as (1) neither includes 1, (2) both include 1, but neither includes 2, (3) both include 1 and 2
// - All triples work so long as (1), (2), (3) but neither include 3, or all include 1, 2, and 3
// etc...
// There are (nC2) such elements

// We can look at a given array segment with a MEX
// The MEX of the segment with something appended is MEX + 1

// don't have time to finish this!