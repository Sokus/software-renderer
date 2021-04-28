#include "parsexec.h"

static void clearArgs()
{
    for(int i=0; i<26; i++)
    {
        args[i] = NULL;
    }
}

static int compareWithTag(char *src, char *tag)
{
    while(*tag != '\0')
    {
        src = skipSpaces(src);
        tag = skipSpaces(tag);

        if( !(compareInsensitive(*src, *tag) || *src == '\0') )
        {
            return 0;
        }

        src++;
        tag++;
    }
    return 1;
}

static Object *findByTag(Object *root, char *input, int *minTagLength, Distance minDistance, Distance maxDistance)
{
    Object *match = NULL;
    for(Object *o=root; o != NULL; o=o->next)
    {
        Distance dist = getDistance(&player, o);
        if(minDistance <= dist && dist <= maxDistance)
        {
            for(int i=0; i<8; i++)
            {
                if(o->tags[i] == NULL) continue;
                int tagLength = getLength((o->tags)[i]);
                if( tagLength <= *minTagLength
                    || !compareWithTag(input, o->tags[i])) continue;
                *minTagLength = tagLength;
                match = o;
            }
        }

        Object *containedMatch = findByTag(o->inventoryHead, input, minTagLength, minDistance, maxDistance);
        if(containedMatch != NULL)
        {
            match = containedMatch;
        }
    }
    return match;
}

static int matchCommand(char *input, Command *cmd)
{
    clearArgs();
    char *pattern = cmd->pattern;
    Object *match = NULL;
    while(*pattern != '\0')
    {
        pattern = skipSpaces(pattern);
        input = skipSpaces(input);

        if(isCapital(*pattern))
        {
            int minTagLength = 0;
            match = findByTag(&objectHead, input, &minTagLength, cmd->minDistance, cmd->maxDistance);

            if(match != NULL)
            {
                // ASCII:  A = 65, A -> 1st array element
                if(args[*pattern-65] == NULL)
                {
                    args[*pattern-65] = match;
                }
                match = NULL;
                
                // skip the name of matching object in the input string
                // to continue the comparison
                for(int i=0; *(input+1) != '\0' && i<minTagLength-1; i++)
                {
                    input++;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if( !(compareInsensitive(*pattern, *input) || *input == '\0') ) return 0;
        }

        pattern++;
        input++;
    }

    return 1;
}

int parseInput(char *input)
{
    Command commands[] = 
    {
        { "quit"        , 0, 0, executeQuit },
        { "go A"        , NEAR, NEAR, executeTravel },
        { "go to A"     , NEAR, NEAR, executeTravel },
        { "enter A"     , NEAR, NEAR, executeTravel },
        { "look around" , 0, 0, executeLookAround },
        { "look at A"   , INVENTORY, NEAR_CONTAINED, executeLookAt },
        { "examine A"   , INVENTORY, NEAR_CONTAINED, executeLookAt },
        { "pick up A"   , NEAR, NEAR_CONTAINED, executePickUp },
        { "get A"       , NEAR, NEAR_CONTAINED, executePickUp },
        { "drop A"      , INVENTORY, INVENTORY_CONTAINED, executeDrop },
        { "help"        , 0, 0, executeHelp }
    };

    size_t commandsLength = sizeof(commands)/sizeof(Command);
    Command *cmd = NULL;
    for(size_t i=0; i<commandsLength; i++)
    {
        if(matchCommand(input, &commands[i]))
        {
            cmd = &commands[i];
            break;
        }
    }

    if(cmd != NULL)
    {
        if(cmd->function != NULL)
        {
            return cmd->function(args);
        }
        return 1;
    }
    else
    {
        printf("I don't know what you're trying to do.\n");
    }
    return 1;
}