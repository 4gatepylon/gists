// Definition for singly-linked list.
#[derive(PartialEq, Eq, Clone, Debug)]
pub struct ListNode {
  pub val: i32,
  pub next: Option<Box<ListNode>>
}

impl ListNode {
  #[inline]
  fn new(val: i32) -> Self {
    ListNode {
      next: None,
      val
    }
  }
}

use std::rc::Rc;
fn main() {
    // list 2 < list 1
    // Note that this works because ListNode's new function is inlined...
    // otherwise I'm not sure the creation of the new box would actually capture it?
    // not sure how boxes work...
    let list1: Option<Box<ListNode>> = Some(Box::new(ListNode::new(32)));
    let list2: Option<Box<ListNode>> = Some(Box::new(ListNode::new(10)));
    assert!(!list1.is_none() && !list2.is_none());

    let list1_bigger: bool = list1.as_ref().unwrap().val < list2.as_ref().unwrap().val;
    let head: Rc<Box<ListNode>> = Rc::new(if list1_bigger {list1.unwrap()} else {list2.unwrap()});
    let h1: Rc<Box<ListNode>> = Rc::new(if list1_bigger {head.clone().next.unwrap()} else {list2.unwrap()});
}