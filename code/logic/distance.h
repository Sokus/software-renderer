#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdbool.h>

#include "objects/object.h"

typedef enum Distance
{
    DISTANCE_SELF,
    DISTANCE_INVENTORY,
    DISTANCE_LOCATION,
    DISTANCE_NEAR,
    DISTANCE_NEAR_CONTAINED,
    DISTANCE_TARGET,
    DISTANCE_ABSENT,
    DISTANCE_MAX
} Distance;

// Distance GetDistance(const Object* from, const Object* to);
int GetDepth(Object* container, Object* obj);
bool IsInRange(Distance value, Distance from, Distance to);

#endif