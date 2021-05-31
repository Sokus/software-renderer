#include "execute.h"

bool ExecuteQuit(Argument args[])
{
    printf("Goodbye.\n");
    return false;
}

bool ExecuteLookAround(Argument args[])
{
    printf("%s\n", gpPlayer->parent->description);
    Object* pObj=gpPlayer->parent->inventoryHead;
    if(pObj) printf("You can see:\n");
    while(pObj)
    {
        if(pObj != gpPlayer)
        {
            char* tag = Copy(pObj->tags[0]);
            tag = AddArticle(tag);
            printf("%s\n", tag);
            free(tag);
        }
        pObj = pObj->next;
    }
    return true;
}

bool ExecuteLookAt(Argument args[])
{
    Argument arg = GetArgumentOfType(ARG_TYPE_TAG, 0);
    if(arg.type == ARG_TYPE_TAG && arg.p != NULL)
    {
        Object* obj = (Object*)arg.p;
        printf("%s\n", obj->description);
    }
    return true;
}

bool ExecuteExamineInventory(Argument args[])
{
    if( !gpPlayer->inventoryHead )
    {
        printf("Your inventory is empty.\n");
    }
    else
    {
        printf("You have:\n");
        ListObjects(gpPlayer->inventoryHead);
    }
    return true;
}

bool ExecutePickUp(Argument args[])
{
    Argument arg = GetArgumentOfType(ARG_TYPE_TAG, 0);
    if(arg.type == ARG_TYPE_TAG && arg.p != NULL)
    {
        Object* obj = (Object*)arg.p;
        if( HasProperty(obj->properties, OBJECT_PROPERTY_COLLECTABLE) )
        {
            RemoveFromInventory(obj);
            AppendInventory(gpPlayer, obj);
            char* tag = AddArticle(Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS)));
            printf("You got %s.\n", tag);
            free(tag);
        }
        else
        {
            char* tag = Capitalise(AddArticle(Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS))));
            printf("%s can't be picked up.\n", tag);
            free(tag);
        }
    }
    return true;
}

bool ExecuteDrop(Argument args[])
{
    Argument arg = GetArgumentOfType(ARG_TYPE_TAG, 0);
    if(arg.type == ARG_TYPE_TAG && arg.p != NULL)
    {
        Object* obj = (Object*)arg.p;
        if( HasProperty(obj->properties, OBJECT_PROPERTY_COLLECTABLE) )
        {
            DropItem(obj);
            char* tag = AddArticle(Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS)));
            printf("You dropped %s.\n", tag);
            free(tag);
        }
        else
        {
            char* tag = Capitalise(AddArticle(Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS))));
            printf("%s can't be dropped.\n", tag);
            free(tag);
        }
    }
    return true;
}

bool ExecutePut(Argument args[])
{
    Object* objA = GetArgumentOfType(ARG_TYPE_TAG, 0).p;
    Object* objB = GetArgumentOfType(ARG_TYPE_TAG, 1).p;

    if(!objA || !objB) return true;

    bool possible = true;

    char* tagA = Copy(GetLongestFromArray(objA->tags, OBJECT_MAX_TAGS));
    char* tagB = Copy(GetLongestFromArray(objB->tags, OBJECT_MAX_TAGS));
    if(objA == objB)
    {
        possible = false;
        char* articledA = AddArticle(Copy(tagA));
        printf("You can't put %s in itself.\n", articledA);
        free(articledA);
    }

    int depth = GetDepth(objA, objB);
    if(depth > 0)
    {
        possible = false;
        printf("The %s is already contained in the %s.\n", tagB, tagA);
    }

    depth = GetDepth(objB, objA);
    if(depth == 1)
    {
        possible = false;
        printf("The %s in already contained in the %s.\n", tagA, tagB);
    }
        
    bool A_isCollectable = HasProperty(objA->properties, OBJECT_PROPERTY_COLLECTABLE);
    bool B_isContainer = HasProperty(objB->properties, OBJECT_PROPERTY_CONTAINER);
    bool B_isOpen = HasProperty(objB->properties, OBJECT_PROPERTY_OPEN);

    if( !A_isCollectable )
    {
        possible = false;
        printf("You can't pick up the %s.\n", tagA);
    }

    if( !B_isContainer )
    {
        possible = false;
        char* capArtB = Capitalise(AddArticle(Copy(tagB)));
        printf("%s isn't a container.\n", capArtB);
        free(capArtB);

    }

    if(B_isContainer && !B_isOpen)
    {
        possible = false;
        printf("The %s is closed.\n", tagB);
    }

    if(possible)
    {
        printf("You placed the %s in the %s.\n", tagA, tagB);
        RemoveFromInventory(objA);
        AppendInventory(objB, objA);
    }
    free(tagA);
    free(tagB);
    return true;
}

bool ExecuteClear(Argument args[])
{
    Console_Clear();
    return true;
}

bool ExecuteList(Argument args[])
{
    Object* head = gpPlayer->parent->inventoryHead;
    ListObjectsRecursive(head, 0);
    return true;
}

bool ExecuteOpen(Argument args[])
{
    Argument argA = GetArgumentOfType(ARG_TYPE_TAG, 0);

    if(argA.p) Open(argA.p);

    return true;
}

bool ExecuteClose(Argument args[])
{
    Argument argA = GetArgumentOfType(ARG_TYPE_TAG, 0);

    if(argA.p) Close(argA.p);

    return true;
}