#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdbool.h>

#include "distance.h"
#include "../objects/object.h"
#include "../output/console.h"
#include "../utility/strops.h"

#define COMMANDS_MAX_COUNT 32
#define COMMANDS_MAX_PATTERNS 8

extern Object* gpObjectRoot;
extern Object* gpPlayer;

typedef struct
{
    char* patterns[COMMANDS_MAX_PATTERNS];
    Distance minDistance;
    Distance maxDistance;
    bool (*function)(Object *args[]);
} Command;
static Command gCommands[COMMANDS_MAX_COUNT];

Command* GetCommands();
static void CreateCommands();

bool ExecuteQuit(Object* args[]);
bool ExecuteTravel(Object* args[]);
bool ExecuteLookAround(Object* args[]);
bool ExecuteLookAt(Object* args[]);
bool ExecuteLookInside(Object* args[]);
bool ExecuteExamineInventory(Object* args[]);
bool ExecutePickUp(Object* args[]);
bool ExecuteDrop(Object* args[]);
bool ExecutePut(Object* args[]);
bool ExecuteClear(Object* args[]);
bool ExecuteHelp(Object* args[]);
#endif