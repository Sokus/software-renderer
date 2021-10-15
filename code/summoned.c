#include "base.h"
#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

#include "summoned.h"

#include "summoned_hash.c"

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
        
        for(int y = screen_min_y; y < screen_max_y; ++y)
        {
            dest = dest_row;
            
            for(int x = screen_min_x; x < screen_max_x; ++x)
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
DrawRectangleEx(OffscreenBuffer *buffer,
                F32 min_x, F32 min_y, F32 max_x, F32 max_y,
                F32 fill_r, F32 fill_g, F32 fill_b, F32 fill_a,
                UIBorder border_style, F32 border_width,
                F32 border_r, F32 border_g, F32 border_b, F32 border_a)
{
    F32 width = max_x - min_x;
    F32 height = max_y - min_y;
    
    // TODO(sokus): Figure out if those functions should even work on floats.
    if(width > 0.0f && height > 0.0f)
    {
        F32 inside_border = (border_style == UIBorder_Centered ? border_width/2 :
                             border_style == UIBorder_Inner ? border_width : 0);
        F32 outside_border = (border_style == UIBorder_Centered ? border_width/2 :
                              border_style == UIBorder_Outer ? border_width : 0);
		
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
    
    for(int y = min_y; y < max_y; ++y)
    {
        U32 *dest = (U32 *)dest_row;
        U32 *source = source_row;
        
        for(int x = min_x; x < max_x; ++x)
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
    for(size_t index = 0; index < source_count; ++index)
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

Rect RectRel(Rect parent,
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

Rect RectAbs(F32 pos_x, F32 pos_y, F32 width, F32 height)
{
    Rect result;
    result.x0 = pos_x;
    result.y0 = pos_y;
    result.x1 = pos_x + width;
    result.y1 = pos_y + height;
    return result;
}

V2 RectCenter(Rect rect)
{
    V2 result;
    result.x = (rect.x0 + rect.x1) / 2.0f;
    result.y = (rect.y0 + rect.y1) / 2.0f;
    return result;
}

bool RectanglesOverlap(Rect a, Rect b)
{
    if(a.x0 == a.x1 || a.y0 == a.y1 || b.x0 == b.x1 || b.y0 == b.y1)
        return false;
    
    // NOTE(sokus): I think those checks will break, if we 
    // have rectangle with negative width or height.
    if(a.x0 >= b.x1 || b.x0 >= a.x1)
        return false;
    
    if(a.y0 >= b.y1 || b.y0 >= a.y1)
        return false;
    
    return true;
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

void InitializeInput(Input *input)
{
    for(int key_idx = 0; key_idx < Input_COUNT; ++key_idx)
    {
        input->keys_down_duration[key_idx] = -1.0f;
    }
}

void InputUpdate(Input *input)
{
    MEMORY_COPY(input->keys_down_duration_previous,
                input->keys_down_duration,
                sizeof(input->keys_down_duration));
    
    for(int key_idx = 0; key_idx < Input_COUNT; ++key_idx)
    {
        F32 old_duration = input->keys_down_duration[key_idx];
        F32 new_duration = (input->keys_down[key_idx] ?
                            (old_duration >= 0.0f ?
                             old_duration + input->time_dt : 0.0f) :
                            -1.0f);
        
        input->keys_down_duration[key_idx] = new_duration;
    }
}

bool IsDown(Input *input, int key_index)
{
    ASSERT(key_index >= 0 && key_index < Input_COUNT);
    bool result = input->keys_down[key_index];
    return result;
} 

bool WasDown(Input *input, int key_index)
{
    ASSERT(key_index >= 0 && key_index < Input_COUNT);
    bool result = input->keys_down_duration_previous[key_index] >= 0.0f;
    return result;
}

bool Pressed(Input *input, int key_index)
{
    ASSERT(key_index >= 0 && key_index < Input_COUNT);
    bool result = IsDown(input, key_index) && !WasDown(input, key_index);
    return result;
}

bool KeyRepeat(UIState *ui, int key_index)
{
    ASSERT(key_index >= 0 && key_index < Input_COUNT);
    Input *input = ui->input;
    bool result = false;
    if(IsDown(input, key_index))
    {
        int repeat_count_previous = (int)(input->keys_down_duration_previous[key_index]
                                          / ui->key_repeat_interval);
        int repeat_count_current = (int)(input->keys_down_duration[key_index]
                                         / ui->key_repeat_interval);
        if(repeat_count_previous != repeat_count_current)
            result = true;
    }
    return result;
}

uint GetID(UIState *ui, char *str)
{
    uint seed = 0;
    if(ui->id_stack_size > 0)
        seed = ui->id_stack[ui->id_stack_size - 1];
    uint result = HashStr(str, 0, seed);
    return result;
}

void PushID(UIState *ui, uint id)
{
    ASSERT(ui->id_stack_size < ARRAY_COUNT(ui->id_stack));
    ui->id_stack[ui->id_stack_size++] = id;
}

void PopID(UIState *ui)
{
    ASSERT(ui->id_stack_size > 0);
    --ui->id_stack_size;
}

UIWindow *GetWindow(UIState *ui, uint id)
{
    uint windows_max = ARRAY_COUNT(ui->windows);
    ASSERT(IsPowerOfTwo(windows_max));
    uint idx = id & (windows_max - 1);
    UIWindow *result = 0;
    for(uint iteration = 0;
        iteration < windows_max;
        ++iteration)
    {
        UIWindow *window = ui->windows + idx;
        if(window->was_active && window->id == id)
        {
            result = window;
            break;
        }
        if(++idx >= windows_max)
            idx = 0;
    }
    return result;
}

UIWindow *CreateWindow(UIState *ui, uint id)
{
    uint windows_max = ARRAY_COUNT(ui->windows);
    ASSERT(IsPowerOfTwo(windows_max));
    uint idx = id & (windows_max - 1);
    UIWindow *result = 0;
    for(uint iteration = 0;
        iteration < windows_max;
        ++iteration)
    {
        UIWindow *window = ui->windows + idx;
        if(!window->was_active && !window->active)
        {
            result = window;
            break;
        }
        if(++idx >= windows_max)
            idx = 0;
    }
    
    ASSERT(result);
    result->id = id;
    return result;
}

void BeginWindow(UIState *ui, uint id)
{
    UIWindow *window = GetWindow(ui, id);
    bool window_created = !window;
    if(window_created)
        window = CreateWindow(ui, id);
    window->active = true;
    window->parent = ui->current_window;
    ui->current_window = window;
    PushID(ui, id);
}

void EndWindow(UIState *ui)
{
    UIWindow *window = ui->current_window;
    ASSERT(window != 0);
    ui->current_window = ui->current_window->parent;
    PopID(ui);
}

bool WasOpen(UIState *ui, uint id)
{
    bool result = false;
    if(ui->was_open_stack_size > 0)
    {
        for(size_t idx = 0; idx < ui->was_open_stack_size; ++idx)
        {
            if(ui->was_open_stack[idx] == id)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

uint WasTopmostOpen(UIState *ui)
{
    uint result = 0;
    if(ui->was_open_stack_size > 0)
    {
        result = ui->was_open_stack[ui->was_open_stack_size - 1];
    }
    return result;
}

void PushOpen(UIState *ui, uint id)
{
    ASSERT(ui->open_stack_size < ARRAY_COUNT(ui->open_stack));
    ui->open_stack[ui->open_stack_size++] = id;
}

void PopOpen(UIState *ui)
{
    ASSERT(ui->open_stack_size > 0);
    --ui->open_stack_size;
}

void NavLogic(UIState *ui, uint id, Rect rect)
{
    UIWindow *current_window = ui->current_window;
    ASSERT(current_window != 0);
    
    // NOTE(sokus): Nav logic
    bool set_nav_id_and_rect = false;
    if(current_window->last_nav_id == 0)
    {
        if(current_window->new_nav_id == 0)
            set_nav_id_and_rect = true;
    }
    else
    {
        if(current_window->last_nav_id == id)
        {
            set_nav_id_and_rect = true;
        }
        else
        {
            if(ui->was_topmost_open_id == current_window->id)
            {
                Rect *nav_clip_rects = current_window->nav_clip_rects;
                for(uint direction_idx = 0;
                    direction_idx < ARRAY_COUNT(current_window->nav_clip_rects);
                    ++direction_idx)
                {
                    if(RectanglesOverlap(rect, nav_clip_rects[direction_idx]))
                    {
                        V2 nav_rect_pos = RectCenter(current_window->last_nav_rect);
                        V2 element_rect_pos = RectCenter(rect);
                        V2 offset = SubtractV2(nav_rect_pos, element_rect_pos);
                        F32 distance = V2LengthSquared(offset);
                        
                        NavCandidate *current_nav_candidate = ui->nav_candidates + direction_idx;
                        if(current_nav_candidate->id == 0
                           || distance <= current_nav_candidate->distance)
                        {
                            current_nav_candidate->id = id;
                            current_nav_candidate->rect = rect;
                            current_nav_candidate->distance = distance;
                        }
                    }
                }
            }
        }
    }
    
    if(set_nav_id_and_rect)
    {
        current_window->new_nav_id = id;
        current_window->new_nav_rect = rect;
    }
    
}

bool BeginMenu(UIState *ui, char *name, Rect rect)
{
    UIWindow *current_window = ui->current_window;
    ASSERT(current_window != 0);
    uint id = GetID(ui, name);
    
    NavLogic(ui, id, rect);
    
    bool focused = current_window->last_nav_id == id;
    bool pressed = (focused
                    && current_window->id == ui->was_topmost_open_id
                    && Pressed(ui->input, Input_ActionDown));
    
    bool is_open = (WasOpen(ui, id) || pressed);
    if(is_open)
        PushOpen(ui, id);
    
    // render
    UIColor fill_color_idx = (is_open ? UIColor_FillActive :
                              focused ? UIColor_FillFocused :
                              UIColor_FillDefault);
    UIColor border_color_idx = (is_open ? UIColor_BorderActive :
                                focused ? UIColor_BorderFocused :
                                UIColor_BorderDefault);
    V4 fill_color = ui->colors[fill_color_idx];
    V4 border_color = ui->colors[border_color_idx];
    DrawRectangleEx(ui->offscreen_buffer,
                    rect.x0, rect.y0, rect.x1, rect.y1,
                    fill_color.r, fill_color.g, fill_color.b, fill_color.a,
                    ui->border_type, ui->border_width,
                    border_color.r, border_color.g, border_color.b, border_color.a);
    
    BeginWindow(ui, id);
    
    return is_open;
}

bool Button(UIState *ui, char *name, Rect rect)
{
    UIWindow *current_window = ui->current_window;
    ASSERT(current_window != 0);
    uint id = GetID(ui, name);
    
    NavLogic(ui, id, rect);
    
    bool focused = current_window->last_nav_id == id;
    bool pressed = (focused
                    && current_window->id == ui->was_topmost_open_id
                    && Pressed(ui->input, Input_ActionDown));
    
    UIColor fill_color_idx = (pressed  ? UIColor_FillActive :
                              focused ? UIColor_FillFocused :
                              UIColor_FillDefault);
    UIColor border_color_idx = (pressed ? UIColor_BorderActive :
                                focused ? UIColor_BorderFocused :
                                UIColor_BorderDefault);
    
    V4 fill_color = ui->colors[fill_color_idx];
    V4 border_color = ui->colors[border_color_idx];
    DrawRectangleEx(ui->offscreen_buffer,
                    rect.x0, rect.y0, rect.x1, rect.y1,
                    fill_color.r, fill_color.g, fill_color.b, fill_color.a,
                    ui->border_type, ui->border_width,
                    border_color.r, border_color.g, border_color.b, border_color.a);
    
    return pressed;
}

void EndMenu(UIState *ui)
{
    EndWindow(ui);
}

void BeginFrame(UIState *ui)
{
    UINavDir nav_dir = (KeyRepeat(ui, Input_DPadUp)    ? UINavDir_Up    :
                        KeyRepeat(ui, Input_DPadDown)  ? UINavDir_Down  :
                        KeyRepeat(ui, Input_DPadLeft)  ? UINavDir_Left  :
                        KeyRepeat(ui, Input_DPadRight) ? UINavDir_Right : UINavDir_None);
    if(nav_dir)
    {
        UIWindow *window = GetWindow(ui, ui->was_topmost_open_id);
        NavCandidate *nav_candidate = ui->nav_candidates + (nav_dir - 1);
        
        if(nav_candidate->id != 0)
        {
            window->new_nav_id = nav_candidate->id;
            window->new_nav_rect = nav_candidate->rect;
        }
    }
    
    ui->current_window = 0;
    MEMORY_COPY(ui->was_open_stack, ui->open_stack, sizeof(uint)*ui->open_stack_size);
    ui->was_open_stack_size = ui->open_stack_size;
    ui->open_stack_size = 0;
    
    UIWindow *window = ui->windows;
    for(uint idx = 0; idx < ARRAY_COUNT(ui->windows); ++idx)
    {
        window->was_active = window->active;
        window->active = false;
        window->last_nav_id = window->new_nav_id;
        window->new_nav_id = 0;
        window->last_nav_rect = window->new_nav_rect;
        ++window;
    }
    
    ui->was_topmost_open_id = WasTopmostOpen(ui);
    
    NavCandidate *nav_candidate = ui->nav_candidates;
    for(uint idx = 0; idx < ARRAY_COUNT(ui->nav_candidates); ++idx)
    {
        nav_candidate->id = 0;
    }
    
    uint window_id = GetID(ui, "root_window");
    BeginWindow(ui, window_id);
    PushOpen(ui, window_id);
    
}

void EndFrame(UIState *ui)
{
    ASSERT(ui->open_stack_size == 1);
    
    if(ui->was_open_stack_size > 0)
    {
        
        UIWindow *window = GetWindow(ui, ui->was_topmost_open_id);
        
        F32 buffer_w = (F32)ui->offscreen_buffer->width;
        F32 buffer_h = (F32)ui->offscreen_buffer->height;
        
        Rect *nav_clip_rects = window->nav_clip_rects;
        Rect last_nav_rect = window->last_nav_rect;
        for(uint idx = 0; idx < ARRAY_COUNT(window->nav_clip_rects); ++idx)
        {
            nav_clip_rects[idx].x0 = CLAMP(0, last_nav_rect.x0, buffer_w);
            nav_clip_rects[idx].y0 = CLAMP(0, last_nav_rect.y0, buffer_h);
            nav_clip_rects[idx].x1 = CLAMP(0, last_nav_rect.x1, buffer_w);
            nav_clip_rects[idx].y1 = CLAMP(0, last_nav_rect.y1, buffer_h);
        }
        
        nav_clip_rects[UINavDir_Up - 1].y0 = nav_clip_rects[UINavDir_Up - 1].y1;
        nav_clip_rects[UINavDir_Up - 1].y1 = buffer_h - 1.0f;
        
        nav_clip_rects[UINavDir_Left - 1].x1 = nav_clip_rects[UINavDir_Left - 1].x0;
        nav_clip_rects[UINavDir_Left - 1].x0 = 0;
        
        nav_clip_rects[UINavDir_Down - 1].y1 = nav_clip_rects[UINavDir_Down - 1].y0;
        nav_clip_rects[UINavDir_Down - 1].y0 = 0.0f;
        
        nav_clip_rects[UINavDir_Right - 1].x0 = nav_clip_rects[UINavDir_Right - 1].x1;
        nav_clip_rects[UINavDir_Right - 1].x1 = buffer_w;
        
        for(uint idx = 0; idx < ARRAY_COUNT(window->nav_clip_rects); ++idx)
        {
            DrawRectangleEx(ui->offscreen_buffer,
                            nav_clip_rects[idx].x0,
                            nav_clip_rects[idx].y0,
                            nav_clip_rects[idx].x1,
                            nav_clip_rects[idx].y1,
                            0, 0, 0, 0,
                            UIBorder_Inner, 1,
                            0.5f, 0.5f, 1.0f, 0.5f);
        }
        
    }
    
    if(Pressed(ui->input, Input_ActionRight) && ui->open_stack_size > 1)
        PopOpen(ui);
    
    EndWindow(ui);
}

void GameUpdateAndRender(GameMemory *memory, Input *input, OffscreenBuffer *buffer, FontPack *font_pack)
{
    UNUSED(font_pack);
    
    GameState *game_state = (GameState *)memory->permanent_storage;
    UIState *ui = &game_state->ui;
    
    if(!(memory->is_initialized))
    {
        InitializeArena(&game_state->arena,
                        (U8 *)memory->permanent_storage + sizeof(GameState),
                        0);
        
        InitializeInput(input);
        
        ui->key_repeat_interval = 0.25f;
        
        ASSERT(IsPowerOfTwo(ARRAY_COUNT(ui->windows)));
        ui->colors[UIColor_FillDefault]   = V4F32(0.125f, 0.133f, 0.144f, 0.6f);
        ui->colors[UIColor_FillFocused]   = V4F32(0.125f, 0.133f, 0.5f,   0.6f);
        ui->colors[UIColor_FillActive]    = V4F32(0.125f, 0.500f, 0.144f, 0.6f);
        ui->colors[UIColor_BorderDefault] = V4F32(0.203f, 0.211f, 0.222f, 0.6f);
        ui->colors[UIColor_BorderFocused] = V4F32(0.203f, 0.211f, 0.6f,   0.6f);
        ui->colors[UIColor_BorderActive]  = V4F32(0.203f, 0.6f,   0.222f, 0.6f);
        ui->colors[UIColor_Text]          = V4F32(0.6f,   0.6f,   0.6f,   0.6f);
        
        ui->border_type = UIBorder_Inner;
        ui->border_width = 4;
        
        memory->is_initialized = true;
    }
    
    InputUpdate(input);
    game_state->time += input->time_dt;
    ui->offscreen_buffer = buffer;
    ui->input = input;
    
    MemoryArena transient_arena;
    InitializeArena(&transient_arena,
                    (U8 *)memory->transient_storage,
                    MEGABYTES(1));
    
    DrawRectangle(buffer, 0, 0, (F32)buffer->width, (F32)buffer->height, 0, 0, 0, 1);
    
    BeginFrame(ui);
    
    Button(ui, "a--", RectAbs(50, 140, 100, 40));
    Button(ui, "b--", RectAbs(50, 95, 100, 40));
    Button(ui, "c--", RectAbs(50, 50, 100, 40));
    
    Button(ui, "-a-", RectAbs(155, 145, 100, 40));
    Button(ui, "-b-", RectAbs(155, 100, 100, 40));
    Button(ui, "-c-", RectAbs(155, 55, 100, 40));
    
    Button(ui, "--a", RectAbs(260, 150, 100, 40));
    Button(ui, "--b", RectAbs(260, 105, 100, 40));
    Button(ui, "--c", RectAbs(260, 60, 100, 40));
    
    EndFrame(ui);
    
    for(int idx = 0; idx < Input_COUNT; ++idx)
    {
        bool is_down = IsDown(input, idx);
        bool was_down = WasDown(input, idx);
        
        F32 r = !was_down && is_down;
        F32 g = is_down;
        F32 b = was_down;
        F32 a = ((r != 0 || g != 0 || b != 0) ? 1.0f : 0.0f);
        DrawRectangle(buffer, (F32)idx*20, 0, (F32)(idx+1)*20, 20, r, g, b, a);
    }
    
    ++game_state->frame_idx;
}
