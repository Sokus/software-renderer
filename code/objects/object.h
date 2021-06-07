#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

#include "output/console.h"
#include "properties.h"
#include "utility/strops.h"

#define OBJECT_MAX_TAGS 4
#define LIST_MAX_ROWS 3

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
Object* SetListPage(Object* member, int page);
Object* MoveListPage(Object* member, int offset);

void RemoveFromInventory(Object* obj);
void AddToInventory(Object* parent, Object* obj);
void DropItem(Object* obj);

void ListObjects(Object* head, int limit);
void PrintObjectInfo(Object* obj);
void PrintInfo();

bool Compare(Object* objA, Object* objB);

#endif