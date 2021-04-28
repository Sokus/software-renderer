#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "input/parsexec.h"
#include "objects/object.h"
#include "logic/interact.h"

Object objectHead = {{"field", "grassy field"}, "a grassy field", "It is a huge field covered in grass.", 9999, 9999, 1, NULL, NULL, NULL };
Object player = {{"player"}, "a player", "This is you.", 100, 20, 1, NULL, NULL, NULL };

// TODO: Move it to separate module
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
    // TODO: Serialization
    Object purse = {{"purse", "coin purse"},
                    "a coin purse",
                    "It is a purse for holding coins.",
                    1, 5, 1,
                    NULL, NULL, NULL };
    Object coin = {{"coin", "silver coin"},
                    "a silver coin",
                    "It is a coin made out of silver.",
                    0, 0, 1,
                    NULL, NULL, NULL };
    Object guard = {{"guard", "city guard"},
                    "a city guard",
                    "This guard makes the city secure.",
                    100, 20, 1,
                    NULL, NULL, NULL };
    objectHead.inventoryHead = &player;
    player.inventoryHead = &purse;
    purse.inventoryHead = &coin;
    player.next = &guard;

    char buffer[INPUT_SIZE];
    printf("Type \"help\" for the list of commands.\n");
    while(getInput(buffer), parseInput(buffer));
    
    printf("Press enter to continue...\n");
    getc(stdin);
    return 0;
}