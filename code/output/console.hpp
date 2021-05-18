#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>

namespace Console
{
    enum Color
    {
        COLOR_BLACK,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_WHITE,
        COLOR_BRIGHT_BLACK,
        COLOR_BRIGHT_RED,
        COLOR_BRIGHT_GREEN,
        COLOR_BRIGHT_YELLOW,
        COLOR_BRIGHT_BLUE,
        COLOR_BRIGHT_MAGENTA,
        COLOR_BRIGHT_CYAN,
        COLOR_BRIGHT_WHITE,
        COLOR_DEFAULT,
        COLOR_RESET
    };
}

std::ostream& operator<<(std::ostream& os, Console::Color color);

#endif