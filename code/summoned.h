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

typedef enum NodeFlag
{
    NodeFlag_None = 0x0,
    NodeFlag_Press = 0x1, 
    NodeFlag_Toggle = 0x4,
    NodeFlag_Open = 0x8,
    NodeFlag_AlwaysOpen = 0x10
} NodeFlag;

typedef enum NodeFocus
{
    NodeFocus_None = 0x0,
    NodeFocus_Focused = 0x1,
    NodeFocus_Pressed = 0x2,
    NodeFocus_Held = 0x4,
    NodeFocus_Released = 0x8,
    NodeFocus_Open = 0x10
} NodeFocus;

typedef enum NodeState
{
    NodeState_Default = 0x0,
    NodeState_Focused = 0x1,
    NodeState_Active = 0x2,
    NodeState_Max = 3
} NodeState;

typedef enum BorderType
{
    BORDERTYPE_NONE = 0,
    BORDERTYPE_CENTERED = 1,
    BORDERTYPE_INNER = 2,
    BORDERSTYLE_DEFAULT = 2,
    BORDERTYPE_OUTER = 3,
} BorderType;

typedef V4 Color;

typedef struct NodeStyle
{
    Color fill_color;
    Color border_color;
    BorderType border_type;
    F32 border_width;
} NodeStyle;

typedef struct NodeStylePack
{
    NodeStyle menu[NodeState_Max];
    NodeStyle button[NodeState_Max];
} NodeStylePack;

typedef struct UINode
{
    struct UINode *parent;
    struct UINode *child_first;
    struct UINode *child_last;
    struct UINode *sibling_next;
    struct UINode *sibling_prev;
    
    struct UINode *focused_child;
    uint focused_child_index;
    
    int id;
    Rect rect;
    uint frame_index;
    
    NodeFlag flags;
    NodeState states;
    NodeState state;
    
    NodeFocus focus;
    
    NodeStyle *styles;
} UINode;

typedef struct UIOpened
{
    UINode *node;
    uint frame_opened;
} UIOpened;

typedef struct UIState
{
    bool is_initialized;
    Input *input;
    
    
    uint frame_index;
    
    UINode *root;
    UINode *current_parent;
    UINode *previous_sibling;
    
    UINode nodes[256];
    UINode free_nodes;
    
    UIOpened opened_stack[32];
    uint stack_used;
    
    NodeStylePack styles;
} UIState;

typedef struct GameState
{
    uint frame_index;
    F32 time;
    
    UIState ui;
    
    MemoryArena arena;
} GameState;

#endif //SUMMONED_H
