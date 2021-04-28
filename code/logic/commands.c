#include "commands.h"

int executeQuit(Object *args[])
{
    printf("Goodbye.\n");
    return 0;
}

int executeTravel(Object *args[])
{
    Object *objA = args[0];
    if(objA != NULL)
    {
        if(objA->destination != NULL)
        {
            doTravel(&player, objA);
        }
        else
        {
            char *temporaryDescription = copy(objA->description);
            capitalise(temporaryDescription);
            printf("%s can't be traveled.\n", temporaryDescription);
            free(temporaryDescription);
        }
    }
    return 1;
}

int executeLookAround(Object *args[])
{
    Object *parent = findParent(&objectHead, &player);
    if(parent != NULL)
    {
        printf("%s\nYou can see:\n", parent->details);
        for(Object *o=parent->inventoryHead; o!=NULL; o=o->next)
        {
            if(o == &player) continue;
            printf("%s,\n", o->description);
        }
    }
    return 1;
}

int executeLookAt(Object *args[])
{
    printf("%s\n", args[0]->details);
    return 1;
}

int executePickUp(Object *args[])
{
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
    return 1;
}

int executeDrop(Object *args[])
{
    Object *objA = args[0];
    if(objA != NULL)
    {
        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(findParent(&objectHead, &player)->capacity >= objA->weight)
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
    return 1;
}

int executeHelp(Object *args[])
{
    // TODO: Move commands array from "prasexec.c" somewhere visible
    // NOTE: This is temporary.
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
}

