#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdio.h>

#include "../types.h"
#include "../properties.h"
#include "../utility/strops.h"

#define OBJECT_MAX_TAGS 8

struct Object
{
    PropertyField properties;

    char* tags[OBJECT_MAX_TAGS];
    char* description;
    char* details;

    Object* parent;
    Object* inventoryHead;
    Object* next;
};

void FreeMemory(Object** obj);
void RemoveFromInventory(Object* obj);
void DropItem(Object* obj);
void AppendInventory(Object* parent, Object* obj);
void ListObjects(Object* head);
bool Compare(Object* objA, Object* objB);

#endif