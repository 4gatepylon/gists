// This is from the following question:
// https://codeforces.com/contest/1773/problem/B
#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unordered_set>
#include <assert.h>
#include <stdexcept>

// Compile this shit with
// clang++ 1773b.cc -std=c++11 -stdlib=libc++ -o a.out
// Run this shit with
// echo "my input string" | ./a.out

using namespace std;

// Our algorithm uses a TOP-DOWN approach
// 1. Find candidates for roots
//    (a candidate must be such that it partitions the elements into always the same two partitions)
// 2. See if we can solve the subtrees to each side of the roots
//    (recurse)
//
// // ~O(n^2k) (it recurses, but the n^2 beats the logn)... this is too slow but a good starter
// https://brilliant.org/wiki/master-theorem/
//
// For each potential candidate we see if it could be a valid root
//   For each permutation find where that candidate is
//     For each element in the permutation coming before the candidate make sure that the partition
//     to that side is correct (by induction we only need to pick one side, because the other side
//     can only include the other elements: true for the root, and then for all others true, because
//     it was true for the parent)
static bool bc(
    // We are solving for the people between i and j in the first permutation
    // (the idea is that for EVERY permutation the root must have the property
    // that everyone to the left and to the right must be the same across permutations
    // so we use that to identify sets with the first permutation)
    int i,
    int j,
    // Just the list of k permutations that should be unique but don't have to
    vector<vector<int>>& permutations,
    // Subtrees and subtree is basically our solution
    vector<int>& subtrees,
    int subtree)
{
    // Sanity
    assert(permutations.size() >= 50);
    assert(j >= i);

    // Base-case
    if (i == j) return true;
    cout << "RECURSE BETWEEN " << i << " " << j << endl; // XXX

    // Make sure that everyone knows what subtree they are in to the highest precision possible
    // (i.e. the parent of the root of this subtree)
    for (int k = i; k < j; k++) subtrees[k] = subtree;

    // For each element, add it if it could be a real root here in the candidates
    // vector
    vector<int> candidate_roots;
    unordered_set<int> left_elems;
    // O(n) * O(k) * O(n) = O(n^2k)
    for (int kc = i; kc < j; kc++) {
        int candidate = permutations[0][kc];
        left_elems.insert(candidate);
        // For every other permutation, find the element where our candidate appears
        // and then make sure that before it and after it (in the same subtree) the same
        // elements appear. To know if they are in the same subtree we can use the subtrees
        // vector and stop once we leave the subtree (we know that the subtree is
        // to the left or right, but to the other side could be a sub-sub tree and so-on
        // which makes the alternative complicated)
        int valid_candidate = true;
        for (int kp = 1; kp < permutations.size(); kp++) {

            int center = 0;
            while (permutations[kp][center] < permutations[kp].size() && permutations[kp][center] != candidate) center++;
            
            // XXX
            cout << "center is " << center << " and candidate is " << candidate << "and we are on permutation " << kp << endl;
            assert(0 <= center && center < permutations[kp].size());
            assert(permutations[kp][center] == candidate);

            // We must check that the left = left and right = right or left = right adn right = left
            // for the subtrees. It suffices to only check one side which can be shown using induction
            bool satisfies_partition = false;
            for (int dx = -1; dx < 2; dx += 2) {
                int startx = dx < 0 ? center - 1 : center + 1;
                int num_present = 0;
                for (
                    int x = startx; 
                    0 <= x && x < permutations[kp].size() && subtrees[permutations[kp][x]] == subtree; 
                    x += dx)
                {
                    if (left_elems.find(permutations[kp][x]) == left_elems.end()) break;
                    num_present ++;
                }
                if (num_present == left_elems.size()) satisfies_partition = true;
                if (satisfies_partition)              break;
            }
            if (!satisfies_partition) valid_candidate = false;
            if (!valid_candidate)     break;
        }
        if (valid_candidate) candidate_roots.push_back(kc);
    }

    // Now that we have a vector of candidates, let's try and see if, for any of these,
    //  we can find a solution downstream, if not simply return false.
    // This is a form of DFS
    for (int kc: candidate_roots) {
        if (
            bc(i, kc, permutations, subtrees, permutations[0][kc]) &&
            bc(kc + 1, j, permutations, subtrees, permutations[0][kc])) return true;
    }
    return false;
}

