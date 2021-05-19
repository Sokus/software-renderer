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
    bool (*function)(Object *args[]);

    Command();
    Command(std::string pattern,
            Distance minDistance, Distance maxDistance,
            bool (*function)(Object **));
};

std::vector<Command> GetCommands();
static void CreateCommands();

bool ExecuteQuit(Object *args[]);
bool ExecuteTravel(Object *args[]);
bool ExecuteLookAround(Object *args[]);
bool ExecuteLookAt(Object *args[]);
bool ExecutePickUp(Object *args[]);
bool ExecuteDrop(Object *args[]);
bool ExecuteHelp(Object *args[]);
#endif