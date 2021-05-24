#include <stdio.h>
#include <stdlib.h>

#include "input/parsexec.h"
#include "temporary.h"
#include "output/console.h"

int main()
{
    CreateObjects();
    char* buffer = Copy("look around");
    while(true)
    {
        if( !ParseInput(buffer)) break;

        printf("\n> ");
        GetInput(&buffer);
    }
    DeleteObjects();
    printf("Press enter to continue...\n");
    getc(stdin);
    return 0;
}