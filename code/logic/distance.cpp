#include "distance.hpp"

Distance getDistance(const Object& from, const Object& to)
{
    if(&from == &to) return DISTANCE_SELF;
    int depth = getDepth(from, to);
    if(depth == 1) return DISTANCE_INVENTORY;
    if(depth > 1) return DISTANCE_INVENTORY_CONTAINED;
    const Object* parent = from.parent;
    if(parent != nullptr)
    {
        if(parent == &to) return DISTANCE_LOCATION;
        depth = getDepth(*parent, to);
        if(depth == 1) return DISTANCE_NEAR;
        if(depth > 1) return DISTANCE_NEAR_CONTAINED;
    }

    return DISTANCE_ABSENT;
}

int getDepth(const Object& container, const Object& obj)
{
    int value;
    for(Object* inv=container.inventoryHead; inv != nullptr; inv = inv->next)
    {
        if(inv == &obj)
        {
            return 1;
        }
        value = getDepth(*inv, obj);
        if(value != 0)
        {
            return 1 + value;
        }
    }
    return 0;
}

bool isInRange(Distance value, Distance from, Distance to)
{
    return from <= value && from <= to;
}