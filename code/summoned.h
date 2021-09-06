/* date = August 6th 2021 3:39 pm */

#ifndef SUMMONED_H
#define SUMMONED_H

typedef struct GameState
{
    F32 time;
    
    F32 position_x;
    F32 position_y;
    
    MemoryArena arena;
} GameState;

typedef enum SplitType
{
    SPLITTYPE_NONE,
    SPLITTYPE_VERTICAL,
    SPLITTYPE_HORIZONTAL
} SplitType;

typedef struct Rect
{
    F32 x0;
    F32 y0;
    F32 x1;
    F32 y1;
    
    F32 width;
    F32 height;
} Rect;

#endif //SUMMONED_H
