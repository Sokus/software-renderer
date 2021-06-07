#include "distance.h"

/*
Distance GetDistance(const Object* from, const Object* to)
{
    if(from == to) return DISTANCE_SELF;
    int depth = GetDepth(from, to);
    if(depth == 1) return DISTANCE_INVENTORY;
    if(depth > 1) return DISTANCE_INVENTORY_CONTAINED;
    const Object* parent = from->parent;
    if(parent != NULL)
    {
        if(parent == to) return DISTANCE_LOCATION;
        depth = GetDepth(parent, to);
        if(depth == 1) return DISTANCE_NEAR;
        if(depth > 1) return DISTANCE_NEAR_CONTAINED;
    }

    return DISTANCE_ABSENT;
}
*/

int GetDepth(Object* container, Object* obj)
{
    int value;
    for(Object* inv=GetFirstFromList(container->inventory);
        inv != NULL;
        inv = inv->next)
    {
        if(inv == obj) return 1;
        value = GetDepth(inv, obj);
        if(value != 0) return 1 + value;
    }
    return 0;
}

bool IsInRange(Distance value, Distance from, Distance to)
{
    return from <= value && value <= to;
}
