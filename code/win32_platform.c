#include "summoned_platform.h"

#include <windows.h>
#include <xinput.h>
#include <stdio.h>

#include "win32_platform.h"

#include "summoned_font_maps.c"

global bool global_running;
global bool global_in_focus;
global Win32OffscreenBuffer global_backbuffer;
global I64 global_perf_count_frequency;
global bool debug_global_show_cursor;
global WINDOWPLACEMENT global_window_position = { .length=sizeof(global_window_position) };

typedef DWORD WINAPI XInputGetStateType(DWORD dwUserIndex, XINPUT_STATE *pState);
DWORD WINAPI XInputGetStateStub(DWORD dwUserIndex, XINPUT_STATE *pState)
{
    UNUSED(dwUserIndex);
    UNUSED(pState);
    return ERROR_DEVICE_NOT_CONNECTED;
}
global XInputGetStateType *global_xinput_get_state = &XInputGetStateStub;

typedef DWORD WINAPI XInputSetStateType(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);
DWORD WINAPI XInputSetStateStub(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
{
    UNUSED(dwUserIndex);
    UNUSED(pVibration);
    return ERROR_DEVICE_NOT_CONNECTED;
}
global XInputSetStateType *global_xinput_set_state = &XInputSetStateStub;

LARGE_INTEGER
Win32GetWallClock(void)
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result;
}

F32
Win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
    F32 result;
    result = ((F32)(end.QuadPart - start.QuadPart) /
              (F32)global_perf_count_frequency);
    return result;
}

internal void
Win32InitEXEPath(Win32State *state)
{
    DWORD size_of_filename = GetModuleFileNameA(0, state->exe_path, sizeof(state->exe_path));
    state->one_past_last_exe_path_slash = state->exe_path;
    for(U32 offset = 0;
        *(state->exe_path+offset) && offset < size_of_filename;
        ++offset)
    {
        if(*(state->exe_path+offset) == '\\')
        {
            state->one_past_last_exe_path_slash = state->exe_path + offset + 1;
        }
    }
}

internal void
Win32BuildEXEPathFilename(Win32State *state, char *filename, int dest_count, char *dest)
{
    size_t path_count = (state->one_past_last_exe_path_slash - state->exe_path);
    CatStrings(path_count, state->exe_path,
               StringLength(filename), filename,
               dest_count, dest);
}

FILETIME
Win32GetLastWriteTime(char *filename)
{
    FILETIME result = {0};
    
    WIN32_FILE_ATTRIBUTE_DATA data;
    if(GetFileAttributesEx(filename, GetFileExInfoStandard, &data))
    {
        result = data.ftLastWriteTime;
    }
    
    return result;
}

internal Win32GameCode
Win32LoadGameCode(char *source_dll_name, char *temp_dll_name, char *lock_name)
{
    Win32GameCode result = {0};
    
    WIN32_FILE_ATTRIBUTE_DATA ignored;
    if(!GetFileAttributesEx(lock_name, GetFileExInfoStandard, &ignored))
    {
        result.last_dll_write_time = Win32GetLastWriteTime(source_dll_name);
        
        CopyFile(source_dll_name, temp_dll_name, false);
        
        result.game_code_dll = LoadLibraryA(temp_dll_name);
        
        if(result.game_code_dll)
        {
            union GameUpdateAndRenderUnion
            {
                GameUpdateAndRenderType *p1;
                FARPROC p2;
            } game_update_and_render_union = {0};
            
            game_update_and_render_union.p2 = GetProcAddress(result.game_code_dll, "GameUpdateAndRender");
            result.update_and_render = game_update_and_render_union.p1;
            
            result.is_valid = (result.update_and_render) != 0;
        }
    }
    
    if(!result.is_valid)
    {
        result.update_and_render = 0;
    }
    
    return result;
}

internal void
Win32UnloadGameCode(Win32GameCode *game_code)
{
    if(game_code->game_code_dll)
    {
        FreeLibrary(game_code->game_code_dll);
        game_code->game_code_dll = 0;
    }
    
    game_code->update_and_render = 0;
    game_code->is_valid = false;
}

