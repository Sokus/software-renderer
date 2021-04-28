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