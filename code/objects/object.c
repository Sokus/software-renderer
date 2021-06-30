#include "object.h"

#include <stdlib.h>

#include "console/output.h"
#include "utility/strops.h"
#include "utility/macros.h"

Object* gpObjectRoot;
Object* gpPlayer;

void CreateObjects()
{
    if(gpObjectRoot) return;
    
    gpObjectRoot = (Object*)malloc(sizeof(Object));
    *gpObjectRoot = (Object){0};    


    Object* field = (Object*)malloc(sizeof(Object));
    *field = (Object) {0, {Copy("grassy field"), Copy("field")}, Copy("Huge field covered in grass.")}; 
    AddToInventory(gpObjectRoot, field);
    
    gpPlayer = (Object*)malloc(sizeof(Object));
    *gpPlayer = (Object) {0, { Copy("player"), Copy("you"), Copy("self")}, Copy("This is you.")}; 
    AddToInventory(field, gpPlayer);

    Object* guard = (Object*)malloc(sizeof(Object));
    *guard = (Object) {0, {Copy("city guard"), Copy("guard")}, Copy("Man who guards the city.")};
    SetProperty(&guard->properties, OP_NPC, true);
    AddToInventory(field, guard);

    Object* pouch = (Object*)malloc(sizeof(Object));
    *pouch = (Object) {0, {Copy("leather pouch"), Copy("pouch")}, Copy("It's a leather pouch.")};
    SetProperty(&pouch->properties, OP_COLLECTABLE, true);
    SetProperty(&pouch->properties, OP_CONTAINER, true);
    SetProperty(&pouch->properties, OP_OPEN, true);
    SetProperty(&pouch->properties, OP_VISIBLE_INVENTORY, true);
    AddToInventory(field, pouch);

    Object* coin = (Object*)malloc(sizeof(Object));
    *coin = (Object) {0, {Copy("silver coin"), Copy("coin")}, Copy("It's a coin made of silver.")};
    SetProperty(&coin->properties, OP_COLLECTABLE, true);
    AddToInventory(gpPlayer, coin);
    
    Object* chest = (Object*)malloc(sizeof(Object));
    *chest = (Object) {0, {Copy("wooden chest"), Copy("chest")}, Copy("It's a chest made of wood.")};
    SetProperty(&chest->properties, OP_CONTAINER, true);
    SetProperty(&chest->properties, OP_OPEN, true);
    SetProperty(&chest->properties, OP_VISIBLE_INVENTORY, true);
    AddToInventory(field, chest);
    
    Object* sword = (Object*)malloc(sizeof(Object));
    *sword = (Object) {0, {Copy("iron sword"), Copy("sword")}, Copy("It's an iron sword, its blade is dull.")};
    SetProperty(&sword->properties, OP_COLLECTABLE, true);
    AddToInventory(gpPlayer, sword);

    Object* pouch2 = (Object*)malloc(sizeof(Object));
    *pouch2 = (Object) {0, {Copy("leather pouch"), Copy("pouch")}, Copy("It's a leather pouch.")};
    SetProperty(&pouch2->properties, OP_COLLECTABLE, true);
    SetProperty(&pouch2->properties, OP_CONTAINER, true);
    SetProperty(&pouch2->properties, OP_OPEN, true);
    SetProperty(&pouch2->properties, OP_VISIBLE_INVENTORY, true);
    AddToInventory(field, pouch2);

    Object* coin2 = (Object*)malloc(sizeof(Object));
    *coin2 = (Object) {0, {Copy("gold coin"), Copy("coin")}, Copy("It's a coin made of gold.")};
    SetProperty(&coin2->properties, OP_COLLECTABLE, true);
    AddToInventory(gpPlayer, coin2);
}

void DeleteObjects()
{
    FreeMemory(gpObjectRoot);
    gpObjectRoot = NULL;
}

void FreeMemory(Object* pObj)
{
    if(pObj)
    {
        for(int i=0; i<OBJECT_MAX_TAGS; i++)
        {
            free(pObj->tags[i]);
            pObj->tags[i] = NULL;
        }

        free(pObj->description);
        pObj->description = NULL;

        if(pObj->inventory)
        {
            LinkedListNode* invNode = GetFirstFromList(&pObj->inventory->list);
            Object* invHead = CONTAINEROF(invNode, Object, list);
            FreeMemory(invHead);
            pObj->inventory = NULL;
        }

        if(pObj->list.next)
        {
            Object* next = CONTAINEROF(pObj->list.next, Object, list);
            FreeMemory(next);
            pObj->list.next = NULL;
        }

        free(pObj);
    }
}

void ListObjects(Object* head, int limit)
{
    int objectsListed = 0;
    Object* pObj=head;
    while(pObj && (objectsListed < limit || limit == 0))
    {
        PrintObjectInfo(pObj);
        objectsListed++;
        if(pObj->list.next)
        {
            pObj = CONTAINEROF(pObj->list.next, Object, list);
        }
        else
        {
            pObj = NULL;
        }
    }
}

