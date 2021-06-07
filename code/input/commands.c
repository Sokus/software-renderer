#include "commands.h"

void CreateCommands()
{
    int index = -1;
    gCommands[++index] = (Command) {    0,
                                        { Copy("look at the $0t13"),
                                          Copy("look at the $0o $1t13") },
                                        Copy("look at the <name>"),
                                        NULL,
                                        ExecuteLookAt };

    gCommands[++index] = (Command) {    0,
                                        { Copy("open the $0t14"),
                                          Copy("open the $0o $1t14") },
                                        Copy("open the <container>"),
                                        NULL,
                                        ExecuteOpen };
                                        
    gCommands[++index] = (Command) {    0,
                                        { Copy("close the inventory") },
                                        Copy("close the inventory"),
                                        NULL,
                                        ExecuteClose };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_INVENTORY_OPEN, true);
                                        
    gCommands[++index] = (Command) {    0,
                                        { Copy("close the $0t55") },
                                        Copy("close the <container>"),
                                        NULL,
                                        ExecuteClose };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_CONTAINER_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("close") },
                                        NULL,
                                        NULL,
                                        ExecuteClose };

    gCommands[++index] = (Command) {    0,
                                        { Copy("inventory page next"),
                                          Copy("inventory next page") },
                                        Copy("inventory page next"),
                                        NULL,
                                        ExecuteInventoryNext };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_INVENTORY_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("inventory page previous") },
                                        Copy("inventory page previous"),
                                        NULL,
                                        ExecuteInventoryPrev };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_INVENTORY_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("inventory $0o page"),
                                          Copy("inventory page $0i") },
                                        Copy("inventory page <number>"),
                                        NULL,
                                        ExecuteInventoryPage };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_INVENTORY_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("inventory")},
                                        NULL,
                                        NULL,
                                        ExecuteInventory };

    gCommands[++index] = (Command) {    0,
                                        { Copy("$0t55 page next"),
                                          Copy("0t55 next page") },
                                        Copy("<container> page next"),
                                        NULL,
                                        ExecuteContainerNext };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_CONTAINER_OPEN, true);


    gCommands[++index] = (Command) {    0,
                                        { Copy("$0t55 page previous"),
                                          Copy("$0t55 previous page") },
                                        Copy("<container> page previous"),
                                        NULL,
                                        ExecuteContainerPrev };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_CONTAINER_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("$0t55 $1o page"),
                                          Copy("$0t55 page $1i") },
                                        Copy("<container> page <number>"),
                                        NULL,
                                        ExecuteContainerPage };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_CONTAINER_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("pick up the $0t24"),
                                          Copy("pick up the $0o $1t24") },
                                        Copy("pick up the <name>"),
                                        NULL,
                                        ExecutePickUp };

    gCommands[++index] = (Command) {    0,
                                        { Copy("drop the $0t11"),
                                          Copy("drop the $0o $1t11"),
                                          Copy("drop the $0t44"),
                                          Copy("drop the $0o $1t44") },
                                        Copy("drop the <name>"),
                                        NULL,
                                        ExecuteDrop };

    gCommands[++index] = (Command) {    0,
                                        { Copy("move the $0t11 to the $1t55"),
                                          Copy("move the $0o $1t11 to the $2t55") },
                                        Copy("move the <name> to the <container>"),
                                        NULL,
                                        ExecuteMoveToContainer };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_INVENTORY_OPEN, true);
    SetProperty(&gCommands[index].contextConditions, CONTEXT_CONTAINER_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("move the $0t44 to the inventory"),
                                          Copy("move the $0o $1t44 to the inventory") },
                                        Copy("move the <name> to the inventory"),
                                        NULL,
                                        ExecuteMoveToInventory };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_INVENTORY_OPEN, true);
    SetProperty(&gCommands[index].contextConditions, CONTEXT_CONTAINER_OPEN, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("clear") },
                                        Copy("clear"),
                                        Copy("clears the console"),
                                        ExecuteClear };
    
    gCommands[++index] = (Command) {    0,
                                        { Copy("help") },
                                        NULL,
                                        NULL,
                                        ExecuteHelp };

    gCommands[++index] = (Command) {    0,
                                        { Copy("quit") },
                                        Copy("quit"),
                                        Copy("leave the game"),
                                        ExecuteQuit };

    gCommands[++index] = (Command) {    0,
                                        { Copy("debug") },
                                        NULL,
                                        NULL,
                                        ExecuteDebug };

    gCommands[++index] = (Command) {    0,
                                        { Copy("property list $0t16"),
                                          Copy("property list $0o $1t16") },
                                        Copy("property list <object name>"),
                                        NULL,
                                        ExecuteListProperties };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_DEBUG_MODE, true);

    gCommands[++index] = (Command) {    0,
                                        { Copy("property set $0t16 $1p $2i"),
                                          Copy("property set $0o $1t16 $2p $3i") },
                                        Copy("property set <object name> <property name> <value>"),
                                        NULL,
                                        ExecuteSetProperty };
    SetProperty(&gCommands[index].contextConditions, CONTEXT_DEBUG_MODE, true);

}

void DeleteCommands()
{
    for(int i=0; i<COMMANDS_MAX_COUNT; i++)
    {
        for(int j=0; j<COMMANDS_MAX_PATTERNS; j++)
        {
            free(gCommands[i].patterns[j]);
        }

        free(gCommands[i].pattern);
        free(gCommands[i].details);
    }
}

