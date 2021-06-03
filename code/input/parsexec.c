#include "parsexec.h"

Argument GetArgumentOfType(ArgType argType, int skip)
{
    for(int i=0; i<ARGS_MAX_COUNT; i++)
    {
        if(gArgs[i].type == argType && skip-- <= 0)
        {
            return gArgs[i];
        }
    }
    return (Argument) {0};
}

static void ClearArgs()
{
    for(int i=0; i<ARGS_MAX_COUNT; i++) gArgs[i] = (Argument){0};
}

bool ParseInput(char* input)
{
    if(input && GetLength(input))
    {
        char* src = Copy(input);
        RemovePadding(&src);
        RemoveDoubleSpaces(&src);

        Command* commands = GetCommands();
        Command* cmd = NULL;
        for(int i=0; i<32; i++)
        {
            if(MatchCommand(src, commands+i))
            {
                cmd = commands+i;
                break;
            }
        }
        free(src);

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

static bool MatchCommand(char* src, Command cmd[])
{
    SearchParameters params = {0};
    for(int i=0; i<COMMANDS_MAX_PATTERNS; i++)
    {
        params.minDistance = cmd->minDistance;
        params.maxDistance = cmd->maxDistance;
        if(MatchPattern(src, cmd->patterns[i], params)) return true;
    }
    return false;
}

static bool MatchPattern(char* src, char* pattern, SearchParameters params)
{
    if(!pattern) return false;

    ClearArgs();
    Argument arg = {0};

    while(*pattern != '\0')
    {
        if(*pattern == '$')
        {
            // get index
            pattern++;
            int index = (int)(*pattern)-48;
            if(index < 0 || 9 < index) return false;

            // get type
            pattern++;
            arg.type = EvaluateType(*pattern);

            // try to read type and return on failure
            int result = 0;
            switch(arg.type)
            {
                case ARG_TYPE_INT:
                { result = ReadArgumentInt(src, &arg.value); } break;

                case ARG_TYPE_ORDINAL:
                { result = ReadArgumentOrdinal(src, &arg.value); } break;

                case ARG_TYPE_TAG:
                {
                    // do the first pass that will find the first matching item
                    // with the longest possible tag
                    params.minTagLength = 0;
                    arg.p = FindByTag(src, &params);

                    // check if the previous argument was of the type "ordinal"
                    // and look through objects again, this time allow tags
                    // of the same length as the match just found
                    if(index > 0 && gArgs[index-1].type == ARG_TYPE_ORDINAL)
                    {
                        params.matchesToSkip = gArgs[index-1].value + 1;
                        Object* possibleMatch = FindByTag(src, &params);
                        if(possibleMatch) arg.p = possibleMatch;
                    }

                    result = params.minTagLength;
                } break;

                default: { } break;
            }

            if(!result) return false;

            // pass argument to gArgs
            gArgs[index] = arg;

            // skip source chunk to align it with the pattern
            src += result-1;
        }
        else
        {
            bool charactersMatch = CompareCharInsensitive(*src, *pattern);
            bool sourceEnded = (*src == '\0');
            if( !charactersMatch || sourceEnded ) return false;
        }
        src++;
        pattern++;
    }
    return true;
}

static ArgType EvaluateType(char type)
{
    return  type == 'i' ? ARG_TYPE_INT :
            type == 'o' ? ARG_TYPE_ORDINAL :
            type == 't' ? ARG_TYPE_TAG :
            ARG_TYPE_INVALID;
}

static int ReadArgumentInt(char* src, int* value)
{
    int number = 0;
    int charactersRead = 0;
    while(*src != ' ' && *src != '\0')
    {
        if(!IsNumber(*src)) return 0;

        // shift numbers to the left
        // then read the number and
        // convert from 'char' to 'int' (-48)
        number *= 10; 
        number += ((int)*src - 48); 

        src++;
        charactersRead++;
    }
    *value = number;
    return charactersRead;
}

static int ReadArgumentOrdinal(char* src, int* value)
{
    
    char* shortOrdinals[] = {"1st", "2nd", "3rd", "4th", "5th",
                            "6th", "7th", "8th", "9th", "10th",
                            "11th", "12th", "13th", "14th", "15th",
                            "16th", "17th", "18th", "19th", "20th"};
    int shortOrdinalsSize = sizeof(shortOrdinals)/sizeof(char*);

    for(int i=0; i<shortOrdinalsSize; i++)
    {
        if(StartsWith(src, shortOrdinals[i]))
        {
            *value = i;
            return GetLength(shortOrdinals[i]);
        }
    }

    char* ordinals[] = {"first", "second", "third", "fourth", "fifth",
                        "sixth", "seventh", "eighth", "ninth", "tenth",
                        "eleventh", "twelfth", "thirteenth", "fourteenth", "fifteenth",
                        "sixteenth", "seventeenth", "eighteenth", "nineteenth", "twentieth"};
    int ordinalsSize = sizeof(ordinals)/sizeof(char*);

    for(int i=0; i<ordinalsSize; i++)
    {
        if(StartsWith(src, ordinals[i]))
        {
            *value = i;
            return GetLength(ordinals[i]);
        }
    }

    char* last = "last";
    if(StartsWith(src, last))
    {
        *value = 9999;
        return GetLength(last);
    }

    return 0;
}

static Object* FindByTag(char* src, SearchParameters* params)
{
    Object* match = NULL;
    if(IsInRange(DISTANCE_SELF, params->minDistance, params->maxDistance)
        && MatchObjectTag(src, gpPlayer, params))
    {
        match = gpPlayer;
        params->matchesToSkip--;
    }

    if(IsInRange(DISTANCE_LOCATION, params->minDistance, params->maxDistance))
    {
        Object* parent = gpPlayer->parent;
        if(MatchObjectTag(src, parent, params))
        {
            match = parent;
            params->matchesToSkip--;
        }
    }

    if(IsInRange(DISTANCE_INVENTORY, params->minDistance, params->maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_INVENTORY_CONTAINED,
                                    params->minDistance,
                                    params->maxDistance);
        Object* possibleMatch = FindByTagRecursive(src, gpPlayer->inventoryHead, params, doDeepSearch);
        if(possibleMatch)
        {
            match = possibleMatch;
            params->matchesToSkip--;
        }
    }

    if(IsInRange(DISTANCE_NEAR, params->minDistance, params->maxDistance))
    {
        bool doDeepSearch = IsInRange(DISTANCE_NEAR_CONTAINED,
                                    params->minDistance,
                                    params->maxDistance);
        Object* possibleMatch = FindByTagRecursive(src, gpPlayer->parent->inventoryHead, params, doDeepSearch);
        if(possibleMatch)
        {
            match = possibleMatch;
            params->matchesToSkip--;
        }
    }

    return match;
}

static Object* FindByTagRecursive(char* src, Object* head,
                                  SearchParameters* params,
                                  bool deepSearch)
{
    Object* match = NULL;
    for(Object* pObj=head; pObj != NULL; pObj=pObj->next)
    {
        if(MatchObjectTag(src, pObj, params))
        {
            match = pObj;
            params->matchesToSkip--;
        }

        if(deepSearch
            && HasProperty(pObj->properties, OBJECT_PROPERTY_VISIBLE_INVENTORY)
            && HasProperty(pObj->properties, OBJECT_PROPERTY_CONTAINER)
            && HasProperty(pObj->properties, OBJECT_PROPERTY_OPEN))
        {
            Object* possibleMatch = FindByTagRecursive(src, pObj->inventoryHead, params, true);
            if(possibleMatch) match = possibleMatch;
        }
    }
    return match;
}

static bool MatchObjectTag(char* src, Object* obj, SearchParameters* params)
{
    if(!obj) return false;
    bool result = false;
    char* tag;
    for(int i=0; i<OBJECT_MAX_TAGS; i++)
    {
        tag = obj->tags[i];
        if(tag)
        {
            int tagLength = GetLength(tag);
            bool skipPossible = params->matchesToSkip > 0;
            bool longEnough;
            if(skipPossible) longEnough = tagLength >= params->minTagLength;
            else longEnough = tagLength > params->minTagLength;

            if( !longEnough
                || !StartsWith(src, tag)) continue;
            params->minTagLength = tagLength;
            result = true;
        }
    }
    return result;
}

void GetInput(char** pBuffer)
{
    if(!pBuffer) return;
    char* buffer = *pBuffer;
    if(!buffer) return;

    int INPUT_SIZE = 255;
    free(buffer);
    buffer = (char*)malloc(sizeof(char)*INPUT_SIZE);

    char input;
    int index = 0;
    
    while(input=getc(stdin), input != '\n')
    {
        if(index < INPUT_SIZE)
        {
            buffer[index] = input;
        }
        index++;
    }
    int end = index < INPUT_SIZE ? index : INPUT_SIZE - 1;
    buffer[end] = '\0';
    *pBuffer = buffer;
}