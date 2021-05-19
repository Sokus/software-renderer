#include "parsexec.h"

static void ClearArgs()
{
    for(int i=0; i<26; i++)
    {
        gArgs[i] = NULL;
    }
}

static bool CompareWithTag(const char* src, const char* tag)
{
    while(*tag != '\0')
    {
        src = SkipSpaces(src);
        tag = SkipSpaces(tag);

        bool charactersMatch = CompareCharInsenitive(*src, *tag);
        bool sourceEnded = (*src == '\0');
        if( !charactersMatch || sourceEnded )
        {
            return false;
        }

        src++;
        tag++;
    }
    return true;
}

static bool MatchObjectTag(const char* src, const Object* obj, int* minTagLength)
{
    bool result = false;
    const char* tag;
    for(int i=0; i<8; i++)
    {
        tag = obj->tags[i];
        if(!tag) continue;
        int tagLength = GetLength(tag);
        if( tagLength <= *minTagLength
            || !CompareWithTag(src, tag)) continue;
        *minTagLength = tagLength;
        result = true;
    }
    return result;
}

static Object* FindByTagRecursive(const char* src, Object* head,
                                  int* minTagLength, bool deepSearch)
{
    Object* match = NULL;
    for(Object* o=head; o != NULL; o=o->next)
    {
        if(o != gpPlayer && MatchObjectTag(src, o, minTagLength))
        {
            match = o;
        }

        if(deepSearch /*&& !ObjectHasProperty(*head, ObjectProperty::OBJECT_PROPERTY_HIDDEN_EQUIPMENT)*/)
        {
            Object* possibleMatch = FindByTagRecursive(src, head->inventoryHead, minTagLength, true);
            if(possibleMatch != NULL) match = possibleMatch;
        }
    }
    return match;
}

static Object* FindByTag(const char* src, int* minTagLength,
                         Distance minDistance, Distance maxDistance)
{
    Object* match = NULL;
    if(IsInRange(DISTANCE_SELF, minDistance, maxDistance)
        && MatchObjectTag(src, gpPlayer, minTagLength))
    {
        match = gpPlayer;
    }

    if(IsInRange(DISTANCE_LOCATION, minDistance, maxDistance))
    {
        Object* parent = gpPlayer->parent;
        if(MatchObjectTag(src, parent, minTagLength))
        {
            match = parent;
        }
    }

    if(IsInRange(DISTANCE_INVENTORY, minDistance, maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_INVENTORY_CONTAINED, minDistance, maxDistance);
        Object* possibleMatch = FindByTagRecursive(src, gpPlayer->inventoryHead, minTagLength, doDeepSearch);
        if(possibleMatch != NULL) match = possibleMatch;
    }

    if(IsInRange(DISTANCE_NEAR, minDistance, maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_NEAR_CONTAINED, minDistance, maxDistance);
        Object* possibleMatch = FindByTagRecursive(src, gpPlayer->parent->inventoryHead, minTagLength, doDeepSearch);
        if(possibleMatch != NULL) match = possibleMatch;
    }

    return match;
}

static bool MatchCommand(const char* src, const Command* cmd)
{
    ClearArgs();
    Object* match = NULL;

    const char* pattern = cmd->pattern;

    while(*pattern != '\0')
    {
        src = SkipSpaces(src);
        pattern = SkipSpaces(pattern);

        if(IsUpper(*pattern))
        {
            int minTagLength = 0;
            match = FindByTag(src, &minTagLength, cmd->minDistance, cmd->maxDistance);

            if(match != NULL)
            {
                // ASCII:  A = 65, A -> 1st array element
                int index = (int)(*pattern) - 65;
                if(gArgs[index] == NULL)
                {
                    gArgs[index] = match;
                }
                match = NULL;

                // skip the name of matching object in the input string
                // to continue the comparison
                src += (minTagLength-1); 
            }
            else
            {
                return false;
            }
        }
        else
        {
            bool charactersMatch = CompareCharInsenitive(*src, *pattern);
            bool sourceEnded = (*src == '\0');
            if( !charactersMatch || sourceEnded ) return false;
        }

        src++;
        pattern++;
    }

    return true;
}

void GetInput(char** buffer)
{
    const int INPUT_SIZE = 255;
    free(*buffer);
    *buffer = (char*)malloc(sizeof(char)*INPUT_SIZE);

    char input;
    int characters_read = 0;

    printf("> ");
    while(input=getc(stdin), input != '\n')
    {
        if(characters_read < INPUT_SIZE)
        {
            (*buffer)[characters_read] = input;
        }
        characters_read++;
    }
    int end_offset = characters_read < INPUT_SIZE ? characters_read : INPUT_SIZE - 1;
    *((*buffer) + end_offset) = '\0';
}

bool ParseInput(const char *input)
{
    Command* commands = GetCommands();
    Command* cmd = NULL;
    for(int i=0; i<32; i++)
    {
        if(commands[i].pattern == NULL) continue;
        if(MatchCommand(input, commands+i))
        {
            cmd = commands+i;
            break;
        }
    }

    if(cmd)
    {
        if(cmd->function)
        {
            return cmd->function(gArgs);
        }
    }
    else
    {
        printf("I don't know what you're trying to do.\n");
    }
    return true;
}
