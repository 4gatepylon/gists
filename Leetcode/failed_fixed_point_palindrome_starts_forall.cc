// Return a list of the places that a character could jump to, such that
// that place is a palindrome. Takes O(n^2)
// Doesn't really work lmao (gets tripped up with repeated characters, skipping more than it should)
// vector<vector<int> > palindrome_jumps(string s) {
//     vector<vector<int> > jumps(s.size(), vector<int>(0));
//     for (int i = 0; i < jumps.size(); i++) {
//         jumps[i].push_back(i + 1);
//         if (i + 1 < s.size() && s[i] == s[i+1]) jumps[i].push_back(i+2);
//     }
//     bool updated_pali = true;
//     while (updated_pali) {
//         updated_pali = false;
//         for (int i = jumps.size() - 1; i > 0; i--) {
//             int jump = back(jumps[i]);
//             if (jump < jumps.size() && back(jumps[i-1]) != jump + 1 && s[i-1] == s[jump]) {
//                 cout << "updated " << i - 1 << " to jump to " << jump + 1 << "\n";
//                 jumps[i-1].push_back(jump + 1);
//                 updated_pali = true;
//             }
//         }
//     }
//     return jumps;
// }