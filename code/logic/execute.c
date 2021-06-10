#include "execute.h"

bool ExecuteClear()
{
    Console_Clear();
    return true;
}

bool ExecuteQuit()
{
    SetProperty(&gContext, CONTEXT_SHUTDOWN, true);
    return false;
}

bool ExecuteHelp()
{
    Console_PrintColored("Available commands:\n", COLOR_CYAN);
    for(int i=0; i<COMMANDS_MAX_COUNT; i++)
    {
        if(gCommands[i].pattern
            && HasProperties(gContext, gCommands[i].contextConditions))
        {
            Console_Print("%s ", gCommands[i].pattern);
            if(gCommands[i].details) 
            {
                Console_PrintColored(gCommands[i].details, COLOR_BRIGHT_BLACK);
            }
            Console_Print("\n");
        }
    }
    Console_Print("\n");
    Color disclaimerColor = COLOR_BRIGHT_YELLOW;
    Console_PrintColored("Disclaimer!\n", disclaimerColor);
    Console_PrintColored("You can put an ordinal (e.g. 'second' or '3th')\n", disclaimerColor);
    Console_PrintColored("before the <name> to implicitly pick an object\n", disclaimerColor);
    Console_PrintColored("(instead of the first matching one)\n", disclaimerColor);
    return false;
}

bool ExecuteLookAt()
{
    Argument arg = GetArgumentOfType(ARG_TYPE_TAG, 0);
    if(arg.type == ARG_TYPE_TAG && arg.p != NULL)
    {
        Object* obj = (Object*)arg.p;
        Console_Print("%s\n", obj->description);
    }
    return false;
}

bool ExecuteInventory()
{
    if( !HasProperty(gContext, CONTEXT_INVENTORY_OPEN) )
    {
        SetProperty(&gContext, CONTEXT_INVENTORY_OPEN, true);
        gpPlayer->inventory = GetFirstFromList(gpPlayer->inventory);
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
    Argument arg0 = GetArgumentOfType(ARG_TYPE_TAG, 0);
    Argument arg1 = GetArgumentOfType(ARG_TYPE_PROPERTY, 0);
    if(arg0.type == ARG_TYPE_TAG && arg0.p)
    {
        if(arg1.type == ARG_TYPE_PROPERTY
            && !HasProperty(gContext, arg1.property))
        return false;

        arg0.p->inventory = GetListPageRelative(arg0.p->inventory, 1);
        return true;
    }
    return false;
}

static bool ExecutePrev()
{
    Argument arg0 = GetArgumentOfType(ARG_TYPE_TAG, 0);
    Argument arg1 = GetArgumentOfType(ARG_TYPE_PROPERTY, 0);
    if(arg0.type == ARG_TYPE_TAG && arg0.p)
    {
        if(arg1.type == ARG_TYPE_PROPERTY
            && !HasProperty(gContext, arg1.property))
        return false;

        arg0.p->inventory = GetListPageRelative(arg0.p->inventory, -1);
        return true;
    }
    return false;
}

static bool ExecutePage()
{
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
        argT.p->inventory = GetListPage(argT.p->inventory, page);
        return true;
    }
    return false;
}

bool ExecuteInventoryNext()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=CONTEXT_INVENTORY_OPEN});
    return ExecuteNext();
}

bool ExecuteInventoryPrev()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=CONTEXT_INVENTORY_OPEN});
    return ExecutePrev();
}

bool ExecuteInventoryPage()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=CONTEXT_INVENTORY_OPEN});
    return ExecutePage();
}

bool ExecuteContainerNext()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->target});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=CONTEXT_CONTAINER_OPEN});
    return ExecuteNext();
}

bool ExecuteContainerPrev()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->target});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=CONTEXT_CONTAINER_OPEN});
    return ExecutePrev();
}

bool ExecuteContainerPage()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->target});
    AppendArgument((Argument){ ARG_TYPE_PROPERTY, .property=CONTEXT_CONTAINER_OPEN});
    return ExecutePage();
}

