/* date = August 2nd 2021 0:08 pm */

#ifndef SUMMONED_PLATFORM_H
#define SUMMONED_PLATFORM_H

//~NOTE(sokus): compiler flags
/*
* SUMMONED_DEBUG - enable ASSERT, debug functions
  */

//~NOTE(sokus): keywords
#define internal static
#define global static
#define local_persist static

//~NOTE(sokus): types
#include <stddef.h>
#include <stdint.h>


typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef uint32_t B32;
#define TRUE 1
#define FALSE 0

typedef float F32;
typedef float F64;

typedef size_t SizeType;
typedef size_t MemoryIndex;

//~NOTE(sokus): macros

#define STATEMENT(statement) do { statement } while (0)
#define ASSERT_BREAK() (*(int *)0 = 0)

#if SUMMONED_DEBUG
# define ASSERT(expression) STATEMENT( if (!(expression)) { ASSERT_BREAK(); } ) 
#else
# define ASSERT(expression)
#endif

#define INVALID_CODE_PATH ASSERT(!"Invalid Code Path")

#define UNUSED(argument) (void)(argument)

#define KILOBYTES(value) ((value)*1024LL)
#define MEGABYTES(value) (KILOBYTES(value)*1024LL)
#define GIGABYTES(value) (MEGABYTES(value)*1024LL)
#define TERABYTES(value) (GIGABYTES(value)*1024LL)

#define ARRAY_COUNT(array) ( sizeof(array)/sizeof((array)[0]) )

#define MIN(a, b) (((a)<(b)) ? (a) : (b))
#define MAX(a, b) (((a)>(b)) ? (a) : (b))
#define CLAMP(a, x, b) (((x)<(a))?(a):\
((x)>(b))?(b):(x))
#define CLAMP_TOP(a, b) MIN(a, b)
#define CLAMP_BOT(a, b) MAX(a, b)



//~NOTE(sokus): Services that the game provides to the platform layer

typedef struct GameOffscreenBuffer
{
    // NOTE(casey): Pixels are always 32-bits wide, Memory Order BB GG RR XX
    void *memory;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel;
} GameOffscreenBuffer;

typedef struct GameMemory
{
    B32 is_initialized;
    
    U64 permanent_storage_size;
    void *permanent_storage; // NOTE(casey): REQUIRED to be cleared to zero at startup
    
    U64 transient_storage_size;
    void *transient_storage; // NOTE(casey): REQUIRED to be cleared to zero at startup
} GameMemory;


typedef struct GameButtonState
{
    int half_transition_count;
    B32 ended_down;
} GameButtonState;

typedef struct GameControllerInput
{
    B32 is_connected;
    B32 is_analog;
    F32 stick_average_x;
    F32 stick_average_y;
    
    union
    {
        GameButtonState buttons[12];
        struct
        {
            GameButtonState move_up;
            GameButtonState move_down;
            GameButtonState move_left;
            GameButtonState move_right;
            
            GameButtonState action_up;
            GameButtonState action_down;
            GameButtonState action_left;
            GameButtonState action_right;
            
            GameButtonState left_bumper;
            GameButtonState right_bumper;
            
            GameButtonState select;
            GameButtonState start;
            
            // NOTE(casey): All buttons must be added above this line
            
            GameButtonState terminator;
        };
    };
} GameControllerInput;

typedef struct GameInput
{
    GameButtonState mouse_buttons[5];
    I32 mouse_x, mouse_y, mouse_z;
    
    F32 dt_for_frame;
    
    GameControllerInput controllers[5];
} GameInput;

GameControllerInput *
GetController(GameInput *input, unsigned int controller_index)
{
    U32 count = ARRAY_COUNT(input->controllers);
    ASSERT(controller_index < count);
    
    GameControllerInput *result = &input->controllers[controller_index];
    return result;
}

typedef void GameUpdateAndRenderType(GameMemory *memory,
                                     GameInput *input,
                                     GameOffscreenBuffer *buffer);

#endif //SUMMONED_PLATFORM_H