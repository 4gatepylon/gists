#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif

void reverse(char* permutation, int start, int end) {
    int i = start;
    int j = end;
    while (i < j) {
        char tmp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = tmp;
        i++;
        j--;
    }
}

void nextPermutation(char* permutation, int n) {
    assert(n > 0);
    if (n <= 1) {
        return;
    }
    assert(n > 1);
    assert(strlen(permutation) == n);
    // `n` will be a special value signifying "largest permutation"
    int pivot = -1;
    int lub = 0;
    for (int i = 0; i < n - 1; i++) {
        assert(permutation[i] != permutation[i + 1]);
        if (permutation[i] > permutation[i + 1] && pivot != -1) {
            // Descending: if you find a lower upper bound then replace it
            if (permutation[pivot] < permutation[i + 1] && permutation[i + 1] < permutation[lub]) {
                lub = i + 1;
            }
        } else if (permutation[i] < permutation[i + 1]) {
            // Ascending: you are not yet at the rightmost descending substring
            pivot = i;
            lub = i + 1;
        }
    }
    if (pivot == -1) {
        // Largest permutation, so just swap
        reverse(permutation, 0, n - 1);
        return;
    }
    assert(pivot < lub);
    assert(0 <= pivot && pivot < n - 1);
    assert(0 < lub && lub < n);
    // Swap the pivot and the lub, then just reverse the rightmost substring
    char tmp = permutation[pivot];
    permutation[pivot] = permutation[lub];
    permutation[lub] = tmp;
    reverse(permutation, pivot + 1, n - 1);
}

char* getPermutation(int n, int k){
    // This code does not support double digits (the problem doesn't require it)
    assert(n < 10);

    // Create the lowest permutation
    char* permutation = malloc(n + 1);
    memset(permutation, 0, (n + 1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        permutation[i] = (char)((i + 1) + (int)'0');
    }

    // Interesting to see if there is a better way to do this (i.e. not having to compute
    // k times).
    for (int i = 1; i < k; i++) {
        nextPermutation(permutation, n);
    }
    return permutation;
}

int fac(int n) {
    assert(n >= 0);
    if (n <= 1) {
        return 1;
    }
    return n * fac(n - 1);
}

int main() {
    // Example run-through of permutations of size 3 (should be 6 of them)
    int n = 3;
    char* permutation = malloc(n + 1);
    memset(permutation, 0, n * sizeof(char));

    for (int i = 0; i < n; i++) {
        permutation[i] = (char)((i + 1) + (int)'0');
    }
    int fn = fac(n);
    for (int i = 1; i <= fn; i++) {
        printf("%d: %s\n", i, permutation);
        nextPermutation(permutation, n);
    }
    // This one should be the same as the beginning (our code should loop around)
    printf("1: %s\n", permutation);
    return 0;
}

// Alternative solution that is faster "with `binned` approach"

