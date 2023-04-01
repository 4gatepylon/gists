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
    ListNode* partition(ListNode* head, int x) {
        if (head == nullptr || head->next == nullptr) return head;

        ListNode* ltHead = nullptr;
        if (head->val >= x) {
            ListNode* ltHeadPrev = head;
            while (ltHeadPrev->next != nullptr && ltHeadPrev->next->val >= x) { ltHeadPrev = ltHeadPrev->next; }

            // We only do anything if there is an element strictly smaller than x
            if (ltHeadPrev->next == nullptr) return head;

            ltHead = ltHeadPrev->next;
            ltHeadPrev->next = ltHead->next;
            ltHead->next = head;
        } else {
            ltHead = head;
        }

        // We will return if there are no elements that can become the new head (first element
        // strictly smaller than x)
        assert(ltHead != nullptr);
        ListNode* ltTail = ltHead;
        while (ltTail->next != nullptr && ltTail->next->val < x) { ltTail = ltTail->next; }

        // If there are any (more) elements to put to the right then move them after the ltTail
        ListNode* gteTailPrev = ltHead;
        while (gteTailPrev->next != nullptr && gteTailPrev->next->val < x) { gteTailPrev = gteTailPrev->next; }

        while (gteTailPrev->next != nullptr) {
            if (gteTailPrev->next->val < x) {
                ListNode* ltTailNewNext = gteTailPrev->next;

                gteTailPrev->next = gteTailPrev->next->next;

                ltTailNewNext->next = ltTail->next;
                ltTail->next = ltTailNewNext;
                ltTail = ltTailNewNext;
            } else {
                gteTailPrev = gteTailPrev->next;
            }
        }

        return ltHead;
    }
};