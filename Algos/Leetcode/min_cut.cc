#include <iostream>
#include <vector>
#include <assert.h>
// #include <bits/stdc++.h> // might be needed for leetcode

using namespace std;

// Get the last element in a vector
int back(vector<int>& v) {
    assert(v.size() >= 1);
    return v[v.size() - 1];
}

// Merge two vectors into a third
void merge(vector<int>& target, vector<int>& src1, vector<int>& src2) {
    int i = 0;
    int j = 0;
    while (i < src1.size() && j < src2.size()) {
        if (src1[i] < src2[j]) {
            assert(target.size() <= 0 || back(target) < src1[i]);
            target.push_back(src1[i++]);
        } else {
            assert(target.size() <= 0 || back(target) < src2[j]);
            target.push_back(src2[j++]);
        }
    }
    while (i < src1.size()) {
        assert(target.size() <= 0 || back(target) < src1[i]);
        target.push_back(src1[i++]);
    }
    while (j < src2.size()) {
        assert(target.size() <= 0 || back(target) < src2[j]);
        target.push_back(src2[j++]);
    }
}

// does the same thing as above but right (store starting points for palindromes)
vector<vector<int> > palindrome_jumps(string s) {
    vector<vector<int> > jumps(s.size(), vector<int>(0));

    // Find even and odd-centered jump arrays, then merge for each element
    vector<vector<int> > even_center_jumps(s.size(), vector<int>(0));
    vector<vector<int> > odd_center_jumps(s.size(), vector<int>(0));
    for (int i = 0; i < s.size(); i++) {
        // Odd Center
        odd_center_jumps[i].push_back(i+1);
        // DEBUG
        assert(0 < odd_center_jumps[i].size() && (back(odd_center_jumps[i]) - i) % 2 == 1);
        for (int R = 1; 0 <= i - R && i + R < s.size() && s[i - R] == s[i + R]; R++) {
            // DEBUG
            assert(back(odd_center_jumps[i - R]) < i + R + 1);
            assert(0 < odd_center_jumps[i - R].size());
            assert((back(odd_center_jumps[i - R]) - (i - R)) % 2 == 1);
            odd_center_jumps[i - R].push_back(i + R + 1);
        }
    }
    for (int i = 0; i < s.size() - 1; i++) {
        // Even center
        if (s[i] == s[i + 1]) {
            even_center_jumps[i].push_back(i + 2);
            // DEBUG
            assert(even_center_jumps[i].size() <= 0 || (back(even_center_jumps[i]) - i) % 2 == 0);
            for (int R = 1; 0 <= i - R && i + 1 + R < s.size() && s[i - R] == s[i + 1 + R]; R++) {
                // DEBUG
                assert(even_center_jumps[i - R].size() <= 0 || back(even_center_jumps[i - R]) < i + R + 2);
                assert(even_center_jumps[i - R].size() <= 0 || (back(even_center_jumps[i - R]) - (i - R)) % 2 == 0);
                even_center_jumps[i - R].push_back(i + R + 2);
            }
        }
    }
    for (int i = 0; i < s.size(); i++) {
        merge(jumps[i], odd_center_jumps[i], even_center_jumps[i]);
    }
    return jumps;
}

int min_num_cuts(string s) {
    // O(n^2) DS
    vector<vector<int> > jumps = palindrome_jumps(s);
    // O(n^2) DP
    // min_num_cuts_after tells you how many cuts you need (at minimum) for the subarray
    // starting at this element (so it is inclusive of this element)
    vector<int> min_num_cuts_after(s.size() + 1, s.size() - 1);
    min_num_cuts_after[s.size()] = -1;
    for (int solve_after = s.size() - 1; solve_after >= 0; solve_after --) {
        // This is the worst case if we were to cut everything
        // (it' number of elements - 1 since we could split everything into a singleton)
        int min_num_cuts_after_this = s.size() - solve_after - 1;
        for (int jump_i = jumps[solve_after].size() - 1; jump_i >= 0; jump_i --) {
            int jump = jumps[solve_after][jump_i];
            // See if we can do better by jumping to the element after this palindrome and then
            // cutting and continuing
            min_num_cuts_after_this = min(min_num_cuts_after_this, 1 + min_num_cuts_after[jump]);
        }
        min_num_cuts_after[solve_after] = min_num_cuts_after_this;
    }
    return min_num_cuts_after[0];
}

int main() {
    string str = "abcbaaaaabdop";
    cout << str << "\n"; 
    vector<vector<int> > sols = palindrome_jumps(str);
    // Print it out going down vertically
    for (int i = 0; i < sols.size(); i++) {
        cout << "> ";
        for (int j = 0; j < sols[i].size(); j++) {
            cout << sols[i][j] << " ";
        }
        cout << "\n";
    }

    int min_cut = min_num_cuts(str);
    cout << "min cut is " << min_cut << "\n";
}