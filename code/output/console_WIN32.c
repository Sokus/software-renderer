#include "console.h"
#include <windows.h>

void Console_Print(const char* message, Console_Color color)
{
    int attribute;
    if(color == COLOR_DEFAULT)
    {
        attribute = 15;
    }
    else
    {
        bool bright = color >= COLOR_BRIGHT_BLACK;
        color = (Console_Color)((int)color % 8);
        attribute = color == COLOR_BLACK   ? 0 :
                    color == COLOR_BLUE    ? 1 :
                    color == COLOR_GREEN   ? 2 :
                    color == COLOR_CYAN    ? 3 :
                    color == COLOR_RED     ? 4 :
                    color == COLOR_MAGENTA ? 5 :
                    color == COLOR_YELLOW  ? 6 :
                    color == COLOR_WHITE   ? 7 : -1;
        if(bright) attribute += 8;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, attribute);
    printf(message);
    SetConsoleTextAttribute(hConsole, 15);
}