#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

#include "command.h"
#include "logic/execute.h"

extern Command gCommands[];

Command* GetCommands();
void CreateCommands();
void DeleteCommands();

#endif