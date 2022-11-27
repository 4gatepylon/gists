#include <algorithm>
class Solution {
private:
    bool diffByOne(string a, string b) {
        if (a.size() != b.size()) return false;
        int mistakes = 0;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) mistakes += 1;
            if (mistakes > 1) return false;
        }
        return true;
    }
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        if (std::find(wordList.begin(), wordList.end(), endWord) == wordList.end()) return 0;
        // Create a graph from Word -> list of neighbor words
        // Run BFS on the graph to find the shortest path (just remember the count)
        int beginWordIdx = std::find(wordList.begin(), wordList.end(), beginWord) - wordList.begin();
        int endWordIdx =   std::find(wordList.begin(), wordList.end(), endWord) - wordList.begin();
        bool pushBeginWord = beginWordIdx == wordList.size();

        // Modify the wordList to create the graph easier
        if (pushBeginWord) {
            beginWordIdx = wordList.size();
            wordList.push_back(beginWord);
        }

        vector<vector<int>> word_idx2word_idxs(wordList.size(), vector<int>(0));
        for (int i = 0; i < wordList.size(); i++) {
            for (int j = i + 1; j < wordList.size(); j++) {
                if (diffByOne(wordList[i], wordList[j])) {
                    word_idx2word_idxs[i].push_back(j);
                    word_idx2word_idxs[j].push_back(i);
                }
            }
        }

        int len = 0;
        queue<int> visit_next;
        visit_next.push(beginWordIdx);
        queue<int> lens;
        lens.push(1);
        unordered_set<int> visited;
        while (!visit_next.empty()) {
            int i = visit_next.front();
            int _len = lens.front();

            cout << "visiting " << i << " with len " << _len << "\n"; // XXX

            visit_next.pop();
            lens.pop();
            if (visited.find(i) == visited.end()) {
                if (i == endWordIdx) {
                    len = _len;
                    break;
                } else {
                    visited.insert(i);
                    for (int j : word_idx2word_idxs[i]) {
                        if (visited.find(j) == visited.end()) {
                            visit_next.push(j);
                            lens.push(_len + 1);
                        }
                    }
                }
            }
        }

        // Undo the modifications we used to try and make the graph easier to deal with
        if (pushBeginWord) wordList.pop_back();

        return len;
    }
};