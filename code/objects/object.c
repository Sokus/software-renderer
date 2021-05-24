#include "object.h"

void FreeMemory(Object** ppObj)
{
    Object* pObj = *ppObj;
    if(pObj)
    {
        for(int i=0; i<OBJECT_MAX_TAGS; i++)
        {
            free(pObj->tags[i]);
            pObj->tags[i] = NULL;
        }

        free(pObj->description);
        pObj->description = NULL;

        free(pObj->details);
        pObj->details = NULL;

        if(pObj->inventoryHead) FreeMemory(&(pObj->inventoryHead));
        if(pObj->next) FreeMemory(&(pObj->next));

        free(pObj);
        *ppObj = NULL;
    }
}

void RemoveFromInventory(Object* obj)
{
    Object* container = obj->parent;
    if(container)
    {
        Object* pInv = container->inventoryHead;
        if(pInv == obj)
        {
            container->inventoryHead = obj->next;
        }
        else
        {
            while(pInv->next != obj) pInv = pInv->next;
            pInv->next = obj->next;
        }
    }
    obj->next = NULL;
}

void DropItem(Object* obj)
{
    RemoveFromInventory(obj);
    Object* container = obj->parent;
    Object* environment;
    if(container) environment = container->parent;
    if(environment) AppendInventory(environment, obj);
}

void AppendInventory(Object* parent, Object* obj)
{
    if(parent->inventoryHead == NULL)
    {
        parent->inventoryHead = obj;
    }
    else
    {
        Object* pInv = parent->inventoryHead;
        while(true)
        {
            if(Compare(pInv, obj))
            {
                // increase count
                return;
            }
            if(pInv->next == NULL) break;
            pInv = pInv->next;
        }
        pInv->next = obj;
    }
    obj->parent = parent;
}

void ListObjects(Object* head)
{
    for(Object* pObj=head; pObj != NULL; pObj=pObj->next)
    {
        printf("%s\n", pObj->description);
    }
}

bool Compare(Object* objA, Object* objB)
{
    if(objA == objB) return true;
    if(objA == NULL || objB == NULL) return false;

    if(objA->properties != objB->properties) return false;
    for(int i=0; i<OBJECT_MAX_TAGS; i++)
    {
        if(!CompareStringInsensitive(objA->tags[i], objB->tags[i])) return false;
    }
    if(!CompareStringInsensitive(objA->description, objB->description)) return false;
    if(!CompareStringInsensitive(objA->details, objB->details)) return false;

    return true;
}