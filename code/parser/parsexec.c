#include "parsexec.h"

Argument GetArgumentOfType(ArgType argType, int skip)
{
    Argument arg = {0};
    for(int i=0; i<ARGS_MAX_COUNT && skip >= 0; i++)
    {
        if(gArgs[i].type == argType) 
        {
            arg = gArgs[i];
            skip--;
        }
    }
    return arg;
}

void AppendArgument(Argument arg)
{
    for(int i=0; i<ARGS_MAX_COUNT; i++)
    {
        if(gArgs[i].type == ARG_TYPE_INVALID)
        {
            gArgs[i] = arg;
            break;
        }
    }
}

static void ClearArgs()
{
    for(int i=0; i<ARGS_MAX_COUNT; i++) gArgs[i] = (Argument){0};
}

void ParseInput(char* input)
{
    if(input && GetLength(input))
    {
        char* src = Copy(input);
        RemovePadding(&src);
        RemoveDoubleSpaces(&src);

        Command* command = NULL;
        Command* cmd = NULL;
        for(int i=0; i<32; i++)
        {
            command = gCommands+i;
            if(HasProperties(gContext, command->contextConditions) 
                && MatchCommand(src, gCommands+i))
            {
                cmd = gCommands+i;
                break;
            }
        }
        free(src);

        if(cmd)
        {
            if(cmd->function)
            {
                if(cmd->function(gArgs))
                {
                    Console_Clear();
                    PrintInfo();
                }
            }
            else
            {
                Console_Print("Command has no functionality.\n");
            }
        }
        else
        {
            Console_Print("I don't know what you're trying to do.\n");
        }
    }
}

static bool MatchCommand(char* src, Command cmd[])
{
    SearchParameters params = {0};
    for(int i=0; i<COMMANDS_MAX_PATTERNS; i++)
    {
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
                    // get range
                    pattern++;
                    Distance dist = (int)(*pattern)-48;
                    if(dist < 0 || DISTANCE_MAX <= dist) return false;
                    params.minDistance = dist;
                    pattern++;
                    dist = (int)(*pattern)-48;
                    if(dist < 0 || DISTANCE_MAX <= dist) return false;
                    params.maxDistance = dist;
                    
                    // do the first pass that will find the first matching item
                    // with the longest possible tag
                    params.minTagLength = 0;
                    arg.p = FindByTag(src, &params);

                    
                    // check if the previous argument was of the type "ordinal"
                    // and read the value to know how many matches to skip
                    params.matchesToSkip = 0;
                    if(index > 0 && gArgs[index-1].type == ARG_TYPE_ORDINAL)
                    {
                        params.matchesToSkip += gArgs[index-1].value;
                    }
                    
                    if(params.matchesToSkip == 0)
                    {
                        // if there was no skipping intended
                        // check if the last match could be
                        // the same as the current one
                        // if so, force a skip
                        Argument last = GetArgumentOfType(ARG_TYPE_TAG, ARGS_MAX_COUNT);
                        if(last.type != ARG_TYPE_INVALID && last.p == arg.p)
                        {
                            params.matchesToSkip += 1;
                        }
                    }
                    
                    // and look through objects again, this time with skips
                    if(params.matchesToSkip > 0)
                    {
                        Object* possibleMatch = FindByTag(src, &params);
                        if(possibleMatch) arg.p = possibleMatch;
                    }

                    result = params.minTagLength;
                } break;

                case ARG_TYPE_PROPERTY:
                { result = ReadArgumentProperty(src, &arg.value); } break;

                case ARG_TYPE_BOOL:
                { result = ReadArgumentBool(src, &arg.value); } break;

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
            type == 'p' ? ARG_TYPE_PROPERTY :
            type == 'b' ? ARG_TYPE_BOOL : 
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

static int ReadArgumentProperty(char* src, int* value)
{
    int charactersRead = ReadArgumentInt(src, value);
    if(charactersRead) return charactersRead;
    else
    {
        int length = 0;
        for(int i=0; i<PROPERTY_MAX; i++)
        {
            length = GetLength(gPropertyNames[i]);
            if(length > charactersRead && StartsWith(src, gPropertyNames[i]))
            {
                charactersRead = length;
                *value = i;
            }
        }
    }
    return charactersRead;
}

static int ReadArgumentBool(char* src, int* value)
{
    int charactersRead = ReadArgumentInt(src, value);
    if(charactersRead) return charactersRead;

    char* values[] = {"false", "true"};
    for(int i=0; i<2; i++)
    {
        if(StartsWith(src, values[i]))
        {
            *value = i;
            return GetLength(values[i]);
        }
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

    if(IsInRange(DISTANCE_INVENTORY, params->minDistance, params->maxDistance)
        && HasProperty(gContext, CONTEXT_INVENTORY_OPEN))
    {
        int objectsChecked = 0;
        for(Object* pObj=gpPlayer->inventory;
            pObj != NULL && objectsChecked < LIST_MAX_ROWS;
            pObj = pObj->next)
        {
            if(MatchObjectTag(src, pObj, params))
            {
                match = pObj;
                params->matchesToSkip--;
                objectsChecked++;
            }
        }
    }

    if(IsInRange(DISTANCE_TARGET, params->minDistance, params->maxDistance))
    {
        if(MatchObjectTag(src, gpPlayer->target, params))
        {
            match = gpPlayer->target;
            params->matchesToSkip--;
        }
    }

    if(IsInRange(DISTANCE_NEAR_CONTAINED, params->minDistance, params->maxDistance)
        && HasProperty(gContext, CONTEXT_CONTAINER_OPEN)
        && gpPlayer->target)
    {
        int objectsChecked = 0;
        for(Object* pObj=gpPlayer->target->inventory;
            pObj != NULL && objectsChecked < LIST_MAX_ROWS;
            pObj = pObj->next)
        {
            if(MatchObjectTag(src, pObj, params))
            {
                match = pObj;
                params->matchesToSkip--;
                objectsChecked++;
            }
        }
    }

    if(IsInRange(DISTANCE_NEAR, params->minDistance, params->maxDistance))
    {
        for(Object* pObj=GetFirstFromList(gpPlayer->parent->inventory);
            pObj != NULL;
            pObj = pObj->next)
        {
            if(pObj != gpPlayer && MatchObjectTag(src, pObj, params))
            {
                match = pObj;
                params->matchesToSkip--;
            }
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
            bool skipPossible = params->matchesToSkip >= 0;
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