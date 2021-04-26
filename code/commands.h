#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#include "defs.h"

int executeQuit(Object *args[]);
int executeTravel(Object *args[]);
int executeLookAround(Object *args[]);
int executeLookAt(Object *args[]);

#endif