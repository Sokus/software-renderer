#include "commands.h"

int executeQuit(Object *args[])
{
    printf("Goodbye.\n");
    return 0;
}

int executeTravel(Object *args[])
{
    printf("Traveling to...\n");
    return 1;
}

int executeLookAround(Object *args[])
{
    printf("You look around...\n");
    return 1;
}

int executeLookAt(Object *args[])
{
    printf("%s\n", args[0]->description);
    return 1;
}
