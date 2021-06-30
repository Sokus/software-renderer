#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdbool.h>

#include "objects/object.h"
#include "utility/array.h"
#include "properties.h"

typedef enum Distance
{
    DISTANCE_SELF,
    DISTANCE_INVENTORY,
    DISTANCE_LOCATION,
    DISTANCE_NEAR,
    DISTANCE_TARGET,
    DISTANCE_TARGET_CONTAINED,
    DISTANCE_ABSENT,
    DISTANCE_MAX
} Distance;

int GetDepth(Object* container, Object* obj);
bool IsInRange(Distance value, Distance from, Distance to);

DynamicArray GetObjectsInDistance(PropertyBitField distanceFlags);

#endif