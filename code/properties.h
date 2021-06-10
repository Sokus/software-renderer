#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <stdbool.h>

#include "console/output.h"

typedef enum Property
{
    // GAME STATE
    CONTEXT_DEBUG_MODE,
    CONTEXT_INVENTORY_OPEN,
    CONTEXT_CONTAINER_OPEN,
    CONTEXT_SHUTDOWN,

    // OBJECTS
    OBJECT_PROPERTY_COLLECTABLE,
    OBJECT_PROPERTY_VISIBLE_INVENTORY,

    OBJECT_PROPERTY_CONTAINER,
    OBJECT_PROPERTY_PASSAGE,
    OBJECT_PROPERTY_NPC,

    OBJECT_PROPERTY_OPEN,
    OBJECT_PROPERTY_NEW,

    PROPERTY_MAX
} Property;

static char* gPropertyNames[] = 
{
    "CONTEXT_DEBUG_MODE",
    "CONTEXT_INVENTORY_OPEN",
    "CONTEXT_CONTAINER_OPEN",
    "CONTEXT_SHUTDOWN",
    "OBJECT_PROPERTY_COLLECTABLE",
    "OBJECT_PROPERTY_VISIBLE_INVENTORY",
    "OBJECT_PROPERTY_CONTAINER",
    "OBJECT_PROPERTY_PASSAGE",
    "OBJECT_PROPERTY_NPC",
    "OBJECT_PROPERTY_OPEN",
    "OBJECT_PROPERTY_NEW",
    "PROPERTY_MAX"
};

typedef unsigned long long PropertyField;

bool HasProperty(PropertyField properties, Property property);
void SetProperty(PropertyField* properties, Property property, bool value);

bool HasProperties(PropertyField field, PropertyField properties);

void PrintProperties(PropertyField properties);
void PrintPropertyName(Property property);

#endif