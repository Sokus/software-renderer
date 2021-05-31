#include "object.h"

Object* gpObjectRoot;
Object* gpPlayer;

void CreateObjects()
{
    if(gpObjectRoot) return;
    
    gpObjectRoot = (Object*)malloc(sizeof(Object));
    *gpObjectRoot = (Object){0};    


    Object* field = (Object*)malloc(sizeof(Object));
    *field = (Object) {0, {Copy("grassy field"), Copy("field")}, Copy("Huge field covered in grass.")}; 
    AppendInventory(gpObjectRoot, field);
    
    gpPlayer = (Object*)malloc(sizeof(Object));
    *gpPlayer = (Object) {0, { Copy("player"), Copy("you"), Copy("self")}, Copy("This is you.")}; 
    AppendInventory(field, gpPlayer);

    Object* guard = (Object*)malloc(sizeof(Object));
    *guard = (Object) {0, {Copy("city guard"), Copy("guard")}, Copy("Man who guards the city.")};
    AppendInventory(field, guard);

    Object* pouch = (Object*)malloc(sizeof(Object));
    *pouch = (Object) {0, {Copy("leather pouch"), Copy("pouch")}, Copy("It's a leather pouch.")};
    SetProperty(&pouch->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    SetProperty(&pouch->properties, OBJECT_PROPERTY_CONTAINER, true);
    SetProperty(&pouch->properties, OBJECT_PROPERTY_OPEN, true);
    SetProperty(&pouch->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY, true);
    AppendInventory(field, pouch);

    Object* coin = (Object*)malloc(sizeof(Object));
    *coin = (Object) {0, {Copy("silver coin"), Copy("coin")}, Copy("It's a coin made of silver.")};
    SetProperty(&coin->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    AppendInventory(pouch, coin);
    
    Object* chest = (Object*)malloc(sizeof(Object));
    *chest = (Object) {0, {Copy("wooden chest"), Copy("chest")}, Copy("It's a chest made of wood.")};
    SetProperty(&chest->properties, OBJECT_PROPERTY_CONTAINER, true);
    SetProperty(&chest->properties, OBJECT_PROPERTY_OPEN, true);
    SetProperty(&chest->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY, true);
    AppendInventory(field, chest);
    
    Object* sword = (Object*)malloc(sizeof(Object));
    *sword = (Object) {0, {Copy("iron sword"), Copy("sword")}, Copy("It's an iron sword, its blade is dull.")};
    SetProperty(&sword->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    AppendInventory(chest, sword);

    Object* pouch2 = (Object*)malloc(sizeof(Object));
    *pouch2 = (Object) {0, {Copy("leather pouch"), Copy("pouch")}, Copy("It's a leather pouch.")};
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_CONTAINER, true);
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_OPEN, true);
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY, true);
    AppendInventory(field, pouch2);

    Object* coin2 = (Object*)malloc(sizeof(Object));
    *coin2 = (Object) {0, {Copy("gold coin"), Copy("coin")}, Copy("It's a coin made of gold.")};
    SetProperty(&coin2->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    AppendInventory(pouch2, coin2);
}

void DeleteObjects()
{
    FreeMemory(&gpObjectRoot);
}

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
            /*
            if(Compare(pInv, obj))
            {
                // increase count
                return;
            }
            */
            if(pInv->next == NULL) break;
            pInv = pInv->next;
        }
        pInv->next = obj;
    }
    obj->parent = parent;
}

void DropItem(Object* obj)
{
    RemoveFromInventory(obj);
    Object* container = obj->parent;
    Object* environment;
    if(container) environment = container->parent;
    if(environment) AppendInventory(environment, obj);
}

void Open(Object* obj)
{
    if(HasProperty(obj->properties, OBJECT_PROPERTY_CONTAINER))
    {
        if(HasProperty(obj->properties, OBJECT_PROPERTY_OPEN))
        {
            printf("The %s is already open.\n", GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
        }
        else
        {    
            printf("You opened the %s.\n", GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
            SetProperty(&obj->properties, OBJECT_PROPERTY_OPEN, true);
        }
    }
    else
    {
        printf("The %s is not something you can open.\n", GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
    }
}

void Close(Object* obj)
{
    if(HasProperty(obj->properties, OBJECT_PROPERTY_CONTAINER))
    {
        if(!HasProperty(obj->properties, OBJECT_PROPERTY_OPEN))
        {
            printf("The %s is already closed.\n", GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
        }
        else
        {    
            printf("You closed the %s.\n", GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
            SetProperty(&obj->properties, OBJECT_PROPERTY_OPEN, false);
        }
    }
    else
    {
        printf("The %s is not something you can close.\n", GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
    }
}

void ListObjects(Object* head)
{
    for(Object* pObj=head; pObj != NULL; pObj=pObj->next)
    {
        printf("%s\n", pObj->tags[0]);
    }
}

void ListObjectsRecursive(Object* head, int padding)
{
    for(Object* pObj=head; pObj != NULL; pObj=pObj->next)
    {
        if(pObj == gpPlayer) continue;
        for(int p=0; p<padding; p++) printf(" ");
        printf("%s\n", pObj->tags[0]);
        if(pObj->inventoryHead
            && HasProperty(pObj->properties, OBJECT_PROPERTY_CONTAINER)
            && HasProperty(pObj->properties, OBJECT_PROPERTY_OPEN)
            && HasProperty(pObj->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY))
        {
            ListObjectsRecursive(pObj->inventoryHead, padding+1);
        }
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

    return true;
}