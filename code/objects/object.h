#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

#include "output/console.h"
#include "properties.h"
#include "utility/strops.h"

#define OBJECT_MAX_TAGS 4
#define LIST_MAX_ROWS 6

typedef struct Object Object;
struct Object
{
    PropertyField properties;

    char* tags[OBJECT_MAX_TAGS];
    char* description;

    Object* parent;
    Object* next;
    Object* prev;
    Object* inventory;

    Object* target;
};

extern Object* gpObjectRoot;
extern Object* gpPlayer;
extern PropertyField gContext;

void CreateObjects();
void DeleteObjects();

void FreeMemory(Object* obj);

void ListPush(Object* member, Object* obj);
void ListAppend(Object* member, Object* obj);
void ListInsert(Object* member, Object* obj);
void ListRemove(Object* obj);
Object* GetFirstFromList(Object* member);
Object* GetLastFromList(Object* member);
int GetListPosition(Object* member);
int GetListLength(Object* member);
Object* GetListPageRelative(Object* member, int offset);
Object* GetListPageAligned(Object* member);
Object* GetListPage(Object* member, int page);

void ListObjects(Object* head, int limit);
void PrintObjectInfo(Object* obj);
void PrintPageInfo(Object* inventory);
void PrintInfo();

void RemoveFromInventory(Object* obj);
void AddToInventory(Object* parent, Object* obj);
bool PickUpItem(Object* parent, Object* obj);
bool DropItem(Object* obj);


#endif