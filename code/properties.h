#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <stdbool.h>


typedef enum Property
{
    // GAME STATE
    GS_DEBUG,
    GS_INVENTORY_OPEN,
    GS_CONTAINER_OPEN,
    GS_SHUTDOWN,

    // OBJECTS
    OP_COLLECTABLE,
    OP_VISIBLE_INVENTORY,
    OP_CONTAINER,
    OP_PASSAGE,
    OP_NPC,
    OP_OPEN,
    OP_NEW,

    PROPERTY_MAX
} Property;

static char* gPropertyNames[] = 
{
    "GS_DEBUG",
    "GS_INVENTORY_OPEN",
    "GS_CONTAINER_OPEN",
    "GS_SHUTDOWN",
    "OP_COLLECTABLE",
    "OP_VISIBLE_INVENTORY",
    "OP_CONTAINER",
    "OP_PASSAGE",
    "OP_NPC",
    "OP_OPEN",
    "OP_NEW",
    "PROPERTY_MAX"
};

typedef unsigned long long PropertyBitField;

bool HasProperty(PropertyBitField properties, Property property);
void SetProperty(PropertyBitField* properties, Property property, bool value);

bool HasProperties(PropertyBitField field, PropertyBitField properties);

void PrintProperties(PropertyBitField properties);
void PrintPropertyName(Property property);

#endif