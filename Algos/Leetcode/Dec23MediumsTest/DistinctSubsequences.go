package main // Eh...

import "fmt"

// From https://leetcode.com/problems/distinct-subsequences/

func numDistinct(s string, t string) int {
    memo := make([][]int, len(s) + 1)
    // Empty t means there is one subsequence (the empty one) that works
    for i := 0; i < len(s) + 1; i ++ {
        memo[i] = make([]int, len(t) + 1)
        memo[i][len(t)] = 1
    }
    // Empty s, but non-empty t means there are no working subsequences
    for j := 0; j < len(t); j ++ { memo[len(s)][j] = 0 }
    if (memo[len(s)][len(t)] != 1) { panic("There should be one way to create an empty subsequence with an empty subsequence") }

    // Do 2D DP where at each step either you take the character
    // (if possible) or don't (always possible)
    for i := len(s) - 1; i >= 0; i -- {
        for j := len(t) - 1; j >= 0; j -- {
            // j = don't take the character
            // i + 1 = take the character
            memo[i][j] = memo[i+1][j]
            if (s[i] == t[j]) { memo[i][j] += memo[i+1][j+1] }
        }
    }    
    return memo[0][0]
}