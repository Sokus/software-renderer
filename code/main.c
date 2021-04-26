#include <stdio.h>

#include "defs.h"
#include "parsexec.h"

void getInput(char *buffer)
{
    char input = '\0';
    int characters_read = 0;
    printf("> ");
    while(input=getc(stdin), input != '\n')
    {
        if(characters_read < INPUT_SIZE)
        {
            buffer[characters_read] = input;
        }
        characters_read++;
    }
    int end_offset = characters_read < INPUT_SIZE ? characters_read : INPUT_SIZE - 1;
    *(buffer + end_offset) = '\0';
}

int main()
{
    Object objectHead;
    Object coin = {{"coin", "silver coin"}, "It is a silver coin."};
    Object guard = {{"guard", "city guard"}, "This guard makes the city secure."};

    objectHead.next = &coin;
    objectHead.next->next = &guard;

    char buffer[INPUT_SIZE];
    while(getInput(buffer), parseInput(buffer, objectHead));
    printf("Press enter to continue...\n");
    getc(stdin);
    return 0;
}