#ifndef DEFS_H
#define DEFS_H

#define INPUT_SIZE 100

typedef struct Object Object;

typedef struct
{
    char *pattern;
    int (*function)(Object *args[]);
} Command;

struct Object
{
    char *tags[8]; // tags you refer to the object by
    char *description;
    Object *next;
};

#endif