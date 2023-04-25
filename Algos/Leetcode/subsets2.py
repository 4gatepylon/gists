def dedupedSubsets(i: int, uniqueNums: List[Tuple[int, int]]) -> List[List[int]]:
    print(f"calling at i={i}")
    if i >= len(uniqueNums):
        return [[]]

    subs = []
    # Otherwise, every subset is made of all subsets of the remaining elements either with
    # or without the latest element.
    subs_of_remaining = dedupedSubsets(i + 1, uniqueNums)
    val, max_length = uniqueNums[i]
    for sub_remaining in subs_of_remaining:
        # By symmetry only the number of times we pick an identical
        # number matters
        for length in range(0, max_length + 1):
            sub_to_add = [val] * length
            sub_with_that_length = sub_to_add + sub_remaining
            subs.append(sub_with_that_length)
    return subs

def uniqueKV(nums: List[int]) -> List[Tuple[int, int]]:
    allNums: List[int] = sorted(nums)
    assert len(allNums) >= 1

    # Turn into a format that takes into account the repeated elements
    # i.e. [1, 2, 2] -> [(1, 1), (2, 2)]
    uniqueNums: List[Tuple[int, int]] = []
    i = None
    for j in range(len(allNums)):
        if i is None or allNums[i] != allNums[j]:
            if i is not None:
                print(f"append from {i} to {j}")
                uniqueNums.append((allNums[i], j - i))
            i = j
    assert i is not None
    uniqueNums.append((allNums[i], len(allNums) - i))
    return uniqueNums

class Solution:
    def subsetsWithDup(self, nums: List[int]) -> List[List[int]]:
        return dedupedSubsets(0, uniqueKV(nums))
