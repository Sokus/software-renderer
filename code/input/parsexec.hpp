#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <iostream>
#include <string>

#include "../logic/commands.hpp"
#include "../objects/object.hpp"
#include "../logic/distance.hpp"
#include "../utility/strops.hpp"

extern Object objectRoot;
extern Object player;

static Object *args[26];

static void clearArgs();
static bool compareWithTag(const std::string& src, const std:: string& tag);
static bool matchObjectTag(const std::string& src, const Object& obj, int& minTagLength);
static Object *findByTagRecursive(Object* head, const std::string& input,
                        int &minTagLength, bool deepSearch);
static Object *findByTag(const std::string& input, int& minTagLength,
                        Distance minDistance, Distance maxDistance);
static bool matchCommand(const std::string& input, const Command& cmd);

extern bool parseInput(const std::string& input);

#endif