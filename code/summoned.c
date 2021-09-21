#include "base.h"
#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

#include "summoned.h"


void
DrawRectangle(OffscreenBuffer *buffer,
              F32 real_min_x, F32 real_min_y, F32 real_max_x, F32 real_max_y,
              F32 r, F32 g, F32 b, F32 a)
{
    if(a > 0.0f)
    {
        int min_x = (int)RoundF32(real_min_x);
        int min_y = (int)RoundF32(real_min_y);
        int max_x = (int)RoundF32(real_max_x);
        int max_y = (int)RoundF32(real_max_y);
        
        int screen_width = buffer->width;
        int screen_height = buffer->height;
        
        int screen_min_x = CLAMP_BOT(min_x, 0);
        int screen_min_y = CLAMP_BOT(screen_height - max_y - 1, 0);
        int screen_max_x = CLAMP_TOP(max_x, screen_width);
        int screen_max_y = CLAMP_TOP(screen_height - min_y - 1, screen_height);
        
        U8 *dest_row = (U8 *)(buffer->memory
                              + screen_min_y*buffer->pitch
                              + screen_min_x*buffer->bytes_per_pixel);
        U8 *dest;
        
        for(int y = screen_min_y;
            y < screen_max_y;
            ++y)
        {
            dest = dest_row;
            
            for(int x = screen_min_x;
                x < screen_max_x;
                ++x)
            {
                *dest = (U8)((a*b*255.0f) + ((F32)(*dest)*(1.0f-a)));
                ++dest;
                
                *dest = (U8)((a*g*255.0f) + ((F32)(*dest)*(1.0f-a)));
                ++dest;
                
                *dest = (U8)((a*r*255.0f) + ((F32)(*dest)*(1.0f-a)));
                ++dest;
                
                ++dest;
            }
            
            dest_row += buffer->pitch;
        }
    }
}

void
DrawRectangleExplicit(OffscreenBuffer *buffer,
                      F32 min_x, F32 min_y, F32 max_x, F32 max_y,
                      F32 fill_r, F32 fill_g, F32 fill_b, F32 fill_a,
                      BorderType border_style, F32 border_width,
                      F32 border_r, F32 border_g, F32 border_b, F32 border_a)
{
    F32 width = max_x - min_x;
    F32 height = max_y - min_y;
    
    // TODO(sokus): Figure out if those functions should even work on floats.
    if(width > 0.0f && height > 0.0f)
    {
        F32 inside_border = (border_style == BorderType_Centered ? border_width/2 :
                             border_style == BorderType_Inner ? border_width : 0);
        F32 outside_border = (border_style == BorderType_Centered ? border_width/2 :
                              border_style == BorderType_Outer ? border_width : 0);
        
        if(fill_a > 0.0f
           && width - 2*inside_border > 0.0f
           && height - 2*inside_border > 0.0f)
        {
            DrawRectangle(buffer,
                          min_x+inside_border, min_y+inside_border, max_x-inside_border, max_y-inside_border,
                          fill_r, fill_g, fill_b, fill_a);
        }
        
        if(border_a > 0.0f
           && (inside_border > 0.0f || outside_border > 0.0f))
        {
            DrawRectangle(buffer,
                          min_x-outside_border, min_y-outside_border, max_x+outside_border, min_y+inside_border,
                          border_r, border_g, border_b, border_a);
            DrawRectangle(buffer,
                          min_x-outside_border, min_y+inside_border, min_x+inside_border, max_y-inside_border,
                          border_r, border_g, border_b, border_a);
            DrawRectangle(buffer,
                          min_x-outside_border, max_y-inside_border, max_x+outside_border, max_y+outside_border,
                          border_r, border_g, border_b, border_a);
            DrawRectangle(buffer,
                          max_x-inside_border, min_y+inside_border, max_x+outside_border, max_y-inside_border,
                          border_r, border_g, border_b, border_a);
        }
        
    }
}