internal bool
Win32RegisterFontFile(char *filename_utf8)
{
    bool result = !!AddFontResourceExA((LPCSTR)filename_utf8, FR_PRIVATE, 0);
    return result;
}

internal bool
Win32MakeAsciiFont(char *font_name_utf8, int font_size, Font *out_font, FontRasterFlags flags)
{
    bool result = false;
    
    bool raster_font = !!(flags & FONT_RASTER_FLAG_RASTER_FONT);
    bool dont_map_unicode = !!(flags & FONT_RASTER_FLAG_DONT_MAP_UNICODE);
    bool bold = !!(flags & FONT_RASTER_FLAG_BOLD);
    
    *out_font = (Font){0};
    
    HDC screen_dc = GetDC(0);
    ASSERT(screen_dc);
    
    HDC font_dc = CreateCompatibleDC(screen_dc);
    ASSERT(font_dc);
    
    ReleaseDC(0, screen_dc);
    
    int height = font_size;
    
    DWORD out_precision = raster_font ? OUT_RASTER_PRECIS : OUT_DEFAULT_PRECIS;
    DWORD quality = raster_font ? NONANTIALIASED_QUALITY : PROOF_QUALITY;
    int font_weight = bold ? FW_BOLD : FW_NORMAL;
    
    HFONT font = CreateFontA(height, 0, 0, 0,
                             font_weight,
                             false,
                             false,
                             false,
                             DEFAULT_CHARSET,
                             out_precision,
                             CLIP_DEFAULT_PRECIS,
                             quality,
                             DEFAULT_PITCH|FF_DONTCARE,
                             font_name_utf8);
    
    U16 *charset = (dont_map_unicode ? codepage_437_ascii : codepage_437_utf16);
    
    if(font)
    {
        if(!SelectObject(font_dc, font))
        {
            INVALID_CODE_PATH;
        }
        
        SIZE size;
        if (!GetTextExtentPoint32A(font_dc, "M", 1, &size))
        {
            INVALID_CODE_PATH;
        }
        
        BITMAPINFO font_bitmap_info = {0};
        BITMAPINFOHEADER *header = &font_bitmap_info.bmiHeader;
        header->biSize = sizeof(*header);
        header->biWidth = 16*size.cx;
        header->biHeight = -16*size.cy;
        header->biPlanes = 1;
        header->biBitCount = 32;
        header->biCompression = BI_RGB;
        
        VOID *font_bits = 0;
        HBITMAP font_bitmap = CreateDIBSection(font_dc, &font_bitmap_info, DIB_RGB_COLORS, &font_bits, 0, 0);
        if(font_bitmap)
        {
            SelectObject(font_dc, font_bitmap);
            SetBkColor(font_dc, RGB(0, 0, 0));
            SetTextAlign(font_dc, TA_TOP|TA_LEFT);
            
            out_font->glyph_w = size.cx;
            out_font->glyph_h = size.cy;
            out_font->glyphs_per_row = 16;
            out_font->glyphs_per_col = 16;
            out_font->w = 16*size.cx;
            out_font->h = 16*size.cy;
            out_font->pitch = out_font->w;
            out_font->data = (U32 *)font_bits;
            
            SetTextColor(font_dc, RGB(255, 255, 255));
            for(int i=0; i<255; ++i)
            {
                int x = i % 16;
                int y = i / 16;
                TextOutW(font_dc, out_font->glyph_w*x, out_font->glyph_h*y, charset + i, 1);
            }
            
            U32 *pixels = out_font->data;
            for(int i=0; i<out_font->pitch*out_font->h; ++i)
            {
                if(*(pixels+i))
                {
                    *(pixels+i) |= 0xFF000000;
                }
            }
            
            result = true;
        }
        
        DeleteObject(font);
    }
    else
    {
        // TODO(sokus): Logging
    }
    
    DeleteDC(font_dc);
    return result;
}

