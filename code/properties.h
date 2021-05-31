#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <stdbool.h>

typedef enum
{
    OBJECT_PROPERTY_COLLECTABLE,
    OBJECT_PROPERTY_CONTAINER,
    OBJECT_PROPERTY_OPEN,
    OBJECT_PROPERTY_VISIBLE_INVENTORY
} ObjectProperty;

typedef unsigned long long PropertyField;
typedef unsigned int Property;

bool HasProperty(PropertyField properties, Property property);
void SetProperty(PropertyField* properties, Property property, bool value);

#endif