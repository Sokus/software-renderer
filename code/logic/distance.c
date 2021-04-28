#include "distance.h"

Distance getDistance(Object *from, Object *to)
{
    if(from == to) return SELF;
    int depth;
    depth = getDepth(from, to);
    if(depth == 1) return INVENTORY;
    if(depth > 1) return INVENTORY_CONTAINED;
    Object *parent = findParent(&objectHead, from);
    if(parent != NULL)
    {
        if(parent == to) return LOCATION;
        depth = getDepth(parent, to);
        if(depth == 1) return NEAR;
        if(depth > 1) return NEAR_CONTAINED;
    }
    
    return ABSENT;
}

int getDepth(Object *container, Object *obj)
{
    int value;
    for(Object *inv=container->inventoryHead; inv != NULL; inv = inv->next)
    {
        if(inv == obj)
        {
            return 1;
        }
        value = getDepth(inv, obj);
        if(value != 0)
        {
            return 1 + value;
        }
    }
    return 0;
}