internal void
Win32LoadXInput(void)
{
    // TODO(casey): Test this on Windows 8
    HMODULE xinput_library = LoadLibraryA("xinput1_4.dll");
    if(!xinput_library)
    {
        xinput_library = LoadLibraryA("xinput9_1_0.dll");
    }
    
    if(!xinput_library)
    {
        xinput_library = LoadLibraryA("xinput1_3.dll");
    }
    
    if(xinput_library)
    {
        union XInputGetStateUnion
        {
            XInputGetStateType *p1;
            FARPROC p2;
        } xinput_get_state_union = {0};
        
        xinput_get_state_union.p2 = GetProcAddress(xinput_library, "XInputGetState");
        global_xinput_get_state = xinput_get_state_union.p1;
        
        if(!global_xinput_get_state)
        {
            global_xinput_get_state = &XInputGetStateStub;
        }
        
        union XInputSetStateUnion
        {
            XInputSetStateType *p1;
            FARPROC p2;
        } xinput_set_state_union = {0};
        
        xinput_set_state_union.p2 = GetProcAddress(xinput_library, "XInputSetState");
        global_xinput_set_state = xinput_set_state_union.p1;
        
        if(!global_xinput_set_state)
        {
            global_xinput_set_state = &XInputSetStateStub;
        }
    }
    else
    {
        
    }
}

internal void
Win32ProcessKeyboardMessage(GameButtonState *state, bool is_down)
{
    if(state->ended_down != is_down)
    {
        state->ended_down = is_down;
        ++(state->half_transition_count);
    }
}

internal void
Win32ProcessXInputDigitalButton(DWORD xinput_button_state, DWORD button_bit,
                                GameButtonState *old_state,
                                GameButtonState *new_state)
{
    new_state->ended_down = ((xinput_button_state & button_bit) == button_bit);
    new_state->half_transition_count = (old_state->ended_down != new_state->ended_down) ? 1 : 0;
}

