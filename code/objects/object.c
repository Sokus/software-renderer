#include "object.h"

void FreeMemory(Object** obj)
{
    Object* this = *obj;
    for(int i=0; i<8; i++)
    {
        free(this->tags[i]);
        this->tags[i] = NULL;
    }

    free(this->description);
    this->description = NULL;

    free(this->details);
    this->details = NULL;

    if(this->inventoryHead) FreeMemory(&(this->inventoryHead));
    if(this->next) FreeMemory(&(this->next));

    free(this);
    *obj = NULL;
}

void AppendInventory(Object *parent, Object *obj)
{
    if(parent->inventoryHead == NULL)
    {
        parent->inventoryHead = obj;
    }
    else
    {
        Object* pInv = parent->inventoryHead;
        while(pInv->next != NULL) pInv = pInv->next;
        pInv->next = obj;
    }
    obj->parent = parent;
}
