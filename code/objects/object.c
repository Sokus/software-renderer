#include "object.h"

Object *findParent(Object *root, Object *obj)
{
    for(Object *o=root; o != NULL; o=o->next)
    {
        for(Object *inv=o->inventoryHead; inv != NULL; inv=inv->next)
        {
            if(inv==obj)
            {
                return o;
            }
            Object *p = findParent(inv, obj);
            if(p != NULL)
            {
                return p;
            }
        }
    }

    return NULL;
}

int contains(Object *container, Object *obj)
{
    for(Object *inv=container->inventoryHead; inv != NULL; inv = inv->next)
    {
        if(inv == obj || contains(inv, obj))
        {
            return 1;
        }
    }
    return 0;
}