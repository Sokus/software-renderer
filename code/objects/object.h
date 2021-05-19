#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

typedef struct Object Object;
struct Object
{
    char* tags[8]; // tags you refer to the object by
    char* description;
    char* details;

    Object* parent;
    Object* inventoryHead;
    Object* next;
};

void FreeMemory(Object** obj);

void AppendInventory(Object *parent, Object *obj);

#endif