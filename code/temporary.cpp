#include "temporary.hpp"

Object* gpObjectRoot = nullptr;
Object* gpPlayer = nullptr;

void CreateObjects()
{
    gpObjectRoot = new Object({}, std::string("root"), std::string(), 0, 0, 0);
    gpPlayer = new Object({"player"},
                        "a player",
                        "This is you.",
                        0, 0, 0);

    Object* field = new Object({"field", "grassy field"},
                        "a grassy field",
                        "Huge field covered in grass.",
                        INT_MAX, INT_MAX, 1);
    Object* guard = new Object({"guard", "city guard"},
                        "a city guard",
                        "Man who guards the city.",
                        120, 40, 1);

    gpObjectRoot->AppendInventory(*field);
    field->AppendInventory(*gpPlayer);
    field->AppendInventory(*guard);
}

void DeleteObjects()
{
    
    delete gpObjectRoot;
}