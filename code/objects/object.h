#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

typedef struct Object Object;

extern Object objectHead, player;

struct Object
{
    char *tags[8]; // tags you refer to the object by
    char *description;
    char *details;

    unsigned int weight;
    unsigned int capacity;
    unsigned int count;

    Object *inventoryHead;

    Object *destination;

    Object *next;
};

Object *findParent(Object *root, Object *obj);

#endif