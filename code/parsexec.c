#include "parsexec.h"

static void clearArgs()
{
    for(int i=0; i<26; i++)
    {
        args[i] = NULL;
    }
}

static int getLength(char *src)
{
    int length = 0;
    while(*src != '\0')
    {
        length++;
        src++;
    }
    return length;
}

static char *skipSpaces(char *src)
{
    while(*src == ' ')
    {
        src++;
    }
    return src;
}

static int isCapital(char letter)
{
    return letter >= 'A' && letter <= 'Z';
}

static int compareInsensitive(char a, char b)
{
    // ASCII:  A(65) - a(97) = 32
    return  a == b    ? 1 :
            a+32 == b ? 1 :
            a == b+32 ? 1 : 0;
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

static int matchCommand(char *input, char *pattern, Object objectHead)
{
    clearArgs();
    Object *match = NULL;
    while(*pattern != '\0')
    {
        pattern = skipSpaces(pattern);
        input = skipSpaces(input);

        if(isCapital(*pattern))
        {
            int maxTagLength = 0;
            for(Object *o=objectHead.next; o != NULL; o=o->next)
            {
                for(int i=0; i<8; i++)
                {
                    if(o->tags[i] == NULL) continue;
                    int tagLength = getLength((o->tags)[i]);
                    if( tagLength <= maxTagLength
                        || !compareWithTag(input, o->tags[i])) continue;
                    maxTagLength = tagLength;
                    match = o;
                }
            }

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
                for(int i=0; *(input+1) != '\0' && i<maxTagLength-1; i++)
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

int parseInput(char *input, Object objectHead)
{
    Command commands[] = 
    {
        { "quit"        , executeQuit },
        { "go A"        , executeTravel },
        { "go to A"     , executeTravel },
        { "enter A"     , executeTravel },
        { "look around" , executeLookAround },
        { "look at A"   , executeLookAt },
        { "examine A"   , executeLookAt }
    };
    size_t commandsLength = sizeof(commands)/sizeof(Command);
    Command *cmd = NULL;
    for(size_t i=0; i<commandsLength; i++)
    {
        if(matchCommand(input, commands[i].pattern, objectHead))
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