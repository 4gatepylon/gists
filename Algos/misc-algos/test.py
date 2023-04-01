# a counter Trie
# class Trie:
#     def __init__(self, count=None):
#         self.children = {}
#         self.count = count
#         self.temp_count = count
    
#     @staticmethod
#     def add(trie, word):
#         for char in word:
#             if char in trie.children:
#                 trie = trie.children[char]
#             else:
#                 _trie = Trie()
#                 trie.children[char] = _trie
#                 trie = _trie
#         trie.count = 1 if trie.count == 0 else trie.count + 1

# # O(NM) for N = len(s) and M = sum of len(word) in words
# def substrings(long_string, words):
#     indices = []

#     root = Trie() # root trie
#     total_chars = 0
#     for word in words:
#         Trie.add(trie, word)
#         total_chars += len(word)
#     for i in range(len(long_string)):
#         trie = root
#         failed = False
#         for j in range(i, i + total_chars):
#             char = long_string[j]
#             if not char in trie:
#     return indices

# for s, words in (("barfoothefoobarman", ["foo","bar"]), ("wordgoodgoodgoodbestword", ["word","good","best","word"]), ("barfoofoobarthefoobarman", ["bar","foo","the"])):
#     print(indices(substrings(s, words)))


# the next permutation if a list can be found like this
# 1. find first the first index after which it is no longer sorted in descending order
#    (sorted in descending order is the "max" permutation)
#    call the element that breaks the sort "breaker"
# 2. check if the sublist AFTER breaker is sorted in descending order
# 3.1 if it is NOT then simply find the next permutation of this sublist
# 3.2 if it IS then find the lowest element ABOVE the breaker by value to the right and swap them 
#     then sort the array to the right of the breaker in ascending order
# 3.2.1 if there is NO element bigger than that element to the right of the breaker... is this possible?
def next_permutation(arr, i, j):
    pass