#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdbool.h>

#include "../objects/object.h"
#include "distance.h"
#include "../utility/strops.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

typedef struct
{
    char *pattern;
    Distance minDistance;
    Distance maxDistance;
    bool (*function)(Object *args[]);
} Command;
static Command commands[32];

Command* GetCommands();
static void CreateCommands();

bool ExecuteQuit(Object *args[]);
bool ExecuteTravel(Object *args[]);
bool ExecuteLookAround(Object *args[]);
bool ExecuteLookAt(Object *args[]);
bool ExecutePickUp(Object *args[]);
bool ExecuteDrop(Object *args[]);
bool ExecuteHelp(Object *args[]);
#endif