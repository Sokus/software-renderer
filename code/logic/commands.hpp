#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>

#include "../temporary.hpp"
#include "distance.hpp"
#include "../objects/object.hpp"
#include "../utility/strops.hpp"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

class Command;
static std::vector<Command> commands;

class Command
{
public:
    std::string pattern;
    Distance minDistance;
    Distance maxDistance;
    int (*function)(Object *args[]);

    Command();
    Command(std::string pattern,
            Distance minDistance, Distance maxDistance,
            int (*function)(Object **));
};

std::vector<Command> GetCommands();
static void CreateCommands();

int ExecuteQuit(Object *args[]);
int ExecuteTravel(Object *args[]);
int ExecuteLookAround(Object *args[]);
int ExecuteLookAt(Object *args[]);
int ExecutePickUp(Object *args[]);
int ExecuteDrop(Object *args[]);
int ExecuteHelp(Object *args[]);
#endif