class TimeMap {
private:
    // https://stackoverflow.com/questions/529831/returning-the-greatest-key-strictly-less-than-the-given-key-in-a-c-map
    unordered_map<string, map<int, string>> s;
public:
    TimeMap() {}
    void print_info() {
        for (pair<string, map<int, string>> kvout : s) {
            cout << "--- key --- " << kvout.first << "\n";
            for (pair<int, string> kvin : kvout.second) {
                cout << "\t" << kvin.first << ": " << kvin.second << "\n";
            }
        }
    }
    
    void set(string key, string value, int timestamp) {
        if (s.find(key) == s.end()) s[key] = map<int, string>();
        s[key][timestamp] = value;
        // print_info();
    }
    
    // I think upper bound and lower bound are wierdly documented? tutorials say different
    // things :/
    string get(string key, int timestamp) {
        if (s.find(key) == s.end()) return "";
        auto it = s[key].upper_bound(timestamp);
        if(it == s[key].begin()) return "";
        // cout << "******\n";
        // cout << timestamp << "\n";
        // cout << it->first << "\n";// << it->second << "\n";
        it --;
        // cout << it->first << "\n";//
        // cout << "******\n";
        return it->second;
    }
};

/**
 * Your TimeMap object will be instantiated and called as such:
 * TimeMap* obj = new TimeMap();
 * obj->set(key,value,timestamp);
 * string param_2 = obj->get(key,timestamp);
 */