// nklogn shit, but you can use rolling hashes perhaps to do k + nlogn
static inline vector<int> bin_coin(vector<vector<int>>& lines) {
    // Sanity check that we parsed correctly
    assert(lines.size() >= 51);                                                    // Make sure we are not missing lines
    assert(lines[0].size() == 2);                                                  // Make sure that the first line has the right size
    assert(lines[0][1] == lines.size() - 1);                                       // Check that k = k
    for (int i = 1; i < lines.size(); i++) assert(lines[i].size() == lines[0][0]); // Check that n = n
    int n = lines[0][0];
    int k = lines[0][1];
    int i = 0;
    int j = n;

    // Get rid of n and k which we have already parsed and verified for clarity
    lines[0] = lines[lines.size() - 1];
    lines.pop_back();

    // Subtrees will have every element pointing to the subtree it's in
    vector<int> subtrees(n);
    // Subtree tells the callee which subtree we are now traversing 
    // (i.e. who was the parent)
    int subtree = -1;
    if (bc(i, j, lines, subtrees, subtree)) return subtrees;
    else                                    throw invalid_argument("Recieved unsolvable bin_coin company tree");
}

// Examples return sequences of vectors where each vector is a line
static inline vector<vector<int>> ex1() {
    return vector<vector<int>>{
        vector<int>{3,50},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{3,2,1},
        vector<int>{3,2,1},
        vector<int>{1,2,3},
        vector<int>{1,2,3},
        vector<int>{3,2,1},
        vector<int>{3,2,1},
        };
}

static inline vector<vector<int>> ex2() {
    return vector<vector<int>>{
        vector<int>{5,60},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,3,4,2},
        vector<int>{3,4,2,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{3,4,2,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{3,4,2,5,1},
        vector<int>{3,4,2,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,3,4,2},
        vector<int>{3,4,2,5,1},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,2,4,3},
        vector<int>{2,4,3,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,2,4,3},
        vector<int>{3,4,2,5,1},
        vector<int>{1,5,3,4,2},
        vector<int>{3,4,2,5,1},
        vector<int>{3,4,2,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,3,4,2},
        vector<int>{2,4,3,5,1},
        vector<int>{2,4,3,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,2,4,3},
        vector<int>{3,4,2,5,1},
        vector<int>{3,4,2,5,1},
        vector<int>{3,4,2,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{1,5,3,4,2},
        vector<int>{1,5,3,4,2},
        vector<int>{2,4,3,5,1},
        vector<int>{3,4,2,5,1},
        vector<int>{1,5,2,4,3},
        vector<int>{3,4,2,5,1},
    };
}

// Compile this shit with
// clang++ 1773b.cc -std=c++11 -stdlib=libc++ -o a.out
// Run this shit with
// echo "my input string" | ./a.out

#define TESTING
int main() {
    #ifdef TESTING
    vector<vector<int>> e1 = ex1();
    // vector<vector<int>> e2 = ex2();
    vector<vector<int>> parents = vector<vector<int>>{bin_coin(e1)};//, bin_coin(e2)};
    for (int i = 0; i < parents.size(); i++) {
        for (int j = 0; j < parents[i].size(); j++) {
            cout << " " << parents[i][j];
        }
        cout << "\n----\n";
    }
    #else
    // Parse the integers and then put them into a vector of vectors
    size_t pos = 0;
    string input_line;
    string input_token;
    vector<vector<int>> lines;

    while(cin) {
        vector<int> line;
        
        getline(cin, input_line);
        // Quick and dirty hack to make sure we get the last token
        input_line += " ";
        // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
        while ((pos = input_line.find(" ")) != string::npos) {
            // Not fast but good enough
            input_token = input_line.substr(0, pos);
            input_line.erase(0, min(pos + 1, input_line.size()));

            // We may have malformed inputs that include extra spaces of different kinds
            // Also, for some reason getline includes the \0 which is annoying
            if (input_token != "\n" && input_token != " " && input_token != "\t" && input_token != "\0") {
                line.push_back(atoi(input_token.c_str()));
            }
        }
        // This should be by copy by value of the vector
        // Some lines will be empty and we want to ignore them
        if (line.size() > 0) lines.push_back(line);
    };
    // Some debug shit, keeping this here if I ever have to do it again
    // cout << "lines has length " << lines.size() << "\n";
    // for (vector<int> line : lines) {
    //     for (int i : line) cout << " " << i;
    //     cout << "\n";
    // }

    // Solve the problem
    vector<int> sol = bin_coin(lines);

    // Print the output
    if (sol.size() == 0) return 0;
    for (int i = 0; i < sol.size() - 1; i++) {
        cout << sol[i] << " ";
    }
    cout << sol[sol.size() - 1];
    #endif

    // Compile this shit with
    // clang++ 1773b.cc -std=c++11 -stdlib=libc++ -o a.out
    // Run this shit with
    // echo "my input string" | ./a.out
}