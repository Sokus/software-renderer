#include "commands.hpp"

Command::Command(std::string pattern,
            Distance minDistance, Distance maxDistance,
            int (*function)(Object **))
    : pattern(pattern),
    minDistance(minDistance), maxDistance(maxDistance),
    function(function) { }

int executeQuit(Object *args[])
{
    std::cout << "Goodbye." << std::endl;
    return 0;
}

int executeTravel(Object *args[])
{
    Object *objA = args[0];
    if(objA != NULL)
    {
        if(objA->destination != NULL)
        {
            //doTravel(&player, objA);
            std::cout << "Traveled " << objA->description << "." << std::endl;
        }
        else
        {
            std::string desc = capitalise(objA->description);
            std::cout << desc << " can't be traveled." << std::endl;
        }
    }
    return 1;
}

int executeLookAround(Object *args[])
{   
    std::cout << "Looking around." << std::endl;
    std::cout << player.parent->details << std::endl;
    std::cout << "You can see:" << std::endl;
    for(Object *o=player.parent->inventoryHead; o!=NULL; o=o->next)
    {
        if(o == &player) continue;
        std::cout << o->description << std::endl;
    }   
    return 1;
}

int executeLookAt(Object *args[])
{
    Object* objA = args[0];
    if(objA != nullptr)
    {
        std::cout << objA->description << std::endl;
    }
    return 1;
}

int executePickUp(Object *args[])
{
    std::cout << "Picked up ..." << std::endl;
    /*
    Object *objA = args[0];
    if(objA != NULL)
    {
        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(player.capacity >= objA->weight)
        {
            doPickup(&player, objA);
        }
        else
        {
            char *temporaryDescription = copy(objA->description);
            capitalise(temporaryDescription);
            printf("%s can't be picked up.\n", temporaryDescription);
            free(temporaryDescription);
        }
    }
    */
    return 1;
}

int executeDrop(Object *args[])
{
    std::cout << "Dropped ..." << std::endl;
    /*
    Object *objA = args[0];
    if(objA != NULL)
    {
        Object *parent = player.parent;

        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(parent != NULL && parent->capacity >= objA->weight)
        {
            doDrop(&player, objA);
        }
        else
        {
            char *temporaryDescription = copy(objA->description);
            capitalise(temporaryDescription);
            printf("%s can't be dropped.\n", temporaryDescription);
            free(temporaryDescription);
        }
    }
    */
    return 1;
}

int executeHelp(Object *args[])
{
    std::cout << "Executing help." << std::endl;
    // TODO: Move commands array from "prasexec.c" somewhere visible
    // NOTE: This is temporary.
    /*
    Command commands[] = 
    {
        { "quit"        , 0, 0, executeQuit       },
        { "go A"        , 0, 0, executeTravel     },
        { "go to A"     , 0, 0, executeTravel     },
        { "enter A"     , 0, 0, executeTravel     },
        { "look around" , 0, 0, executeLookAround },
        { "look at A"   , 0, 0, executeLookAt     },
        { "examine A"   , 0, 0, executeLookAt     },
        { "pick up A"   , 0, 0, executePickUp     },
        { "get A"       , 0, 0, executePickUp     },
        { "drop A"      , 0, 0, executeDrop       },
        { "help"        , 0, 0, executeHelp       }
    };
    size_t length = sizeof(commands)/sizeof(Command);
    for(size_t i=0; i<length; i++)
    {
        printf("%s\n", commands[i].pattern);
    }
    */
   return 1;
}

