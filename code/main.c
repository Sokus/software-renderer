#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "properties.h"

#include "parser/parsexec.h"

PropertyBitField gContext;



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
        if(HasProperty(gContext, GS_SHUTDOWN)) break;
        
        Console_SetActiveColor(COLOR_BRIGHT_WHITE);
        Console_Print("> ");
        GetInput(&buffer);
        Console_ResetColor();
    }
    DeleteObjects();
    DeleteCommands();
    #endif

    while(1)
    {
        ProcessInput();
    }

    printf("Press enter to continue...\n");
    getc(stdin);
    return 0;
}