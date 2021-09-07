/* date = August 2nd 2021 0:08 pm */

#ifndef SUMMONED_PLATFORM_H
#define SUMMONED_PLATFORM_H

typedef struct OffscreenBuffer
{
    // NOTE(casey): Pixels are always 32-bits wide, Memory Order BB GG RR XX
    void *memory;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel;
} OffscreenBuffer;

typedef struct GameMemory
{
    bool is_initialized;
    
    U64 permanent_storage_size;
    void *permanent_storage; // NOTE(casey): REQUIRED to be cleared to zero at startup
    
    U64 transient_storage_size;
    void *transient_storage; // NOTE(casey): REQUIRED to be cleared to zero at startup
} GameMemory;

typedef struct ButtonState
{
    int half_transition_count;
    bool ended_down;
} ButtonState;

typedef struct ControllerInput
{
    bool is_connected;
    bool is_analog;
    F32 stick_average_x;
    F32 stick_average_y;
    
    union
    {
        ButtonState buttons[12];
        struct
        {
            ButtonState move_up;
            ButtonState move_down;
            ButtonState move_left;
            ButtonState move_right;
            
            ButtonState action_up;
            ButtonState action_down;
            ButtonState action_left;
            ButtonState action_right;
            
            ButtonState left_bumper;
            ButtonState right_bumper;
            
            ButtonState select;
            ButtonState start;
            
            // NOTE(casey): All buttons must be added above this line
            
            ButtonState terminator;
        };
    };
} ControllerInput;

typedef struct Input
{
    ControllerInput controllers[5];
    ButtonState mouse_buttons[5];
    I32 mouse_x, mouse_y, mouse_z;
    
    F32 dt_for_frame;
} Input;

ControllerInput *
GetController(Input *input, uint controller_index)
{
    U32 count = ARRAY_COUNT(input->controllers);
    ASSERT(controller_index < count);
    
    ControllerInput *result = &input->controllers[controller_index];
    return result;
}

typedef enum FontRasterFlags
{
    FONT_RASTER_FLAG_RASTER_FONT = 1,
    FONT_RASTER_FLAG_DONT_MAP_UNICODE = 2,
    FONT_RASTER_FLAG_BOLD = 4
} FontRasterFlags;

typedef struct Font
{
    int glyph_w;
    int glyph_h;
    int glyphs_per_row;
    int glyphs_per_col;
    int w;
    int h;
    int pitch;
    U32 *data;
} Font;

typedef struct FontPack
{
    char *name;
    char *filename;
    int size;
    
    Font regular;
    // Font bold;
} FontPack;

typedef void GameUpdateAndRenderType(GameMemory *memory,
                                     Input *input,
                                     OffscreenBuffer *buffer,
                                     FontPack *font_pack);

#endif //SUMMONED_PLATFORM_H
