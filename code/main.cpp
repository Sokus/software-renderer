#include <iostream>

#include "input/parsexec.hpp"
#include "temporary.hpp"
#include "output/console.hpp"

int main()
{
    CreateObjects();
    while(ParseInput(GetInput()))
    { 
        // game loop
    }
    DeleteObjects();

    std::cout << "Press ENTER to continue...";
    std::cin.get();
    return 0;
}