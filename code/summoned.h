/* date = August 6th 2021 3:39 pm */

#ifndef SUMMONED_H
#define SUMMONED_H


typedef struct MemoryArena
{
    U8 *base;
    MemoryIndex size;
    MemoryIndex used;
} MemoryArena;

internal void
InitializeArena(MemoryArena *arena, U8 *base, MemoryIndex size)
{
    arena->base = base;
    arena->size = size;
    arena->used = 0;
}

#define PUSH_STRUCT(arena, type) (type *)PushSize_(arena, sizeof(type))
#define PUSH_ARRAY(arena, count, type) (type *)PushSize_(arena, (count)*sizeof(type)) 
void *
PushSize_(MemoryArena *arena, MemoryIndex size)
{
    ASSERT((arena->used + size) <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    
    return result;
}

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
