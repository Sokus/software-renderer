#ifdef _WIN32

#include "console/output.h"

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

void Console_SetDefaultColor(Color color)
{
    defaultColor = color;
}

void Console_SetActiveColor(Color color)
{
    activeColor = color;
    int attribute;
    if(color == COLOR_DEFAULT) color = defaultColor;
    
    bool bright = color >= COLOR_BRIGHT_BLACK;
    color = (color-1) % 8 + 1;
    attribute = color == COLOR_BLACK   ? 0 :
                color == COLOR_BLUE    ? 1 :
                color == COLOR_GREEN   ? 2 :
                color == COLOR_CYAN    ? 3 :
                color == COLOR_RED     ? 4 :
                color == COLOR_MAGENTA ? 5 :
                color == COLOR_YELLOW  ? 6 :
                color == COLOR_WHITE   ? 7 : -1;
    if(bright) attribute += 8;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, attribute);
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
    // From: "https://docs.microsoft.com/en-us/windows/console/clearing-the-screen"

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

#endif // WIN32