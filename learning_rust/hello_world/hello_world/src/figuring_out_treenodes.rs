use std::rc::Rc;
use std::cell::RefCell;

#[derive(PartialEq)]
pub struct Dummy {
    pub v: i32
}

impl Dummy {
    #[inline]
    pub fn new() -> Self {
        Dummy {
            v: 3
        }
    }
}

// Definition for a binary tree node.
#[derive(Debug, PartialEq, Eq)]
pub struct TreeNode {
  pub val: i32,
  pub left: Option<Rc<RefCell<TreeNode>>>,
  pub right: Option<Rc<RefCell<TreeNode>>>,
}

impl TreeNode {
  #[inline]
  pub fn new(val: i32) -> Self {
    TreeNode {
      val,
      left: None,
      right: None
    }
  }
}

fn main_dummy() {
    let rc_examples = TreeNode::new(3);
    // let little_rc: Rc<TreeNode> = Rc::new(rc_examples);
    // assert!(little_rc.val == 3);

    let rf: RefCell<TreeNode> = RefCell::new(rc_examples);
    assert!(rf.borrow().val == 3);

    let rc = Rc::new(rf);
    assert!(rc.borrow().val == 3);

    let root_node = rc;
    let child: Rc<RefCell<TreeNode>> = Rc::new(RefCell::new(TreeNode::new(6)));
    root_node.borrow_mut().left = Some(child);

    // First borrow
    let v: i32 = root_node.borrow().val;
    // Then borrow again for another usage
    let l: Option<Rc<RefCell<TreeNode>>> = root_node.borrow().left.clone();
    let inverted = TreeNode {
        val: v,
        left: l,
        right: None
    };
    println!("{}", inverted.val);

    let opt: Option<i32> = None;
    assert!(opt.clone().is_none());
}