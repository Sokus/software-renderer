#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdbool.h>

#include "command.h"
#include "../logic/execute.h"

static Command gCommands[COMMANDS_MAX_COUNT];

Command* GetCommands();
static void CreateCommands();

#endif