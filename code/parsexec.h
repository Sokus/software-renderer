#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <stdio.h>
#include "defs.h"
#include "commands.h"

static Object *args[26];

static void clearArgs();
static int getLength(char *src);
static char *skipSpaces(char *src);
static int isCapital(char letter);
static int compareInsensitive(char a, char b);
static int compareWithTag(char *src, char *tag);
static int matchCommand(char *input, char *pattern, Object objectHead);

extern int parseInput(char *input, Object objectHead);

#endif