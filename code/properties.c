#include "properties.h"

#include "console/output.h"

bool HasProperty(PropertyBitField properties, Property property)
{
    return properties & (1ULL << property);
}

void SetProperty(PropertyBitField* properties, Property property, bool value)
{
    if(value) *properties |= (1ULL << property);
    else *properties &= ~(1ULL << property);
}

bool HasProperties(PropertyBitField field, PropertyBitField properties)
{
    return (field & properties) == properties;
}

void PrintPropertyName(Property property)
{
    Console_Print(gPropertyNames[property]);
}

void PrintProperties(PropertyBitField properties)
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