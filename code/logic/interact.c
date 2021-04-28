#include "interact.h"

void doPickup(Object *caller, Object *target)
{
    char *temp = copy(target->description);
    capitalise(temp);
    printf("%s was picked up by %s\n.", temp, caller->description);
    free(temp);
}

void doDrop(Object *caller, Object *target)
{
    char *temp = copy(target->description);
    capitalise(temp);
    printf("%s was dropped by %s\n.", temp, caller->description);
    free(temp);
}

void doTravel(Object *caller, Object *target)
{
    char *temp = copy(target->description);
    capitalise(temp);
    printf("%s was traveled by %s\n.", temp, caller->description);
    free(temp);
}
