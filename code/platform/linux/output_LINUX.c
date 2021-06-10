#include "console/output.h"

void Console_SetDefaultColor(Color color)
{
    defaultColor = color;
}

void Console_SetActiveColor(Color color)
{
    activeColor = color;
    if(color == COLOR_DEFAULT) color = defaultColor;

    bool bright = color >= COLOR_BRIGHT_BLACK;
    int attribute = (color-1 % 8) + 30;
    char * format = bright ? "\e[%d;1m" : "\e[%dm";
    printf("\e[0m");
    printf(format, attribute);
}

void Console_ResetColor()
{
    Console_SetActiveColor(defaultColor);
}

void Console_PrintColored(char* message, Color color, ...)
{
    Color oldColor = activeColor;
    Console_SetActiveColor(color);
    
    va_list argList;
    va_start(argList, color);
    vprintf(message, argList);
    va_end(argList);

    Console_SetActiveColor(oldColor);
}

void Console_Print(char* message, ...)
{
    va_list argList;
    va_start(argList, message);
    vprintf(message, argList);
    va_end(argList);
}

void Console_Clear()
{
    printf("\e[H\e[J");
}