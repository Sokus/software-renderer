#ifndef DISTANCE_H
#define DISTANCE_H

#include "../objects/object.h"

extern Object objectHead;

typedef enum
{
    SELF,
    INVENTORY,
    INVENTORY_CONTAINED,
    LOCATION,
    NEAR,
    NEAR_CONTAINED,
    ABSENT
} Distance;

Distance getDistance(Object *from, Object *to);
int getDepth(Object *container, Object *obj);

#endif