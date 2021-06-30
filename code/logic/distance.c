#include "distance.h"

#include <stdlib.h>

#include "utility/linked_list.h"
#include "utility/macros.h"

int GetDepth(Object* container, Object* obj)
{
    if(!container || !container->inventory || !obj) return 0;
    int value = 0;
    Object* inv = NULL;
    for(LinkedListNode* node=GetFirstFromList(&container->inventory->list);
        node != NULL;
        node = node->next)
    {
        inv = CONTAINEROF(node, Object, list);
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

DynamicArray GetObjectsInDistance(PropertyBitField distanceFlags)
{
    DynamicArray objects = {0};
    Array_New(&objects, 0, sizeof(Object*));

    if(HasProperty(distanceFlags, DISTANCE_SELF))
    {
        Array_Append(&objects, &gpPlayer);
    }

    if(HasProperty(distanceFlags, DISTANCE_INVENTORY))
    {
        if(gpPlayer->inventory)
        {
            Object* p = NULL;
            for(LinkedListNode* inv=GetFirstFromList(&gpPlayer->inventory->list);
                inv != NULL;
                inv = inv->next)
            {
                p = CONTAINEROF(inv, Object, list);
                Array_Append(&objects, &p);
            }
        }
    }

    if(HasProperty(distanceFlags, DISTANCE_LOCATION))
    {
        Array_Append(&objects, &gpPlayer->parent);
    }

    if(HasProperty(distanceFlags, DISTANCE_NEAR))
    {
        Object* p = NULL;
        for(LinkedListNode* near=GetFirstFromList(&gpPlayer->list);
            near != NULL;
            near = near->next)
        {
            p = CONTAINEROF(near, Object, list);
            Array_Append(&objects, &p);
        }
    }

    if(HasProperty(distanceFlags, DISTANCE_TARGET))
    {
        if(gpPlayer->target)
        {
            Array_Append(&objects, &gpPlayer->target);
        }
    }
    
    if(HasProperty(distanceFlags, DISTANCE_TARGET_CONTAINED))
    {
        if(gpPlayer->target && gpPlayer->target->inventory)
        {
            Object* p = NULL;
            for(LinkedListNode* inv=GetFirstFromList(&gpPlayer->target->inventory->list);
                inv != NULL;
                inv = inv->next)
            {
                p = CONTAINEROF(inv, Object, list);
                Array_Append(&objects, &p);
            }
        }
    }
    
    return objects;
}