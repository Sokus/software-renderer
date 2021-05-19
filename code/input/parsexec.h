#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../logic/commands.h"
#include "../objects/object.h"
#include "../utility/strops.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer; 

static Object* gArgs[26];

static void ClearArgs();
static bool CompareWithTag(const char* src, const char* tag);
static bool MatchObjectTag(const char* src, const Object* obj, int* minTagLength);
static Object* FindByTagRecursive(const char* src, Object* head,
                                  int* minTagLength, bool deepSearch);
static Object* FindByTag(const char* src, int* minTagLength,
                         Distance minDistance, Distance maxDistance);
static bool MatchCommand(const char* src, const Command* cmd);

void GetInput(char** buffer);
bool ParseInput(const char *input);

#endif