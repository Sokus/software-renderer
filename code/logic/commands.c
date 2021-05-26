#include "commands.h"

Command* GetCommands()
{
    static bool commandsCreated = false;
    if( !commandsCreated )
    {
        commandsCreated = true;
        CreateCommands();
    }

    return gCommands;
}

static void CreateCommands()
{

    gCommands[0] = (Command) { {Copy("look around")},
                                0, 0, ExecuteLookAround };
    
    gCommands[1] = (Command) { {Copy("look at A"), Copy("examine A"), Copy("check A")},
                                DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecuteLookAt };

    gCommands[2] = (Command) { {Copy("look inside A"), Copy("look into A"), Copy("look in A")},
                                DISTANCE_SELF, DISTANCE_NEAR_CONTAINED, ExecuteLookInside };

    gCommands[3] = (Command) { {Copy("check inventory"), Copy("inventory")},
                                0, 0, ExecuteExamineInventory };

    gCommands[4] = (Command) { {Copy("pick up A"), Copy("get A")},
                                DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecutePickUp };

    gCommands[5] = (Command) { {Copy("put A into B"), Copy("put A in B"), Copy("insert A into B"), Copy("place A in B")},
                                DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecutePut };

    gCommands[6] = (Command) { {Copy("drop A")},
                                DISTANCE_INVENTORY, DISTANCE_INVENTORY_CONTAINED, ExecuteDrop };

    /*
    gCommands[7] = (Command) { {Copy("go A"), Copy("go to A"), Copy("enter A")},
                                DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel };
    */
    gCommands[8] = (Command) { {Copy("clear")}, 0, 0, ExecuteClear };

    gCommands[9] = (Command) { {Copy("quit")}, 0, 0, ExecuteQuit };

    gCommands[10] = (Command) { {Copy("help")},
                                0, 0, ExecuteHelp };
}

bool ExecuteQuit(Object *args[])
{
    printf("Goodbye.\n");
    return false;
}

bool ExecuteTravel(Object *args[])
{
    /*
    Object *objA = args[0];
    if(objA != nullptr)
    {
        if(objA->destination != nullptr && ObjectHasProperty(*objA, ObjectProperty::OBJECT_PROPERTY_TRAVERSABLE))
        {
            //doTravel(&player, objA);
        }
        else
        {
            std::string description = Capitalise(objA->description);
            std::cout << description << " can't be traveled." << std::endl;
        }
    }
    */
    return true;
}

bool ExecuteLookAround(Object *args[])
{
    printf("%s\n", gpPlayer->parent->details);
    Object* pObj=gpPlayer->parent->inventoryHead;
    if(pObj) printf("You can see:\n");
    while(pObj)
    {
        if(pObj != gpPlayer) printf("%s\n", pObj->description);
        pObj = pObj->next;
    }
    return true;
}

bool ExecuteLookAt(Object *args[])
{
    Object* objA = args[0];
    if(objA)
    {
        printf("%s\n", objA->details);
    }
    return true;
}

bool ExecuteLookInside(Object* args[])
{
    Object* objA = args[0];
    if(objA)
    {
        if( !HasProperty(objA->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY) )
        {
            printf("You can't look into %s.\n", objA->description);
        }
        else
        {
            printf("You look into %s.\n", objA->description);
            if( !objA->inventoryHead )
            {
                printf("There is nothing.\n");
            }
            else
            {
                printf("There is:\n");
                ListObjects(objA->inventoryHead);
            }
        }
    }
    return true;
}

bool ExecuteExamineInventory(Object* args[])
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

bool ExecutePickUp(Object *args[])
{
    Object *objA = args[0];
    if(objA)
    {
        if( HasProperty(objA->properties, OBJECT_PROPERTY_COLLECTABLE) )
        {
            RemoveFromInventory(objA);
            AppendInventory(gpPlayer, objA);
            printf("You got %s.\n", objA->description);
        }
        else
        {
            char* description = Copy(objA->description);
            Capitalise(description);
            printf("%s can't be picked up.\n", description);
            free(description);
        }
    }
    return true;
}

bool ExecuteDrop(Object *args[])
{
    Object *objA = args[0];
    if(objA)
    {
        if( HasProperty(objA->properties, OBJECT_PROPERTY_COLLECTABLE) )
        {
            DropItem(objA);
            printf("You dropped %s.\n", objA->description);
        }
        else
        {
            char* description = Copy(objA->description);
            Capitalise(description);
            printf("%s can't be dropped.\n", description);
            free(description);
        }
    }
    return true;
}

bool ExecutePut(Object* args[])
{
    Object *objA = args[0];
    Object *objB = args[1];
    if(objA && objB)
    {
        if(objA == objB)
        {
            printf("You can't put %s in itself.\n", objA->description);
            return true;
        }

        int depth = GetDepth(objA, objB);
        if(depth > 0)
        {
            printf("You can't do that, %s is contained in %s.\n",
                    objB->description, objA->description);
            return true;
        }
        
        bool A_isCollectable = HasProperty(objA->properties, OBJECT_PROPERTY_COLLECTABLE);
        bool B_isContainer = HasProperty(objB->properties, OBJECT_PROPERTY_CONTAINER );

        if( !A_isCollectable )
        {
            printf("You can't pick up %s.\n", objA->description);
            return true;
        }

        if( !B_isContainer )
        {
            char* description = Copy(objB->description);
            Capitalise(description);
            printf("%s isn't a container.\n", description);
            free(description);
        }

        if(A_isCollectable && B_isContainer)
        {
            printf("You placed %s in %s.\n", objA->description, objB->description);
            RemoveFromInventory(objA);
            AppendInventory(objB, objA);
        }
    }
    return true;
}

bool ExecuteClear(Object* args[])
{
    Console_Clear();
    return true;
}

bool ExecuteHelp(Object *args[])
{
    for(int c_i=0; c_i<COMMANDS_MAX_COUNT; c_i++)
    {
        bool anyPatternFound = false;
        for(int p_i=0; p_i<COMMANDS_MAX_PATTERNS; p_i++)
        {
            char* pattern = gCommands[c_i].patterns[p_i];
            if(pattern)
            {
                anyPatternFound = true;
                if(p_i != 0) printf(", ");
                printf(pattern);
            }
        }
        if(anyPatternFound) printf("\n");
    }
    return true;
}