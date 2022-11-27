class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        unordered_map<string, int> f;
        for (string w : words) {
            if (f.find(w) == f.end()) f[w] = 1;
            else f[w] ++;
        }
        // Make a min queue
        // Lambda returns whether l is more frequent than r or beats it lexicographically
        auto cmp = [&f](string l, string r){ return f[l] > f[r] || f[l] == f[r] && r.compare(l) > 0; };
        priority_queue<
            string,
            vector<string>,
            decltype(cmp)> pq(cmp);
        for (pair<string, int> wf : f) {
            string w = wf.first;
            if (pq.size() < k || cmp(w, pq.top())) pq.push(w);
            while (pq.size() > k) pq.pop();
        }  
        vector<string> k_common;
        while (!pq.empty()) {
            k_common.push_back(pq.top());
            pq.pop();
        }
        sort(k_common.begin(), k_common.end(), cmp);
        return k_common;
    }
};