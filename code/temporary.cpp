#include "temporary.hpp"

Object* gpObjectRoot = nullptr;
Object* gpPlayer = nullptr;

void CreateObjects()
{
    gpObjectRoot = new Object({}, std::string("root"), std::string());
    gpPlayer = new Object({"player"},
                        "a player",
                        "This is you.");

    Object* field = new Object({"field", "grassy field"},
                        "a grassy field",
                        "Huge field covered in grass.");
    Object* guard = new Object({"guard", "city guard"},
                        "a city guard",
                        "Man who guards the city.");

    gpObjectRoot->AppendInventory(*field);
    field->AppendInventory(*gpPlayer);
    field->AppendInventory(*guard);
}

void DeleteObjects()
{
    delete gpObjectRoot;
}