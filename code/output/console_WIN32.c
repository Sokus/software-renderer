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