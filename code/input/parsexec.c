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

static Object *findByTag(Object *root, char *input, int *minTagLength)
{
    Object *match = NULL;
    for(Object *o=root; o != NULL; o=o->next)
    {
        if(o != &player)
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

        Object *containedMatch = findByTag(o->inventoryHead, input, minTagLength);
        if(containedMatch != NULL)
        {
            match = containedMatch;
        }
    }
    return match;
}

static int matchCommand(char *input, char *pattern)
{
    clearArgs();
    Object *match = NULL;
    while(*pattern != '\0')
    {
        pattern = skipSpaces(pattern);
        input = skipSpaces(input);

        if(isCapital(*pattern))
        {
            int minTagLength = 0;
            match = findByTag(&objectHead, input, &minTagLength);

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
        { "quit"        , executeQuit       },
        { "go A"        , executeTravel     },
        { "go to A"     , executeTravel     },
        { "enter A"     , executeTravel     },
        { "look around" , executeLookAround },
        { "look at A"   , executeLookAt     },
        { "examine A"   , executeLookAt     },
        { "pick up A"   , executePickUp     },
        { "get A"       , executePickUp     },
        { "drop A"      , executeDrop       },
        { "help"        , executeHelp       }
    };

    size_t commandsLength = sizeof(commands)/sizeof(Command);
    Command *cmd = NULL;
    for(size_t i=0; i<commandsLength; i++)
    {
        if(matchCommand(input, commands[i].pattern))
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