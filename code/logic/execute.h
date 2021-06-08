#ifndef EXECUTE_H
#define EXECUTE_H

#include "properties.h"
#include "input/argument.h"
#include "input/parsexec.h"
#include "objects/object.h"
#include "output/console.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

extern PropertyField gContext;

bool ExecuteClear();
bool ExecuteQuit();
bool ExecuteHelp();
bool ExecuteList();

bool ExecuteLookAt();

bool ExecuteInventory();

static bool ExecuteNext();
static bool ExecutePrev();
static bool ExecutePage();
bool ExecuteInventoryNext();
bool ExecuteInventoryPrev();
bool ExecuteInventoryPage();
bool ExecuteContainerNext();
bool ExecuteContainerPrev();
bool ExecuteContainerPage();
bool ExecuteLocationNext();
bool ExecuteLocationPrev();
bool ExecuteLocationPage();

bool ExecutePickUp();
bool ExecuteDrop();
bool ExecuteOpen();
bool ExecuteMoveToInventory();
bool ExecuteMoveToContainer();
bool ExecuteClose();
bool ExecuteCloseInventory();
bool ExecuteCloseContainer();

bool ExecuteDebug();
bool ExecuteListProperties();
bool ExecuteSetProperty();

#endif