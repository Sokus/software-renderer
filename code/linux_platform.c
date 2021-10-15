#include "base.h"

#include "summoned_platform.h"
#include "summoned_intrinsics.h"

#include "summoned_font_maps.c"

#include <stdio.h>
#include <time.h> 
#include <sys/mman.h> // mmap
#include <sys/stat.h> // stat
#include <linux/limits.h> 
#include <x86intrin.h> // _rdtsc
#include <unistd.h>
#include <dlfcn.h> // dlopen, dlsym, dlclose

#include "SDL.h"
#include "SDL_ttf.h"

#include "linux_platform.h"


global SDLOffscreenBuffer global_backbuffer = { .bytes_per_pixel = 4 };
global bool global_is_running;

#define MAX_CONTROLLERS 4
global SDL_GameController *global_controller_handles[MAX_CONTROLLERS];
global SDL_Haptic *global_rumble_handles[MAX_CONTROLLERS];

internal SDLWindowDimension
SDLGetWindowDimension(SDL_Window *window)
{
    SDLWindowDimension result;
    
    SDL_GetWindowSize(window, &result.width, &result.height);
    
    return result;
}

#if 0
internal int
SDLGetWindowRefreshRate(SDL_Window *window)
{
    SDL_DisplayMode mode;
    int display_index = SDL_GetWindowDisplayIndex(window);
    int default_refresh_rate = 60;
    if(SDL_GetDesktopDisplayMode(display_index, &mode) != 0)
    {
        return default_refresh_rate;
    }
    if(mode.refresh_rate == 0)
    {
        return default_refresh_rate;
    }
    
    return mode.refresh_rate;
}
#endif 

internal F32
SDLGetSecondsElapsed(U64 start_counter, U64 end_counter)
{
    F32 result;
    
    result = ((F32)(end_counter - start_counter) / (F32)(SDL_GetPerformanceFrequency()));
    
    return result;
}

timespec
LinuxGetWallClock()
{
    timespec result;
    clock_gettime(CLOCK_MONOTONIC, &result);
    return result;
}

internal bool
LinuxTimespecsAreTheSame(timespec a, timespec b)
{
    bool result = false;
    
    result = (a.tv_sec == b.tv_sec
              && a.tv_nsec == b.tv_nsec);
    
    return result;
}

internal void
SDLResizeTexture(SDLOffscreenBuffer *buffer, SDL_Renderer *renderer, int width, int height)
{
    int bytes_per_pixel = 4;
    if(buffer->memory)
    {
        munmap(buffer->memory,
               width * height * bytes_per_pixel);
    }
    
    if(buffer->texture)
    {
        SDL_DestroyTexture(buffer->texture);
    }
    
    buffer->texture = SDL_CreateTexture(renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        width,
                                        height);
    buffer->width = width;
    buffer->height = height;
    buffer->pitch = width * bytes_per_pixel;
    buffer->bytes_per_pixel = bytes_per_pixel;
    buffer->memory = mmap(0,
                          width * height * buffer->bytes_per_pixel,
                          PROT_READ|PROT_WRITE,
                          MAP_ANONYMOUS|MAP_PRIVATE,
                          -1,
                          0);
}

internal void
SDLUpdateWindow(SDLOffscreenBuffer *buffer, SDL_Renderer *renderer)
{
    SDL_UpdateTexture(buffer->texture,
                      0,
                      buffer->memory,
                      buffer->pitch);
    SDL_Rect dstrect = {0, 0, buffer->width, buffer->height};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, buffer->texture, 0, &dstrect);
    SDL_RenderPresent(renderer);
}

internal void
SDLProcessKeyPress(ButtonState *new_state, bool is_down)
{
    new_state->ended_down = is_down;
    ++new_state->half_transition_count;
}

