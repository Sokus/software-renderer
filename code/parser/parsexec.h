#ifndef PARSEXEC_H
#define PRASEXEC_H

#include "utility/array.h"
#include "utility/linked_list.h"
#include "stdbool.h"

void ProcessInput();
static void PushCharacter(char c);
static void PopCharacter();

static void Reset();

static void ResetCommands(DynamicArray* pCommands);
static void ResetPatterns(LinkedListNode* pNode);
static void FreeMatches(DynamicArray* pMatches);

bool CompareMatches(Match* pA, Match* pB);
int MergeMatches(Match* pA, Match* pB);
int AddMatchToArray(DynamicArray* pArr, Match* pM);

#endif