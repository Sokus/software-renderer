#include "properties.h"

bool HasProperty(const Object* obj, ObjectProperty property)
{
    if(obj == NULL) return false;
    return obj->properties & (1ULL << property);
}

void SetProperty(Object* obj, ObjectProperty property)
{
    if(obj) obj->properties |= (1ULL << property);
}

void RemoveProperty(Object* obj, ObjectProperty property)
{
    if(obj) obj->properties &= ~(1ULL << property);
}
