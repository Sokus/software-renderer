#include "commands.h"

#include <stdlib.h>

#include "parstypes.h"
#include "utility/strops.h"
#include "utility/macros.h"

static DynamicArray gCommands = {0};

DynamicArray* GetCommands()
{
    return &gCommands;
}

void CreateCommands()
{
    if(!gCommands.p) return;
   Array_New(&gCommands, 12, sizeof(Command));

    // inventory
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR

        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("inventory");
        cmd.patterns = &p1->list;

        Array_Append(&gCommands, &cmd);
    }

    // open the container
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR

        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("open the");
        cmd.patterns = &p1->list;

        Pattern* p2 = (Pattern*)malloc(sizeof(Pattern));
        *p2 = (Pattern){0};
        p1->type = PT_TAG;
        SetProperty(&p2->distanceFlags, DISTANCE_INVENTORY, true);
        SetProperty(&p2->distanceFlags, DISTANCE_LOCATION, true);
        SetProperty(&p2->distanceFlags, DISTANCE_NEAR, true);
        SetProperty(&p2->distanceFlags, DISTANCE_TARGET_CONTAINED, true);
        ListAppend(cmd.patterns, &p2->list);

        Array_Append(&gCommands, &cmd);
    }

    // close the inventory
    {
        Command cmd = {0};
        SetProperty(&cmd.contextConditions, GS_INVENTORY_OPEN, true);
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("close the inventory");
        cmd.patterns = &p1->list;

        Array_Append(&gCommands, &cmd);
    }

    // close the container
    {
        Command cmd = {0};
        SetProperty(&cmd.contextConditions, GS_CONTAINER_OPEN, true);
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("close the");
        cmd.patterns = &p1->list;

        Pattern* p2 = (Pattern*)malloc(sizeof(Pattern));
        *p2 = (Pattern){0};
        p1->type = PT_TAG;
        SetProperty(&p2->distanceFlags, DISTANCE_TARGET, true);
        ListAppend(cmd.patterns, &p2->list);

        Array_Append(&gCommands, &cmd);
    }

    // close
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("close");
        cmd.patterns = &p1->list;

        Array_Append(&gCommands, &cmd);
    }

    // pick up the item
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("pick up the");
        cmd.patterns = &p1->list;

        Pattern* p2 = (Pattern*)malloc(sizeof(Pattern));
        *p2 = (Pattern){0};
        p1->type = PT_TAG;
        SetProperty(&p2->distanceFlags, DISTANCE_LOCATION, true);
        SetProperty(&p2->distanceFlags, DISTANCE_NEAR, true);
        SetProperty(&p2->distanceFlags, DISTANCE_TARGET_CONTAINED, true);
        ListAppend(cmd.patterns, &p2->list);

        Array_Append(&gCommands, &cmd);
    }

    // drop the item
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("drop the");
        cmd.patterns = &p1->list;

        Pattern* p2 = (Pattern*)malloc(sizeof(Pattern));
        *p2 = (Pattern){0};
        p1->type = PT_TAG;
        SetProperty(&p2->distanceFlags, DISTANCE_INVENTORY, true);
        SetProperty(&p2->distanceFlags, DISTANCE_TARGET_CONTAINED, true);
        ListAppend(cmd.patterns, &p2->list);

        Array_Append(&gCommands, &cmd);
    }

    // move the item to the inventory
    {
        Command cmd = {0};
        SetProperty(&cmd.contextConditions, GS_INVENTORY_OPEN, true);
        SetProperty(&cmd.contextConditions, GS_CONTAINER_OPEN, true);
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("move the");
        cmd.patterns = &p1->list;

        Pattern* p2 = (Pattern*)malloc(sizeof(Pattern));
        *p2 = (Pattern){0};
        p1->type = PT_TAG;
        SetProperty(&p2->distanceFlags, DISTANCE_TARGET_CONTAINED, true);
        ListAppend(cmd.patterns, &p2->list);
        
        Pattern* p3 = (Pattern*)malloc(sizeof(Pattern));
        *p3 = (Pattern){0};
        p3->type = PT_STRING;
        p3->text = Copy("to the inventory");
        ListAppend(cmd.patterns, &p3->list);

        Array_Append(&gCommands, &cmd);
    } 

    // move the item to the container
    {
        Command cmd = {0};
        SetProperty(&cmd.contextConditions, GS_INVENTORY_OPEN, true);
        SetProperty(&cmd.contextConditions, GS_CONTAINER_OPEN, true);
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("move the");
        cmd.patterns = &p1->list;

        Pattern* p2 = (Pattern*)malloc(sizeof(Pattern));
        *p2 = (Pattern){0};
        p1->type = PT_TAG;
        SetProperty(&p2->distanceFlags, DISTANCE_INVENTORY, true);
        ListAppend(cmd.patterns, &p2->list);
        
        Pattern* p3 = (Pattern*)malloc(sizeof(Pattern));
        *p3 = (Pattern){0};
        p3->type = PT_STRING;
        p3->text = Copy("to the");
        ListAppend(cmd.patterns, &p3->list);

        Pattern* p4 = (Pattern*)malloc(sizeof(Pattern));
        *p4 = (Pattern){0};
        p4->type = PT_TAG;
        SetProperty(&p4->distanceFlags, DISTANCE_TARGET, true);
        ListAppend(cmd.patterns, &p4->list);

        Array_Append(&gCommands, &cmd);
    }

    // clear
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("clear");
        cmd.patterns = &p1->list;

        Array_Append(&gCommands, &cmd);
    }

    // help
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("help");
        cmd.patterns = &p1->list;

        Array_Append(&gCommands, &cmd);
    }

    // quit
    {
        Command cmd = {0};
        // cmd.function = FUNCTIONPTR
        
        Pattern* p1 = (Pattern*)malloc(sizeof(Pattern));
        *p1 = (Pattern){0};
        p1->type = PT_STRING;
        p1->text = Copy("quit");
        cmd.patterns = &p1->list;

        Array_Append(&gCommands, &cmd);
    }
}

void DeleteCommands()
{
    for(int i=0; i<gCommands.length; i++)
    {
        Command* cmd = Array_Get(&gCommands, i);
        
        LinkedListNode* node = GetFirstFromList(cmd->patterns);
        while(node)
        {
            Pattern* p = CONTAINEROF(node, Pattern, list);
            if(p->type == PT_STRING) free(p->text);
            node = node->next;
            free(p);
        }
    }

    Array_Free(&gCommands);
}
