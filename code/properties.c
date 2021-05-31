#include "properties.h"

bool HasProperty(PropertyField properties, Property property)
{
    return properties & (1ULL << property);
}

void SetProperty(PropertyField* properties, Property property, bool value)
{
    if(value) *properties |= (1ULL << property);
    else *properties &= ~(1ULL << property);
}