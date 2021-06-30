#include "execute.h"

#include "console/output.h"
#include "utility/linked_list.h"
#include "utility/macros.h"

bool ExecuteClear()
{
    Console_Clear();
}

bool ExecuteQuit()
{
    SetProperty(&gContext, GS_SHUTDOWN, true);
}

bool ExecuteHelp()
{
}

bool ExecuteLookAt()
{
}

bool ExecuteInventory()
{
    if( !HasProperty(gContext, GS_INVENTORY_OPEN) )
    {
        SetProperty(&gContext, GS_INVENTORY_OPEN, true);
        if(gpPlayer->inventory)
        {
            LinkedListNode* node = GetFirstFromList(&gpPlayer->inventory->list);
            gpPlayer->inventory = CONTAINEROF(node, Object, list);
        }
        return true;
    }
    else
    {
        Console_Print("Inventory is already open.\n");
    }
    return false;
}

static bool ExecuteNext()
{
    /*
    Argument arg0 = GetArgumentOfType(ARG_TYPE_TAG, 0);
    Argument arg1 = GetArgumentOfType(ARG_TYPE_PROPERTY, 0);
    if(arg0.type == ARG_TYPE_TAG && arg0.p)
    {
        if(arg1.type == ARG_TYPE_PROPERTY
            && !HasProperty(gContext, arg1.property))
        return false;

        if(arg0.p->inventory)
        {
            LinkedListNode* node = GetListPageRelative(&arg0.p->inventory->objList, 1);
            arg0.p->inventory = CONTAINEROF(node, Object, objList);
        }
        return true;
    }
    return false;
    */
}

static bool ExecutePrev()
{
    /*
    Argument arg0 = GetArgumentOfType(ARG_TYPE_TAG, 0);
    Argument arg1 = GetArgumentOfType(ARG_TYPE_PROPERTY, 0);
    if(arg0.type == ARG_TYPE_TAG && arg0.p)
    {
        if(arg1.type == ARG_TYPE_PROPERTY
            && !HasProperty(gContext, arg1.property))
        return false;

        if(arg0.p->inventory)
        {
            LinkedListNode* node = GetListPageRelative(&arg0.p->inventory->objList, -1);
            arg0.p->inventory = CONTAINEROF(node, Object, objList);
        }
        return true;
    }
    return false;
    */
}

static bool ExecutePage()
{
    /*
    Argument argT = GetArgumentOfType(ARG_TYPE_TAG, 0);
    Argument argP = GetArgumentOfType(ARG_TYPE_PROPERTY, 0);
    if(argT.type == ARG_TYPE_TAG && argT.p)
    {
        if(argP.type == ARG_TYPE_PROPERTY
            && !HasProperty(gContext, argP.property))
        return false;

        int page = 0;
        Argument argI = GetArgumentOfType(ARG_TYPE_INT, 0);
        Argument argO = GetArgumentOfType(ARG_TYPE_ORDINAL, 0);
        if(argI.type == ARG_TYPE_INT) page = argI.value - 1;
        if(argO.type == ARG_TYPE_ORDINAL) page = argO.value;
        if(argT.p->inventory)
        {
            LinkedListNode* node = GetListPage(&argT.p->inventory->objList, page);
            argT.p->inventory = CONTAINEROF(node, Object, objList);
        }
        return true;
    }
    return false;
    */
}

bool ExecuteInventoryNext()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=GS_INVENTORY_OPEN});
    return ExecuteNext();
    */
}

bool ExecuteInventoryPrev()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=GS_INVENTORY_OPEN});
    return ExecutePrev();
    */
}

bool ExecuteInventoryPage()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=GS_INVENTORY_OPEN});
    return ExecutePage();
    */
}

bool ExecuteContainerNext()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->target});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=GS_CONTAINER_OPEN});
    return ExecuteNext();
    */
}

bool ExecuteContainerPrev()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->target});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=GS_CONTAINER_OPEN});
    return ExecutePrev();
    */
}

bool ExecuteContainerPage()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->target});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=GS_CONTAINER_OPEN});
    return ExecutePage();
    */
}

bool ExecuteLocationNext()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->parent});
    return ExecuteNext();
    */
}

bool ExecuteLocationPrev()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->parent});
    return ExecutePrev();
    */
}

bool ExecuteLocationPage()
{
    /*
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->parent});
    return ExecutePage();
    */
}

bool ExecutePickUp()
{
    /*
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;

    bool success = PickUpItem(gpPlayer, obj);
    if(!success)
    {
        char* tag = Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
        AddArticle(&tag);
        Capitalise(&tag);
        Console_Print("%s can't be picked up.\n", tag);
        free(tag);
    }

    return success;
    */
}

// NOTE(sokus): Same to ExecutePickUp
bool ExecuteDrop()
{
    /*
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;

    bool success = DropItem(obj);
    if(!success)
    {
        char* tag = Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
        AddArticle(&tag);
        Capitalise(&tag);
        Console_Print("%s can't be dropped.\n", tag);
        free(tag);
    }

    return success;
    */
}

