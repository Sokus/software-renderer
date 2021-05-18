#include "console.hpp"
#include "windows.h"

std::ostream& operator<<(std::ostream& os, Console::Color color)
{
    using namespace Console;

    int attribute;
    if(color == COLOR_DEFAULT || color == COLOR_RESET)
    {
        attribute = 15;
    }
    else
    {
        bool bright = color >= COLOR_BRIGHT_BLACK;
        color = (Console::Color)((int)color % 8);
        attribute = color == COLOR_BLACK   ? 0 :
                    color == COLOR_BLUE    ? 1 :
                    color == COLOR_GREEN   ? 2 :
                    color == COLOR_CYAN    ? 3 :
                    color == COLOR_RED     ? 4 :
                    color == COLOR_MAGENTA ? 5 :
                    color == COLOR_YELLOW  ? 6 :
                    color == COLOR_WHITE   ? 7 : -1;
        if(attribute < 0) { /* something went wrong */ }
        if(bright) attribute += 8;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, attribute);
    return os;
}