#include "temporary.h"

Object* gpObjectRoot;
Object* gpPlayer;

void CreateObjects()
{
    if(gpObjectRoot) return;
    
    gpObjectRoot = (Object*)malloc(sizeof(Object));
    *gpObjectRoot = (Object){0};    

    gpPlayer = (Object*)malloc(sizeof(Object));
    *gpPlayer = (Object) {{ Copy("player")}, Copy("a player"), Copy("This is you.")}; 

    Object* field = (Object*)malloc(sizeof(Object));
    *field = (Object) {{Copy("field"), Copy("grassy field")}, Copy("a grassy field"), Copy("Huge field covered in grass.")}; 

    Object* guard = (Object*)malloc(sizeof(Object));
    *guard = (Object) {{Copy("guard"), Copy("city guard")}, Copy("a city guard"), Copy("Man who guards the city.")};

    AppendInventory(gpObjectRoot, field);
    AppendInventory(field, gpPlayer);
    AppendInventory(field, guard);
}

void DeleteObjects()
{
    FreeMemory(&gpObjectRoot);
}