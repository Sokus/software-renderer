#include "parsexec.h"

#include <stdio.h>
#include <stdbool.h>

#include "parstypes.h"
#include "commands.h"
#include "console/keycode.h"
#include "console/input.h"
#include "utility/strops.h"
#include "utility/macros.h"

#define INPUT_BUFFER_SIZE 256

static char gBuffer[INPUT_BUFFER_SIZE] = {0};
static int gTop = -1;

void ProcessInput()
{
    KeyCode kc = GetInput();
    
    bool isText = IsLetter(kc) || IsNumber(kc);
    bool isBackspace = (kc == KC_BACKSPACE);

    if(isText)
    {
        PushCharacter(kc);
    }
    else if(isBackspace)
    {
        PopCharacter();
    }
}

static void PushCharacter(char c)
{
    if(gTop < INPUT_BUFFER_SIZE-1) gBuffer[++gTop] = c;
}

static void PopCharacter()
{
    if(gTop >= 0) gBuffer[gTop--] = 0;
}

static void Reset()
{
    DynamicArray* pCommands = GetCommands();
    ResetCommands(pCommands);
}

static void ResetCommands(DynamicArray* pCommands)
{
    if(!Array_Initialized(pCommands)) return;

    for(size_t i=0; i<pCommands->length; i++)
    {
        Command* cmd = Array_Get(pCommands, i);
        ResetPatterns(cmd->patterns);
        cmd->active = HasProperties(gContext, cmd->contextConditions);
    }
}

static void ResetPatterns(LinkedListNode* pNode)
{
    pNode = GetFirstFromList(pNode);
    for(;
        pNode != NULL;
        pNode = pNode->next)
    {
        Pattern* pPat = CONTAINEROF(pNode, Pattern, list);
        FreeMatches(&pPat->possibleMatches);
        Array_Free(&pPat->possibleMatches);
        pPat->match = NULL;
        char* readFrom = NULL;
    }
}

static void FreeMatches(DynamicArray* pMatches)
{
    for(size_t i=0; i<pMatches->length; i++)
    {
        Match* pMatch = Array_Get(pMatches, i);
        Array_Free(&pMatch->patterns);
        Array_Free(&pMatch->objects);
    }
}

bool CompareMatches(Match* pA, Match* pB)
{
    if(!pA || !pB) return false;
    return pA->type==pB->type && !CompareStringInsensitive(pA->text, pB->text);
}

int MergeMatches(Match* pA, Match* pB)
{
    if(!pA || !pB
        || pA->type != pB->type
        || CompareStringInsensitive(pA->text, pB->text)) return -1;
    
}

int AddMatchToArray(DynamicArray* pArr, Match* pM)
{
    if(!pArr || !pM || !Array_Initialized(pArr)) return -1;

    Match* p = NULL;
    for(size_t m_i=0; m_i<pArr->length; m_i++)
    {
        p = Array_Get(pArr, m_i);
        if(CompareMatches(p, pM))
        {
            Array_Copy(&pM->patterns, &p->patterns);
            Array_Copy(&pM->objects, &p->objects);
            return 0;
        }
    }
}
