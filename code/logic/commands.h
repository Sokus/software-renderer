#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>

#include "distance.h"
#include "interact.h"
#include "../objects/object.h"
#include "../utility/strops.h"

extern Object objectHead;
extern Object player;

typedef struct
{
    char *pattern;
    Distance minDistance;
    Distance maxDistance;
    int (*function)(Object *args[]);
} Command;

int executeQuit(Object *args[]);
int executeTravel(Object *args[]);
int executeLookAround(Object *args[]);
int executeLookAt(Object *args[]);
int executePickUp(Object *args[]);
int executeDrop(Object *args[]);
int executeHelp(Object *args[]);
#endif