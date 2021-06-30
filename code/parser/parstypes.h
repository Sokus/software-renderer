#ifndef PARSTYPES_H
#define PARSTYPES_H

#include "utility/array.h"
#include "properties.h"
#include "logic/distance.h"
#include "utility/linked_list.h"

#include <stdbool.h>

typedef enum PatternType
{
    PT_STRING,
    PT_TAG,
    PT_INTEGER,
    PT_PROPERTY,
} PatternType;

typedef struct Match
{
    PatternType type;
    bool active;
    char* text;
    int value;
    DynamicArray patterns;
    DynamicArray objects;
} Match;

typedef struct Pattern
{
    PatternType type;

    char* text;
    PropertyBitField distanceFlags;

    DynamicArray possibleMatches;
    Match* match;

    char* readFrom;

    LinkedListNode list;
} Pattern;


typedef struct Command
{
    LinkedListNode* patterns;
    PropertyBitField contextConditions;
    bool active;
    void (*function)();
} Command;

#endif