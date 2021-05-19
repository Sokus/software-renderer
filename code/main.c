#include <stdio.h>
#include <stdlib.h>

#include "input/parsexec.h"
#include "temporary.h"
#include "output/console.h"

int main()
{
    CreateObjects();
    for(int i=1; i<16; i++)
    {
        Console_Print("I have had enough...\n", (Console_Color)i);
    }
    /*
    char* buffer;
    GetInput(&buffer);
    while(ParseInput(buffer))
    {
        // game loop

        GetInput(&buffer);
    }
    */
    DeleteObjects();
    printf("Press enter to continue...\n");
    getc(stdin);
    return 0;
}