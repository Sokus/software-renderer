#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

#include "command.h"
#include "logic/execute.h"

Command gCommands[COMMANDS_MAX_COUNT];

Command* GetCommands();
void CreateCommands();
void DeleteCommands();

#endif