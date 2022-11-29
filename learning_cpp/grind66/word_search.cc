// Tried some complicated BFS to no avail
// https://quantum5.ca/2019/04/05/using-unordered-data-structures-on-std-pair-cpp/
// A 5-tuple
typedef tuple<int, int> HyperCoord;
namespace std {
    template<>
    struct hash<HyperCoord> {
        std::size_t operator()(const HyperCoord &tup) const {
            return (
                std::hash<int>()(get<0>(tup)) ^ 
                std::hash<int>()(get<1>(tup))
            );
        }
    };
}
class Solution {
private:
    unordered_set<HyperCoord> vis;
    void init_visited() { vis.clear(); }
    bool visited(int i, int j) { 
        return vis.find(make_tuple(i, j)) != vis.end();
    }
    void visit(int i, int j) {
        vis.insert(make_tuple(i, j));
    }
    void unvisit(int i, int j) {
        vis.erase(make_tuple(i, j));
    }
    bool dfs(
        vector<vector<char>>& board,
        string word,
        int wis,
        int i,
        int j
    ){
        if (visited(i, j)) return false;
        if (word[wis] != board[i][j]) return false;
        if (wis == word.size() - 1 && word[wis] == board[i][j]) return true;

        // Visit it and see if we can finish the path or not
        visit(i, j);
        for (int k = 0; k < 2; k++) {
            for (int d = -1; d < 2; d += 2) {
                int ip = k == 0 ? i + d : i;
                int jp = k == 1 ? j + d : j;
                if (
                    0 <= ip && ip < board.size() && 
                    0 <= jp && jp < board[0].size() &&
                    dfs(board, word, wis + 1, ip, jp)
                ) return true;
            }
        }

        // If no success yet, it's not possible
        unvisit(i, j);
        return false;
    }
public:
    // Tried to do BFS but couldn't get it ugh!
    bool exist(vector<vector<char>>& board, string word) {
        // Deal with their edge cases
        unordered_set<char> welems(word.begin(), word.end());
        for (auto& br : board) {
            for (auto belem: br) {
                if (welems.find(belem) != welems.end()) welems.erase(belem);
            }
        }
        if(welems.size() > 0) return false;

        // O(mnW)
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j ++) {
                if (word[0] == board[i][j]) {
                    init_visited();
                    if (dfs(board, word, 0, i, j)) return true;
                }
            }
        }
        return false;
    }
};