void
DrawBitmap(OffscreenBuffer *buffer,
           U32 *bitmap_data, int bitmap_w, int bitmap_h,
           F32 part_x, F32 part_y, F32 part_w, F32 part_h,
           F32 pos_x, F32 pos_y,
           F32 r, F32 g, F32 b, F32 a)
{
    UNUSED(bitmap_h);
    
    bitmap_data = bitmap_data + (int)part_y*(int)bitmap_w + (int)part_x;
    
    int min_x = (int)RoundF32(pos_x);
    int min_y = (int)RoundF32(pos_y);
    int max_x = (int)RoundF32(pos_x + part_w);
    int max_y = (int)RoundF32(pos_y + part_h);;
    
    int source_offset_x = 0;
    if(min_x < 0)
    {
        source_offset_x = -min_x;
        min_x = 0;
    }
    
    int source_offset_y = 0;
    if(min_y < 0)
    {
        source_offset_y = -min_y;
        min_y = 0;;
    }
    
    max_x = CLAMP_TOP(max_x, buffer->width);
    max_y = CLAMP_TOP(max_y, buffer->height);
    
    U32 *source_row = bitmap_data + source_offset_y*(int)bitmap_w + source_offset_x;
    U8 *dest_row = (U8 *)buffer->memory + min_y*buffer->pitch + min_x*buffer->bytes_per_pixel;
    
    for(int y = min_y;
        y < max_y;
        ++y)
    {
        U32 *dest = (U32 *)dest_row;
        U32 *source = source_row;
        
        for(int x = min_x;
            x < max_x;
            ++x)
        {
            F32 new_a = (F32)(*source >> 24) / 255.0f * a;
            
            F32 source_r = (F32)((*source >> 16) & 0xFF) * r;
            F32 source_g = (F32)((*source >> 8) & 0xFF) * g;
            F32 source_b = (F32)((*source >> 0) & 0xFF) * b;
            
            F32 dest_r = (F32)((*dest >> 16) & 0xFF);
            F32 dest_g = (F32)((*dest >> 8) & 0xFF);
            F32 dest_b = (F32)((*dest >> 0) & 0xFF);
            
            F32 new_r = (1.0f-new_a)*dest_r + new_a*source_r;
            F32 new_g = (1.0f-new_a)*dest_g + new_a*source_g;
            F32 new_b = (1.0f-new_a)*dest_b + new_a*source_b;
            
            *dest = (((U32)(new_r + 0.5f) << 16) |
                     ((U32)(new_g + 0.5f) << 8) |
                     ((U32)(new_b + 0.5f) << 0));
            ++dest;
            ++source;
        }
        
        dest_row += buffer->pitch;
        source_row += bitmap_w;
    }
}

void
DrawAsci(OffscreenBuffer *buffer, Font *font,
         U8 character,
         F32 pos_x, F32 pos_y,
         F32 r, F32 g, F32 b, F32 a)
{
    int glyph_pos_x = character % font->glyphs_per_row;
    int glyph_pos_y = character / font->glyphs_per_col;
    
    DrawBitmap(buffer,
               font->data,
               font->w,
               font->h,
               (F32)(glyph_pos_x*font->glyph_w),
               (F32)(glyph_pos_y*font->glyph_h),
               (F32)font->glyph_w,
               (F32)font->glyph_h,
               pos_x, pos_y,
               r, g, b, a);
}

void
DrawString(OffscreenBuffer *buffer, Font *font,
           char *source, size_t source_count,
           F32 pos_x, F32 pos_y,
           F32 r, F32 g, F32 b, F32 a)
{
    for(size_t index = 0;
        index < source_count;
        index++)
    {
        F32 offset_x = (F32)((int)index*font->glyph_w);
        
        DrawAsci(buffer, font, (U8)(source[index]), pos_x+offset_x, pos_y, r, g, b, a);
    }
}


