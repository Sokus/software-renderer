/* date = August 6th 2021 3:39 pm */

#ifndef SUMMONED_H
#define SUMMONED_H

typedef struct Rect
{
    F32 x0;
    F32 y0;
    F32 x1;
    F32 y1;
} Rect;

typedef enum BorderType
{
    BorderType_None,
    BorderType_Centered,
    BorderType_Inner,
    BorderType_Outer
} BorderType;

typedef struct UIContext
{
    Input *input;
} UIContext;

typedef struct GameState
{
    uint frame_index;
    F32 time;
    
    MemoryArena arena;
} GameState;

#endif //SUMMONED_H
