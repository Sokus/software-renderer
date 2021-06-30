#ifndef OBJECT_H
#define OBJECT_H

#include "properties.h"
#include "utility/linked_list.h"

#define OBJECT_MAX_TAGS 4

typedef struct Object Object;
struct Object
{
    PropertyBitField properties;

    char* tags[OBJECT_MAX_TAGS];
    char* description;

    Object* parent;
    LinkedListNode list;
    Object* inventory;

    Object* target;
};

extern Object* gpObjectRoot;
extern Object* gpPlayer;
extern PropertyBitField gContext;

void CreateObjects();
void DeleteObjects();

void FreeMemory(Object* obj);

void ListObjects(Object* head, int limit);
void PrintObjectInfo(Object* obj);
void PrintPageInfo(Object* inventory);
void PrintInfo();

void RemoveFromInventory(Object* obj);
void AddToInventory(Object* parent, Object* obj);
bool PickUpItem(Object* parent, Object* obj);
bool DropItem(Object* obj);


#endif