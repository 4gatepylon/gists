class Solution:
    def isScramble(self, s1: str, s2: str) -> bool:
        return self.isScrambleUnique(s1, s2, greatest_split_only=True) # Does this just work? Apparently not :(
    def isScrambleUnique(self, s1: str, s2: str, greatest_split_only=False) -> bool:
        assert len(s1) == len(s2), f"s1 is {s1}, but s2 is {s2}"
        # print("SOLVE", s1, s2) # FIXME
        if len(s1) <= 3:
            return sorted(s1) == sorted(s2)
        for d in range(-1, 2, 2):
            assert d == -1 or d == 1
            pre_start = -1 if d == 1 else len(s1)
            start = 0 if d == 1 else len(s1) - 1
            end = len(s1) - 1 if d == 1 else 0 # Ignore the last one because inclusivity
            
            prefixes_s1 = [-1]
            prefixes_s2 = [pre_start]
            # Errors map character to how many more s2 has
            errors = {}
            # Prefix is all indices < i and suffix is all indices > i
            for i, j in enumerate(range(start, end, d)):
                # Add the next index
                c1 = s1[i]
                c2 = s2[j]
                if c1 != c2:
                    for c, dc in [(c1, -1), (c2, 1)]:
                        errors[c] = dc if not c in errors else errors[c] + dc
                        if errors[c] == 0:
                            del errors[c]
                # print("---", i, j, "<", end, c1, c2, errors) # FIXME
                if len(errors) == 0:
                    prefixes_s1.append(i)
                    prefixes_s2.append(j)
            # All are prefixes or all are suffixes and there should be least one or it is impossible
            assert len(prefixes_s1) == len(prefixes_s2)
            # print(prefixes_s1) # FIXME
            # print(prefixes_s2) # FIXME
            if len(prefixes_s1) > 1:
                if greatest_split_only:
                    prefixes_s1 = [-1, prefixes_s1[-1]]
                    prefixes_s2 = [pre_start, prefixes_s2[-1]]

                sol_found = True

                post_end = len(s1) - 1 if d == 1 else 0 # Oops: had to be inclusive cuz above too
                prefixes_s1.append(len(s1) - 1)
                prefixes_s2.append(post_end)
                # print(prefixes_s1) # FIXME
                # print(prefixes_s2) # FIXME

                assert len(prefixes_s1) > 2
                for k in range(0, len(prefixes_s1) - 1, 1):
                    # Oops: had to increment by one because incluse instead of exclusive
                    i1 = prefixes_s1[k] + 1
                    j1 = prefixes_s1[k + 1] + 1
                    assert i1 < j1

                    # Oops: didn't make it symmetric
                    i2 = prefixes_s2[k] + (1 if d == 1 else 0)
                    j2 = prefixes_s2[k + 1] + (1 if d == 1 else 0)
                    i2, j2 = min(i2, j2), max(i2, j2)
                    assert i2 < j2

                    # TODO avoid copying to save O(n) + memory (I guess caching? This should be read-only possible.)
                    p1 = s1[i1:j1]
                    p2 = s2[i2:j2]

                    # print(i1, j1, "same as", i2, j2, "for", p1, p2, "forwards" if d == 1 else "backwards") # FIXME
                    sol_found = sol_found and self.isScrambleUnique(p1, p2)
                    if not sol_found:
                        break
                if sol_found:
                    # print("GOT", True) # FIXME
                    return True

        # print("GOT", False) # FIXME
        return False

# FIXME: failed because didn't consider duplicate characters case!
if __name__ == '__main__':
    s = Solution()
    tests = [
        # ("great", "rgeat", True),
        # ("abcde", "caebd", False),
        # ("a", "a", True),
        # ("a", "b", False),
        # ("ab", "ba", True),
        # ("ab", "ab", True),
        # ("abcdbdacbdac", "bdacabcdbdac", True),
        ("bccbccaaabab", "ccababcaabcb", False),
    ]
    for test in tests:
        s1, s2, exp = test
        ans = s.isScramble(s1, s2)
        if exp != ans:
            print(f"FAIL. s1: {s1}, s2: {s2}, exp: {exp}, ans: {ans}")