#include "input/parsexec.h"
#include "objects/object.h"
#include "output/console.h"

PropertyField gContext;

int main()
{
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
    return 0;
}