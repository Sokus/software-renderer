#include <stdio.h>
#include <stdlib.h>

#include "input/parsexec.h"
#include "objects/object.h"
#include "output/console.h"

int main()
{
    CreateObjects();
    char* buffer = Copy("list");
    while(true)
    {
        if( !ParseInput(buffer)) break;

        printf("> ");
        GetInput(&buffer);
    }
    DeleteObjects();
    printf("Press enter to continue...\n");
    getc(stdin);
    return 0;
}