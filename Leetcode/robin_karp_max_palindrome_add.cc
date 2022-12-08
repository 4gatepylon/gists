class Solution {
public:
    // We will have a rolling hash function for `Robin Karp`
    // Base 27 cuz it's is co-prime with num buckets
    // (not really a number of buckets, but just a modulus
    // that keeps our hashes from overflowing)
    uint64_t num_buckets = 1ull << 63; // Largest power of two such that 
    uint64_t largest_pow = 1ull << 58; // 63 - 5 = 58, and 2^5 = 32 is the smallest power of two above 27
    uint64_t base = 27ull;
    uint64_t pow = 1ull;
    // The start is the point to start copying reversed from
    int start = 1;
    // Find the longest prefix that is a palindrome
    string shortestPalindrome(string s) {
        // Store the hash of this prefix going forwards and going backwards
        uint64_t hash_forwards = 0;   // MSD left
        uint64_t hash_backwards = 0;  // LSD left
        for (int i = 0; i < s.size(); i ++) {
            #define MODDED(x, p) ((x) % (p))
            hash_backwards = MODDED(hash_backwards * base + (uint64_t)(s[i] - 'a'), num_buckets);
            hash_forwards = MODDED(hash_forwards + pow * (uint64_t)(s[i] - 'a'), num_buckets);
            pow *= base;
            // This will introduce collisions but if the string is like over 10-20 characters it is necessary...
            pow = MODDED(pow, largest_pow);

            if (hash_forwards == hash_backwards) {
                // Check that the prefix is indeed a palindrome
                // (iff length is even then it's an even palindrome
                // and vice versa; an even palindrome has a pair as
                // its center)
                // Odd index <=> even length from start because of zero-indexxing (and we are inclusive)
                int center_left =  i / 2;
                int center_right = i & 1u ? i / 2 + 1 : i / 2;
                assert((i & 1u) && center_left + 1 == center_right || !(i & 1u) && center_left == center_right);
                int R = 0;
                bool is_pali = true;
                while (center_right + R <= i) {
                    assert(center_right + R < s.size());
                    assert(0 <= center_left - R);
                    if (s[center_left - R] != s[center_right + R]) {
                        is_pali = false;
                        break;
                    }
                    R ++;
                }
                // We are a palindrome up until i so we want to copy after i
                if (is_pali) start = i + 1;
            }
        }

        // Copy over the palindrome
        vector<char> copy;
        for (int j = s.size() - 1; j >= start; j--) { copy.push_back(s[j]); }
        for (int j = 0; j < s.size(); j++) { copy.push_back(s[j]); }
        return string(copy.begin(), copy.end());
    }
};