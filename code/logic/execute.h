#ifndef EXECUTE_H
#define EXECUTE_H

#include "../input/argument.h"
#include "../input/parsexec.h"
#include "../objects/object.h"
#include "../output/console.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

bool ExecuteQuit(Argument args[]);
bool ExecuteLookAround(Argument args[]);
bool ExecuteLookAt(Argument args[]);
bool ExecuteExamineInventory(Argument args[]);
bool ExecutePickUp(Argument args[]);
bool ExecuteDrop(Argument args[]);
bool ExecutePut(Argument args[]);
bool ExecuteClear(Argument args[]);
bool ExecuteList(Argument args[]);
bool ExecuteOpen(Argument args[]);
bool ExecuteClose(Argument args[]);

#endif