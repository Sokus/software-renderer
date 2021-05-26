#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <stdbool.h>

typedef enum
{
    OBJECT_PROPERTY_COLLECTABLE,
    OBJECT_PROPERTY_CONTAINER,
    OBJECT_PROPERTY_VISIBLE_INVENTORY
} ObjectProperty;

typedef unsigned long long PropertyField;

bool HasProperty(PropertyField properties, ObjectProperty property);
void SetProperty(PropertyField* properties, ObjectProperty property);
void RemoveProperty(PropertyField* properties, ObjectProperty property);

#endif