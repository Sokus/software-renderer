#include "temporary.h"

Object* gpObjectRoot;
Object* gpPlayer;

void CreateObjects()
{
    if(gpObjectRoot) return;
    
    gpObjectRoot = (Object*)malloc(sizeof(Object));
    *gpObjectRoot = (Object){0};    


    Object* field = (Object*)malloc(sizeof(Object));
    *field = (Object) {0, {Copy("field"), Copy("grassy field")}, Copy("a grassy field"), Copy("Huge field covered in grass.")}; 
    AppendInventory(gpObjectRoot, field);

    gpPlayer = (Object*)malloc(sizeof(Object));
    *gpPlayer = (Object) {0, { Copy("you"), Copy("self"), Copy("player")}, Copy("a player"), Copy("This is you.")}; 
    AppendInventory(field, gpPlayer);

    Object* guard = (Object*)malloc(sizeof(Object));
    *guard = (Object) {0, {Copy("guard"), Copy("city guard")}, Copy("a city guard"), Copy("Man who guards the city.")};
    AppendInventory(field, guard);

    Object* pouch = (Object*)malloc(sizeof(Object));
    *pouch = (Object) {0, {Copy("pouch"), Copy("leather pouch")}, Copy("a leather pouch"), Copy("It's a leather pouch.")};
    SetProperty(pouch, OBJECT_PROPERTY_VISIBLE_INVENTORY);
    SetProperty(pouch, OBJECT_PROPERTY_COLLECTABLE);
    AppendInventory(field, pouch);

    Object* coin = (Object*)malloc(sizeof(Object));
    *coin = (Object) {0, {Copy("coin"), Copy("silver coin")}, Copy("a silver coin"), Copy("It's a coin made of silver.")};
    SetProperty(coin, OBJECT_PROPERTY_COLLECTABLE);
    AppendInventory(pouch, coin);
    
    Object* chest = (Object*)malloc(sizeof(Object));
    *chest = (Object) {0, {Copy("chest"), Copy("wooden chest")}, Copy("a wooden chest"), Copy("It's a chest made of wood.")};
    SetProperty(chest, OBJECT_PROPERTY_VISIBLE_INVENTORY);
    AppendInventory(field, chest);
    
    Object* sword = (Object*)malloc(sizeof(Object));
    *sword = (Object) {0, {Copy("sword"), Copy("iron sword")}, Copy("an iron sword"), Copy("It's an iron sword, its blade is dull.")};
    SetProperty(sword, OBJECT_PROPERTY_COLLECTABLE);
    AppendInventory(chest, sword);
}

void DeleteObjects()
{
    FreeMemory(&gpObjectRoot);
}