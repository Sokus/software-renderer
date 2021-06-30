#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define LIST_MAX_ROWS 6

typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode
{
    LinkedListNode* prev;
    LinkedListNode* next;
};

void ListPush(LinkedListNode* list, LinkedListNode* node);
void ListAppend(LinkedListNode* list, LinkedListNode* node);
void ListInsert(LinkedListNode* list, LinkedListNode* node);
void ListRemove(LinkedListNode* node);

LinkedListNode* GetFirstFromList(LinkedListNode* list);
LinkedListNode* GetLastFromList(LinkedListNode* list);
int GetListPosition(LinkedListNode* list);
int GetListLength(LinkedListNode* list);

LinkedListNode* GetListPageRelative(LinkedListNode* list, int offset);
LinkedListNode* GetListPageAligned(LinkedListNode* list);
LinkedListNode* GetListPage(LinkedListNode* list, int page);

#endif