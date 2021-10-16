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

typedef enum InputKey
{
    Input_DPadUp,      // W
    Input_DPadLeft,    // A
    Input_DPadDown,    // S
    Input_DPadRight,   // D
    Input_ActionUp,    // R
    Input_ActionLeft,  // F
    Input_ActionDown,  // E      // Confirm
    Input_ActionRight, // Q      // Cancel
    Input_BumperLeft,  // Ctrl
    Input_BumperRight, // Shift
    Input_Select,      // Tab
    Input_Start,       // Escape
    Input_MouseLeft,
    Input_MouseRight,
    Input_MouseMiddle,
    Input_MouseM4,
    Input_MouseM5,
    Input_COUNT
} InputKey;

typedef struct Input
{
    bool keys_down[Input_COUNT];
    F32 keys_down_duration[Input_COUNT];
    F32 keys_down_duration_previous[Input_COUNT];
    
    F32 stick_x, stick_y;
    I32 mouse_x, mouse_y, mouse_z;
    
    uint frame_index;
    F32 time_dt;
} Input;

typedef enum FontRasterFlags
{
    FontRasterFlag_RasterFont = 1,
    FontRasterFlag_DontMapUnicode = 2,
    FontRasterFlag_Bold = 4
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
