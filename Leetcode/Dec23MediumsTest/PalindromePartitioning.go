package main

include "fmt"
// From this problem: https://leetcode.com/problems/palindrome-partitioning/
// Find all palindromes starting at the first index
// Solve to the right of that location
// For each index, keep a list of the partitions that are valid for that index

func pali(s string) bool {
    for i := 0; i < len(s) / 2; i++ { if s[i] != s[len(s) - 1 - i] { return false } }
    return true
}
// Memo has at most O(n) elements
func fill_memo(s string, i int, memo [][][]string) {
    // fmt.Printf("Calling Fill memo at index %d\n", i)
    if i > len(s) { panic("Recursion should stop at len(s)") }
    if i == len(s) || memo[i] != nil { /*fmt.Printf("At %d has stored %d partitions\n", i, len(memo[i])); */return }

    // fmt.Printf("Filling memo at index %d\n", i)

    partitions := make([][]string, 0)
    // O(n) iterations
    for j := i+1; j <= len(s); j++ {
        // O(n) to check if pali
        if pali(s[i:j]) {
            // This will cut off if the memo is already full there (at most slow, once)
            fill_memo(s, j, memo)
            
            // O(n * #partitions) since each partition is O(n) to copy
            for _, remaining_partitioning := range memo[j] {
                // fmt.Printf("Found remaining partition of length %d\n", len(remaining_partitioning)); // DEBUG

                partition := make([]string, 1)
                partition[0] = s[i:j]
                // This should be by copying... I hope!
                partition = append(partition, remaining_partitioning...)
                partitions = append(partitions, partition)
                // fmt.Printf("Partitions now has length %d\n", len(partitions)) // DEBUG
            }
        }
    }
    // fmt.Printf("Partitions at the very end (i=%d) now has length %d\n", i, len(partitions)) // DEBUG
    memo[i] = partitions
}
func partition(s string) [][]string {
    // [i][][] -> palindromes starting at index i (where [][] just means that it is 
    // a set of partitionings, i.e. a set of lists of strings)
    memo := make([][][]string, len(s)+1, len(s)+1)
    // The partitionings of the empty string are just the empty string
    memo[len(s)] = make([][]string, 1)
    memo[len(s)][0] = make([]string, 0)
    // O(n^3)
    fill_memo(s, 0, memo)
    return memo[0];
}