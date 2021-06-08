#ifndef PARSEXEC_H
#define PARSEXEC_H

#include <stdlib.h>
#include <stdbool.h>

#include "argument.h"
#include "command.h"
#include "commands.h"
#include "objects/object.h"
#include "output/console.h"
#include "properties.h"

#define ARGS_MAX_COUNT 10

extern Object* gpObjectRoot;
extern Object* gpPlayer;

extern Command gCommands[];

extern PropertyField gContext;

typedef struct
{
    int minTagLength;
    int matchesToSkip;
    Distance minDistance;
    Distance maxDistance;
} SearchParameters;

static Argument gArgs[ARGS_MAX_COUNT];

Argument GetArgumentOfType(ArgType argType, int skip);
void AppendArgument(Argument arg);
static void ClearArgs();

void ParseInput(char* input);
static bool MatchCommand(char* src, Command cmd[]);
static bool MatchPattern(char* src, char* pattern, SearchParameters params);

static ArgType EvaluateType(char type);
static int ReadArgumentInt(char* src, int* value);
static int ReadArgumentOrdinal(char* src, int* value);
static int ReadArgumentTag(char* src, void** value, SearchParameters* params);
static int ReadArgumentProperty(char* src, int* value);
static int ReadArgumentBool(char* src, int* value);


static Object* FindByTag(char* src, SearchParameters* params);
static Object* FindByTagRecursive(char* src, Object* head, SearchParameters* params, bool deepSearch);
static bool MatchObjectTag(char* src, Object* obj, SearchParameters* params);

void GetInput(char** buffer);

#endif