void PrintObjectInfo(Object* obj)
{
    if(!obj) return;
    char* tag = GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS);
    if(HasProperty(obj->properties, OP_NEW))
    {
        Console_PrintColored("%s ", COLOR_BRIGHT_CYAN, tag);
        SetProperty(&obj->properties, OP_NEW, false);
    }
    else
    {
        Console_Print("%s ", tag);
    }

    char* symbol = "\xB3 ";
    if(HasProperty(obj->properties, OP_CONTAINER))
    {
        int items = (obj->inventory) ?
                    GetListLength(&obj->inventory->list) : 
                    0;
        Console_PrintColored("[%d] ", COLOR_YELLOW, items);
    }
    if(HasProperty(obj->properties, OP_PASSAGE))
    {
        Console_PrintColored(symbol, COLOR_BLUE);
    }
    if(HasProperty(obj->properties, OP_NPC))
    {
        Console_PrintColored(symbol, COLOR_CYAN);
    }
    Console_Print("\n");
}

void PrintPageInfo(Object* inventory)
{
    if(!inventory) return;
    int currentPage = GetListPosition(&inventory->list)/LIST_MAX_ROWS;
    int totalPages = (GetListLength(&inventory->list)-1)/LIST_MAX_ROWS;
    if(totalPages) Console_PrintColored("[%d/%d]", COLOR_CYAN, currentPage+1, totalPages+1);
}

void PrintInfo()
{
    Color headerColor = COLOR_CYAN;
    if(HasProperty(gContext, GS_INVENTORY_OPEN))
    {
        Console_PrintColored("Inventory: ", headerColor);
        PrintPageInfo(gpPlayer->inventory);
        Console_Print("\n");
        ListObjects(gpPlayer->inventory, LIST_MAX_ROWS);
        if(!gpPlayer->inventory)
        {
            Console_Print("(empty)\n");
        }
    }

    if(HasProperty(gContext, GS_CONTAINER_OPEN) && gpPlayer->target)
    {
        char* tag = Copy(GetLongestFromArray(gpPlayer->target->tags, OBJECT_MAX_TAGS));
        Capitalise(&tag);
        Console_PrintColored("%s: ", headerColor, tag);
        free(tag);

        PrintPageInfo(gpPlayer->target->inventory);
        Console_Print("\n");
        ListObjects(gpPlayer->target->inventory, LIST_MAX_ROWS);
        if(!gpPlayer->target->inventory)
        {
            Console_Print("(empty)\n");
        }
    }

    {
        char* tag = Copy(GetLongestFromArray(gpPlayer->parent->tags, OBJECT_MAX_TAGS));
        Capitalise(&tag);
        Console_PrintColored("%s: ", headerColor, tag);
        free(tag);

        PrintPageInfo(gpPlayer->parent->inventory);
        Console_Print("\n");
        ListObjects(gpPlayer->parent->inventory, LIST_MAX_ROWS);
        if(!gpPlayer->parent->inventory)
        {
            Console_Print("(empty)\n");
        }
    }
}

void RemoveFromInventory(Object* obj)
{
    if(!obj || !obj->parent) return;

    if(obj->parent->inventory == obj)
    {
        LinkedListNode* newInvNode = (obj->list.next) ?
                                    obj->list.next :
                                    obj->list.prev; 
        obj->parent->inventory = CONTAINEROF(newInvNode, Object, list);
    }
    ListRemove(&obj->list);
    obj->parent = NULL;
}

void AddToInventory(Object* parent, Object* obj)
{
    if(!parent || !obj) return;

    obj->parent = parent;

    if(parent->inventory)
    {
        /* 
        for(Object* pObj = GetFirstFromList(parent->inventory);
            pObj != NULL;
            pObj = pObj->next)
        {
            if(CompareObjects(pObj, obj))
            {
                // increase count
                return;
            }
        }
        */

       ListAppend(&parent->inventory->list, &obj->list);
    }
    else
    {
        parent->inventory = obj;
        obj->list.next = NULL;
        obj->list.prev = NULL;
    }
}

bool PickUpItem(Object* parent, Object* obj)
{
    if(!parent || !obj) return false;
    if(!HasProperty(obj->properties, OP_COLLECTABLE))
        return false;
    Object* oldParent = obj->parent;
    RemoveFromInventory(obj);
    if(oldParent)
    {
        LinkedListNode* node = GetListPageAligned(&oldParent->inventory->list);
        oldParent->inventory = CONTAINEROF(node, Object, list);
    }
    AddToInventory(parent, obj);
    SetProperty(&obj->properties, OP_NEW, true);
    return true;
}

bool DropItem(Object* obj)
{
    if(!obj) return false;
    if(!HasProperty(obj->properties, OP_COLLECTABLE))
        return false;
    Object* container = obj->parent;
    if(!container) return false;
    Object* environment = container->parent;
    if(!environment) return false;

    RemoveFromInventory(obj);
    if(container->inventory)
    {
        LinkedListNode* node = GetListPageAligned(&container->inventory->list);
        container->inventory = CONTAINEROF(node, Object, list);
    }
    AddToInventory(environment, obj);
    SetProperty(&obj->properties, OP_NEW, true);
    return true;
}