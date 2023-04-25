# Goal
# ----
# You will be entering a maze filled with traps. Your goal is to get out of it!

# API
# ---
# ENDPOINT = “https://loud-dingos-drive-66-108-228-33.loca.lt/“
# The maze will be represented as an API with one GET endpoint.
# To enter the maze, head over to https://loud-dingos-drive-66-108-228-33.loca.lt/
# To go to a specific step in the maze, GET /<STEP_ID>
# The final step of the maze will be a url that returns a "CONGRATS" message

# Instructions
# ------------
# - Print the STEP_ID of the final step

import requests
import json
import random
import pathlib
from pprint import PrettyPrinter
pprint = PrettyPrinter(indent=4).pprint

class Graph:
    DEFAULT_STORE = './graph.json'
    SOURCE_NAME = 'SOURCE'

    # {
    #   "nodes": {
    #       ['name' : str] : {
    #       'name' : str
    #       'to'   : set()
    #   }},
    #   'visited': set()
    # }
    @staticmethod
    def visited_to_list(g):
        return {k : (v if k != 'visited' else list(v)) for k, v in g.items()}
    @staticmethod
    def visited_to_set(g):
        return {k : (v if k != 'visited' else set(v)) for k, v in g.items()}

    def __init__(self, store_loc=None, load_store=True, url_base='https://loud-dingos-drive-66-108-228-33.loca.lt'):
        self.url_base = url_base
        self.store_loc = store_loc if store_loc else Graph.DEFAULT_STORE
        self.graph =  {
            "visited": set(),
            "nodes": {}
        }
        if load_store and pathlib.Path(self.store_loc).exists():
            self.graph = json.loads(self.store_loc)
            self.graph = Graph.visited_to_set(self.graph)
        
        source_neighbors = self.go_to('')
        self.graph['nodes'][Graph.SOURCE_NAME] = {
            'name': Graph.SOURCE_NAME,
            'to': set(source_neighbors)
        }
        pprint(self.graph)
    
    # Go somewhere and then return the list of places that place goes to
    def go_to(self, next_step: str) -> list:
        req = requests.get(f"{self.url_base}/{next_step}")
        print(f"*** Going to {next_step}") # TODO remove this
        try:
            return req.json()['next_steps']
        except Exception as e:
            print(f"*** In going to {next_step} we got Exception {e}")
            return []
    
    # Save the graph to the self.store_loc file as a JSON
    def store(self):
        with open(self.store_loc, 'w') as f:
            json.dump(Graph.visited_to_list(self.graph), f)
    
    # Increment will try to expand our graph by one node
    # - where: can choose where to increment from (i.e. what node to try to get a new one from
    # - do_store: call the store function after a successful increment
    # NOTE doesn't check that it's a valid node
    def __increment(self, where: str) -> bool:
        # TODO there is a bug here where the node we pick should be the from node not the to node
        # FIXME
        print(f"Increment at {where}") # TODO remove this
        for to in self.graph[where]['to']:
            if to not in self.graph['visited']:
                try:
                    tos_subsequent_neighbors = self.go_to(to)
                    self.graph['nodes'][to] = {
                        'name': to,
                        'to': set(tos_subsequent_neighbors)
                    }
                    self.graph['visited'].add(to)
                    return True
                except Exception as e:
                    # TODO log (but avoid logspam in case re-thrown)
                    return False
        return False
    
    def increment(self, where=None, do_store=True) -> bool:
        where = where if where else random.choice(list(self.graph["nodes"].keys()))
        if where not in self.graph["nodes"]:
            # TODO might want to just try to add new nodes out of the blue in the future if we want to do a random/grid serach
            print("Tried a non-valid node!")
            return False
        
        # Invoke helper
        incr = self.__increment(where)
        if incr and do_store:
            self.store()
        return incr
    
    # Keep calling increment (i.e. run a BFS of sorts)
    # TODO make this pick the right node to bfs from... may want additional state
    def increment_forever(self, max_count=None):
        count = 0
        while max_count is None or count < max_count:
            self.increment()
            count += 1

if __name__ == '__main__':
    g = Graph()
    g.increment_forever(max_count=20)