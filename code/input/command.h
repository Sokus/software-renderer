#ifndef COMMAND_H
#define COMMAND_H

#define COMMANDS_MAX_COUNT 32
#define COMMANDS_MAX_PATTERNS 8

#include "argument.h"
#include "../logic/distance.h"

typedef struct Command Command;
struct Command
{
    char* patterns[COMMANDS_MAX_PATTERNS];
    Distance minDistance;
    Distance maxDistance;
    bool (*function)(Argument args[]);
};

#endif