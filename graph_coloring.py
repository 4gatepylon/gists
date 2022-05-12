from copy import copy
from pprint import PrettyPrinter
pp = PrettyPrinter(indent=2)

# cool to look into https://vldb.org/pvldb/vol11/p338-yuan.pdf

def pick(elements):
    for element in elements:
        return element

# O(n|C|^n)... not sure how to lower to O(|C|^n)
def gc(keys, i, choices, graph, colors):
    # print("keys:", keys)
    # print("i:", i)
    # print("choices:", choices)
    # print("graph:", graph)
    # print("colors:", colors)
    
    if i == len(keys):
        return True
    elif len(colors) == 0:
        return False
    
    # Get the available colors
    avail = copy(colors)
    for neighbor in graph[keys[i]]:
        if choices[neighbor] != None and choices[neighbor] in avail:
            avail.remove(choices[neighbor])
    
    # Try to color which each valid color
    for color in avail:
        choices[i] = color
        
        # Signify first whether early termination and then whether the graph coloring is valid
        works = gc(keys, i+1, choices, graph, colors)
        if works:
            # The rest of the graph is solved, return
            return True
        else:
            choices[i] = None

    return False

def chaitlins(graph, choices, stack, colors):
    if len(graph) == 0:
        return True

    avail = copy(colors)
    node = stack.pop()
    for neighbor in graph[node]:
        if choices[neighbor] != None and choices[neighbor] in avail:
            avail.remove(choices[neighbor])
    for color in avail:
        choices[node] = color
        if chaitlins(graph, choices, stack, colors):
            return True
        stack.pop()
        choices[node] = None
    return True
     

# Memory intensive but runtime is O(|C|^n) in the worst case, but ideally it's not bad
# because we will sort the graph keys by their degree, which means that by symmetry we should prune
# pretty quickly.
def graph_coloring(graph, colors):
    # O(|C|n) space and time
    keys = sorted(graph.keys(),key=lambda key: len(graph[key]), reverse=True)
    choices = [None for _ in range(len(graph))]
    works = gc(keys, 0, choices, graph, colors)
    if works:
        return True, {keys[i] : choices[i] for i in range(len(keys))}
    return False, None

if __name__ == "__main__":
    graph = {
        0: [1, 2, 3, 4, 5, 6, 7],
        1: [6, 7],
        2: [0, 1, 5, 6, 7],
        3: [0, 2, 4],
        4: [0, 2, 5, 6, 7],
        5: [0, 1, 2],
        6: [0, 2, 3, 4, 5],
        7: [0, 1, 2, 6],
    }

    # Complete
    big_graph = {
        x : set((y for y in range(100))) for x in range(100)
    }
    for i in range(100):
        big_graph[i].remove(i)

    # Test
    for key, items in graph.items():
        assert not key in items
    for i in range(8):
        print("********************")
        works, coloring = graph_coloring(graph, set(range(i)))
        if works:
            pp.pprint(coloring)
        else:
            print(f"Can't color with {i} colors")
        print("********************\n")
    print("********************************************************************************\n")
    # You can see that some graphs are very hard!
    for i in range(101):
        print("********************")
        works, coloring = graph_coloring(big_graph, set(range(i)))
        if works:
            pp.pprint(coloring)
        else:
            print(f"Can't color with {i} colors")
        print("********************\n")