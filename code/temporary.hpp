#ifndef TEMPORARY_H
#define TEMPORARY_H

#include "objects/object.hpp"
#include "limits.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

void CreateObjects();
void DeleteObjects();

#endif