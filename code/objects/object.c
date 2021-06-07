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
    AddToInventory(gpObjectRoot, field);
    
    gpPlayer = (Object*)malloc(sizeof(Object));
    *gpPlayer = (Object) {0, { Copy("player"), Copy("you"), Copy("self")}, Copy("This is you.")}; 
    AddToInventory(field, gpPlayer);

    Object* guard = (Object*)malloc(sizeof(Object));
    *guard = (Object) {0, {Copy("city guard"), Copy("guard")}, Copy("Man who guards the city.")};
    SetProperty(&guard->properties, OBJECT_PROPERTY_NPC, true);
    AddToInventory(field, guard);

    Object* pouch = (Object*)malloc(sizeof(Object));
    *pouch = (Object) {0, {Copy("leather pouch"), Copy("pouch")}, Copy("It's a leather pouch.")};
    SetProperty(&pouch->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    SetProperty(&pouch->properties, OBJECT_PROPERTY_CONTAINER, true);
    SetProperty(&pouch->properties, OBJECT_PROPERTY_OPEN, true);
    SetProperty(&pouch->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY, true);
    AddToInventory(field, pouch);

    Object* coin = (Object*)malloc(sizeof(Object));
    *coin = (Object) {0, {Copy("silver coin"), Copy("coin")}, Copy("It's a coin made of silver.")};
    SetProperty(&coin->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    AddToInventory(gpPlayer, coin);
    
    Object* chest = (Object*)malloc(sizeof(Object));
    *chest = (Object) {0, {Copy("wooden chest"), Copy("chest")}, Copy("It's a chest made of wood.")};
    SetProperty(&chest->properties, OBJECT_PROPERTY_CONTAINER, true);
    SetProperty(&chest->properties, OBJECT_PROPERTY_OPEN, true);
    SetProperty(&chest->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY, true);
    AddToInventory(field, chest);
    
    Object* sword = (Object*)malloc(sizeof(Object));
    *sword = (Object) {0, {Copy("iron sword"), Copy("sword")}, Copy("It's an iron sword, its blade is dull.")};
    SetProperty(&sword->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    AddToInventory(chest, sword);

    Object* pouch2 = (Object*)malloc(sizeof(Object));
    *pouch2 = (Object) {0, {Copy("leather pouch"), Copy("pouch")}, Copy("It's a leather pouch.")};
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_COLLECTABLE, true);
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_CONTAINER, true);
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_OPEN, true);
    SetProperty(&pouch2->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY, true);
    AddToInventory(field, pouch2);

    Object* coin2 = (Object*)malloc(sizeof(Object));
    *coin2 = (Object) {0, {Copy("gold coin"), Copy("coin")}, Copy("It's a coin made of gold.")};
    SetProperty(&coin2->properties, OBJECT_PROPERTY_COLLECTABLE, true);
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

        FreeMemory(GetFirstFromList(pObj->inventory));
        pObj->inventory = NULL;

        FreeMemory(pObj->next);
        pObj->next = NULL;

        free(pObj);
    }
}

void ListPush(Object* member, Object* obj)
{
    if(!member || !obj) return;
    Object* first = GetFirstFromList(member);
    first->prev = obj;
    obj->next = first;
    obj->prev = NULL;
}

void ListAppend(Object* member, Object* obj)
{
    if(!member || !obj) return;
    Object* last = GetLastFromList(member);
    last->next = obj;
    obj->next = NULL;
    obj->prev = last;
}

void ListInsert(Object* member, Object* obj)
{
    if(!member || !obj) return;
    
    if(member->next) member->next->prev = obj;
    member->next = obj;
    obj->next = member->next;
    obj->prev = member;
}

void ListRemove(Object* obj)
{
    if(!obj) return;

    if(obj->next) obj->next->prev = obj->prev;
    if(obj->prev) obj->prev->next = obj->next;
    obj->next = NULL;
    obj->prev = NULL;
}

Object* GetFirstFromList(Object* member)
{
    if(!member) return NULL;
    Object* pObj = member;
    while(pObj->prev) pObj=pObj->prev;
    return pObj;
}

Object* GetLastFromList(Object* member)
{
    if(!member) return NULL;
    Object* pObj = member;
    while(pObj->next) pObj=pObj->next;
    return pObj;
}

void RemoveFromInventory(Object* obj)
{
    if(!obj || !obj->parent) return;

    if(obj->parent->inventory == obj)
    {
        obj->parent->inventory = (obj->next) ? obj->next : obj->prev; 
    }
    obj->parent = NULL;
    ListRemove(obj);
}

void AddToInventory(Object* parent, Object* obj)
{
    if(!parent || !obj) return;

    obj->parent = parent;
    if(parent->inventory == NULL)
    {
        parent->inventory = obj;
        obj->next = NULL;
        obj->prev = NULL;
    }
    else
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
       ListAppend(parent->inventory, obj);
    }
}

