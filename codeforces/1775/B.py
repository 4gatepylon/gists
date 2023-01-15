from __future__ import annotations

# -- TEMPLATE --
import os
import sys
from atexit import register
from io import BytesIO
sys.stdout = BytesIO()
# THIS WILL MAKE IT PRINT EVERYTHING ALL AT ONCE IN THE END
register(lambda: os.write(1, sys.stdout.getvalue()))
input = BytesIO(os.read(0, os.fstat(0).st_size)).readline
print = lambda x: sys.stdout.write(bytes(f"{x}\n", "utf-8"))
# -- END TEMPLATE --

# If there exists two different subsequences that have the same bitwise OR, then
# I can just add all the remaining elements to one of them and all those same elements
# to the other one (via bitwise OR) and that will yield the bitwise OR of the whole
# array. So if there exists such subsequence then there must exist a subsequence that
# is not the enire array that contains all the 1's that the entire array contains.
#
# That means that if there does not exist a subsequence that contains all the 1's of
# the entire array there do not exist such pairs of subsequences. Thus, I will simply create
# a set of the indices for the entire array, and then we'll look to see if there's any element
# we can remove to create that same subsequence, since if there is a valid subsequence of
# our kind then there is a similar one that has all the elements minus a single one that is not
# in the global array in our set.
#
# We can use a frequency map for this. Time is linear. (really its O(cn) where c is the number of
# indices per location)
def exists_subsq(indices_by_loc: list[list[int]]) -> bool:
    counts = {}
    for indices in indices_by_loc:
        assert len(set(indices)) == len(indices)
        for i in indices:
            if i in counts:
                counts[i] += 1
            else:
                counts[i] = 1
    for indices in indices_by_loc:
        bottlenecked = False
        for i in indices:
            if counts[i] == 1:
                bottlenecked = True
                break
        if not bottlenecked:
            return True
    return False

# There are t test-cases in total so we'll take O(tn(log(n) + log(m))) in total.
# That will be less than O(100,000*log(100)) iterations, which for log-base 2 is
# at worst O(700,000) iterations which should fall within bounds (hopefully).
T = int(input())
for _ in range(T):
    # For each test case get the first line as n and m if necessary (we don't have to
    # because our language gets the lengths fortunately)
    # and then get the lines for a and b
    #
    n = int(input())
    indices_by_loc = []
    for _ in range(n):
        info = list(map(int, filter(lambda x: len(x) > 0, input().split())))
        indices_by_loc.append(info[1:])
    assert len(indices_by_loc) > 0
    print("Yes" if exists_subsq(indices_by_loc) else "No")