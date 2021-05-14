#include <iostream>
#include "limits.h"


#include "input/parsexec.hpp"
#include "objects/object.hpp"

Object objectHead = Object({}, std::string("root"), std::string(), 0, 0, 0);
Object player = Object({"player"},
                        "a player",
                        "This is you.",
                        0, 0, 0);

std::string getInput()
{
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int main()
{
    Object field = Object({"field", "grassy field"},
                        "a grassy field",
                        "Huge field covered in grass.",
                        INT_MAX, INT_MAX, 1);
    Object guard = Object({"guard", "city guard"},
                        "a city guard",
                        "Man who guards the city.",
                        120, 40, 1);

    
    objectHead.appendInv(field);
    field.appendInv(player);
    field.appendInv(guard);

    std::string input = getInput();
    while(parseInput(input)) input = getInput();

    std::cout << "Press ENTER to continue...";
    std::cin.get();
    return 0;
}