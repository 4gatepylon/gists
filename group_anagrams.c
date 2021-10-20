#ifndef NULL
#define NULL 0
#endif

// We will hash the letter counts to get an index in an array (of length <= num_words) and each will
// have a length <= num_words list of indices. For now, because this is C and dynamic data structures
// are not native, I'm going to make each of these length num_words. Thus we will take up O(n^2) space
// for (O(n) bags) * (1 letter count + 1 indices_len + O(n) indices). With a dynamic data structure
// this could be made smaller. We could also shrink it to around 2000-3000 indices by using uint64_t
// packing like for the letter counts.
struct bag {
    // The unique letter count of this bag of words
    struct letter_count* count;
    // Number of words with this letter count
    int* indices;
    // Index to add at
    int indices_len;
};

// Stores a count of the number of letters in each word. Since we have 26 letters (a-z) and 
// at most 100 letters (<= 7 bits of information which corresponds to 128 states), we can
// use 26 * 7 = 182 bits to store. That comes out to 3 * 64 = 192 bits using three uint64_t.
struct letter_count {
    uint64_t lower;
    uint64_t mid;
    uint64_t upper;
}

// Return whether two letter counts are equal or not
bool eq_letter_count(struct letter_count* c1, struct letter_count* c2) {
    return (c1->lower == c2 -> lower) && (c1->mid == c2->mid) && (c1->upper == c2->upper);
}

// Generate a letter count from a word
struct letter_count* get_letter_count(char* string) {
    uint8_t idx_start, idx_end = 0;
    struct letter_count counts;
    counts.lower = 0;
    counts.mid = 0;
    counts.upper = 0;
    for (int i = 0; string[i] != '\0'; i++) {
        // Encode with 7 bits for each count (<= 127 per letter)
        idx_start = 7 * (string[i] - 97);
        idx_end = idx_start + 7;
        if (idx_start >= 128) {
            idx_start -= 128;
            counts.upper = ((counts.upper >> idx_start) + 1) << idx_start;
        } else if (idx_end >= 128) {
            idx_end -= 128;
            idx_start -= 64;
            // It only overflows if the relative lower its are all `1` and then
            // we can simply add to the relative upper bits instead
            if ((counts.mid >> idx_start) + 1 >= (1 << (64 - idx_start))) {
                counts.mid = (counts.mid & ((1 << idx_start) - 1));
                counts.upper ++;
            } else {
                // Same as the cases where the range falls inside a single uint64_t
                counts.mid = ((counts.mid >> idx_start) + 1) << idx_start
            }
        } else if (idx_start >= 64) {
            idx_start -= 64;
            counts.mid = ((counts.mid >> idx_start) + 1) << idx_start;
        } else if (idx_end >= 64) {
            idx_end -= 64;
            // Same idea as idx_end >= 128
            if ((counts.lower >> idx_start) + 1 >= (1 << (64 - idx_start))) {
                counts.lower = (counts.lower * ((1 << idx_start) -1));
                counts.mid ++;
            } else {
                counts.lower = ((counts.lower >> idx_start) + 1) << idx_start
            }
        } else {
            counts.lower = ((counts.lower >> idx_start) + 1) << idx_start
        }
    }
    return &counts;
}

char *** groupAnagrams(char ** strs, int strsSize, int* returnSize, int** returnColumnSizes){
    // TODO
    // 1. create an array of strings called output with length strsSize
    // 2. initialize int* column_sizes to keep track of column sizes
    // 3. create any necessary DSs
    // 4. do some sort of algorithm
    // 5. place the words onto the list some way

    // LASTLY Create an array of pointers and have them point into the array from 1 when we know the sub-lengths
    // AND initialize returnSize to the size of this array, also initialize returnColumnSizes to be the columnsizes

}