internal F32
Win32ProcessXInputStickValue(SHORT value, SHORT deadzone_threshold)
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
Win32ResizeDIBSection(Win32OffscreenBuffer *buffer, int width, int height)
{
    // TODO(casey): Bulletproof this.
    // Maybe don't free first, free after, then free first if that fails.
    if(buffer->memory)
    {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }
    
    buffer->width = width;
    buffer->height = height;
    
    int bytes_per_pixel = 4;
    buffer->bytes_per_pixel = bytes_per_pixel;
    
    // NOTE(casey): When the biHeight field is negative, this is the clue to
    // Windows to treat this bitmap as top-down, not bottom-up, meaning that
    // the first three bytes of the image are the color for the top left pixel
    // in the bitmap, not the bottom left!
    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = -buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;
    
    // NOTE(casey): Thank you to Chris Hecker of Spy Party fame
    // for clarifying the deal with StretchDIBits and BitBlt!
    // No more DC for us.
    int bitmap_memory_size = buffer->width * buffer->height * buffer->bytes_per_pixel;
    buffer->memory = VirtualAlloc(0, bitmap_memory_size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    buffer->pitch = buffer->width * buffer->bytes_per_pixel;
    
    
    // TODO(casey): Probably clear this to black
}

internal Win32WindowDimension
Win32GetWindowDimension(HWND window)
{
    Win32WindowDimension result;
    
    RECT client_rect;
    GetClientRect(window, &client_rect);
    result.width = client_rect.right - client_rect.left;
    result.height = client_rect.bottom - client_rect.top;
    
    return result;
}

internal void
Win32DisplayBufferInWindow(Win32OffscreenBuffer *buffer,
                           HDC device_context,
                           int window_width,
                           int window_height)
{
    // TODO(casey): Centering / black bars?
    
    if((window_width >= buffer->width*2) &&
       (window_height >= buffer->height*2))
    {
        StretchDIBits(device_context,
                      0, 0, 2*buffer->width, 2*buffer->height,
                      0, 0, buffer->width, buffer->height,
                      buffer->memory,
                      &buffer->info,
                      DIB_RGB_COLORS, SRCCOPY);
    }
    else
    {
        int offset_x = 10;
        int offset_y = 10;
        
        PatBlt(device_context, 0, 0, window_width, offset_y, BLACKNESS);
        PatBlt(device_context, 0, offset_y+buffer->height, window_width, window_height, BLACKNESS);
        PatBlt(device_context, 0, 0, offset_x, window_height, BLACKNESS);
        PatBlt(device_context, offset_x+buffer->width, 0, window_width, window_height, BLACKNESS);
        
        // NOTE(casey): For prototyping purposes, we're going to always blit
        // 1-to-1 pixels to make sure we don't introduce artifacts with
        // stretching while we are learning to code the renderer!
        StretchDIBits(device_context,
                      offset_x, offset_y, buffer->width, buffer->height,
                      0, 0, buffer->width, buffer->height,
                      buffer->memory,
                      &buffer->info,
                      DIB_RGB_COLORS, SRCCOPY);
    }
}


internal LRESULT CALLBACK
Win32MainWindowCallback(HWND window,
                        UINT message,
                        WPARAM wparam,
                        LPARAM lparam)
{
    LRESULT result = 0;
    
    switch(message)
    {
        case WM_CLOSE:
        {
            // TODO(casey): Handle this with a message to the user?
            global_running = false;
        } break;
        
        case WM_SETCURSOR:
        {
            if(debug_global_show_cursor)
            {
                result = DefWindowProcA(window, message, wparam, lparam);
            }
            else
            {
                SetCursor(0);
            }
        } break;
        
        case WM_ACTIVATEAPP:
        {
            if(wparam == 1)
            {
                SetLayeredWindowAttributes(window, RGB(0, 0, 0), 255, LWA_ALPHA);
            }
            else
            {
                SetLayeredWindowAttributes(window, RGB(0, 0, 0), 64, LWA_ALPHA);
            }
        } break;
        
        case WM_DESTROY:
        {
            // TODO(casey): Handle this as an error - recreate window?
            global_running = false;
        } break;
#if 0
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            ASSERT(!"Keyboard input came in through a non-dispatch message!");
        } break;
#endif
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC device_context = BeginPaint(window, &paint);
            Win32WindowDimension dimension = Win32GetWindowDimension(window);
            Win32DisplayBufferInWindow(&global_backbuffer, device_context,
                                       dimension.width, dimension.height);
            EndPaint(window, &paint);
        } break;
        
        case WM_SETFOCUS:
        {
            global_in_focus = true;
        } break;
        
        case WM_KILLFOCUS:
        {
            global_in_focus = false;
        } break;
        
        default:
        {
            // OutputDebugStringA("default\n");
            result = DefWindowProcA(window, message, wparam, lparam);
        } break;
    }
    
    return result;
}

internal void
Win32ToggleFullscreen(HWND window)
{
    // NOTE(casey): This follows Raymond Chen's prescription
    // for fullscreen toggling, see:
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
    
    DWORD style = (DWORD)GetWindowLong(window, GWL_STYLE);
    if(style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO monitor_info = { .cbSize=sizeof(monitor_info) };
        if(GetWindowPlacement(window, &global_window_position) &&
           GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitor_info))
        {
            SetWindowLong(window, GWL_STYLE, (I32)style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(window, HWND_TOP,
                         monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                         monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                         monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(window, GWL_STYLE, (I32)style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(window, &global_window_position);
        SetWindowPos(window, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}


internal void
Win32ProcessPendingMessages(GameControllerInput *keyboard_controller)
{
    MSG message;
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_QUIT:
            {
                global_running = false;
            } break;
            
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                U32 vk_code = (U32)message.wParam;
                
                // NOTE(casey): Since we are comparing WasDown to IsDown,
                // we MUST use == and != to convert these bit tests to actualy
                // 0 or 1 value.
                bool was_down = ((message.lParam & (1 << 30)) != 0);
                bool is_down = ((message.lParam & (1 << 31)) == 0);
                if(was_down != is_down)
                {
                    if(vk_code == 'W')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->move_up, is_down);
                    }
                    else if(vk_code == 'A')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->move_left, is_down);
                    }
                    else if(vk_code == 'S')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->move_down, is_down);
                    }
                    else if(vk_code == 'D')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->move_right, is_down);
                    }
                    else if(vk_code == 'R')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->action_up, is_down);
                    }
                    else if(vk_code == 'F')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->action_left, is_down);
                    }
                    else if(vk_code == 'E')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->action_down, is_down);
                    }
                    else if(vk_code == 'Q')
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->action_right, is_down);
                    }
                    else if(vk_code == VK_ESCAPE)
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->start, is_down);
                    }
                    else if(vk_code == VK_TAB)
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->select, is_down);
                    }
                    else if(vk_code == VK_SHIFT)
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->right_bumper, is_down);
                    }
                    else if(vk_code == VK_CONTROL)
                    {
                        Win32ProcessKeyboardMessage(&keyboard_controller->left_bumper, is_down);
                    }
                    
                    
                }
                
                if(is_down)
                {
                    bool alt_key_was_down = (message.lParam & (1 << 29));
                    if((vk_code == VK_F4) && alt_key_was_down)
                    {
                        global_running = false;
                    }
                    else if ((vk_code == VK_RETURN) && alt_key_was_down)
                    {
                        if(message.hwnd)
                        {
                            Win32ToggleFullscreen(message.hwnd);
                        }
                    }
                }
            } break;
            
            default:
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            } break;
        }
    }
}

