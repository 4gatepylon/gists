class Solution:
    def firstMissingPositive(self, nums: List[int]) -> int:
        mx = max(nums) + 1
        if mx <= 0:
            return 1
        candidate = mx
        for num in nums:
            if num > 0 and num < candidate:
                candidate = num - 1
        if candidate <= 0:
            return mx
        return candidate
