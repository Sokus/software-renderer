#include "properties.h"

bool HasProperty(PropertyField properties, ObjectProperty property)
{
    return properties & (1ULL << property);
}

void SetProperty(PropertyField* properties, ObjectProperty property)
{
    *properties |= (1ULL << property);
}

void RemoveProperty(PropertyField* properties, ObjectProperty property)
{
    *properties &= ~(1ULL << property);
}