bool ExecuteLocationNext()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->parent});
    return ExecuteNext();
}

bool ExecuteLocationPrev()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->parent});
    return ExecutePrev();
}

bool ExecuteLocationPage()
{
    AppendArgument((Argument){ ARG_TYPE_TAG, gpPlayer->parent});
    return ExecutePage();
}

bool ExecutePickUp()
{
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
}

// NOTE(sokus): Same to ExecutePickUp
bool ExecuteDrop()
{
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
}

bool ExecuteOpen()
{
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;

    if(obj)
    {
        if( HasProperty(obj->properties, OBJECT_PROPERTY_CONTAINER) )
        {
            bool alreadyInContainer = HasProperty(gContext, CONTEXT_CONTAINER_OPEN);
            bool noTarget = !gpPlayer->target;
            if( noTarget || alreadyInContainer)
            {
                SetProperty(&gContext, CONTEXT_CONTAINER_OPEN, true);
                gpPlayer->target = obj;
                obj->inventory = GetFirstFromList(obj->inventory);
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
}

// NOTE(sokus): Same to ExecutePickUp
bool ExecuteMoveToInventory()
{
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
}

bool ExecuteMoveToContainer()
{
    Object* obj0 = GetArgumentOfType(ARG_TYPE_TAG, 0).p;
    Object* obj1 = GetArgumentOfType(ARG_TYPE_TAG, 1).p;
    if(!obj0 || !obj1) return false;

    bool success = false;
    bool isContainer = HasProperty(obj1->properties, OBJECT_PROPERTY_CONTAINER);
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
}

bool ExecuteClose()
{
    if(HasProperty(gContext, CONTEXT_CONTAINER_OPEN))
        return ExecuteCloseContainer();
    else if(HasProperty(gContext, CONTEXT_INVENTORY_OPEN))
        return ExecuteCloseInventory();
    return false;
}

bool ExecuteCloseInventory()
{
    if(HasProperty(gContext, CONTEXT_INVENTORY_OPEN))
    {
        SetProperty(&gContext, CONTEXT_INVENTORY_OPEN, false);
        return true;
    }
    return false;
}

bool ExecuteCloseContainer()
{
    if(HasProperty(gContext, CONTEXT_CONTAINER_OPEN))
    {
        bool hasTarget = gpPlayer->target != NULL;
        Object* parent = hasTarget ? gpPlayer->target->parent : NULL;
        bool targetInContainer =
            parent && HasProperty(parent->properties, OBJECT_PROPERTY_CONTAINER);
        bool containerIsPlayerOrEnv = parent == gpPlayer || parent == gpPlayer->parent;

        if(hasTarget && targetInContainer && !containerIsPlayerOrEnv)
        {
            gpPlayer->target = parent;
        }
        else
        {
            SetProperty(&gContext, CONTEXT_CONTAINER_OPEN, false);
            gpPlayer->target = NULL;
        }
        return true;
    }
    return false;
}
bool ExecuteDebug()
{   
    bool debug = HasProperty(gContext, CONTEXT_DEBUG_MODE);
    SetProperty(&gContext, CONTEXT_DEBUG_MODE, !debug);
    if(!debug)
        Console_PrintColored("Debug mode enabled.\n", COLOR_BRIGHT_RED);
    else
        Console_PrintColored("Debug mode disabled.\n", COLOR_BRIGHT_RED);
    return false;
}

bool ExecuteListProperties()
{
    Object* obj = GetArgumentOfType(ARG_TYPE_TAG, 0).p;
    if(obj)
    {
        char* tag = GetLongestFromArray(obj->tags, OBJECT_MAX_TAGS);
        Console_Print("Properties of the %s:\n", tag);
        PrintProperties(obj->properties);
    }
    return false;
}

bool ExecuteSetProperty()
{
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
}