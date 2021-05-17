#include "commands.hpp"

Command::Command() : pattern(""), minDistance((Distance)0), maxDistance((Distance)0), function(nullptr) { }

Command::Command(std::string pattern, Distance minDistance, Distance maxDistance, int (*function)(Object **))
: pattern(pattern), minDistance(minDistance), maxDistance(maxDistance), function(function) { }

std::vector<Command> GetCommands()
{
    if(commands.size() == 0) CreateCommands();
    return commands;
}

static void CreateCommands()
{
    commands.reserve(11);
    commands.emplace_back(Command("quit",(Distance)0, (Distance)0, ExecuteQuit));
    commands.emplace_back(Command("go A",DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel));
    commands.emplace_back(Command("go to A",DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel));
    commands.emplace_back(Command("enter A",DISTANCE_NEAR, DISTANCE_NEAR, ExecuteTravel));
    commands.emplace_back(Command("look around", (Distance)0, (Distance)0, ExecuteLookAround));
    commands.emplace_back(Command("look at A", DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecuteLookAt));
    commands.emplace_back(Command("examine A", DISTANCE_INVENTORY, DISTANCE_NEAR_CONTAINED, ExecuteLookAt));
    commands.emplace_back(Command("pick up A", DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecutePickUp));
    commands.emplace_back(Command("get A", DISTANCE_NEAR, DISTANCE_NEAR_CONTAINED, ExecutePickUp));
    commands.emplace_back(Command("drop A", DISTANCE_INVENTORY, DISTANCE_INVENTORY_CONTAINED, ExecuteDrop));
    commands.emplace_back(Command("help", (Distance)0, (Distance)0, ExecuteHelp));
}

int ExecuteQuit(Object *args[])
{
    std::cout << "Goodbye." << std::endl;
    return 0;
}

int ExecuteTravel(Object *args[])
{
    Object *objA = args[0];
    if(objA != NULL)
    {
        if(objA->destination != NULL)
        {
            //doTravel(&player, objA);
            std::cout << "Traveled " << objA->description << "." << std::endl;
        }
        else
        {
            std::string description = Capitalise(objA->description);
            std::cout << description << " can't be traveled." << std::endl;
        }
    }
    return 1;
}

int ExecuteLookAround(Object *args[])
{
    std::cout << gpPlayer->parent->details << std::endl;
    std::cout << "You can see:" << std::endl;
    for(Object *o=gpPlayer->parent->inventoryHead; o!=NULL; o=o->next)
    {
        if(o == gpPlayer) continue;
        std::cout << o->description << std::endl;
    }   
    return 1;
}

int ExecuteLookAt(Object *args[])
{
    Object* objA = args[0];
    if(objA != nullptr)
    {
        std::cout << objA->description << std::endl;
    }
    return 1;
}

int ExecutePickUp(Object *args[])
{
    Object *objA = args[0];
    if(objA != NULL)
    {
        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(gpPlayer->capacity >= objA->weight)
        {
            std::cout << "Picked up " << objA->description << "." << std::endl;
            //doPickup(&player, objA);
        }
        else
        {
            std::string description = Capitalise(objA->description);
            std::cout << description << " won't fit in your pockets." << std::endl;
        }
    }
    
    return 1;
}

int ExecuteDrop(Object *args[])
{
    Object *objA = args[0];
    if(objA != NULL)
    {
        Object *parent = gpPlayer->parent;
        // TODO: Calculate available capacity
        // NOTE: This condition is temporary
        // just for testing command system
        if(parent != NULL && parent->capacity >= objA->weight)
        {
            std::cout << "Dropped " << objA->description << "." << std::endl;
            //doDrop(&player, objA);
        }
        else
        {
            std::cout << "There is nowhere to drop " << objA->description << "." << std::endl;
        }
    }
    return 1;
}

int ExecuteHelp(Object *args[])
{
    size_t size = commands.size();
    for(size_t i=0; i<size; i++)
    {
        std::cout << commands[i].pattern;
        if(i < (size-1))
        {
            if(commands[i].function == commands[i+1].function)
            {
                std::cout << ", ";
            }
            else
            {
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
    return 1;
}

