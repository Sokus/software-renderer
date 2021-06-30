#include "linked_list.h"

#include <stdlib.h>

void ListPush(LinkedListNode* list, LinkedListNode* node)
{
    if(!list || !node) return;
    LinkedListNode* first = GetFirstFromList(list);
    first->prev = node;
    node->next = first;
    node->prev = NULL;
}

void ListAppend(LinkedListNode* list, LinkedListNode* node)
{
    if(!list || !node) return;
    LinkedListNode* last = GetLastFromList(list);
    last->next = node;
    node->next = NULL;
    node->prev = last;
}

void ListInsert(LinkedListNode* list, LinkedListNode* node)
{
    if(!list || !node) return;
    
    if(list->next) list->next->prev = node;
    list->next = node;
    node->next = list->next;
    node->prev = list;
}

void ListRemove(LinkedListNode* node)
{
    if(!node) return;

    if(node->next) node->next->prev = node->prev;
    if(node->prev) node->prev->next = node->next;
    node->next = NULL;
    node->prev = NULL;
}

LinkedListNode* GetFirstFromList(LinkedListNode* list)
{
    if(!list) return NULL;
    LinkedListNode* pnode = list;
    while(pnode->prev) pnode=pnode->prev;
    return pnode;
}

LinkedListNode* GetLastFromList(LinkedListNode* list)
{
    if(!list) return NULL;
    LinkedListNode* pnode = list;
    while(pnode->next) pnode=pnode->next;
    return pnode;
}

int GetListPosition(LinkedListNode* list)
{
    if(!list) return 0;
    int position = 0;
    while(list->prev)
    {
        list = list->prev;
        position++;
    }
    return position;
}

int GetListLength(LinkedListNode* list)
{
    if(!list) return 0;
    int length = 1;
    LinkedListNode* p = list;
    while(p->prev)
    {
        p = p->prev;
        length++;
    }
    p = list;
    while(p->next)
    {
        p = p->next;
        length++;
    }
    return length;
}

LinkedListNode* GetListPageRelative(LinkedListNode* list, int offset)
{
    if(!list) return NULL;
    int newPage = GetListPosition(list)/LIST_MAX_ROWS + offset;
    int maxPage = (GetListLength(list)-1)/LIST_MAX_ROWS;
    newPage =   (newPage < 0)       ? 0 :
                (newPage > maxPage) ? maxPage :
                                    newPage;
    return GetListPage(list, newPage);
}

LinkedListNode* GetListPageAligned(LinkedListNode* list)
{
    int page = GetListPosition(list)/LIST_MAX_ROWS;
    return GetListPage(list, page);
}

LinkedListNode* GetListPage(LinkedListNode* list, int page)
{
    if(!list) return NULL;
    LinkedListNode* p = list;
    while(p->prev) p = p->prev;
    int position = page*LIST_MAX_ROWS;
    for(int i=0;
        i<position && p->next != NULL;
        i++)
    {
        p = p->next;
    }
    return p;
}