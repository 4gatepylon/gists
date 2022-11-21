use std::collections::HashMap;

impl Solution {
    pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
        let mut value2idx: HashMap<i32, i32> = HashMap::new();
        // Enumerate seems to return an index and a pointer (assuming that
        // this is like a vector of objects of some kind)
        for (idx, elem) in nums.iter().enumerate() {
            let idxv: i32 = idx as i32;
            let ptarget: i32 = target - *elem;
            match value2idx.get(&ptarget) {    
                Some(pidx) => {
                    return vec![idxv, *pidx];
                }
                None => {
                    // Not sure what will happen here if we already have the value in the array
                    value2idx.insert(*elem, idxv);
                }
            }
        }
        // Did not find (guaranteed to not happen)
        return vec![-1, -1];
    }
}