#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdio.h>

#include "../properties.h"
#include "../utility/strops.h"

#define OBJECT_MAX_TAGS 8

typedef struct Object Object;
struct Object
{
    PropertyField properties;

    char* tags[OBJECT_MAX_TAGS];
    char* description;

    Object* parent;
    Object* inventoryHead;
    Object* next;
};

extern Object* gpObjectRoot;
extern Object* gpPlayer;

void CreateObjects();
void DeleteObjects();

void FreeMemory(Object** obj);

void RemoveFromInventory(Object* obj);
void AppendInventory(Object* parent, Object* obj);
void DropItem(Object* obj);
void Open(Object* obj);
void Close(Object* obj);

void ListObjects(Object* head);
void ListObjectsRecursive(Object* head, int padding);

bool Compare(Object* objA, Object* objB);

#endif