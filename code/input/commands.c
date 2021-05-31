#include "commands.h"

Command* GetCommands()
{
    static bool commandsCreated = false;
    if( !commandsCreated )
    {
        commandsCreated = true;
        CreateCommands();
    }

    return gCommands;
}

static void CreateCommands()
{

    gCommands[0] = (Command) { {Copy("list")},
                                0, 0, ExecuteList };

    gCommands[1] = (Command) { {Copy("look around")},
                                0, 0, ExecuteLookAround };
    
    gCommands[2] = (Command) { {Copy("look at the $0t"), Copy("look at the $0o $1t")},
                                DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecuteLookAt };

    gCommands[3] = (Command) { {Copy("inventory")},
                                0, 0, ExecuteExamineInventory };

    gCommands[4] = (Command) { {Copy("pick up the $0t"), Copy("pick up the $0o $1t")},
                                DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecutePickUp };

    gCommands[5] = (Command) { {Copy("put the $0t in the $1t"), Copy("put the $0o $1t in the $2t"),
                                Copy("put the $0t in the $1o $2t"), Copy("put the $0o $1t in the $2o $3t")},
                                DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecutePut };

    gCommands[6] = (Command) { {Copy("drop the $0t"), Copy("drop the $0o $1t")},
                                DISTANCE_INVENTORY, DISTANCE_INVENTORY_CONTAINED, ExecuteDrop };

    gCommands[7] = (Command) { {Copy("open the $0t"), Copy("open the $0o $1t")},
                                DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecuteOpen };

    gCommands[8] = (Command) { {Copy("close the $0t"), Copy("close the $0o $1t")},
                                DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecuteClose };

    gCommands[9] = (Command) { {Copy("clear")}, 0, 0, ExecuteClear };

    gCommands[10] = (Command) { {Copy("quit")}, 0, 0, ExecuteQuit };
}