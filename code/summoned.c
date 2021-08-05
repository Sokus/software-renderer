#include "summoned_platform.h"
#include "summoned_math.h"
#include "summoned_intrinsics.h"

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


GAME_UPDATE_AND_RENDER_SIG(GameUpdateAndRender)
{
    DrawRectangle(buffer,
                  V2F32(0, 0), V2F32((F32)buffer->width, (F32)buffer->height),
                  0.1f, 0.1f, 0.1f);
    DrawRectangle(buffer,
                  V2F32(0, 0), V2F32((F32)buffer->width/2.0f, (F32)buffer->height/2.0f),
                  0.7f, 0.0f, 0.0f);
}