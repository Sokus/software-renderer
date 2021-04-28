#ifndef INTERACT_H
#define INTERACT_H

#include <stdio.h>

#include "../objects/object.h"
#include "../utility/strops.h"

void doPickup(Object *caller, Object *target);
void doDrop(Object *caller, Object *target);
void doTravel(Object *caller, Object *target);

#endif