void DropItem(Object* obj)
{
    if(!obj) return;
    Object* container = obj->parent;
    if(!container) return;
    Object* environment = container->parent;
    if(!environment) return;
    RemoveFromInventory(obj);
    AddToInventory(environment, obj);
}

void ListObjects(Object* head, int limit)
{
    if(!head) return;

    int objectsListed = 0;
    for(Object* pObj=head;
        pObj != NULL && (objectsListed < limit || limit == 0);
        pObj=pObj->next)
    {
        if(pObj == gpPlayer) continue;

        PrintObjectInfo(pObj);
        objectsListed++;
    }
}

int GetListPosition(Object* member)
{
    if(!member) return 0;
    int position = 0;
    while(member->prev)
    {
        member = member->prev;
        position++;
    }
    return position;
}

int GetListLength(Object* member)
{
    if(!member) return 0;
    int length = 1;
    Object* p = member;
    while(p->prev)
    {
        p = p->prev;
        length++;
    }
    p = member;
    while(p->next)
    {
        p = p->next;
        length++;
    }
    return length;
}

Object* MoveListPage(Object* member, int offset)
{
    if(!member) return NULL;
    int newPage = GetListPosition(member)/LIST_MAX_ROWS + offset;
    int maxPage = GetListLength(member)/LIST_MAX_ROWS;
    newPage =   (newPage < 0)       ? 0 :
                (newPage > maxPage) ? maxPage :
                                    newPage;
    return SetListPage(member, newPage);
}

Object* SetListPage(Object* member, int page)
{
    if(!member) return NULL;
    Object* p = member;
    while(p->prev) p = p->prev;
    int position = page*LIST_MAX_ROWS;
    for(int i=0;
        i<position && p->next != NULL;
        i++)
    {
        p = p->next;
    }
    return p;
}

void PrintObjectInfo(Object* obj)
{
    char* tag = GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS);
    if(HasProperty(obj->properties, OBJECT_PROPERTY_NEW))
    {
        Console_PrintColored("%s ", COLOR_BRIGHT_CYAN, tag);
        SetProperty(&obj->properties, OBJECT_PROPERTY_NEW, false);
    }
    else
    {
        Console_Print("%s ", tag);
    }

    char* symbol = (char*)malloc(sizeof(char)*2);
    symbol[0] = (char)179, symbol[1] = '\0';

    if(HasProperty(obj->properties, OBJECT_PROPERTY_CONTAINER))
    {
        int items = GetListLength(obj->inventory);
        Console_PrintColored("[%d]", COLOR_YELLOW, items);
    }
    if(HasProperty(obj->properties, OBJECT_PROPERTY_PASSAGE))
    {
        Console_PrintColored(symbol, COLOR_BLUE);
    }
    if(HasProperty(obj->properties, OBJECT_PROPERTY_NPC))
    {
        Console_PrintColored("npc", COLOR_CYAN);
    }
    free(symbol);
    Console_Print("\n");
}

void PrintInfo()
{
    Console_Color headerColor = COLOR_CYAN;
    if(HasProperty(gContext, CONTEXT_INVENTORY_OPEN))
    {
        Console_PrintColored("Inventory: ", headerColor);

        int curPage = GetListPosition(gpPlayer->inventory)/LIST_MAX_ROWS + 1;
        int totalPages = (GetListLength(gpPlayer->inventory)-1)/LIST_MAX_ROWS + 1;
        if(totalPages>1) Console_PrintColored("[%d/%d]", headerColor, curPage, totalPages);

        Console_PrintColored("\n", headerColor);

        if(gpPlayer->inventory == NULL)
            Console_Print("(empty)\n");
        ListObjects(gpPlayer->inventory, LIST_MAX_ROWS);
    }

    if(HasProperty(gContext, CONTEXT_CONTAINER_OPEN))
    {
        char* tag = Copy(GetLongestFromArray(gpPlayer->target->tags, OBJECT_MAX_TAGS));
        Capitalise(&tag);
        Console_PrintColored("%s: ", headerColor, tag);
        free(tag);

        int curPage = GetListPosition(gpPlayer->target->inventory)/LIST_MAX_ROWS + 1;
        int totalPages = (GetListLength(gpPlayer->target->inventory)-1)/LIST_MAX_ROWS + 1;
        if(totalPages>1) Console_PrintColored("[%d/%d] ", headerColor, curPage, totalPages);

        Console_PrintColored("\n", headerColor);

        if(gpPlayer->target->inventory == NULL)
            Console_Print("(empty)\n");
        ListObjects(gpPlayer->target->inventory, LIST_MAX_ROWS);
    }

    char* tag = Copy(GetLongestFromArray(gpPlayer->parent->tags, OBJECT_MAX_TAGS));
    Capitalise(&tag);
    Console_PrintColored("%s: \n", headerColor, tag);
    free(tag);

    ListObjects(GetFirstFromList(gpPlayer->parent->inventory), 0);
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