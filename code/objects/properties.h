#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <stdbool.h>

#include "../types.h"
#include "object.h"

typedef enum
{
    OBJECT_PROPERTY_COLLECTABLE,
    OBJECT_PROPERTY_VISIBLE_INVENTORY
} ObjectProperty;

bool HasProperty(const Object* obj, ObjectProperty property);
void SetProperty(Object* obj, ObjectProperty property);
void RemoveProperty(Object* obj, ObjectProperty property);

#endif