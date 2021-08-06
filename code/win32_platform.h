/* date = August 1st 2021 8:51 pm */

#ifndef WIN32_SUMMONED_H
#define WIN32_SUMMONED_H

typedef struct Win32WindowDimension
{
    int width;
    int height;
} Win32WindowDimension;

typedef struct Win32OffscreenBuffer
{
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel;
} Win32OffscreenBuffer;

typedef struct Win32GameCode
{
    HMODULE game_code_dll;
    FILETIME last_dll_write_time;
    
    GameUpdateAndRenderType *update_and_render;
    
    B32 is_valid;
} Win32GameCode;

typedef struct Win32State
{
    U64 total_size;
    void *game_memory_block;
    
    char exe_path[MAX_PATH];
    char *one_past_last_exe_path_slash;
} Win32State;



#endif //WIN32_SUMMONED_H
