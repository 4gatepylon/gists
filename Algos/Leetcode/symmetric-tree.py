# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
def equiv(t1: Optional[TreeNode], t2: Optional[TreeNode]) -> bool:
    return (
        t1 is None and t2 is None or 
        (
            t1 is not None and t2 is not None and
            t1.val == t2.val and
            equiv(t1.left, t2.right) and
            equiv(t1.right, t2.left)
        )
    )
class Solution:
    def isSymmetric(self, root: Optional[TreeNode]) -> bool:
        return root is None or equiv(root.left, root.right)
