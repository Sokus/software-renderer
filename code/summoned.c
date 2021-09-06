#include "base.h"
#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

#include "summoned.h"


void
DrawRectangle(GameOffscreenBuffer *buffer,
              int min_x, int min_y, int max_x, int max_y,
              F32 r, F32 g, F32 b, F32 a)
{
    if(a > 0.0f)
    {
        min_x = CLAMP_BOT(min_x, 0);
        min_y = CLAMP_BOT(min_y, 0);
        max_x = CLAMP_TOP(max_x, buffer->width);
        max_y = CLAMP_TOP(max_y, buffer->height);
        
        U8 *dest_row = (U8 *)buffer->memory + min_y*buffer->pitch + min_x*buffer->bytes_per_pixel;
        U8 *dest;
        
        for(int y = min_y;
            y < max_y;
            ++y)
        {
            
            dest = dest_row;
            
            for(int x = min_x;
                x < max_x;
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

typedef enum BorderStyle
{
    BORDERSTYLE_NONE = 0,
    BORDERSTYLE_CENTERED = 1,
    BORDERSTYLE_INNER = 2,
    BORDERSTYLE_OUTER = 3,
    BORDERSTYLE_DEFAULT = 2
} BorderStyle;

void
DrawRectangleExplicit(GameOffscreenBuffer *buffer,
                      I32 min_x, I32 min_y, I32 max_x, I32 max_y,
                      F32 fill_r, F32 fill_g, F32 fill_b, F32 fill_a,
                      BorderStyle border_style, I32 border_width,
                      F32 border_r, F32 border_g, F32 border_b, F32 border_a)
{
    I32 width = max_x - min_x;
    I32 height = max_y - min_y;
    
    if(width > 0 && height > 0)
    {
        I32 inside_border = (border_style == BORDERSTYLE_CENTERED ? border_width/2 :
                             border_style == BORDERSTYLE_INNER ? border_width : 0);
        I32 outside_border = (border_style == BORDERSTYLE_CENTERED ? border_width/2 :
                              border_style == BORDERSTYLE_OUTER ? border_width : 0);
        
        if(fill_a > 0.0f
           && width - 2*inside_border > 0
           && height - 2*inside_border > 0)
        {
            DrawRectangle(buffer,
                          min_x+inside_border, min_y+inside_border, max_x-inside_border, max_y-inside_border,
                          fill_r, fill_g, fill_b, fill_a);
        }
        
        if(border_a > 0.0f && (inside_border > 0 || outside_border > 0))
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
DebugDrawButtonState(GameOffscreenBuffer *buffer, GameButtonState state,
                     F32 pos_x, F32 pos_y, F32 tile_size)
{
    F32 r = !!state.ended_down;
    F32 g = 0.0f;
    F32 b = (F32)(state.half_transition_count % 2);
    
    if(r || g || b)
    {
        DrawRectangle(buffer, pos_x, pos_y, pos_x+tile_size, pos_y+tile_size, r, g, b, 1.0f);
    }
}

void
DebugDrawAllControllerButtonStates(GameOffscreenBuffer *buffer, GameControllerInput *controller,
                                   F32 pos_x, F32 pos_y, F32 tile_size)
{
    V2 position = V2F32(pos_x, pos_y);
    V2 tile_dim = V2F32(tile_size, tile_size);
    
    DrawRectangle(buffer, pos_x, pos_y, pos_x+tile_size, pos_y+tile_size, 0.0f, 1.0f, 0.6f, 1.0f);
    
    DebugDrawButtonState(buffer, controller->move_up, pos_x + tile_size, pos_y + 2*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->move_left, pos_x, pos_y + 3*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->move_down, pos_x + tile_size, pos_y + 4*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->move_right, pos_x + 2*tile_size, pos_y + 3*tile_size, tile_size);
    
    DebugDrawButtonState(buffer, controller->action_up, pos_x + 9*tile_size, pos_y + 2*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->action_left, pos_x + 8*tile_size, pos_y + 3*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->action_down, pos_x + 9*tile_size, pos_y + 4*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->action_right, pos_x + 10*tile_size, pos_y + 3*tile_size, tile_size);
    
    DebugDrawButtonState(buffer, controller->select, pos_x + 4*tile_size, pos_y + 2*tile_size, tile_size);
    DebugDrawButtonState(buffer, controller->start, pos_x + 6*tile_size, 2*tile_size, tile_size);
    
    DebugDrawButtonState(buffer, controller->left_bumper, pos_x + tile_size, pos_y, tile_size);
    DebugDrawButtonState(buffer, controller->right_bumper, pos_x + 9*tile_size, pos_y, tile_size);
    
    
    
    if(controller->is_analog)
    {
        V2 stick_position = V2F32(tile_size*controller->stick_average_x, -tile_size*controller->stick_average_y);
        V2 half_tile_size = ScaleV2(0.25f, tile_dim);
        V2 offset_min = SubtractV2(stick_position, half_tile_size);
        V2 offset_max = AddV2(stick_position, half_tile_size);
        V2 marker_position = AddV2(position, V2F32(4*tile_size, 4*tile_size));
        
        DrawRectangle(buffer,
                      marker_position.x + offset_min.x, marker_position.x + offset_min.y,
                      marker_position.x + offset_max.x, marker_position.x + offset_max.y,
                      1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void
DrawAllControllerStates(GameOffscreenBuffer *buffer, GameInput *input)
{
    for(size_t controller_index = 0;
        controller_index < ARRAY_COUNT(input->controllers);
        ++controller_index)
    {
        if(input->controllers[controller_index].is_connected)
        {
            F32 horizontal_index = (F32)(controller_index % 3);
            F32 vertical_index = (F32)(controller_index / 3);
            F32 tile_size = 20;
            DebugDrawAllControllerButtonStates(buffer,
                                               &input->controllers[controller_index],
                                               11*tile_size*horizontal_index, 5*tile_size*vertical_index,
                                               tile_size);
        }
    }
}

void
DrawBitmap(GameOffscreenBuffer *buffer,
           U32 *bitmap_data, U32 bitmap_w, U32 bitmap_h,
           U32 part_x, U32 part_y, U32 part_w, U32 part_h,
           F32 pos_x, F32 pos_y,
           F32 r, F32 g, F32 b, F32 a)
{
    UNUSED(bitmap_h);
    
    bitmap_data = bitmap_data + part_y*bitmap_w + part_x;
    
    I32 min_x = RoundF32ToI32(pos_x);
    I32 min_y = RoundF32ToI32(pos_y);
    I32 max_x = RoundF32ToI32(pos_x + part_w);
    I32 max_y = RoundF32ToI32(pos_y + part_h);;
    
    I32 source_offset_x = 0;
    if(min_x < 0)
    {
        source_offset_x = -min_x;
        min_x = 0;
    }
    
    I32 source_offset_y = 0;
    if(min_y < 0)
    {
        source_offset_y = -min_y;
        min_y = 0;;
    }
    
    max_x = CLAMP_TOP(max_x, buffer->width);
    max_y = CLAMP_TOP(max_y, buffer->height);
    
    U32 *source_row = bitmap_data + source_offset_y*bitmap_w + source_offset_x;
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
DrawAsci(GameOffscreenBuffer *buffer, Font *font,
         U8 character,
         F32 pos_x, F32 pos_y,
         F32 r, F32 g, F32 b, F32 a)
{
    U32 glyph_pos_x = character % font->glyphs_per_row;
    U32 glyph_pos_y = character / font->glyphs_per_col;
    
    DrawBitmap(buffer,
               font->data, font->w, font->h,
               glyph_pos_x*font->glyph_w, glyph_pos_y*font->glyph_h, font->glyph_w, font->glyph_h,
               pos_x, pos_y,
               r, g, b, a);
}

void
DrawString(GameOffscreenBuffer *buffer, Font *font,
           char *source, size_t source_count,
           F32 pos_x, F32 pos_y,
           F32 r, F32 g, F32 b, F32 a)
{
    for(size_t index = 0;
        index < source_count;
        index++)
    {
        F32 offset_x = (F32)index*font->glyph_w;
        
        DrawAsci(buffer, font, *(source+index), pos_x+offset_x, pos_y, r, g, b, a);
    }
}

Rect
RectFromBuffer(GameOffscreenBuffer *buffer)
{
    Rect result;
    
    result.x0 = 0;
    result.y0 = 0;
    result.x1 = buffer->width;
    result.y1 = buffer->height;
    
    return result;
}

Rect
RectRelativeExplicit(Rect parent,
                     F32 x0_pct, F32 y0_pct, F32 x1_pct, F32 y1_pct,
                     F32 top, F32 left, F32 bottom, F32 right)
{
    Rect result;
    
    F32 parent_width = parent.x1 - parent.x0;
    F32 parent_height = parent.y1 - parent.y0;
    
    result.x0 = parent.x0 + x0_pct*parent_width + left;
    result.y0 = parent.y0 + y0_pct*parent_height + bottom;
    result.x1 = parent.x0 + x1_pct*parent_width - right;
    result.y1 = parent.y0 + y1_pct*parent_height - top;
    
    return result;
}

Rect
RectRelative(Rect parent, F32 x0_pct, F32 y0_pct, F32 x1_pct, F32 y1_pct)
{
    Rect result = RectRelativeExplicit(parent, x0_pct, y0_pct, x1_pct, y1_pct, 0, 0, 0, 0);
    return result;
}

typedef enum SplitType
{
    SPLITTYPE_NONE,
    SPLITTYPE_VERTICAL,
    SPLITTYPE_HORIZONTAL
} SplitType;

void
SplitRectExplicit(Rect parent,
                  SplitType split_type, F32 split_pct,
                  Rect *rect0, Rect *rect1,
                  F32 top, F32 left, F32 bottom, F32 right)
{
    switch(split_type)
    {
        case SPLITTYPE_VERTICAL:
        {
            *rect0 = RectRelativeExplicit(parent, 0, 0, split_pct, 1, top, left, bottom, right);
            *rect1 = RectRelativeExplicit(parent, split_pct, 0, 1, 1, top, left, bottom, right);
        } break;
        
        case SPLITTYPE_HORIZONTAL:
        {
            *rect0 = RectRelativeExplicit(parent, 0, 0, 1, split_pct, top, left, bottom, right);
            *rect1 = RectRelativeExplicit(parent, 0, split_pct, 1, 1, top, left, bottom, right);
        } break;
        
        default: { } break;
    }
}

void
SplitRect(Rect parent, Rect *rect0, Rect *rect1,
          SplitType split_type, F32 split_pct)
{
    SplitRectExplicit(parent, split_type, split_pct, rect0, rect1, 0, 0, 0, 0);
}

#if 0
String8List
String8ListFitInRect(MemoryArena *memory_arena, String8List *list, Rect rect, int glyph_w, int glyph_h, F32 spacing_pct)
{
    
}
#endif

void GameUpdateAndRender(GameMemory *memory, GameInput *input, GameOffscreenBuffer *buffer, FontPack *font_pack)
{
    ASSERT((&input->controllers[0].terminator - &input->controllers[0].buttons[0]) ==
           ARRAY_COUNT(input->controllers[0].buttons));
    
    GameState *game_state = (GameState *)memory->permanent_storage;
    if(!(memory->is_initialized))
    {
        game_state->time = 0;
        
        game_state->position_x = 0;
        game_state->position_y = 0;
        
        InitializeArena(&game_state->arena,
                        (U8 *)memory->permanent_storage + sizeof(GameState),
                        0);
        
        
        memory->is_initialized = true;
    }
    
    MemoryArena transient_arena;
    InitializeArena(&transient_arena,
                    (U8 *)memory->transient_storage,
                    5000);
    
    
    game_state->time += input->dt_for_frame;
    
    game_state->position_x += (input->dt_for_frame / 4.0f);
    if(game_state->position_x > 0.5f)
    {
        game_state->position_x = 0;
    }
    
    DrawRectangle(buffer, 0, 0, (F32)buffer->width, (F32)buffer->height, 0, 0, 0, 1);
    
    Rect screen_rect = RectFromBuffer(buffer);
    
    Rect left_panel, right_panel;
    SplitRectExplicit(screen_rect, SPLITTYPE_VERTICAL, 0.5f, &left_panel, &right_panel, 5, 5, 5, 5);
    
    DrawRectangleExplicit(buffer,
                          left_panel.x0, left_panel.y0, left_panel.x1, left_panel.y1,
                          0.3f, 0.3f, 0.3f, 0.7f, BORDERSTYLE_INNER, 5, 0.5f, 0.5f, 0.5f, 1.0f);
    
    DrawRectangleExplicit(buffer,
                          right_panel.x0, right_panel.y0, right_panel.x1, right_panel.y1,
                          0.3f, 0.3f, 0.3f, 0.7f, BORDERSTYLE_INNER, 5, 0.5f, 0.5f, 0.5f, 1.0f);
    
    String8 text = STRING8_FROM_LITERAL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    
    
    
    
}
