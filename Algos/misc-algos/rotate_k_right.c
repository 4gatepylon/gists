/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


struct ListNode* rotateRight(struct ListNode* head, int k){
    if (k == 0 || head == NULL || head->next == NULL) {
        return head;
    }
    // Make it circular, then simply set the head
    struct ListNode* tail = head;
    int length = 1;
    while (tail->next != NULL) {
        tail = tail->next;
        length += 1;
    }
    tail->next = head;
    k = k % length;
    // Rotate left by length - k <=> rotate right by k
    for (int i = 0; i < length - k; i++) {
        tail = head;
        head = head->next;
    }
    tail->next = NULL;
    return head;
}
