// Return index of number that meets the following criteria:
//  If last
//    return the last element such that the criterion is met between i and j
//  Else
//    Return the first element such that the criterion is met bween i and j
// i should be inclusive and j should be exclusive (i.e. [0, nums.size())
int bs(vector<int>& nums, int i, int j, bool last, bool (*)(int) criterion) {
    assert(0 <= i && j <= nums.size())

    // Whether the first half is evaluating to true or not
    bool firsthalf_crit = criterion(nums[i]);

    // All False or all True
    if (firsthalf_crit == criterion(j - 1) && j > i + 1) return firsthalf_crit ? (last ? j - 1 : i) : -1;

    // If we pass these statements then
    // last  => True -> False
    // !last => False -> True
    if (last && criterion(nums[j - 1]))                return j - 1;
    if (!last && criterion(nums[i]))                   return i;

    // Find the last element in the first half
    auto in_firsthalf = [](int probe) -> bool { return criterion(probe) == firsthalf_crit; }
    int x = i, y = j;
    while (y - x > 1) {
        int mid = x + (y - x) / 2;
        if (in_firsthalf(nums[mid])) {
            x = mid;
        } else if (nums[mid] > target) {
            y = mid;
        }
    }
    assert(i <= x && x < y && y <= j);
    assert(criterion(nums[x]) == firsthalf_crit);
    if (last) return x;                    // True -> False
    else      return x + 1 >= j ? - 1 : x; // False -> True

    if (last_below && nums[probe] > target || !last_below && nums[probe] < target) return -1;
    return !must_equal ? probe : (nums[probe] == target ? probe : -1);
}