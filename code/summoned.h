/* date = August 6th 2021 3:39 pm */

#ifndef SUMMONED_H
#define SUMMONED_H

typedef enum UIBorder
{
    UIBorder_None,
    UIBorder_Centered,
    UIBorder_Inner,
    UIBorder_Outer
} UIBorder;

typedef enum UIColor
{
    UIColor_FillDefault,
    UIColor_FillFocused,
    UIColor_FillActive,
    UIColor_BorderDefault,
    UIColor_BorderFocused,
    UIColor_BorderActive,
    UIColor_Text,
    UIColor_COUNT
} UIColor;

typedef struct UIWindow
{
    uint id;
    bool active;
    bool was_active;
    uint nav_new_id;
    uint nav_old_id;
    Rect nav_new_rect;
    Rect nav_old_rect;
    struct UIWindow* parent;
} UIWindow;

typedef struct UINavCandidate
{
    uint id;
    Rect rect;
    F32 distance;
} UINavCandidate;

typedef struct UIState
{
    // Data
    uint id_stack[32];
    size_t id_stack_size;
    UIWindow windows[32];
    UIWindow *current_window;
    uint open_stack[32];
    uint was_open_stack[32];
    size_t open_stack_size;
    size_t was_open_stack_size;
    uint topmost_open;
    uint was_topmost_open;
    
    // Navigation
    UINavCandidate nav_candidates[4];
    Rect nav_clip_regions[4];
    F32 key_repeat_interval;
    
    // Render info
    V4 colors[UIColor_COUNT];
    UIBorder border_type;
    F32 border_width;
    F32 padding;
    
    // External resources
    OffscreenBuffer *offscreen_buffer;
    Input *input;
    
} UIState;

typedef enum PrimType
{
    PrimType_None,
    PrimType_Triangle
} PrimType;

typedef struct RenderTask
{
    PrimType prim_type;
    uint texture_id;
    uint *index_data;
    uint index_count;
    struct RenderTask *next;
} RenderTask;

typedef struct RenderTaskList
{
    RenderTask *first;
    RenderTask *last;
} RenderTaskList;

typedef struct Vertex
{
    V4 pos;
    V4 col;
    V2 tex;
} Vertex;

typedef struct RenderData
{
    RenderTask tasks[512];
    uint tasks_used;
    Vertex vertices[4096];
    uint vertices_used;
    uint indices[1024];
    uint indices_used;
} RenderData;

typedef struct GameState
{
    MemoryArena arena;
    UIState ui;
    
    uint frame_idx;
    F32 time;
} GameState;

#endif //SUMMONED_H