internal void
SDLHandleEvent(SDL_Event *event, ControllerInput *keyboard_controller)
{
    switch(event->type)
    {
        case SDL_QUIT:
        {
            global_is_running = false;
        } break;
        
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            SDL_Keycode key_code = event->key.keysym.sym;
            bool is_down = (event->key.state == SDL_PRESSED);
            bool was_down = false;
            if(event->key.state == SDL_RELEASED)
            {
                was_down = true;
            }
            else if (event->key.repeat != 0)
            {
                was_down = true;
            }
            
            if(event->key.repeat == 0)
            {
                if(key_code == SDLK_w)
                {
                    SDLProcessKeyPress(&keyboard_controller->move_up, is_down);
                }
                else if(key_code == SDLK_a)
                {
                    SDLProcessKeyPress(&keyboard_controller->move_left, is_down);
                }
                else if(key_code == SDLK_s)
                {
                    SDLProcessKeyPress(&keyboard_controller->move_down, is_down);
                }
                else if(key_code == SDLK_d)
                {
                    SDLProcessKeyPress(&keyboard_controller->move_right, is_down);
                }
                else if(key_code == SDLK_r)
                {
                    SDLProcessKeyPress(&keyboard_controller->action_up, is_down);
                }
                else if(key_code == SDLK_f)
                {
                    SDLProcessKeyPress(&keyboard_controller->action_left, is_down);
                }
                else if(key_code == SDLK_e)
                {
                    SDLProcessKeyPress(&keyboard_controller->action_down, is_down);
                }
                else if(key_code == SDLK_q)
                {
                    SDLProcessKeyPress(&keyboard_controller->action_right, is_down);
                }
                else if(key_code == SDLK_TAB)
                {
                    SDLProcessKeyPress(&keyboard_controller->select, is_down);
                }
                else if(key_code == SDLK_ESCAPE)
                {
                    SDLProcessKeyPress(&keyboard_controller->start, is_down);
                }
                else
                {
                    bool lshift_key_was_down = (event->key.keysym.mod & KMOD_LSHIFT);
                    bool ctrl_key_was_down = (event->key.keysym.mod & KMOD_CTRL);
                    
                    if(lshift_key_was_down)
                    {
                        SDLProcessKeyPress(&keyboard_controller->right_bumper, is_down);
                    }
                    else if(ctrl_key_was_down)
                    {
                        SDLProcessKeyPress(&keyboard_controller->left_bumper, is_down);
                    }
                }
            }
            
            bool alt_key_was_down = (event->key.keysym.mod & KMOD_ALT);
            if (key_code == SDLK_F4 && alt_key_was_down)
            {
                global_is_running = false;
            }
        } break;
        
        case SDL_WINDOWEVENT:
        {
            switch(event->window.event)
            {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
#if 0
                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    SDL_Renderer *renderer = SDL_GetRenderer(window);
                    //SDLResizeTexture(&global_backbuffer, renderer, event->window.data1, event->window.data2);
#endif
                } break;
                
                case SDL_WINDOWEVENT_EXPOSED:
                {
                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    SDL_Renderer *renderer = SDL_GetRenderer(window);
                    SDLUpdateWindow(&global_backbuffer, renderer);
                } break;
            }
        } break;
    }
}

internal void
SDLOpenGameControllers(void)
{
    int max_joysticks = SDL_NumJoysticks();
    int controller_idx = 0;
    for(int joystick_idx = 0;
        joystick_idx < max_joysticks && joystick_idx < MAX_CONTROLLERS;
        ++joystick_idx)
    {
        if(!SDL_IsGameController(joystick_idx))
        {
            continue;
        }
        
        global_controller_handles[controller_idx] = SDL_GameControllerOpen(joystick_idx);
        SDL_Joystick *joystick_handle =
            SDL_GameControllerGetJoystick(global_controller_handles[controller_idx]);
        global_rumble_handles[controller_idx] = SDL_HapticOpenFromJoystick(joystick_handle);
        if(SDL_HapticRumbleInit(global_rumble_handles[controller_idx]) != 0)
        {
            SDL_HapticClose(global_rumble_handles[controller_idx]);
            global_rumble_handles[controller_idx] = 0;
        }
        
        ++controller_index;
    }
}

internal void
SDLProcessGameControllerButton(ButtonState *old_state,
                               ButtonState *new_state,
                               bool ended_down)
{
    new_state->ended_down = ended_down;
    if(new_state->ended_down == old_state->ended_down)
    {
        ++new_state->half_transition_count;
    }
}

