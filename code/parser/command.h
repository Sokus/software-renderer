#ifndef COMMAND_H
#define COMMAND_H

#define COMMANDS_MAX_COUNT 32
#define COMMANDS_MAX_PATTERNS 8

#include "argument.h"
#include "logic/distance.h"
#include "properties.h"

typedef struct Command Command;
struct Command
{
    PropertyField contextConditions;
    char* patterns[COMMANDS_MAX_PATTERNS];
    char* pattern;
    char* details;
    bool (*function)();
};

#endif