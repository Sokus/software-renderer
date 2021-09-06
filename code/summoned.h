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

typedef struct Rect
{
    F32 x0;
    F32 y0;
    F32 x1;
    F32 y1;
} Rect;

#endif //SUMMONED_H