OffscreenBuffer
OffscreenBufferTrim(OffscreenBuffer *screen_buffer, int min_x, int min_y, int max_x, int max_y)
{
    int min_x_clamped = CLAMP(0, min_x, screen_buffer->width);
    int min_y_clamped = CLAMP(0, min_y, screen_buffer->height);
    int max_x_clamped = CLAMP(min_x_clamped, max_x, screen_buffer->width);
    int max_y_clamped = CLAMP(min_y_clamped, max_y, screen_buffer->height);
    
    OffscreenBuffer result = {0};
    result.memory = (void *)((U8 *)screen_buffer->memory
                             + (min_y_clamped*screen_buffer->pitch)
                             + min_x_clamped*screen_buffer->bytes_per_pixel);
    result.width = max_x_clamped - min_x_clamped;
    result.height = max_y_clamped - min_y_clamped;
    result.pitch = screen_buffer->pitch;
    result.bytes_per_pixel = screen_buffer->bytes_per_pixel;
    
    return result;
}

Rect UIRect(Rect parent,
            F32 x0_pct, F32 y0_pct, F32 x1_pct, F32 y1_pct,
            F32 top, F32 left, F32 bottom, F32 right)
{
    F32 parent_width = parent.x1 - parent.x0;
    F32 parent_height = parent.y1 - parent.y0;
    Rect result;
    result.x0 = parent.x0 + x0_pct*parent_width + left;
    result.y0 = parent.y0 + y0_pct*parent_height + bottom;
    result.x1 = parent.x0 + x1_pct*parent_width - right;
    result.y1 = parent.y0 + y1_pct*parent_height - top;
    return result;
}

Rect UIAbsoluteRect(F32 pos_x, F32 pos_y, F32 width, F32 height)
{
    Rect result;
    result.x0 = pos_x;
    result.y0 = pos_y;
    result.x1 = pos_x + width;
    result.y1 = pos_y + height;
    return result;
}

void String8ListPlaceInRect(String8List *list, Rect rect, int glyph_w, int glyph_h, F32 line_jump, bool start_at_bottom)
{
    F32 current_x = 0;
    F32 current_y = 0;
    
    F32 rect_width = rect.x1 - rect.x0;
    F32 rect_height = rect.y1 - rect.y0;
    
    for(String8Node *node = list->first;
        node != 0;
        node = node->next)
    {
        if(current_x > 0
           && current_x + (F32)((int)node->string.size * glyph_w) > rect_width)
        {
            current_x = 0;
            current_y += line_jump;
        }
        
        node->style.pos_x = current_x;
        node->style.pos_y = current_y;
        
        if(node->style.flags & STYLEFLAG_LINE_END)
        {
            current_x = 0;
            current_y += line_jump;
        }
        else
        {
            current_x += (F32)(((int)node->string.size + 1) * glyph_w);
        }
    }
    
    if(start_at_bottom)
    {
        for(String8Node *node = list->first;
            node != 0;
            node = node->next)
        {
            F32 offset = -current_y - (F32)glyph_h + rect_height;
            node->style.pos_y += offset;
        }
    }
}

void DrawString8List(OffscreenBuffer *buffer, Font *font, String8List *list)
{
    for(String8Node *node = list->first;
        node != 0;
        node = node->next)
    {
        F32 pos_x = node->style.pos_x;
        F32 pos_y = node->style.pos_y;
        
        F32 node_width_in_pixels = (F32)((int)node->string.size * font->glyph_w);
        
        bool fits_in_buffer = (pos_y + (F32)font->glyph_h >= 0
                               && pos_y < (F32)buffer->height
                               && pos_x + node_width_in_pixels >= 0
                               && pos_x < (F32)buffer->width);
        if(fits_in_buffer)
        {
            DrawString(buffer, font,
                       (char *)(node->string.str),
                       node->string.size,
                       pos_x, pos_y,
                       node->style.r, node->style.g, node->style.b, node->style.a);
        }
    }
}