int CALLBACK
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR commandline,
        int show_code)
{
    UNUSED(prev_instance);
    UNUSED(commandline);
    UNUSED(show_code);
    
    Win32State win32_state = {0};
    
    LARGE_INTEGER perf_count_frequency_result;
    QueryPerformanceFrequency(&perf_count_frequency_result);
    global_perf_count_frequency = perf_count_frequency_result.QuadPart;
    
    Win32InitEXEPath(&win32_state);
    
    char source_game_code_dll_path[MAX_PATH];
    Win32BuildEXEPathFilename(&win32_state, "summoned.dll",
                              sizeof(source_game_code_dll_path), source_game_code_dll_path);
    
    char temp_game_code_dll_path[MAX_PATH];
    Win32BuildEXEPathFilename(&win32_state, "summoned_temp.dll",
                              sizeof(temp_game_code_dll_path), temp_game_code_dll_path);
    
    char game_code_lock_path[MAX_PATH];
    Win32BuildEXEPathFilename(&win32_state, "lock.tmp",
                              sizeof(game_code_lock_path), game_code_lock_path);
    
    // NOTE(casey): Set the Windows scheduler granularity to 1ms
    // so that our Sleep() can be more granular.
    UINT desired_scheduler_ms = 1;
    bool sleep_is_granular = (timeBeginPeriod(desired_scheduler_ms) == TIMERR_NOERROR);
    
    Win32LoadXInput();
    
#if SUMMONED_DEBUG
    debug_global_show_cursor = true;
#endif
    WNDCLASSA window_class = {0};
    
    /* NOTE(casey): 1080p display mode is 1920x1080 -> Half of that is 960x540
       1920 -> 2048 = 2048-1920 -> 128 pixels
       1080 -> 2048 = 2048-1080 -> pixels 968
       1024 + 128 = 1152
    */
    Win32ResizeDIBSection(&global_backbuffer, 960, 540);
    
    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = Win32MainWindowCallback;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    // window_class.hIcon;
    window_class.lpszClassName = "SummonedWindowClass";
    
    if(RegisterClassA(&window_class))
    {
        
        HWND window = CreateWindowExA(0, // WS_EX_TOPMOST|WS_EX_LAYERED,
                                      window_class.lpszClassName,
                                      "Summoned",
                                      WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      0,
                                      0,
                                      instance,
                                      0);
        
        if(window)
        {
            
            // TODO(casey): How do we reliably query this on Windows?
            int monitor_refresh_hz = 60;
            HDC refresh_dc = GetDC(window);
            int win32_refresh_rate = GetDeviceCaps(refresh_dc, VREFRESH);
            ReleaseDC(window, refresh_dc);
            if(win32_refresh_rate > 1)
            {
                monitor_refresh_hz = win32_refresh_rate;
            }
            F32 game_update_hz = ((F32)monitor_refresh_hz / 2.0f);
            F32 target_seconds_per_frame = 1.0f / (F32)game_update_hz;
            
            
            global_running = true;
#if SUMMONED_DEBUG
            LPVOID base_address = (LPVOID)TERABYTES(2);
#else
            LPVOID base_address = 0;
#endif
            
            GameMemory game_memory = {0};
            game_memory.permanent_storage_size = MEGABYTES(256);
            game_memory.transient_storage_size = GIGABYTES(1);
            
            // TODO(casey): Handle various memory footprints (USING
            // SYSTEM METRICS)
            
            // TODO(casey): Use MEM_LARGE_PAGES and
            // call adjust token privileges when not on Windows XP?
            
            // TODO(casey): TransientStorage needs to be broken up
            // into game transient and cache transient, and only the
            // former need be saved for state playback.
            win32_state.total_size = 
                game_memory.permanent_storage_size + game_memory.transient_storage_size;
            win32_state.game_memory_block = VirtualAlloc(base_address, win32_state.total_size,
                                                         MEM_RESERVE|MEM_COMMIT,
                                                         PAGE_READWRITE);
            game_memory.permanent_storage = win32_state.game_memory_block;
            game_memory.transient_storage = ((U8 *)game_memory.permanent_storage +
                                             game_memory.permanent_storage_size);
            
            FontPack font_pack = { .name = "Liberation Mono", .filename="liberation-mono.ttf", .size = 24 };
            char font_path[MAX_PATH];
            Win32BuildEXEPathFilename(&win32_state, font_pack.filename,
                                      sizeof(font_path), font_path);
            
            if(!Win32RegisterFontFile(font_path))
            {
                INVALID_CODE_PATH;
            }
            
            
            if(!Win32MakeAsciiFont(font_pack.name, font_pack.size, &font_pack.regular,
                                   FONT_RASTER_FLAG_RASTER_FONT))
            {
                INVALID_CODE_PATH;
            }
            
            if(!Win32MakeAsciiFont(font_pack.name, font_pack.size, &font_pack.bold,
                                   FONT_RASTER_FLAG_RASTER_FONT|FONT_RASTER_FLAG_BOLD))
            {
                INVALID_CODE_PATH;
            }
            
            if(game_memory.permanent_storage && game_memory.transient_storage)
            {
                LARGE_INTEGER last_counter = Win32GetWallClock();
                LARGE_INTEGER flip_wall_clock = Win32GetWallClock();
                // LARGE_INTEGER last_code_reload_check = Win32GetWallClock();
                
                GameInput input[2] = {0};
                GameInput *new_input = &input[0];
                GameInput *old_input = &input[1];
                
                Win32GameCode game = Win32LoadGameCode(source_game_code_dll_path,
                                                       temp_game_code_dll_path,
                                                       game_code_lock_path);
                while(global_running)
                {
                    FILETIME new_dll_write_time = Win32GetLastWriteTime(source_game_code_dll_path);
                    if(CompareFileTime(&new_dll_write_time, &game.last_dll_write_time) != 0)
                    {
                        Win32UnloadGameCode(&game);
                        game = Win32LoadGameCode(source_game_code_dll_path,
                                                 temp_game_code_dll_path,
                                                 game_code_lock_path);
                    }
                    
                    new_input->dt_for_frame = target_seconds_per_frame;
                    
                    // TODO(casey): Zeroing macro
                    // TODO(casey): We can't zero everything because the up/down state will
                    // be wrong!!!
                    GameControllerInput *old_keyboard_controller = GetController(old_input, 0);
                    GameControllerInput *new_keyboard_controller = GetController(new_input, 0);
                    *new_keyboard_controller = (GameControllerInput){0};
                    new_keyboard_controller->is_connected = true;
                    for(U64 button_index = 0;
                        button_index < ARRAY_COUNT(new_keyboard_controller->buttons);
                        ++button_index)
                    {
                        new_keyboard_controller->buttons[button_index].ended_down =
                            old_keyboard_controller->buttons[button_index].ended_down;
                    }
                    
                    Win32ProcessPendingMessages(new_keyboard_controller);
                    
                    if(!global_in_focus)
                    {
                        for(U32 button_index = 0;
                            button_index < ARRAY_COUNT(new_keyboard_controller->buttons);
                            ++button_index)
                        {
                            Win32ProcessKeyboardMessage(&new_keyboard_controller->buttons[button_index], false);
                        }
                    }
                    
                    POINT mouse_position;
                    GetCursorPos(&mouse_position);
                    ScreenToClient(window, &mouse_position);
                    new_input->mouse_x = mouse_position.x;
                    new_input->mouse_y = mouse_position.y;
                    new_input->mouse_z = 0; // TODO(casey): Support mousewheel?
                    Win32ProcessKeyboardMessage(&new_input->mouse_buttons[0],
                                                GetKeyState(VK_LBUTTON) & (1<<15));
                    Win32ProcessKeyboardMessage(&new_input->mouse_buttons[1],
                                                GetKeyState(VK_MBUTTON) & (1<<15));
                    Win32ProcessKeyboardMessage(&new_input->mouse_buttons[2],
                                                GetKeyState(VK_RBUTTON) & (1<<15));
                    Win32ProcessKeyboardMessage(&new_input->mouse_buttons[3],
                                                GetKeyState(VK_XBUTTON1) & (1<<15));
                    Win32ProcessKeyboardMessage(&new_input->mouse_buttons[4],
                                                GetKeyState(VK_XBUTTON2) & (1<<15));
                    
                    // TODO(casey): Need to not poll disconnected controllers to avoid
                    // xinput frame rate hit on older libraries...
                    // TODO(casey): Should we poll this more frequently
                    DWORD max_controller_count = XUSER_MAX_COUNT;
                    max_controller_count =
                        CLAMP_TOP(max_controller_count, (ARRAY_COUNT(new_input->controllers) - 1));
                    
                    for(DWORD controller_index = 0;
                        controller_index < max_controller_count;
                        ++controller_index)
                    {
                        DWORD our_controller_index = controller_index + 1;
                        GameControllerInput *old_controller = GetController(old_input, our_controller_index);
                        GameControllerInput *new_controller = GetController(new_input, our_controller_index);
                        
                        XINPUT_STATE controller_state;
                        if(global_xinput_get_state(controller_index, &controller_state) == ERROR_SUCCESS)
                        {
                            new_controller->is_connected = true;
                            new_controller->is_analog = old_controller->is_analog;
                            
                            
                            // NOTE(casey): This controller is plugged in
                            // TODO(casey): See if ControllerState.dwPacketNumber increments too rapidly
                            XINPUT_GAMEPAD *pad = &controller_state.Gamepad;
                            
                            // TODO(casey): This is a square deadzone, check XInput to
                            // verify that the deadzone is "round" and show how to do
                            // round deadzone processing.
                            new_controller->stick_average_x =
                                Win32ProcessXInputStickValue(pad->sThumbLX,
                                                             XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                            new_controller->stick_average_y =
                                Win32ProcessXInputStickValue(pad->sThumbLY,
                                                             XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                            if((new_controller->stick_average_x != 0.0f) ||
                               (new_controller->stick_average_y != 0.0f))
                            {
                                new_controller->is_analog = true;
                            }
                            else
                            {
                                new_controller->is_analog = false;
                            }
                            
                            bool dpad_up = !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                            bool dpad_left = !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                            bool dpad_down = !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                            bool dpad_right = !!(pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                            
                            Win32ProcessXInputDigitalButton(dpad_left,
                                                            1,
                                                            &old_controller->move_left,
                                                            &new_controller->move_left);
                            Win32ProcessXInputDigitalButton(dpad_right,
                                                            1,
                                                            &old_controller->move_right,
                                                            &new_controller->move_right);
                            Win32ProcessXInputDigitalButton(dpad_down,
                                                            1,
                                                            &old_controller->move_down,
                                                            &new_controller->move_down);
                            Win32ProcessXInputDigitalButton(dpad_up,
                                                            1,
                                                            &old_controller->move_up,
                                                            &new_controller->move_up);
                            
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_A,
                                                            &old_controller->action_down,
                                                            &new_controller->action_down);
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_B,
                                                            &old_controller->action_right,
                                                            &new_controller->action_right);
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_X,
                                                            &old_controller->action_left,
                                                            &new_controller->action_left);
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_Y,
                                                            &old_controller->action_up,
                                                            &new_controller->action_up);
                            
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER,
                                                            &old_controller->left_bumper,
                                                            &new_controller->left_bumper);
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER,
                                                            &old_controller->right_bumper,
                                                            &new_controller->right_bumper);
                            
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_BACK,
                                                            &old_controller->select,
                                                            &new_controller->select);
                            Win32ProcessXInputDigitalButton(pad->wButtons, XINPUT_GAMEPAD_START,
                                                            &old_controller->start,
                                                            &new_controller->start);
                        }
                        else
                        {
                            new_controller->is_connected = false;
                        }
                        
                    }
                    
                    
                    GameOffscreenBuffer buffer = {0};
                    buffer.memory = global_backbuffer.memory;
                    buffer.width = global_backbuffer.width;
                    buffer.height = global_backbuffer.height;
                    buffer.pitch = global_backbuffer.pitch;
                    buffer.bytes_per_pixel = global_backbuffer.bytes_per_pixel;
                    
                    LARGE_INTEGER game_work_start = Win32GetWallClock();
                    
                    if(game.update_and_render)
                    {
                        game.update_and_render(&game_memory, new_input, &buffer, &font_pack);
                    }
                    
                    LARGE_INTEGER work_counter = Win32GetWallClock();
                    F32 platform_work_seconds_elapsed = Win32GetSecondsElapsed(last_counter, game_work_start);
                    F32 game_work_seconds_elapsed = Win32GetSecondsElapsed(game_work_start, work_counter);
                    
                    // TODO(casey): NOT TESTED YET! PROBABLY BUGGY!!!!!
                    F32 seconds_elapsed_for_frame = platform_work_seconds_elapsed + game_work_seconds_elapsed;
                    if(seconds_elapsed_for_frame < target_seconds_per_frame)
                    {
                        if(sleep_is_granular)
                        {
                            DWORD sleep_ms = (DWORD)(1000.0f *(target_seconds_per_frame -
                                                               seconds_elapsed_for_frame));
                            if(sleep_ms > 0)
                            {
                                Sleep(sleep_ms);
                            }
                        }
                        
                        F32 test_seconds_elapsed_for_frame =
                            Win32GetSecondsElapsed(last_counter,
                                                   Win32GetWallClock());
                        if(test_seconds_elapsed_for_frame < target_seconds_per_frame)
                        {
                            // TODO(casey): LOG MISSED SLEEP HERE
                        }
                        
                        // NOTE(sokus): CPU BURNER
                        while(seconds_elapsed_for_frame < target_seconds_per_frame)
                        {
                            seconds_elapsed_for_frame =
                                Win32GetSecondsElapsed(last_counter,
                                                       Win32GetWallClock());
                        }
                    }
                    else
                    {
                        // TODO(casey): MISSED FRAME RATE!
                        // TODO(casey): Logging
                    }
                    
                    LARGE_INTEGER end_counter = Win32GetWallClock();
                    // F32 ms_per_frame = 1000.0f*Win32GetSecondsElapsed(last_counter, end_counter);
                    last_counter = end_counter;
                    
                    Win32WindowDimension dimension = Win32GetWindowDimension(window);
                    HDC device_context = GetDC(window);
                    Win32DisplayBufferInWindow(&global_backbuffer, device_context,
                                               dimension.width, dimension.height);
                    ReleaseDC(window, device_context);
                    
                    flip_wall_clock = Win32GetWallClock();
                    
                    GameInput *swap_input = new_input;
                    new_input = old_input;
                    old_input = swap_input;
                }
            }
            else
            {
                // TODO(casey): Logging
            }
        }
        else
        {
            // TODO(casey): Logging
        }
    }
    else
    {
        // TODO(casey): Logging
    }
    
    return 0;
}
