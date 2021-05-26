#include "parsexec.h"

bool ParseInput(const char *input)
{
    if(input && GetLength(input))
    {
        Command* commands = GetCommands();
        Command* cmd = NULL;
        for(int i=0; i<32; i++)
        {
            if(MatchCommand(input, commands+i))
            {
                cmd = commands+i;
                break;
            }
        }

        if(cmd)
        {
            if(cmd->function) return cmd->function(gArgs);
        }
        else
        {
            printf("I don't know what you're trying to do.\n");
        }
    }
    return true;
}

static bool MatchCommand(const char* src, const Command cmd[])
{
    for(int i=0; i<COMMANDS_MAX_PATTERNS; i++)
    {
        const char* pattern = cmd->patterns[i];
        if(MatchPattern(src, pattern, cmd->minDistance, cmd->maxDistance))
        {
            return true;
        }
    }
    return false;
}

static bool MatchPattern(const char* src, const char* pattern,
                        Distance minDistance, Distance maxDistance)
{
    if(!pattern) return false;

    ClearArgs();
    Object* match = NULL;

    while(*pattern != '\0')
    {
        src = SkipSpaces(src);
        pattern = SkipSpaces(pattern);

        if(IsUpper(*pattern))
        {
            int minTagLength = 0;
            match = FindByTag(src, &minTagLength, minDistance, maxDistance);

            if(match)
            {
                int index = (int)(*pattern) - 65;
                if(!gArgs[index]) gArgs[index] = match;
                match = NULL;
                src += (minTagLength-1); 
            }
            else
            {
                return false;
            }
        }
        else
        {
            bool charactersMatch = CompareCharInsensitive(*src, *pattern);
            bool sourceEnded = (*src == '\0');
            if( !charactersMatch || sourceEnded )
            {
                return false;
            }
        }

        src++;
        pattern++;
    }

    return true;
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
        if(possibleMatch) match = possibleMatch;
    }

    if(IsInRange(DISTANCE_NEAR, minDistance, maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_NEAR_CONTAINED, minDistance, maxDistance);
        Object* possibleMatch = FindByTagRecursive(src, gpPlayer->parent->inventoryHead, minTagLength, doDeepSearch);
        if(possibleMatch) match = possibleMatch;
    }

    return match;
}

static Object* FindByTagRecursive(const char* src, Object* head,
                                  int* minTagLength, bool deepSearch)
{
    Object* match = NULL;
    for(Object* pObj=head; pObj != NULL; pObj=pObj->next)
    {
        if(MatchObjectTag(src, pObj, minTagLength))
        {
            match = pObj;
        }

        if(deepSearch && HasProperty(pObj->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY))
        {
            Object* possibleMatch = FindByTagRecursive(src, pObj->inventoryHead, minTagLength, true);
            if(possibleMatch != NULL) match = possibleMatch;
        }
    }
    return match;
}

static bool MatchObjectTag(const char* src, const Object* obj, int* minTagLength)
{
    if(!obj) return false;
    bool result = false;
    const char* tag;
    for(int i=0; i<OBJECT_MAX_TAGS; i++)
    {
        tag = obj->tags[i];
        if(tag)
        {
            int tagLength = GetLength(tag);
            if( tagLength <= *minTagLength
                || !CompareWithTag(src, tag)) continue;
            *minTagLength = tagLength;
            result = true;
        }
    }
    return result;
}

static bool CompareWithTag(const char* src, const char* tag)
{
    while(*tag != '\0')
    {
        src = SkipSpaces(src);
        tag = SkipSpaces(tag);

        bool charactersMatch = CompareCharInsensitive(*src, *tag);
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

void GetInput(char** buffer)
{
    const int INPUT_SIZE = 255;
    free(*buffer);
    *buffer = (char*)malloc(sizeof(char)*INPUT_SIZE);

    char input;
    int characters_read = 0;
    
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

static void ClearArgs()
{
    for(int i=0; i<26; i++)
    {
        gArgs[i] = NULL;
    }
}
