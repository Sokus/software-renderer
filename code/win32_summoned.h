#ifndef WIN32_SUMMONED_H
#define WIN32_SUMMONED_H

#include "summoned_platform.h"

#include <windows.h>

typedef struct win32_offscreen_buffer
{
    // NOTE(casey): Pixels are always 32-bits wide, Memory Order BB GG RR XX
    BITMAPINFO info;
    void* memory;
    int width;
    int height;
    int pitch;
} win32_offscreen_buffer;

typedef struct win32_window_dimension
{
    int width;
    int height;
} win32_window_dimension;

typedef struct win32_sound_output
{
    int samples_per_second;
    uint32 running_sample_index;
    int bytes_per_sample;
    int secondary_buffer_size;
    real32 sine_t;
    int latency_sample_count;
} win32_sound_output;

#endif