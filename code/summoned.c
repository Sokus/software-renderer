#include "base.h"
#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

#include "summoned.h"

#include "summoned_hash.c"

void
DrawPixel(OffscreenBuffer *buffer, int x, int y, F32 r, F32 g, F32 b, F32 a)
{
    if(x >= 0 && x < buffer->width && y >= 0 && y < buffer->height
       && a > 0.0f)
    {
        U8 *dest = (U8 *)(buffer->memory
                          + buffer->pitch*(buffer->height-1) - y*buffer->pitch
                          + x*buffer->bytes_per_pixel);
        *dest = (U8)((a*b*255.0f) + ((F32)(*dest)*(1.0f-a)));
        ++dest;
        *dest = (U8)((a*g*255.0f) + ((F32)(*dest)*(1.0f-a)));
        ++dest;
        *dest = (U8)((a*r*255.0f) + ((F32)(*dest)*(1.0f-a)));
        ++dest;
    }
}

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
        if((window->was_active || window->active) && window->id == id)
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
    if(current_window->nav_old_id == 0)
    {
        if(current_window->nav_new_id == 0)
        {
            // set next nav_id if we are not focused on anything
            // and no next nav_id was set yet
            current_window->nav_new_id = id;
            current_window->nav_new_rect = rect;
        }
    }
    else
    {
        if(current_window->nav_old_id == id)
        {
            // keep the nav_id alive
            current_window->nav_new_id = id;
            current_window->nav_new_rect = rect;
        }
        else
        {
            if(current_window->id == ui->topmost_open)
            {
                UINavCandidate *nav_candidate = ui->nav_candidates;
                Rect *nav_clip_region = ui->nav_clip_regions;
                
                for(uint dir_idx = 0;
                    dir_idx < ARRAY_COUNT(ui->nav_clip_regions);
                    ++dir_idx)
                {
                    if(RectanglesOverlap(rect, *nav_clip_region))
                    {
                        V2 nav_rect_center = RectCenter(current_window->nav_old_rect);
                        V2 element_center = RectCenter(rect);
                        F32 distance = MagnitudeSq(SubtractV2(nav_rect_center, element_center));
                        if(nav_candidate->id == 0
                           || distance <= nav_candidate->distance)
                        {
                            nav_candidate->id = id;
                            nav_candidate->rect = rect;
                            nav_candidate->distance = distance;
                        }
                    }
                    
                    ++nav_candidate;
                    ++nav_clip_region;
                }
            }
        }
    }
}

