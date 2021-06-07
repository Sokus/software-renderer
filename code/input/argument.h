#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "objects/object.h"

typedef enum
{
    ARG_TYPE_INVALID,
    ARG_TYPE_INT,
    ARG_TYPE_ORDINAL,
    ARG_TYPE_TAG,
    ARG_TYPE_PROPERTY
} ArgType;

typedef struct Argument Argument;
struct Argument
{
    ArgType type;
    union
    {
        Object* p;
        int value;
    };
    
};

#endif