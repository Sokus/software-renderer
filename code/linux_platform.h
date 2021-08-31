/* date = August 28th 2021 4:01 pm */

#ifndef LINUX_PLATFORM_H
#define LINUX_PLATFORM_H

typedef struct SDLOffscreenBuffer
{
    SDL_Texture *texture;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel;
} SDLOffscreenBuffer;

typedef struct SDLWindowDimension
{
    int width;
    int height;
} SDLWindowDimension;

typedef struct timespec timespec;

typedef struct LinuxGameCode
{
    void *game_code_dll;
    timespec last_dll_write_time;
    
    GameUpdateAndRenderType *game_update_and_render;
    
    bool is_valid;
} LinuxGameCode;

typedef struct LinuxState
{
    U64 total_size;
    void *game_memory_block;
    
    char exe_path[PATH_MAX];
    char *one_past_last_exe_path_slash;
} LinuxState;

#endif //LINUX_PLATFORM_H
