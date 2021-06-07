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

bool HasProperties(PropertyField field, PropertyField properties)
{
    return (field & properties) == properties;
}

void PrintPropertyName(Property property)
{
    Console_Print(gPropertyNames[property]);
}

void PrintProperties(PropertyField properties)
{
    for(int i=0; i<=PROPERTY_MAX; i++)
    {
        if(HasProperty(properties, i))
        {
            PrintPropertyName(i);
            Console_Print("\n");
        }
    }
}