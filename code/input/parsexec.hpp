#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <iostream>
#include <string>
#include <vector>

#include "../logic/commands.hpp"
#include "../objects/object.hpp"
#include "../logic/distance.hpp"
#include "../utility/strops.hpp"
#include "../temporary.hpp"

extern Object* gpObjectRoot;
extern Object* gpPlayer;

static Object *gArgs[26];

static void ClearArgs();
static bool CompareWithTag(const std::string& src, const std:: string& tag);
static bool MatchObjectTag(const std::string& src, const Object& obj, int& minTagLength);
static Object *FindByTagRecursion(Object* head, const std::string& input,
                        int &minTagLength, bool deepSearch);
static Object *FindByTag(const std::string& input, int& minTagLength,
                        Distance minDistance, Distance maxDistance);
static bool MatchCommand(const std::string& input, const Command& cmd);

extern std::string GetInput();
extern bool ParseInput(const std::string& input);

#endif