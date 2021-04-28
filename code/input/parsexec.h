#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <stdio.h>

#include "../logic/commands.h"
#include "../objects/object.h"
#include "../utility/strops.h"

extern Object objectHead;
extern Object player; 

static Object *args[26];

static void clearArgs();
static int compareWithTag(char *src, char *tag);
static Object *findByTag(Object *root, char *input, int *minTagLength);
static int matchCommand(char *input, char *pattern);

extern int parseInput(char *input);

#endif