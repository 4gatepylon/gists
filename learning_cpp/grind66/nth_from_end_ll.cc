/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    // Reverses the list twice and uses O(1) memory without any stack
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        if (head->next == nullptr) return nullptr;

        ListNode* follower = head;
        ListNode* leader = head->next;
        head->next = nullptr;

        // Go to end and reverse list so we can go back
        while (leader->next != nullptr) {
            ListNode* next_leader = leader->next;

            leader->next = follower;
            follower = leader;
            leader = next_leader;
        }

        // Flip last case and start moving backwards
        ListNode* _tmp = follower;
        follower = leader;
        leader = _tmp;

        // Find the n'th element from the back (1-indexxed)
        for (int i = 1; i < n; i++) {
            ListNode* next_leader = leader->next;

            leader->next = follower;
            follower = leader;
            leader = next_leader;
        }
        
        // Edge case if n == size of list
        if (leader == nullptr) {
            return follower->next;
        }

        // Fix the elements around the wrong element
        ListNode* skip_to = follower->next;
        follower = leader;
        leader = leader->next;
        follower->next = skip_to;

        // Fix the front of the list
        while (leader != nullptr) {
            ListNode* next_leader = leader->next;

            leader->next = follower;
            follower = leader;
            leader = next_leader;
        }
        return follower;
    }
};