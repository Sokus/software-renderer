#ifndef DISTANCE_H
#define DISTANCE_H

#include "../objects/object.hpp"

extern Object objectHead;

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

Distance getDistance(const Object& from, const Object& to);
int getDepth(const Object& container, const Object& obj);
bool isInRange(Distance value, Distance from, Distance to);

#endif