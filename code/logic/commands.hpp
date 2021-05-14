#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>

#include "distance.hpp"
//#include "interact.h"
#include "../objects/object.hpp"
#include "../utility/strops.hpp"

extern Object objectRoot;
extern Object player;

class Command
{
public:
    std::string pattern;
    Distance minDistance;
    Distance maxDistance;
    int (*function)(Object *args[]);

    Command(std::string pattern,
            Distance minDistance, Distance maxDistance,
            int (*function)(Object **));
};

int executeQuit(Object *args[]);
int executeTravel(Object *args[]);
int executeLookAround(Object *args[]);
int executeLookAt(Object *args[]);
int executePickUp(Object *args[]);
int executeDrop(Object *args[]);
int executeHelp(Object *args[]);
#endif