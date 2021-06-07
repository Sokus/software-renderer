#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum Console_Color
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
    COLOR_DEFAULT
} Console_Color;

static Console_Color defaultColor;
static Console_Color activeColor;

void Console_SetDefaultColor(Console_Color color);
void Console_SetActiveColor(Console_Color color);
void Console_ResetColor();
void Console_PrintColored(char* message, Console_Color color, ...);
void Console_Print(char* message, ...);
void Console_Clear();

#endif