bool ButtonIsDownExplicit(Input *input, size_t button_index)
{
    ASSERT(button_index >= 0
           && button_index < ARRAY_COUNT(((ControllerInput *)0)->buttons));
    bool result = false;
    for(size_t controller_index = 0;
        controller_index < ARRAY_COUNT(input->controllers);
        ++controller_index)
    {
        ControllerInput *controller = &input->controllers[controller_index];
        if(controller->is_connected)
        {
            if(controller->buttons[button_index].ended_down)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

bool ButtonWasDownExplicit(Input *input, size_t button_index)
{
    ASSERT(button_index >= 0
           && button_index < ARRAY_COUNT(((ControllerInput *)0)->buttons));
    bool result = false;
    for(size_t controller_index = 0;
        controller_index < ARRAY_COUNT(input->controllers);
        ++controller_index)
    {
        ControllerInput *controller = &input->controllers[controller_index];
        if(controller->is_connected)
        {
            ButtonState button = controller->buttons[button_index];
            bool button_changed_state = (button.half_transition_count % 2) != 0;
            if(button.ended_down != button_changed_state)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

#define BUTTON_INDEX(button_name) ((OFFSET_OF(ControllerInput, button_name)\
- OFFSET_OF(ControllerInput, buttons))/sizeof(ButtonState))

#define BUTTON_IS_DOWN(input_ptr, button_name) ButtonIsDownExplicit(input_ptr, BUTTON_INDEX(button_name))
#define BUTTON_WAS_DOWN(input_ptr, button_name) ButtonWasDownExplicit(input_ptr, BUTTON_INDEX(button_name))

UINode *UINodeCreate(UIState *ui)
{
    UINode *result = ui->free_nodes.child_first;
    ASSERT(result);
    DLL_REMOVE_EXPLICIT(ui->free_nodes.child_first,
                        ui->free_nodes.child_last,
                        result,
                        sibling_next, sibling_prev);
    *result = (UINode){0};
    return result;
}

void UINodeFree(UIState *ui, UINode *node)
{
    UINode *child = node->child_first;
    UINode *next;
    
    while(child != 0)
    {
        next = child->sibling_next;
        UINodeFree(ui, child);
        child = next;
    }
    
    DLL_PUSH_FRONT_EXPLICIT(ui->free_nodes.child_first,
                            ui->free_nodes.child_last,
                            node,
                            sibling_next,
                            sibling_prev);
}

UINode *UIGetCurrentParent(UIState *ui)
{
    UINode *result = ui->current_parent;
    return result;
}

void UISetCurrentParent(UIState *ui, UINode *node)
{
    ui->current_parent = node;
}

void UISetPreviousSibling(UIState *ui, UINode *node)
{
    ui->previous_sibling = node;
}

void UIInit(UIState *ui)
{
    *ui = (UIState){0};
    
    for(size_t node_index = 0;
        node_index < ARRAY_COUNT(ui->nodes);
        ++node_index)
    {
        DLL_PUSH_BACK_EXPLICIT(ui->free_nodes.child_first,
                               ui->free_nodes.child_last,
                               ui->nodes + node_index,
                               sibling_next,
                               sibling_prev);
    }
    
    NodeStyle button_default =
    {
        .fill_color = {{ 0.125f, 0.133f, 0.144f, 0.6f }},
        .border_color = {{ 0.203f, 0.211f, 0.222f, 0.6f }},
        .border_type = BorderType_Inner,
        .border_width = 4,
    };
    NodeStyle button_focused =
    {
        .fill_color = {{ 0.125f, 0.133f, 0.5f, 0.6f }},
        .border_color = {{ 0.203f, 0.211f, 0.6f, 0.6f }},
        .border_type = BorderType_Inner,
        .border_width = 4,
    };
    
    NodeStyle button_active =
    {
        .fill_color = {{ 0.125f, 0.5f, 0.144f, 0.6f }},
        .border_color = {{ 0.203f, 0.6f, 0.222f, 0.6f }},
        .border_type = BorderType_Inner,
        .border_width = 4,
    };
    
    NodeStyle menu_default =
    {
        .fill_color = {{ 0.325f, 0.133f, 0.144f, 0.6f }},
        .border_color = {{ 0.503f, 0.211f, 0.222f, 0.6f }},
        .border_type = BorderType_Inner,
        .border_width = 4,
    };
    NodeStyle menu_focused =
    {
        .fill_color = {{ 0.325f, 0.133f, 0.5f, 0.6f }},
        .border_color = {{ 0.503f, 0.211f, 0.6f, 0.6f }},
        .border_type = BorderType_Inner,
        .border_width = 4,
    };
    
    NodeStyle menu_active =
    {
        .fill_color = {{ 0.325f, 0.5f, 0.144f, 0.6f }},
        .border_color = {{ 0.503f, 0.6f, 0.222f, 0.6f }},
        .border_type = BorderType_Inner,
        .border_width = 4,
    };
    
    ui->styles.menu[NodeState_Default] = menu_default;
    ui->styles.menu[NodeState_Focused] = menu_focused;
    ui->styles.menu[NodeState_Active] = menu_active;
    
    ui->styles.button[NodeState_Default] = button_default;
    ui->styles.button[NodeState_Focused] = button_focused;
    ui->styles.button[NodeState_Active] = button_active;
}

void UIRenderNode(OffscreenBuffer *buffer, UINode *node)
{
    if(node->styles)
    {
        Rect rect = node->rect;
        
        NodeStyle *style = node->styles + node->state;
        Color fill_color = style->fill_color;
        Color border_color = style->border_color;
        
        DrawRectangleExplicit(buffer,
                              rect.x0, rect.y0, rect.x1, rect.y1,
                              fill_color.r, fill_color.g, fill_color.b, fill_color.a,
                              style->border_type, style->border_width,
                              border_color.r, border_color.g, border_color.b, border_color.a);
    }
    
    for(UINode *child = node->child_first;
        child != 0;
        child = child->sibling_next)
    {
        UIRenderNode(buffer, child);
    }
}

void UIRender(OffscreenBuffer *buffer, UIState *ui)
{
    UIRenderNode(buffer, ui->root);
}

void UIFrameStart(UIState *ui, uint frame_index)
{
    if(ui->root == 0)
    {
        ui->root = UINodeCreate(ui);
    }
    
    UISetCurrentParent(ui, ui->root);
    UISetPreviousSibling(ui, 0);
    
    ui->frame_index = frame_index;
}

UINode *UIGetNextMenuItem(UINode *node)
{
    UINode *result = node->focused_child;
    if(result == 0) result = node->child_first;
    
    UINode *next = 0;
    if(result)
    {
        next = result->sibling_next;
        while(next && next->flags & NodeFlag_Menu)
        {
            next = next->sibling_next;
        }
    }
    
    if(next)
    {
        result = next;
    }
    
    return result;
}


UINode *UIGetPreviousMenuItem(UINode *node)
{
    UINode *result = node->focused_child;
    if(result == 0) result = node->child_first;
    
    UINode *prev = 0;
    if(result)
    {
        prev = result->sibling_prev;
        while(prev && prev->flags & NodeFlag_Menu)
        {
            prev = prev->sibling_prev;
        }
    }
    
    if(prev)
    {
        result = prev;
    }
    
    return result;
}

void UIFreeOutdatedInTree(UIState *ui, UINode *node)
{
    UINode *child = node->child_first;
    UINode *next;
    
    while(child != 0)
    {
        next = child->sibling_next;
        if(child->frame_index != ui->frame_index)
        {
            
            if(node->focused_child == child)
            {
                UINode *new_focused_child = UIGetNextMenuItem(node);
                if(new_focused_child == 0)
                {
                    new_focused_child = UIGetPreviousMenuItem(node);
                }
                node->focused_child = new_focused_child;
            }
            
            DLL_REMOVE_EXPLICIT(node->child_first,
                                node->child_last,
                                child,
                                sibling_next,
                                sibling_prev);
            UINodeFree(ui, child);
        }
        else
        {
            UIFreeOutdatedInTree(ui, child);
        }
        child = next;
    }
}

void
UIFrameEnd(UIState *ui)
{
    uint stack_freed = 0;
    for(uint index = 0;
        index < ui->stack_used;
        ++index)
    {
        UIOpened opened = ui->opened_stack[index];
        if(opened.node->frame_index != ui->frame_index)
        {
            ++stack_freed;
        }
        else
        {
            if(stack_freed > 0)
            {
                ui->opened_stack[index - stack_freed] = opened;
            }
        }
    }
    ui->stack_used -= stack_freed;
    
    UIFreeOutdatedInTree(ui, ui->root);
}

UINode *UIGetOrInsertNodeInTree(UIState *ui, int id)
{
    UINode *result = 0;
    for(UINode *node = ui->current_parent->child_first;
        node != 0;
        node = node->sibling_next)
    {
        if(node->id == id)
        {
            result = node;
            break;
        }
    }
    
    if(result != 0)
    {
        DLL_REMOVE_EXPLICIT(ui->current_parent->child_first,
                            ui->current_parent->child_last,
                            result,
                            sibling_next,
                            sibling_prev);
    }
    else
    {
        result = UINodeCreate(ui);
        result->id = id;
    }
    
    if(ui->previous_sibling)
    {
        result->sibling_next = ui->previous_sibling->sibling_next;
        if(result->sibling_next)
        {
            result->sibling_next->sibling_prev = result;
        }
        else
        {
            ui->current_parent->child_last = result;
        }
        
        result->sibling_prev = ui->previous_sibling;
        ui->previous_sibling->sibling_next = result;
    }
    else
    {
        DLL_PUSH_FRONT_EXPLICIT(ui->current_parent->child_first,
                                ui->current_parent->child_last,
                                result,
                                sibling_next,
                                sibling_prev);
    }
    
    result->parent = ui->current_parent;
    
    return result; 
}

void UIPushOpen(UIState *ui, UINode *node)
{
    ASSERT(ui->stack_used < ARRAY_COUNT(ui->opened_stack));
    UIOpened new_opened = { node, ui->frame_index };
    ui->opened_stack[ui->stack_used++] = new_opened;
}

void UIPopOpen(UIState *ui)
{
    ASSERT(ui->stack_used > 0);
    --ui->stack_used;
    if(ui->stack_used > 0)
    {
        ui->opened_stack[ui->stack_used].frame_opened = ui->frame_index;
    }
}

bool UIJustChangedOpen(UIState *ui)
{
    bool result = false;
    if(ui->stack_used > 0)
    {
        UIOpened last_opened = ui->opened_stack[ui->stack_used-1];
        result = (last_opened.frame_opened == ui->frame_index);
    }
    return result;
}

bool UIIsOpen(UIState *ui, UINode *node)
{
    bool result = false;
    for(uint index = 0;
        index < ui->stack_used;
        ++index)
    {
        if(ui->opened_stack[index].node == node)
        {
            result = true;
            break;
        }
    }
    return result;
}

bool UITopmostOpen(UIState *ui, UINode *node)
{
    bool result = false;
    if(ui->stack_used > 0)
    {
        if(ui->opened_stack[ui->stack_used-1].node == node)
        {
            result = true;
        }
    }
    return result;
}

bool UITopmostChild(UIState *ui, UINode *child)
{
    bool result = (ui->stack_used == 0
                   || UITopmostOpen(ui, child->parent));
    return result;
}

void UIUpdateNodeState(UIState *ui, UINode *node)
{
    bool state_focused = node->parent->focused_child == node;
    bool state_active = false;
    
    bool confirm_is_down = BUTTON_IS_DOWN(ui->input, action_down);
    bool confirm_was_down = BUTTON_WAS_DOWN(ui->input, action_down);
    
    bool confirm_pressed = confirm_is_down && !confirm_was_down;
    bool confirm_held = confirm_is_down && confirm_was_down;
    bool confirm_released = !confirm_is_down && confirm_was_down;
    
    bool focus_pressed = false;
    bool focus_held = false;
    bool focus_released = false;
    
    bool is_open = UIIsOpen(ui, node);
    
    if(node->flags & NodeFlag_Menu)
    {
        bool should_open = node->flags & NodeFlag_ShouldOpen;
        
        if(!is_open)
        {
            node->focused_child = 0;
            
            if(should_open && UITopmostChild(ui, node))
            {
                UIPushOpen(ui, node);
                is_open = true;
            }
        }
        
        if(is_open && node->focused_child == 0)
        {
            node->focused_child = node->child_first;
        }
        
        if(is_open && UITopmostOpen(ui, node))
        {
            bool quit_pressed = (BUTTON_IS_DOWN(ui->input, action_right)
                                 && !BUTTON_WAS_DOWN(ui->input, action_right));
            bool next_pressed = (BUTTON_IS_DOWN(ui->input, move_down)
                                 && !BUTTON_WAS_DOWN(ui->input, move_down));
            bool prev_pressed = (BUTTON_IS_DOWN(ui->input, move_up)
                                 && !BUTTON_WAS_DOWN(ui->input, move_up));
            
            if(quit_pressed && !should_open)
            {
                UIPopOpen(ui);
                is_open = false;
            }
            
            if(next_pressed)
            {
                node->focused_child = UIGetNextMenuItem(node);
            }
            
            if(prev_pressed)
            {
                node->focused_child = UIGetPreviousMenuItem(node);
            }
            
            state_active = is_open;
        }
    }
    
    if(node->flags & NodeFlag_Press)
    {
        if(state_focused && confirm_pressed
           && !UIJustChangedOpen(ui) && UITopmostChild(ui, node))
        {
            state_active = true;
            focus_pressed = true;
        }
    }
    else if(node->flags & NodeFlag_Hold)
    {
        if(state_focused
           && (confirm_pressed | confirm_held | confirm_released)
           && !UIJustChangedOpen(ui) && UITopmostChild(ui, node))
        {
            state_active = confirm_held;
            focus_pressed = confirm_pressed;
            focus_held = confirm_held;
            focus_released = confirm_released;
        }
    }
    else if(node->flags & NodeFlag_Toggle)
    {
        bool old_active = node->states & NodeState_Active;
        
        if(state_focused && confirm_pressed
           && !UIJustChangedOpen(ui) && UITopmostChild(ui, node))
        {
            state_active = !old_active;
        }
        
        focus_pressed = !old_active && state_active;
        focus_held = old_active && state_active;
        focus_released = old_active && !state_active;
    }
    
    SET_FLAG(node->states, NodeState_Focused, state_focused);
    SET_FLAG(node->states, NodeState_Active, state_active);
    SET_FLAG(node->focus, NodeFocus_Focused, state_focused);
    SET_FLAG(node->focus, NodeFocus_Pressed, focus_pressed);
    SET_FLAG(node->focus, NodeFocus_Held, focus_held);
    SET_FLAG(node->focus, NodeFocus_Released, focus_released);
    SET_FLAG(node->focus, NodeFocus_Open, is_open);
    
    node->state = (state_active ? NodeState_Active :
                   state_focused ? NodeState_Focused : 0);
}

void UINodeStart(UIState *ui, int id, Rect rect, NodeFlag flags, NodeStyle styles[NodeState_Max])
{
    UINode *node = UIGetOrInsertNodeInTree(ui, id);
    node->rect = rect;
    node->flags = flags;
    node->frame_index = ui->frame_index;
    node->styles = styles;
    
    UIUpdateNodeState(ui, node);
    
    UISetCurrentParent(ui, node);
    UISetPreviousSibling(ui, 0);
}

void UINodeEnd(UIState *ui)
{
    UINode *node = UIGetCurrentParent(ui);
    UISetCurrentParent(ui, node->parent);
    UISetPreviousSibling(ui, node);
}

NodeFocus UIBeginMenu(UIState *ui, int id, bool open)
{
    NodeFlag flags = 0;
    SET_FLAG(flags, NodeFlag_Menu, 1);
    SET_FLAG(flags, NodeFlag_ShouldOpen, open);
    UINodeStart(ui, id, (Rect){0}, flags, ui->styles.menu);
    UINode *node = UIGetCurrentParent(ui);
    NodeFocus result = node->focus;
    return result;
}

void UIEndMenu(UIState *ui)
{
    UINodeEnd(ui);
}

NodeFocus UIButton(UIState *ui, int id, Rect rect)
{
    UINodeStart(ui, id, rect, NodeFlag_Press, ui->styles.button);
    UINode *node = UIGetCurrentParent(ui);
    
    
    NodeFocus result = node->focus;
    UINodeEnd(ui);
    return result;
}

NodeFocus UIToggleButton(UIState *ui, int id, Rect rect)
{
    UINodeStart(ui, id, rect, NodeFlag_Toggle, ui->styles.button);
    UINode *node = UIGetCurrentParent(ui);
    NodeFocus result = node->focus;
    UINodeEnd(ui);
    return result;
}

void GameUpdateAndRender(GameMemory *memory, Input *input, OffscreenBuffer *buffer, FontPack *font_pack)
{
    UNUSED(font_pack);
    ASSERT((&input->controllers[0].terminator - &input->controllers[0].buttons[0]) ==
           ARRAY_COUNT(input->controllers[0].buttons));
    
    GameState *game_state = (GameState *)memory->permanent_storage;
    UIState *ui = &game_state->ui;
    
    if(!(memory->is_initialized))
    {
        game_state->time = 0;
        
        InitializeArena(&game_state->arena,
                        (U8 *)memory->permanent_storage + sizeof(GameState),
                        0);
        
        memory->is_initialized = true;
    }
    
    if(!ui->is_initialized)
    {
        UIInit(ui);
        ui->is_initialized = true;
    }
    
    game_state->time += input->dt_for_frame;
    ui->input = input;
    
    MemoryArena transient_arena;
    InitializeArena(&transient_arena,
                    (U8 *)memory->transient_storage,
                    MEGABYTES(1));
    
    DrawRectangle(buffer, 0, 0, (F32)buffer->width, (F32)buffer->height, 0, 0, 0, 1);
    
    UIFrameStart(ui, game_state->frame_index);
    
    F32 pad = 5;
    
    if(UIBeginMenu(ui, __LINE__, 1))
    {
        UIButton(ui, __LINE__, (Rect){0+pad, 300+pad, 200-pad, 400-pad});
        UIButton(ui, __LINE__, (Rect){0+pad, 200+pad, 200-pad, 300-pad});
        NodeFocus button0 = UIButton(ui, __LINE__, (Rect){0+pad, 100+pad, 200-pad, 200-pad});
        if(UIBeginMenu(ui, __LINE__, button0 & NodeFocus_Pressed) & NodeFocus_Open)
        {
            UIButton(ui, __LINE__, (Rect){200+pad, 300+pad, 400-pad, 400-pad});
            NodeFocus button1 = UIButton(ui, __LINE__, (Rect){200+pad, 200+pad, 400-pad, 300-pad});
            if(UIBeginMenu(ui, __LINE__, button1 & NodeFocus_Pressed) & NodeFocus_Open)
            {
                UIButton(ui, __LINE__, (Rect){400+pad, 300+pad, 600-pad, 400-pad});
                UIButton(ui, __LINE__, (Rect){400+pad, 200+pad, 600-pad, 300-pad});
                UIButton(ui, __LINE__, (Rect){400+pad, 100+pad, 600-pad, 200-pad});
                UIButton(ui, __LINE__, (Rect){400+pad, 0+pad, 600-pad, 100-pad});
            }
            UIEndMenu(ui);
            UIButton(ui, __LINE__, (Rect){200+pad, 100+pad, 400-pad, 200-pad});
            UIButton(ui, __LINE__, (Rect){200+pad, 0+pad, 400-pad, 100-pad});
        }
        UIEndMenu(ui);
        UIButton(ui, __LINE__, (Rect){0+pad, 0+pad, 200-pad, 100-pad});
    }
    UIEndMenu(ui);
    
    UIFrameEnd(ui);
    
    UIRender(buffer, ui);
    
    ++game_state->frame_index;
}
