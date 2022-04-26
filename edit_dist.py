lass Solution:
    def minDistance(self, word1: str, word2: str) -> int:
        table = [[None for _ in range(len(word2) + 1)] for _ in range(len(word1)+1)]
        sq = md(word1, 0, word2, 0, table)
        return sq #[-1][0]

# commented out sequence generator
def md(w1, i, w2, j, table):
    if not table[i][j] is None:
        return table[i][j]
    else:
        if i == len(w1):
            ret = len(w2) - j #[(len(w2) -j, f"insert {len(w2) - j}")]
        elif j == len(w2):
            ret = len(w1) - i #[(len(w1) - i, f"delete {len(w1) - i}")]
        else:
            delete = md(w1, i+1, w2, j, table)
            replace_eat = md(w1, i+1, w2, j+1, table)
            insert = md(w1, i, w2, j+1, table)

            # If you can eat instead of replace, take that option
            delta = 0 if w1[i] == w2[j] else 1
            # text = "eat" if w1[i] == w2[j] else "replace"

            delete += 1 #[(delete[-1][0]+1, "delete")]
            replace_eat += delta #[(replace_eat[-1][0] + delta, text)]
            insert += 1 #[(insert[-1][0]+1, "insert")]

            ret = min((delete, replace_eat, insert)) #, key=lambda opt: opt[-1][0])
        table[i][j] = ret
        return ret
