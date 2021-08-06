#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

#include "summoned.h"

#include <stdio.h>

internal void
DrawRectangle(GameOffscreenBuffer *buffer, V2 min, V2 max, F32 R, F32 G, F32 B)
{
    I32 min_x = RoundF32ToI32(min.x);
    I32 min_y = RoundF32ToI32(min.y);
    I32 max_x = RoundF32ToI32(max.x);
    I32 max_y = RoundF32ToI32(max.y);
    
    min_x = CLAMP_BOT(min_x, 0);
    min_y = CLAMP_BOT(min_y, 0);
    max_x = CLAMP_TOP(max_x, buffer->width);
    max_y = CLAMP_TOP(max_y, buffer->height);
    
    U32 color = ((RoundF32ToU32(R*255.0f) << 16) |
                 (RoundF32ToU32(G*255.0f) << 8) |
                 (RoundF32ToU32(B*255.0f) << 0));
    
    U8 *row = ((U8 *)buffer->memory +
               min_x * buffer->bytes_per_pixel +
               min_y * buffer->pitch);
    
    for(int y=min_y; y<max_y; ++y)
    {
        U32 *pixel = (U32 *)row;
        for(int x=min_x; x<max_x; ++x)
        {
            *pixel++ = color;
        }
        
        row += buffer->pitch;
    }
}

internal void
DebugDrawButtonState(GameOffscreenBuffer *buffer, GameButtonState state, V2 position, V2 tile_dim)
{
    F32 r = !!state.ended_down;
    F32 g = 0.0f;
    F32 b = state.half_transition_count % 2;
    
    if(r || g || b)
    {
        DrawRectangle(buffer, position, AddV2(position, tile_dim), r, g, b);
    }
}

#if SUMMONED_DEBUG
internal void
DebugDrawAllControllerButtonStates(GameOffscreenBuffer *buffer, GameControllerInput *controller, V2 position, F32 tile_size)
{
    V2 tile_dim = V2F32(tile_size, tile_size);
    
    DrawRectangle(buffer, position, AddV2(position, tile_dim), 0.0f, 1.0f, 0.6f);
    
    DebugDrawButtonState(buffer, controller->move_up, AddV2(position, V2F32(tile_size, 2*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->move_left, AddV2(position, V2F32(0, 3*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->move_down, AddV2(position, V2F32(tile_size, 4*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->move_right, AddV2(position, V2F32(2*tile_size, 3*tile_size)), tile_dim);
    
    DebugDrawButtonState(buffer, controller->action_up, AddV2(position, V2F32(9*tile_size, 2*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->action_left, AddV2(position, V2F32(8*tile_size, 3*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->action_down, AddV2(position, V2F32(9*tile_size, 4*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->action_right, AddV2(position, V2F32(10*tile_size, 3*tile_size)), tile_dim);
    
    DebugDrawButtonState(buffer, controller->select,AddV2(position, V2F32(4*tile_size, 2*tile_size)), tile_dim);
    DebugDrawButtonState(buffer, controller->start, AddV2(position, V2F32(6*tile_size, 2*tile_size)), tile_dim);
    
    DebugDrawButtonState(buffer, controller->left_bumper, AddV2(position, V2F32(tile_size, 0)), tile_dim);
    DebugDrawButtonState(buffer, controller->right_bumper, AddV2(position, V2F32(9*tile_size, 0)), tile_dim);
    
    if(controller->is_analog)
    {
        V2 stick_position = V2F32(tile_size*controller->stick_average_x, -tile_size*controller->stick_average_y);
        V2 half_tile_size = ScaleV2(0.25f, tile_dim);
        V2 offset_min = SubtractV2(stick_position, half_tile_size);
        V2 offset_max = AddV2(stick_position, half_tile_size);
        V2 marker_position = AddV2(position, V2F32(4*tile_size, 4*tile_size));
        
        DrawRectangle(buffer,
                      AddV2(marker_position, offset_min),
                      AddV2(marker_position, offset_max),
                      1.0f, 1.0f, 1.0f);
    }
}

internal void
DebugDrawAllControllerStates(GameOffscreenBuffer *buffer, GameInput *input)
{
    for(SizeType controller_index = 0;
        controller_index < ARRAY_COUNT(input->controllers);
        ++controller_index)
    {
        if(input->controllers[controller_index].is_connected)
        {
            F32 horizontal_index = controller_index % 3;
            F32 vertical_index = controller_index / 3;
            F32 tile_size = 20;
            V2 offset = V2F32(tile_size, tile_size);
            DebugDrawAllControllerButtonStates(buffer,
                                               &input->controllers[controller_index],
                                               AddV2(V2F32(11*tile_size*horizontal_index, 5*tile_size*vertical_index), offset),
                                               tile_size);
        }
    }
}
#endif // SUMMONED_DEBUG

void GameUpdateAndRender(GameMemory *memory, GameInput *input, GameOffscreenBuffer *buffer)
{
    ASSERT((&input->controllers[0].terminator - &input->controllers[0].buttons[0]) ==
           ARRAY_COUNT(input->controllers[0].buttons));
    
    GameState *game_state = (GameState *)memory->permanent_storage;
    if(!(memory->is_initialized))
    {
        game_state->time = 0;
        
        InitializeArena(&game_state->arena,
                        (U8 *)memory->permanent_storage + sizeof(GameState),
                        0);
        
        memory->is_initialized = TRUE;
    }
    
    game_state->time += input->dt_for_frame;
    
    DrawRectangle(buffer,
                  V2F32(0, 0), V2F32((F32)buffer->width, (F32)buffer->height),
                  0, 0, 0);
    
    DebugDrawAllControllerStates(buffer, input);
}