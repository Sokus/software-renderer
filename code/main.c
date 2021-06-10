#include "input/parsexec.h"
#include "objects/object.h"
#include "output/console.h"

PropertyField gContext;

#include <conio.h>

int main()
{
    #ifdef S_RUN
    CreateObjects();
    CreateCommands();
    Console_SetDefaultColor(COLOR_WHITE);
    Console_ResetColor();
    char* buffer = Copy("");
    PrintInfo();
    while(true)
    {
        ParseInput(buffer);
        if(HasProperty(gContext, CONTEXT_SHUTDOWN)) break;
        
        Console_SetActiveColor(COLOR_BRIGHT_WHITE);
        Console_Print("> ");
        GetInput(&buffer);
        Console_ResetColor();
    }
    DeleteObjects();
    DeleteCommands();
    #endif

    char c;
    while(c = getch(), c != 'x')
    {
        printf("%c", c);
    }

    return 0;
}