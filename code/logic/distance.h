#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdbool.h>

#include "../objects/object.h"

typedef enum
{
    DISTANCE_SELF,
    DISTANCE_INVENTORY,
    DISTANCE_INVENTORY_CONTAINED,
    DISTANCE_LOCATION,
    DISTANCE_NEAR,
    DISTANCE_NEAR_CONTAINED,
    DISTANCE_ABSENT
} Distance;

Distance GetDistance(const Object* from, const Object* to);
int GetDepth(const Object* container, const Object* obj);
bool IsInRange(Distance value, Distance from, Distance to);

#endif