internal F32
SDLProcessGameControllerAxisValue(I16 value, I16 deadzone_threshold)
{
    F32 result = 0;
    
    if(value < -deadzone_threshold)
    {
        result = (F32)((value + deadzone_threshold) / (32768.0f - deadzone_threshold));
    }
    else if(value > deadzone_threshold)
    {
        result = (F32)((value - deadzone_threshold) / (32767.0f - deadzone_threshold));
    }
    
    return result;
}

internal void
SDLCloseGameControllers()
{
    for(int controller_idx = 0; controller_idx < MAX_CONTROLLERS; ++controller_idx)
    {
        if(global_controller_handles[controller_idx])
        {
            if(global_rumble_handles[controller_idx])
            {
                SDL_HapticClose(global_rumble_handles[controller_idx]);
            }
            
            SDL_GameControllerClose(global_controller_handles[controller_idx]);
        }
    }
}

internal void
LinuxInitEXEPath(LinuxState *state)
{
    size_t bytes_read = readlink("/proc/self/exe", state->exe_path, PATH_MAX);
    ASSERT(bytes_read > 0);
    
    state->one_past_last_exe_path_slash = state->exe_path;
    for(size_t offset = 0;
        *(state->exe_path + offset) && offset < bytes_read;
        ++offset)
    {
        if(*(state->exe_path+offset) == '/')
        {
            state->one_past_last_exe_path_slash = state->exe_path + offset + 1;
        }
    }
}

internal void
LinuxAppendToParentDirectoryPath(LinuxState *state, char *filename, char *dest, size_t dest_count)
{
    size_t path_count = (state->one_past_last_exe_path_slash - state->exe_path);
    ConcatenateStrings(state->exe_path, path_count,
                       filename, StringLength(filename),
                       dest, dest_count);
}


internal timespec
LinuxGetLastWriteTime(char *filename)
{
    struct timespec result;
    
    struct stat file_stat;
    if(stat(filename, &file_stat))
    {
        ASSERT(!"Error getting file stat");
    }
    
    result = file_stat.st_mtim;
    
    return result;
}

internal LinuxGameCode
LinuxLoadGameCode(char *source_dll_name)
{
    LinuxGameCode result = {0};
    
    result.last_dll_write_time = LinuxGetLastWriteTime(source_dll_name);
    
    result.game_code_dll = dlopen(source_dll_name, RTLD_NOW);
    if(result.game_code_dll)
    {
        result.game_update_and_render = (GameUpdateAndRenderType *)
            dlsym(result.game_code_dll, "GameUpdateAndRender");
        
        result.is_valid = !!(result.game_update_and_render);
    }
    
    return result;
}

internal void
LinuxUnloadGameCode(LinuxGameCode *game_code)
{
    if(game_code->game_code_dll)
    {
        dlclose(game_code->game_code_dll);
        game_code->game_code_dll = 0;
    }
    
    game_code->game_update_and_render = 0;
    
    game_code->is_valid = false;
}

