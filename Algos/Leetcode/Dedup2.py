class Solution:
    def removeDuplicates(self, nums: List[int]) -> int:
        # demented closure rules?
        insertion_point = [0]
        read_point = [0]
        num_dups = [0]
        curr_count = [0]

        def updateRead():
            if read_point[0] > 0 and nums[read_point[0]] != nums[read_point[0] - 1]:
                curr_count[0] = 0

        def insertIfOkElseReadOn():
            if curr_count[0] < 2:
                nums[insertion_point[0]] = nums[read_point[0]]
                curr_count[0] += 1

                insertion_point[0] += 1
            else:
                num_dups[0] += 1
            read_point[0] += 1

        while read_point[0] < len(nums):
            updateRead()
            insertIfOkElseReadOn()

        return len(nums) - num_dups[0]
