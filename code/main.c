#include "parser/parsexec.h"
#include "objects/object.h"
#include "console/output.h"

PropertyField gContext;

#include <stdio.h>

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

    printf("Hello\n");
    return 0;
}