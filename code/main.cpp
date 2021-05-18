
#include <iostream>
#include "input/parsexec.hpp"
#include "temporary.hpp"
#include "output/console.hpp"

int main()
{
    CreateObjects();
    for(int i=1; i<16; i++)
    {
        std::cout << (Console::Color)i << "Am I in the matrix yet?\n" << Console::COLOR_RESET;
    }
    //while(ParseInput(GetInput()))
    { /* game loop */ }

    DeleteObjects();

    std::cout << "Press ENTER to continue...";
    std::cin.get();
    return 0;
}