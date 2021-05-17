#ifndef DISTANCE_H
#define DISTANCE_H

#include "../objects/object.hpp"

enum Distance
{
    DISTANCE_SELF,
    DISTANCE_INVENTORY,
    DISTANCE_INVENTORY_CONTAINED,
    DISTANCE_LOCATION,
    DISTANCE_NEAR,
    DISTANCE_NEAR_CONTAINED,
    DISTANCE_ABSENT
};

Distance GetDistance(const Object& from, const Object& to);
int GetDepth(const Object& container, const Object& obj);
bool IsInRange(Distance value, Distance from, Distance to);

#endif