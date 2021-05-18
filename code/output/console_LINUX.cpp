#include "console.hpp"

std::ostream& operator<<(std::ostream& os, Console::Color color)
{
    using namespace Console;

    // Reset:       \u001b[0m
    // Black:       \u001b[30m
    // BrightBlack: \u001b[30;1m

    if(color == COLOR_DEFAULT || color == COLOR_RESET)
    {
        os << "\u001b[0m";
    }
    else
    {
        bool bright = color >= COLOR_BRIGHT_BLACK;
        int attribute = color % 8;
        os << "\x001b[" << 30+attribute;
        if(bright) os << ";1";
        os << "m";
    }

    return os;
}