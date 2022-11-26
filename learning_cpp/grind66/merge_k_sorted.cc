/**
 * SDL doesn't work :)
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.size() == 0) return nullptr;
        // Initialize the head of the new linkedlist with the with
        // the smallest value
        // https://stackoverflow.com/questions/2953491/finding-the-position-of-the-maximum-element
        // Doesn't work even if you do min or max or > or < or >= or <=
        // auto midx = min_element(
        //         lists.begin(),
        //         lists.end(),
        //         // This is a "less than" comparator
        //         [](ListNode* l, ListNode* r) {
        //             if (l == nullptr) return r;
        //             if (r == nullptr) return l;
        //             if (l->val < r->val) return l;
        //             return r;
        // });
        int midx = 0;
        for (int i = 0; i < lists.size(); i++) {
            if (lists[midx] == nullptr) midx = i;
            else if (lists[i] != nullptr && lists[i]->val < lists[midx]->val) midx = i;
        }

        // Initialize head and tail of our new linkedlist
        ListNode* head = lists[midx];
        ListNode* tail = head;
        if (head == nullptr) return nullptr;
        lists[midx] = head->next;
        head->next = nullptr;

        // Copy all the heads to _seelction
        // https://stackoverflow.com/questions/21204676/modern-way-to-filter-stl-container
        vector<ListNode*> _selection;
        copy_if(
            lists.begin(),
            lists.end(),
            back_inserter(_selection),
            [](ListNode* n){ return n != nullptr; }
        );

        // Create min queue from _selection s.t.
        // we'll be able to pop it's top to add it
        // to the new linkedlist (after tail)
        priority_queue<
            ListNode*,
            vector<ListNode*>,
            auto(*)(ListNode*, ListNode*)->bool
        > selection(
            // This is a "less than" comparator
            // But we are flipping it because it is default
            // a max-queue
            [](ListNode* l, ListNode* r){ return l->val > r->val; },
             _selection
        );

        // Iterate always picking the smallest element out of all
        // the options.
        while (!selection.empty()) {
            ListNode* next = selection.top();
            selection.pop();

            if (next->next != nullptr) selection.push(next->next);
            tail->next = next;
            tail = next;
            next->next = nullptr;
        }

        return head;
    }
};