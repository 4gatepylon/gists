// Very painful to figure out how borrowing/cloning works here
// (turns out because we are cloning an `Rc` (reference count)
// it is actually very cheap to count it (we just create a new
// reference that is counted))
use std::rc::Rc;
use std::cell::RefCell;
impl Solution {
    pub fn invert_tree(root: Option<Rc<RefCell<TreeNode>>>) -> Option<Rc<RefCell<TreeNode>>> {
        return match root {
            Some(rootNode) => {
                Some(Rc::new(RefCell::new(
                    TreeNode {
                        val: rootNode.borrow().val,
                        left: Solution::invert_tree(rootNode.borrow().right.clone()),
                        right: Solution::invert_tree(rootNode.borrow().left.clone())
                    }
                )))
            }
            None => {
                None
            }
        }
    }
}