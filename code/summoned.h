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

typedef enum UINavDir
{
    UINavDir_None,
    UINavDir_Up,
    UINavDir_Left,
    UINavDir_Down,
    UINavDir_Right
} UINavDir;

typedef struct UIWindow
{
    uint id;
    bool active;
    bool was_active;
    uint new_nav_id;
    uint last_nav_id;
    Rect new_nav_rect;
    Rect last_nav_rect;
    Rect nav_clip_rects[4];
    struct UIWindow* parent;
} UIWindow;

typedef struct NavCandidate
{
    uint id;
    Rect rect;
    F32 distance;
} NavCandidate;

typedef struct UIState
{
    // Data
    uint id_stack[32];
    size_t id_stack_size;
    UIWindow windows[32];
    UIWindow *current_window;
    uint open_stack[32];
    size_t open_stack_size;
    uint was_open_stack[32];
    size_t was_open_stack_size;
    uint was_topmost_open_id;
    
    // Navigation
    NavCandidate nav_candidates[4];
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

typedef struct GameState
{
    MemoryArena arena;
    UIState ui;
    
    uint frame_idx;
    F32 time;
} GameState;

#endif //SUMMONED_H