bool BeginMenu(UIState *ui, char *name, Rect rect)
{
    UIWindow *current_window = ui->current_window;
    ASSERT(current_window != 0);
    uint id = GetID(ui, name);
    
    NavLogic(ui, id, rect);
    
    bool focused = current_window->nav_old_id == id;
    bool pressed = (focused
                    && current_window->id == ui->was_topmost_open
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
    
    bool focused = current_window->nav_old_id == id;
    bool pressed = (focused
                    && current_window->id == ui->was_topmost_open
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
    ui->current_window = 0;
    
    if(ui->topmost_open != 0)
    {
        UIWindow *topmost_window = GetWindow(ui, ui->topmost_open);
        if(ui->was_topmost_open == ui->topmost_open)
        {
            Input *input = ui->input;
            
            InputKey directional_keys[] = { Input_DPadUp, Input_DPadLeft, Input_DPadDown, Input_DPadRight };
            uint last_pressed_directional_key_idx = 0;
            F32 shortest_key_press_duration = -1.0f;
            
            for(uint our_key_idx = 0; our_key_idx < ARRAY_COUNT(directional_keys); ++our_key_idx)
            {
                InputKey key_idx = directional_keys[our_key_idx];
                F32 key_press_duration = input->keys_down_duration[key_idx];
                
                if(key_press_duration >= 0.0f
                   && (shortest_key_press_duration < 0.0f
                       || key_press_duration < shortest_key_press_duration))
                {
                    last_pressed_directional_key_idx = our_key_idx;
                    shortest_key_press_duration = key_press_duration;
                }
            }
            
            UINavCandidate *nav_candidate = ui->nav_candidates + last_pressed_directional_key_idx;
            if(shortest_key_press_duration >= 0.0f
               && nav_candidate->id != 0
               && KeyRepeat(ui, directional_keys[last_pressed_directional_key_idx]))
            {
                topmost_window->nav_new_id = nav_candidate->id;
                topmost_window->nav_new_rect = nav_candidate->rect;
            }
        }
        
        Rect *nav_clip_regions = ui->nav_clip_regions;
        Rect nav_new_rect = topmost_window->nav_new_rect;
        F32 buffer_w = (F32)ui->offscreen_buffer->width;
        F32 buffer_h = (F32)ui->offscreen_buffer->height;
        for(uint idx = 0; idx < ARRAY_COUNT(ui->nav_clip_regions); ++idx)
        {
            nav_clip_regions[idx].x0 = CLAMP(0, nav_new_rect.x0, buffer_w);
            nav_clip_regions[idx].y0 = CLAMP(0, nav_new_rect.y0, buffer_h);
            nav_clip_regions[idx].x1 = CLAMP(0, nav_new_rect.x1, buffer_w);
            nav_clip_regions[idx].y1 = CLAMP(0, nav_new_rect.y1, buffer_h);
        }
        
        nav_clip_regions[0].y0 = nav_clip_regions[0].y1;
        nav_clip_regions[0].y1 = buffer_h - 1.0f;
        
        nav_clip_regions[1].x1 = nav_clip_regions[1].x0;
        nav_clip_regions[1].x0 = 0.0f;
        
        nav_clip_regions[2].y1 = nav_clip_regions[2].y0;
        nav_clip_regions[2].y0 = 0.0f;
        
        nav_clip_regions[3].x0 = nav_clip_regions[3].x1;
        nav_clip_regions[3].x1 = buffer_w;
        
        for(uint idx = 0; idx < ARRAY_COUNT(ui->nav_clip_regions); ++idx)
        {
            DrawRectangleEx(ui->offscreen_buffer,
                            nav_clip_regions[idx].x0,
                            nav_clip_regions[idx].y0,
                            nav_clip_regions[idx].x1,
                            nav_clip_regions[idx].y1,
                            0, 0, 0, 0,
                            UIBorder_Inner, 1,
                            0.5f, 0.5f, 1.0f, 0.5f);
        }
    }
    
    for(uint idx = 0; idx < ARRAY_COUNT(ui->nav_candidates); ++idx)
    {
        ui->nav_candidates[idx].id = 0;
    }
    
    for(UIWindow *window_iter = ui->windows;
        window_iter < ui->windows + ARRAY_COUNT(ui->windows);
        ++window_iter)
    {
        window_iter->was_active = window_iter->active;
        window_iter->active = false;
        window_iter->nav_old_id = window_iter->nav_new_id;
        window_iter->nav_new_id = 0;
        window_iter->nav_old_rect = window_iter->nav_new_rect;
    }
    
    uint window_id = GetID(ui, "root_window");
    BeginWindow(ui, window_id);
    PushOpen(ui, window_id);
}


void EndFrame(UIState *ui)
{
    
    if(Pressed(ui->input, Input_ActionRight) && ui->open_stack_size > 1)
        PopOpen(ui);
    
    ASSERT(ui->id_stack_size == 1);
    EndWindow(ui);
    
    ui->was_topmost_open = ui->topmost_open;
    ui->topmost_open = (ui->open_stack_size > 0 ? ui->open_stack[ui->open_stack_size - 1] : 0);
    MEMORY_COPY(ui->was_open_stack, ui->open_stack, sizeof(uint)*ui->open_stack_size);
    ui->was_open_stack_size = ui->open_stack_size;
    ui->open_stack_size = 0;
}

Vertex *PushVertices(RenderData *data, uint vertex_count)
{
    ASSERT(data->vertices_used + vertex_count <= ARRAY_COUNT(data->vertices));
    Vertex *result = data->vertices + data->vertices_used;
    data->vertices_used += vertex_count;
    return result;
}

uint *PushIndices(RenderData *data, uint index_count)
{
    ASSERT(data->indices_used + index_count <= ARRAY_COUNT(data->indices));
    uint *result = data->indices + data->indices_used;
    data->indices_used += index_count;
    return result;
}

RenderTask *PushTask(RenderData *data)
{
    ASSERT(data->tasks_used < ARRAY_COUNT(data->tasks));
    RenderTask *result = data->tasks + data->tasks_used;
    ++data->tasks_used;
    return result;
}

void PrimRect(RenderData *data, RenderTaskList *task_list,
              F32 x0, F32 y0, F32 x1, F32 y1,
              F32 r, F32 g, F32 b, F32 a)
{
    if(x1 <= x0 || y1 <= y0)
        return;
    
    uint rect_vertex_count = 4;
    uint rect_index_count = 6;
    
    RenderTask *task = PushTask(data);
    Vertex *vtxs = PushVertices(data, rect_vertex_count);
    uint *idxs = PushIndices(data, rect_index_count);
    
    task->prim_type = PrimType_Triangle;
    task->texture_id = 0;
    task->index_data = idxs;
    task->index_count = rect_index_count;
    
    vtxs[0].pos.x = x0, vtxs[0].pos.y = y0;
    vtxs[1].pos.x = x1, vtxs[1].pos.y = y0;
    vtxs[2].pos.x = x1, vtxs[2].pos.y = y1;
    vtxs[3].pos.x = x0, vtxs[3].pos.y = y1;
    
    for(uint v_i = 0; v_i < rect_vertex_count; ++v_i)
    {
        vtxs[v_i].col.r = r;
        vtxs[v_i].col.g = g;
        vtxs[v_i].col.b = b;
        vtxs[v_i].col.a = a;
    }
    
    idxs[0] = 0, idxs[1] = 1, idxs[2] = 3;
    idxs[3] = 1, idxs[4] = 2, idxs[5] = 3;
    
    SLL_QUEUE_PUSH_BACK(task_list->first, task_list->last, task);
}

#if 0
void DrawHorizontalLine(OffscreenBuffer *buffer, int x1, V4 color1, int x2, V4 color2, int y) {
    if (x1 > x2) { SWAP(x1, x2, int); SWAP(color1, color2, V4); }
    V4 dc = ScaleV4(1.0f/(F32)(x2-x1), SubtractV4(color2, color1));
    V4 c = color1;
    for (int x = x1; x <= x2; x++)
    {
        DrawPixel(buffer, x, y, c.r, c.g, c.b, c.a);
        c.r += dc.r; c.g += dc.g; c.b += dc.b; c.a += dc.a;
    }
}
void SortTriangleWithAttributes(int *x1, int *y1,
                                int *x2, int *y2,
                                int *x3, int *y3,
                                V4 *c1, V4 *c2, V4 *c3)
{
    if (*y2 > *y3) { SWAP(*x2, *x3, int); SWAP(*y2, *y3, int); SWAP(*c2, *c3, V4); }
    if (*y1 > *y2) { SWAP(*x1, *x2, int); SWAP(*y1, *y2, int); SWAP(*c1, *c2, V4); }
    if (*y2 > *y3) { SWAP(*x2, *x3, int); SWAP(*y2, *y3, int); SWAP(*c2, *c3, V4); }
    if (*y1 == *y2 && *x1 > *x2) { SWAP(*x1, *x2, int); SWAP(*y1, *y2, int); SWAP(*c1, *c2, V4); }
}

/**
* --Task--
* Finish this function.
* It should increase y and move on both non-horizontal lines of the triangle, changing the color and x.
*/
void FillFlatBottomTriangle(int x1, int y1,
                            int x2, int y2,
                            int x3, int y3,
                            V4 c1, V4 c2, V4 c3) {
    // 1) Find how much x should change, when y changes by 1 for both lines (e.g. deltaX1, deltaX2)
    //        float deltaXA = float(x1 - x3) / (y1 - y3);
    // 2) Find how much the color should change, when y changes by 1 for both lines.
    // 3) Make some variables to hold the current x and color values
    // 4) Create a loop, where we change the y by 1 and then add the change of x and color to the corresponding variables.
    // 4.1) Inside the loop call the horizontal gradient line drawing function (DrawHorizontalLine).
}

/**
* --Task--
* Finish this function also.
*/
void FillFlatTopTriangle(int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
                         V4 c1, V4 c2, V4 c3) {
    // Same as flat bottom triangle, but start from bottom vertex.
}

void DrawColoredTriangle(int x1, int y1,
                         int x2, int y2,
                         int x3, int y3, 
                         V4 c1, V4 c2, V4 c3)
{
    SortTriangleWithAttributes(&x1, &y1, &x2, &y2, &x3, &y3, &c1, &c2, &c3);
    
    if(y2 == y3)
    {
        FillFlatBottomTriangle(x1, y1, x2, y2, x3, y3, c1, c2, c3);
    }
    else if (y1 == y2)
    {
        FillFlatTopTriangle(x1, y1, x2, y2, x3, y3, c1, c2, c3);
    }
    else
    {
        FillFlatTopTriangle(x1, y1, x2, y2, x3, y3, c1, c2, c3);
        FillFlatBottomTriangle(x1, y1, x2, y2, x3, y3, c1, c2, c3);
    }
}
#endif

void PushCube(RenderData *data, RenderTaskList *task_list)
{
    uint cube_vertex_count = 8;
    uint cube_index_count = 36;
    
    RenderTask *task = PushTask(data);
    Vertex *vtxs = PushVertices(data, cube_vertex_count);
    uint *idxs = PushIndices(data, cube_index_count);
    
    task->prim_type = PrimType_Triangle;
    task->texture_id = 0;
    task->index_data = idxs;
    task->index_count = cube_index_count;
    
    //   7---6
    //  /|  /|
    // 3---2 |
    // | 4-|-5
    // |/  |/ 
    // 0---1
    
    vtxs[0].pos.x = -0.5f, vtxs[0].pos.y = -0.5f, vtxs[0].pos.z = -0.5f;
    vtxs[1].pos.x =  0.5f, vtxs[1].pos.y = -0.5f, vtxs[1].pos.z = -0.5f; 
    vtxs[2].pos.x =  0.5f, vtxs[2].pos.y =  0.5f, vtxs[2].pos.z = -0.5f;
    vtxs[3].pos.x = -0.5f, vtxs[3].pos.y =  0.5f, vtxs[3].pos.z = -0.5f;
    vtxs[4].pos.x = -0.5f, vtxs[4].pos.y = -0.5f, vtxs[4].pos.z =  0.5f;
    vtxs[5].pos.x =  0.5f, vtxs[5].pos.y = -0.5f, vtxs[5].pos.z =  0.5f;
    vtxs[6].pos.x =  0.5f, vtxs[6].pos.y =  0.5f, vtxs[6].pos.z =  0.5f;
    vtxs[7].pos.x = -0.5f, vtxs[7].pos.y =  0.5f, vtxs[7].pos.z =  0.5f;
    
    for(uint i = 0; i < 8; ++i)
    {
        vtxs[i].pos.z += 5.0f;
    }
    
    // bottom:
    idxs[0] = 4, idxs[1] = 5, idxs[2] = 1, idxs += 3;
    idxs[0] = 4, idxs[1] = 1, idxs[2] = 0, idxs += 3;
    // front:
    idxs[0] = 0, idxs[1] = 1, idxs[2] = 2, idxs += 3;
    idxs[0] = 0, idxs[1] = 2, idxs[2] = 3, idxs += 3;
    // right:
    idxs[0] = 1, idxs[1] = 5, idxs[2] = 6, idxs += 3;
    idxs[0] = 1, idxs[1] = 6, idxs[2] = 2, idxs += 3;
    // left:
    idxs[0] = 4, idxs[1] = 0, idxs[2] = 3, idxs += 3;
    idxs[0] = 4, idxs[1] = 3, idxs[2] = 7, idxs += 3;
    // back:
    idxs[0] = 5, idxs[1] = 4, idxs[2] = 7, idxs += 3;
    idxs[0] = 5, idxs[1] = 7 ,idxs[2] = 6, idxs += 3;
    // up:
    idxs[0] = 3, idxs[1] = 2, idxs[2] = 6, idxs += 3;
    idxs[0] = 3, idxs[1] = 6, idxs[2] = 7, idxs += 3;
    
    SLL_QUEUE_PUSH_BACK(task_list->first, task_list->last, task);
}

V4 Project(F32 x, F32 y, F32 z)
{
    V4 camera_pos = {0};
    V4 camera_rot = {0};
    F32 FOV = 65;
    int width = 960;
    int height = 540;
    
    F32 tx = x - camera_pos.x;
    F32 ty = z - camera_pos.z;
    F32 tz = y - camera_pos.y;
    
    F32 cx = Cos(camera_rot.pitch);
    F32 cy = Cos(camera_rot.yaw);
    F32 cz = Cos(camera_rot.roll);
    
    F32 sx = Sin(camera_rot.pitch);
    F32 sy = Sin(camera_rot.yaw);
    F32 sz = Sin(camera_rot.roll);
    
    F32 dx = cy * (sz * ty + cz * tx) - sy * tz;
    F32 dy = sx * (cy * tz + sy * (sz * ty + cz * tx)) + cx * (cz * ty - sz * tx);
    F32 dz = cx * (cy * tz + sy * (sz * ty + cz * tx)) - sx * (cz * ty - sz * tx);
    
    F32 ez = 1.0f / Tan(FOV / 2.0f);
    
    F32 bx = ez / dz * dx;
    F32 by = ez / dz * dy;
    
    if(dz < 0.0)
    {
        bx = -bx;
        by = -by;
    }
    
    V4 result;
    result.x = ((F32)width + bx * (F32)height) / 2.0f;
    result.y = ((F32)height + by * (F32)height) / 2.0f;
    result.z = dz;
    result.w = 0.0f;
    
    return result;
}

void Bresenham(OffscreenBuffer *buffer, int x0, int y0, int x1, int y1)
{
    int dx = ABS(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = ABS(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;
    
    for(;;)
    {
        DrawPixel(buffer, x0, y0, 1, 1, 1, 1);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void Render(OffscreenBuffer *buffer, RenderData *data, RenderTaskList *task_list)
{
    for(RenderTask *task = task_list->first; task != 0; task = task->next)
    {
        switch(task->prim_type)
        {
            case PrimType_Triangle:
            {
                ASSERT(task->index_count > 0 && task->index_count % 3 == 0);
                for(uint idx_i = 0; idx_i < task->index_count; idx_i += 3)
                {
                    Vertex *v0 = data->vertices + data->indices[idx_i];
                    Vertex *v1 = data->vertices + data->indices[idx_i + 1];
                    Vertex *v2 = data->vertices + data->indices[idx_i + 2];
                    V4 p0 = Project(v0->pos.x, v0->pos.y, v0->pos.z);
                    V4 p1 = Project(v1->pos.x, v1->pos.y, v1->pos.z);
                    V4 p2 = Project(v2->pos.x, v2->pos.y, v2->pos.z);
                    Bresenham(buffer, (int)p0.x, (int)-p0.y, (int)p1.x, (int)-p1.y);
                    Bresenham(buffer, (int)p1.x, (int)-p1.y, (int)p2.x, (int)-p2.y);
                    Bresenham(buffer, (int)p2.x, (int)-p2.y, (int)p0.x, (int)-p0.y);
                }
            } break;
            
            default:
            {
                INVALID_CODE_PATH;
            }
        }
    }
}

void GameUpdateAndRender(GameMemory *memory, Input *input, OffscreenBuffer *buffer, Font *font)
{
    UNUSED(font);
    
    GameState *game_state = (GameState *)memory->permanent_storage;
    UIState *ui = &game_state->ui;
    
    if(!(memory->is_initialized))
    {
        InitializeArena(&game_state->arena,
                        (U8 *)memory->permanent_storage + sizeof(GameState),
                        0);
        
        InitializeInput(input);
        
        ui->key_repeat_interval = 0.125f;
        
        ASSERT(IsPowerOfTwo(ARRAY_COUNT(ui->windows)));
        ui->colors[UIColor_FillDefault]   = V4F32(0.125f, 0.133f, 0.144f, 0.8f);
        ui->colors[UIColor_FillFocused]   = V4F32(0.125f, 0.133f, 0.5f,   0.8f);
        ui->colors[UIColor_FillActive]    = V4F32(0.125f, 0.500f, 0.144f, 0.8f);
        ui->colors[UIColor_BorderDefault] = V4F32(0.203f, 0.211f, 0.222f, 0.8f);
        ui->colors[UIColor_BorderFocused] = V4F32(0.203f, 0.211f, 0.6f,   0.8f);
        ui->colors[UIColor_BorderActive]  = V4F32(0.203f, 0.6f,   0.222f, 0.8f);
        ui->colors[UIColor_Text]          = V4F32(0.6f,   0.6f,   0.6f,   0.8f);
        
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
    
    RenderData render_data = {0};
    RenderTaskList render_tasks = {0};
    PushCube(&render_data, &render_tasks);
    Render(buffer, &render_data, &render_tasks);
    
#if 0
    for(int idx = 0; idx < Input_COUNT; ++idx)
    {
        bool is_down = IsDown(input, idx);
        bool was_down = WasDown(input, idx);
        
        F32 r = !was_down && is_down;
        F32 g = is_down;
        F32 b = was_down;
        F32 a = CLAMP(0.0f, r+g+b, 1.0f);
        DrawRectangle(buffer, (F32)idx*20, 0, (F32)(idx+1)*20, 20, r, g, b, a);
    }
#endif 
    ++game_state->frame_idx;
}
