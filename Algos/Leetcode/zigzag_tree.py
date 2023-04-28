 Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
def flatten(l):
    o = []
    for x in l:
        for y in x:
            o.append(y)
    return o

def vals(l):
    return [x.val for x in l]

class Solution:
    def zigzagLevelOrder(self, root: Optional[TreeNode]) -> List[List[int]]:
        if root is None:
            return []
        traversals = [[root]]
        backwards = False
        while len(traversals[-1]) > 0:
            # Be able to read the level queue
            level_queue_forwards = list(reversed(traversals[-1])) if backwards else traversals[-1]

            # Create the next level queue
            backwards = not backwards
            next_level_queue_forwards = [[x.left, x.right] for x in level_queue_forwards]
            next_level_queue_forwards = [x for x in flatten(next_level_queue_forwards) if x is not None]

            # Put in right order and append
            next_level_queue = list(reversed(next_level_queue_forwards) if backwards else next_level_queue_forwards)
            traversals.append(next_level_queue)
        
        traversals = traversals[:-1]
        traversals_as_values = [[x.val for x in l] for l in traversals]
        return traversals_as_values
