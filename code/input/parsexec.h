#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../logic/commands.h"
#include "../objects/object.h"
#include "../objects/properties.h"
#include "../utility/strops.h"

extern Object* gpObjectRoot;
extern Object* gpPlayer; 

static Object* gArgs[26];

bool ParseInput(const char *input);
static bool MatchCommand(const char* src, const Command* cmd);
static bool MatchPattern(const char* src, const char* pattern,
                        Distance minDistance, Distance maxDistance);
static Object* FindByTag(const char* src, int* minTagLength,
                         Distance minDistance, Distance maxDistance);
static Object* FindByTagRecursive(const char* src, Object* head,
                                  int* minTagLength, bool deepSearch);
static bool MatchObjectTag(const char* src, const Object* obj, int* minTagLength);
static bool CompareWithTag(const char* src, const char* tag);

void GetInput(char** buffer);
static void ClearArgs();

#endif