bool ExecuteOpen()
{
    /*
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;

    if(obj)
    {
        if( HasProperty(obj->properties, OP_CONTAINER) )
        {
            bool alreadyInContainer = HasProperty(gContext, GS_CONTAINER_OPEN);
            bool noTarget = !gpPlayer->target;
            if( noTarget || alreadyInContainer)
            {
                SetProperty(&gContext, GS_CONTAINER_OPEN, true);
                gpPlayer->target = obj;
                if(obj->inventory)
                {
                    LinkedListNode* node = GetFirstFromList(&obj->inventory->objList);
                    obj->inventory = CONTAINEROF(node, Object, objList);
                }
                return true;
            }
            else
            {
                Console_Print("You are already doing something else.\n");
            }
        }
        else
        {
            char* tag = Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
            AddArticle(&tag);
            Capitalise(&tag);
            Console_Print("%s can't be opened.\n", tag);
            free(tag);
        }
    }
    return false;
    */
}

// NOTE(sokus): Same to ExecutePickUp
bool ExecuteMoveToInventory()
{
    /*
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;

    bool success = PickUpItem(gpPlayer, obj);
    if(!success)
    {
        char* tag = Copy(GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS));
        AddArticle(&tag);
        Capitalise(&tag);
        Console_Print("%s can't be moved.\n", tag);
        free(tag);
    }

    return success;
    */
}

bool ExecuteMoveToContainer()
{
    /*
    Object* obj0 = GetArgumentOfType(ARG_TYPE_TAG, 0).p;
    Object* obj1 = GetArgumentOfType(ARG_TYPE_TAG, 1).p;
    if(!obj0 || !obj1) return false;

    bool success = false;
    bool isContainer = HasProperty(obj1->properties, OP_CONTAINER);
    if(isContainer)
    {
        success = PickUpItem(obj1, obj0);
        if(!success)
        {
            char* tag = Copy(GetLongestFromArray(obj0->tags, OBJECT_MAX_TAGS));
            AddArticle(&tag);
            Capitalise(&tag);
            Console_Print("%s can't be moved.\n", tag);
            free(tag);
        }
    }
    else
    {
        char* tag = Copy(GetLongestFromArray(obj1->tags, OBJECT_MAX_TAGS));
        AddArticle(&tag);
        Capitalise(&tag);
        Console_Print("%s isn't a container.\n", tag);
        free(tag);
    }
    return success;
    */
}

bool ExecuteClose()
{
    /*
    if(HasProperty(gContext, GS_CONTAINER_OPEN))
        return ExecuteCloseContainer();
    else if(HasProperty(gContext, GS_INVENTORY_OPEN))
        return ExecuteCloseInventory();
    return false;
    */
}

bool ExecuteCloseInventory()
{
    /*
    if(HasProperty(gContext, GS_INVENTORY_OPEN))
    {
        SetProperty(&gContext, GS_INVENTORY_OPEN, false);
        return true;
    }
    return false;
    */
}

bool ExecuteCloseContainer()
{
    /*
    if(HasProperty(gContext, GS_CONTAINER_OPEN))
    {
        bool hasTarget = gpPlayer->target != NULL;
        Object* parent = hasTarget ? gpPlayer->target->parent : NULL;
        bool targetInContainer =
            parent && HasProperty(parent->properties, OP_CONTAINER);
        bool containerIsPlayerOrEnv = parent == gpPlayer || parent == gpPlayer->parent;

        if(hasTarget && targetInContainer && !containerIsPlayerOrEnv)
        {
            gpPlayer->target = parent;
        }
        else
        {
            SetProperty(&gContext, GS_CONTAINER_OPEN, false);
            gpPlayer->target = NULL;
        }
        return true;
    }
    return false;
    */
}
bool ExecuteDebug()
{   
    /*
    bool debug = HasProperty(gContext, GS_DEBUG);
    SetProperty(&gContext, GS_DEBUG, !debug);
    if(!debug)
        Console_PrintColored("Debug mode enabled.\n", COLOR_BRIGHT_RED);
    else
        Console_PrintColored("Debug mode disabled.\n", COLOR_BRIGHT_RED);
    return false;
    */
}

bool ExecuteListProperties()
{
    /*
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;
    if(obj)
    {
        char* tag = GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS);
        Console_Print("Properties of the %s:\n", tag);
        PrintProperties(obj->properties);
    }
    return false;
    */
}

bool ExecuteSetProperty()
{
    /*
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;
    Property property = GetArgumentOfType(ARG_TYPE_PROPERTY, 0).value;
    bool value = GetArgumentOfType(ARG_TYPE_BOOL, 0).value != 0;
    if(obj)
    {
        bool currentValue = HasProperty(obj->properties, property);
        if(value == currentValue)
        {
            Console_Print("Property ");
            PrintPropertyName(property);
            Console_Print(" is already set to %d.\n", value);
            return false;
        }
        else
        {
            SetProperty(&obj->properties, property, value);
            return true;
        }
    }
    return false;
    */
}