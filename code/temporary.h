#ifndef TEMPORARY_H
#define TEMPORARY_H

#include "objects/object.h"
#include "utility/strops.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

void CreateObjects();
void DeleteObjects();

#endif