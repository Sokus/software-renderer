#include "commands.h"

Command* GetCommands()
{
    static bool commandsCreated = false;
    if( !commandsCreated )
    {
        commandsCreated = true;
        CreateCommands();
    }

    return commands;
}

static void CreateCommands()
{
    commands[0] = (Command) { Copy("quit"),(Distance)0, (Distance)0, ExecuteQuit };
    commands[1] = (Command) { Copy("go A"),DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel };
    commands[2] = (Command) { Copy("go to A"),DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel };
    commands[3] = (Command) { Copy("enter A"),DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel };
    commands[4] = (Command) { Copy("look around"), (Distance)0, (Distance)0, ExecuteLookAround };
    commands[5] = (Command) { Copy("look at A"), DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecuteLookAt };
    commands[6] = (Command) { Copy("examine A"), DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecuteLookAt };
    commands[7] = (Command) { Copy("pick up A"), DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecutePickUp };
    commands[8] = (Command) { Copy("get A"), DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecutePickUp };
    commands[9] = (Command) { Copy("drop A"), DISTANCE_INVENTORY, DISTANCE_INVENTORY_CONTAINED, ExecuteDrop };
    commands[10] = (Command) { Copy("help"), (Distance)0, (Distance)0, ExecuteHelp };
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
    printf("You can see:\n");
    for(Object* o=gpPlayer->parent->inventoryHead; o != NULL; o = o->next)
    {
        if(o == gpPlayer) continue;
        printf("%s\n", o->description);
    }
    return true;
}

bool ExecuteLookAt(Object *args[])
{
    Object* objA = args[0];
    if(objA != NULL)
    {
        printf("%s\n", objA->description);
    }
    return true;
}

bool ExecutePickUp(Object *args[])
{
    /*
    Object *objA = args[0];
    if(objA != NULL)
    {
        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(gpPlayer->capacity >= objA->weight)
        {
            std::cout << "Picked up " << objA->description << "." << std::endl;
            //doPickup(&player, objA);
        }
        else
        {
            std::string description = Capitalise(objA->description);
            std::cout << description << " won't fit in your pockets." << std::endl;
        }
    }
    */
    return true;
}

bool ExecuteDrop(Object *args[])
{
    /*
    Object *objA = args[0];
    if(objA != NULL)
    {
        Object *parent = gpPlayer->parent;
        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(parent != NULL && parent->capacity >= objA->weight)
        {
            std::cout << "Dropped " << objA->description << "." << std::endl;
            //doDrop(&player, objA);
        }
        else
        {
            std::cout << "There is nowhere to drop " << objA->description << "." << std::endl;
        }
    }
    */
    return true;
}

bool ExecuteHelp(Object *args[])
{
    int size = 32;
    for(int i=0; i<size; i++)
    {
        printf("%s", commands[i].pattern);
        if(i < (size-1))
        {
            if(commands[i+1].pattern == NULL) break;
            if(commands[i].function == commands[i+1].function)
            {
                printf(", ");
            }
            else
            {
                printf("\n");
            }
        }
    }
    printf("\n");
    return true;
}