bool
SDLMakeAsciiFont(char *font_name_utf8, int font_size, Font *out_font, FontRasterFlags flags)
{
    bool result = false;
    
    // bool raster_font = !!(flags & FontRasterFlag_RasterFont);
    bool dont_map_unicode = !!(flags & FontRasterFlag_DontMapUnicode);
    bool bold = !!(flags & FontRasterFlag_Bold);
    
    *out_font = (Font){0};
    
    TTF_Font *font = TTF_OpenFont(font_name_utf8, font_size);
    
    ASSERT(font);
    if(font)
    {
        U16* charset = (dont_map_unicode ? codepage_437_ascii : codepage_437_utf16);
        
        int style = bold ? TTF_STYLE_BOLD : TTF_STYLE_NORMAL;
        TTF_SetFontStyle(font, style);
        
        int glyph_w;
        int glyph_h;
        TTF_SizeText(font, "M", &glyph_w, &glyph_h);
        
        int bytes_per_pixel = 4;
        out_font->glyph_w = glyph_w;
        out_font->glyph_h = glyph_h;
        out_font->glyphs_per_row = 16;
        out_font->glyphs_per_col = 16;
        out_font->w = 16*glyph_w;
        out_font->h = 16*glyph_h;
        out_font->pitch = bytes_per_pixel * out_font->w;
        
        out_font->data = (U32 *)mmap(0, (out_font->pitch * out_font->h),
                                     PROT_READ | PROT_WRITE,
                                     MAP_ANON | MAP_PRIVATE,
                                     -1, 0);
        
        for(int char_idx = 0; char_idx < 255; ++char_idx)
        {
            int atlas_x = char_idx % 16;
            int atlas_y = char_idx / 16;
            
            U32 *first_char_pixel = (out_font->data +
                                     (atlas_y * out_font->glyph_h * out_font->w) +
                                     (atlas_x * out_font->glyph_w));
            
            // NOTE(sokus): Color doesn't work, but we don't care since the letters are supposed
            // to be white anyway.
            SDL_Color text_color = {255, 255, 255, 255};
            SDL_Surface *surface = TTF_RenderGlyph_Blended(font, charset[char_idx], text_color); 
            
            // NOTE(sokus): We will not get a surface for (char)0 etc
            if(surface)
            {
                SDL_LockSurface(surface);
                
                for(int y = 0; y < surface->h; ++y)
                {
                    U32 *surface_pixel = (U32 *)((U8 *)surface->pixels + (y*surface->pitch));
                    for(int x = 0; x < surface->w; ++x)
                    {
                        U32 surface_a = (*surface_pixel >> 24) & 0xFF;
                        U32 surface_r = (*surface_pixel >> 16) & 0xFF;
                        U32 surface_g = (*surface_pixel >> 8) & 0xFF;
                        U32 surface_b = (*surface_pixel >> 0) & 0xFF;
                        
                        surface_r = (U32)((F32)surface_r * (F32)surface_a / 255.0f);
                        surface_g = (U32)((F32)surface_g * (F32)surface_a / 255.0f);
                        surface_b = (U32)((F32)surface_b * (F32)surface_a / 255.0f);
                        
                        U32 *dest_pixel = first_char_pixel + (y * out_font->w) + x;
                        *dest_pixel = ((surface_a << 24) |
                                       (surface_r << 16) |
                                       (surface_g << 8) |
                                       (surface_b << 0));
                        ++surface_pixel;
                    }
                }
                
                SDL_UnlockSurface(surface);
            }
            SDL_FreeSurface(surface);
        }
        
        result = true;
    }
    TTF_CloseFont(font);
    
    return result;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO |
             SDL_INIT_GAMECONTROLLER |
             SDL_INIT_HAPTIC);
    TTF_Init();
    
    FontPack font_pack = { .font_name = "Liberation Mono", .filename = "liberation-mono.ttf", .size = 24 };
    
    if(!SDLMakeAsciiFont(font_pack.filename, font_pack.size, &font_pack.regular, 0))
    {
        INVALID_CODE_PATH;
    }
    
    if(!SDLMakeAsciiFont(font_pack.filename, font_pack.size, &font_pack.bold, FontRasterFlag_Bold))
    {
        INVALID_CODE_PATH;
    }
    
    LinuxState linux_state = {0};
    LinuxInitEXEPath(&linux_state);
    
    char source_game_code_dll_path[PATH_MAX];
    LinuxAppendToParentDirectoryPath(&linux_state, "summoned.so", source_game_code_dll_path, PATH_MAX);
    LinuxGameCode game_code = LinuxLoadGameCode(source_game_code_dll_path);
    
    // U64 perf_count_frequency = SDL_GetPerformanceFrequency();
    
    SDLOpenGameControllers();
    
    SDL_Window *window = SDL_CreateWindow("Summoned",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          960,
                                          540,
                                          0);
    
    if(window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        
        int game_update_hz = 60;
        F32 target_seconds_per_frame = 1.0f / (F32)game_update_hz;
        
        if(renderer)
        {
            global_is_running = true;
            
            SDLWindowDimension dimension = SDLGetWindowDimension(window);
            SDLResizeTexture(&global_backbuffer, renderer, dimension.width, dimension.height);
            //SDLResizeTexture(&global_backbuffer, renderer, 960, 540);
            
            Input input[2] = {0};
            Input *new_input = &input[0];
            Input *old_input = &input[1];
            
#if SUMMONED_DEBUG
            // TODO(sokus): This will fail on 32-bit
            void *base_address = (void *)TERABYTES(2);
#else
            void *base_address = (void *)(0);
#endif
            
            GameMemory game_memory = {0};
            game_memory.permanent_storage_size = MEGABYTES(64);
            game_memory.transient_storage_size = GIGABYTES(4);
            
            size_t total_storage_size = game_memory.permanent_storage_size + game_memory.transient_storage_size;
            linux_state.total_size = total_storage_size;
            linux_state.game_memory_block = mmap(base_address, total_storage_size,
                                                 PROT_READ | PROT_WRITE,
                                                 MAP_ANON | MAP_PRIVATE,
                                                 -1, 0);
            ASSERT(linux_state.game_memory_block != MAP_FAILED);
            
            game_memory.permanent_storage = linux_state.game_memory_block;
            game_memory.transient_storage = ((U8 *)(linux_state.game_memory_block) +
                                             game_memory.permanent_storage_size); 
            
            U64 last_counter = SDL_GetPerformanceCounter();
            U64 last_cycle_count = _rdtsc();
            
            while(global_is_running)
            {
                
                timespec new_dll_write_time = LinuxGetLastWriteTime(source_game_code_dll_path);
                if(!LinuxTimespecsAreTheSame(new_dll_write_time, game_code.last_dll_write_time))
                {
                    printf("Reloading game code.\n");
                    LinuxUnloadGameCode(&game_code);
                    game_code = LinuxLoadGameCode(source_game_code_dll_path);
                }
                
                new_input->dt_for_frame = target_seconds_per_frame;
                
                ControllerInput *old_keyboard_controller = GetController(old_input, 0);
                ControllerInput *new_keyboard_controller = GetController(new_input, 0);
                *new_keyboard_controller = (ControllerInput){0};
                
                for(size_t button_idx = 0;
                    ++button_idx < ARRAY_COUNT(new_keyboard_controller->buttons);
                    ++button_idx)
                {
                    new_keyboard_controller->buttons[button_idx].ended_down =
                        old_keyboard_controller->buttons[button_idx].ended_down;
                }
                
                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                    SDLHandleEvent(&event, new_keyboard_controller);
                }
                
                for(int controller_idx = 0;
                    controller_idx < MAX_CONTROLLERS;
                    ++controller_idx)
                {
                    if(global_controller_handles[controller_idx] != 0 &&
                       SDL_GameControllerGetAttached(global_controller_handles[controller_idx]))
                    {
                        ControllerInput *old_controller = GetController(old_input, controller_idx+1);
                        ControllerInput *new_controller = GetController(new_input, controller_idx+1);
                        
                        new_controller->is_connected = true;
                        
                        bool move_up = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                   SDL_CONTROLLER_BUTTON_DPAD_UP);
                        bool move_left = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                     SDL_CONTROLLER_BUTTON_DPAD_LEFT);
                        bool move_down = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                     SDL_CONTROLLER_BUTTON_DPAD_DOWN);
                        bool move_right = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                      SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
                        bool action_up = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                     SDL_CONTROLLER_BUTTON_Y);
                        bool action_left = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                       SDL_CONTROLLER_BUTTON_X);
                        bool action_down = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                       SDL_CONTROLLER_BUTTON_A);
                        bool action_right = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                        SDL_CONTROLLER_BUTTON_B);
                        bool select = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                  SDL_CONTROLLER_BUTTON_BACK);
                        bool start = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                 SDL_CONTROLLER_BUTTON_START);
                        bool left_bumper = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                       SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
                        bool right_bumper = SDL_GameControllerGetButton(global_controller_handles[controller_idx],
                                                                        SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
                        SDLProcessGameControllerButton(&(old_controller->move_up),
                                                       &(new_controller->move_up),
                                                       move_up);
                        SDLProcessGameControllerButton(&(old_controller->move_left),
                                                       &(new_controller->move_left),
                                                       move_left);
                        SDLProcessGameControllerButton(&(old_controller->move_down),
                                                       &(new_controller->move_down),
                                                       move_down);
                        SDLProcessGameControllerButton(&(old_controller->move_right),
                                                       &(new_controller->move_right),
                                                       move_right);
                        SDLProcessGameControllerButton(&(old_controller->action_up),
                                                       &(new_controller->action_up),
                                                       action_up);
                        SDLProcessGameControllerButton(&(old_controller->action_left),
                                                       &(new_controller->action_left),
                                                       action_left);
                        SDLProcessGameControllerButton(&(old_controller->action_down),
                                                       &(new_controller->action_down),
                                                       action_down);
                        SDLProcessGameControllerButton(&(old_controller->action_right),
                                                       &(new_controller->action_right),
                                                       action_right);
                        SDLProcessGameControllerButton(&(old_controller->select),
                                                       &(new_controller->select),
                                                       select);
                        SDLProcessGameControllerButton(&(old_controller->start),
                                                       &(new_controller->start),
                                                       start);
                        SDLProcessGameControllerButton(&(old_controller->left_bumper),
                                                       &(new_controller->left_bumper),
                                                       left_bumper);
                        SDLProcessGameControllerButton(&(old_controller->right_bumper),
                                                       &(new_controller->right_bumper),
                                                       right_bumper);
                        
                        I16 stick_average_x_raw =
                            SDL_GameControllerGetAxis(global_controller_handles[controller_idx], 
                                                      SDL_CONTROLLER_AXIS_LEFTX);
                        new_controller->stick_average_x = SDLProcessGameControllerAxisValue(stick_average_x_raw, 1);
                        
                        I16 stick_average_y_raw =
                            SDL_GameControllerGetAxis(global_controller_handles[controller_idx],
                                                      SDL_CONTROLLER_AXIS_LEFTY);
                        new_controller->stick_average_y = -SDLProcessGameControllerAxisValue(stick_average_y_raw, 1);
                        
                        if((new_controller->stick_average_x != 0.0f) ||
                           (new_controller->stick_average_y != 0.0f))
                        {
                            new_controller->is_analog = true;
                        }
                        
                    }
                    else
                    {
                        // NOTE(sokus): This controller is not plugged in.
                    }
                }
                
                buffer.memory = global_backbuffer.memory;
                buffer.width = global_backbuffer.width;
                buffer.height = global_backbuffer.height;
                buffer.pitch = global_backbuffer.pitch;
                buffer.bytes_per_pixel = global_backbuffer.bytes_per_pixel;
                
                if(game_code.game_update_and_render)
                {
                    game_code.game_update_and_render(&game_memory, new_input, &buffer, &font_pack);
                }
                
                // Get this before SDLUpdateWindow() so that we don't keep missing VBlanks.
                U64 work_counter = SDL_GetPerformanceCounter();
                
                if(SDLGetSecondsElapsed(last_counter, work_counter) < target_seconds_per_frame)
                {
                    F32 time_to_sleep_in_seconds = target_seconds_per_frame -
                        SDLGetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()); 
                    I32 time_to_sleep_in_ms = (I32)(time_to_sleep_in_seconds * 1000) + 1;
                    
                    if(time_to_sleep_in_ms > 0)
                    {
                        SDL_Delay(time_to_sleep_in_ms);
                    }
                    
                    while(SDLGetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()) <
                          target_seconds_per_frame)
                    {
                        printf("CPU SPIN\n");
                        // CPU SPIN
                    }
                }
                
                SDLUpdateWindow(&global_backbuffer, renderer);
                
                U64 end_cycle_count = _rdtsc();
                U64 cycles_elapsed = end_cycle_count - last_cycle_count;
                
                Input *temp = new_input;
                new_input = old_input;
                old_input = temp;
                
                last_cycle_count = _rdtsc();
                last_counter = SDL_GetPerformanceCounter();
            }
        }
    }
    
    SDLCloseGameControllers();
    
    LinuxUnloadGameCode(&game_code);
    
    TTF_Quit();
    SDL_Quit();
    return 0;
}
