class Tree(object):
    # Initialize a tree
    #   `name`    : usually an integer uniquely identifying the node
    #   `children`: a list of Tree objects
    #   `meta`    : some metadata about this node
    def __init__(self, 
        name,
        children=[],
        meta=None,
    ):
        self.name = name
        self.children = children
        self.meta = meta
    
    # Use DFS to populate the meta field
    #   `reducer`          : a function that takes in two values of the same type and returns a combination of them
    #   `reduction_default`: default value of the reducer's domain; reduction with it should yield the identity
    #   `passdown`         : a function that takes a value the parent passes down and transforms it to that for this node
    #   `passdown_value`   : what the parent passed down
    def dfs_meta(self, 
        reducer=lambda x, y: None, 
        reduction_default=None, 
        passdown=lambda x: None,
        passdown_value=None,
    ):
        reduction = reduction_default
        passdown_value = passdown(passdown_value)

        reduction = reducer(reduction, passdown_value)
        for child in self.children:
            # Recursively calculate the children's values
            child.dfs_meta(
                reducer,
                reduction_default=reduction_default, 
                passdown=passdown,
                passdown_value=passdown_value,
            )
            # Reduce upwards the tree
            # NOTE if you want to use the passdown value, then just keep it in a 
            # vector or other data structure in the meta
            reduction = reducer(child.meta, reduction)
        
        # ...
        self.meta = reduction
        # ...
    
    # Enumerate the subtrees (nodes)
    def subtrees(self):
        st = [self]
        for child in self.children:
            st += child.subtrees()
        return st
    
    @staticmethod
    def adj_dict2size_tree(d, root=None):
        if root is None:
            for key in d.keys():
                root = key
                break
            assert(not root is None), "Empty Tree Not Allowed"
        
        visited = set(root)
        def create(d, root_name, visited):
            children_names = [c for c in d[root_name] if not c in visited]

            # Make sure we can't visit again
            children_trees = []
            for c in children_names:
                visited.add(c)
                children_trees.append(create(d, c, visited))
            return Tree(root, children=children_trees, meta=None)
        
        tree = create(d, root, visited)
        tree.dfs_meta(
            reducer=lambda x, y: x + y, 
            reduction_default = 1, 
            passdown = 0, 
            passdown_value = 0,
        )
        return tree
    
        
def edge_split(adj_dict):
    t = Tree.adj_dict2size_tree(adj_dict, root=None)
    # All subtrees except the root
    st = t.subtrees()
    edges = {}
    for parent in st:
        for child in parent.children:
            edges[(parent, child)] = (len(st) - child.meta, child.meta)
            edges[(child, parent)] = (child.meta, len(st) - child.meta)
    assert(len(edges) == 2 * (len(st) - 1)), "Trees mathematically should behave a certain way"
    return edges

if __name__ == "__main__":
    pass