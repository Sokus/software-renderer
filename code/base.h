/* date = September 6th 2021 1:04 pm */

#ifndef BASE_H
#define BASE_H

//~NOTE(sokus): compiler flags
/*
* SUMMONED_DEBUG - enable ASSERT, debug functions
  */

//~NOTE(sokus): necessary includes
#include <string.h> // memcpy, memset
#include <stdio.h> // vsnprintf
#include <stdarg.h> // variadic arguments

//~NOTE(sokus): keywords
#define internal static
#define global static
#define local_persist static

//~NOTE(sokus): types
#include <stddef.h> // size_t
#include <stdbool.h> // bool

#include <stdint.h> // int8_t, uint16_t...

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef unsigned int uint;

typedef float F32;
typedef double F64;

//~NOTE(sokus): common macros

#define STATEMENT(statement) do { statement } while (0)
//#define ASSERT_BREAK() (*(int *)0 = 0)
#define ASSERT_BREAK() (*((char*)-1) = 'x')

#if SUMMONED_DEBUG
# define ASSERT(expression) STATEMENT( if (!(expression)) { ASSERT_BREAK(); } ) 
#else
# define ASSERT(expression)
#endif

#define INVALID_CODE_PATH ASSERT("Invalid Code Path" == 0)

#define OFFSET_OF(struct_type, member_name) ((size_t) &((struct_type *)0)->member_name)

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

#define ABS(a) ((a) >= 0) ? (a) : -(a)

#define SWAP(a, b, type) STATEMENT(type swap=a; a=b; b=swap;)

#define PI32 3.14159265359f

#define SET_FLAG(bits, flag, value) STATEMENT(if(value) {(bits) |= (flag);} else {(bits) &= ~(flag);})

//~NOTE(sokus): Singly Linked Lists

#define SLL_QUEUE_PUSH_BACK_EXPLICIT(f, l, n, next) ((f)==0?\
((f)=(l)=(n)):\
((l)->next=(n), (l)=(n)),\
(n)->next=0)

#define SLL_QUEUE_PUSH_BACK(f, l, n) SLL_QUEUE_PUSH_BACK_EXPLICIT(f, l, n, next)

#define SLL_QUEUE_PUSH_FRONT_EXPLICIT(f, l, n, next) ((f)==0?\
((f)=(l)=(n), (n)->next=0):\
((n)->next=(f), (f)=(n)))

#define SLL_QUEUE_PUSH_FRONT(f, l, n) SLL_QUEUE_PUSH_FRONT_EXPLICIT(f, l, n, next)

#define SLL_QUEUE_POP_EXPLICIT(f, l, next) ((f)==(l)?\
((f)=(l)=0):\
((f)=(f)->next))

#define SLL_QUEUE_POP(f, l) SLL_QUEUE_POP_EXPLICIT(f, l, next)

#define SLL_STACK_PUSH_EXPLICIT(f, n, next) ((f)==0?\
((f)=(n)):\
((n)->next=(f), (f)=(n)))

#define SLL_STACK_PUSH(f, n) SLL_STACK_PUSH_EXPLICIT(f, n, next)

#define SLL_STACK_POP_EXPLICIT(f, next) ((f)==0?\
0:\
((f)=(f)->next))

#define SLL_STACK_POP(f) SLL_STACK_POP_EXPLICIT(f, next)

//~NOTE(sokus): Doubly Linked Lists
#define DLL_PUSH_BACK_EXPLICIT(f, l, n, next, prev) ((f)==0?\
((f)=(l)=(n), (n)->next=(n)->prev=0):\
((n)->prev=(l), (l)->next=(n), (l)=(n)),\
(n)->next=0)

#define DLL_PUSH_BACK(f, l, n) DLL_PUSH_BACK_EXPLICIT(f, l, n, next, prev)

#define DLL_PUSH_FRONT_EXPLICIT(f, l, n, next, prev) DLL_PUSH_BACK_EXPLICIT(l, f, n, prev, next)
#define DLL_PUSH_FRONT(f, l, n) DLL_PUSH_FRONT_EXPLICIT(f, l, n, next, prev)

#if 0
#define DLL_REMOVE_EXPLICIT(f, l, n, next, prev) ((f)==(n)?\
((f)=(f)->next,(f)->prev=0):\
((l)==(n)?\
((l)=(l)->prev, (l)->next=0):\
((n)->next->prev=(n)->prev,(n)->prev->next=(n)->next)))
#else
#define DLL_REMOVE_EXPLICIT(f, l, n, next, prev) ((f)==(n)?(f)=(f)->next:0,\
(l)==(n)?(l)=(l)->prev:0,\
(n->next)?(n)->next->prev=(n)->prev:0,\
(n->prev)?(n)->prev->next=(n)->next:0)
#endif

#define DLL_REMOVE(f, l, n) DLL_REMOVE_EXPLICIT(f, l, n, next, prev);

//~NOTE(sokus): memcpy, memset wrappers

#define MEMORY_COPY(destination, source, size) memcpy(destination, source, size)
#define MEMORY_SET(destination, value, size) memset(destination, value, size)


//~NOTE(sokus): memory arenas

typedef struct MemoryArena
{
    U8 *base;
    size_t size;
    size_t used;
} MemoryArena;

internal void
InitializeArena(MemoryArena *arena, U8 *base, size_t size)
{
    arena->base = base;
    arena->size = size;
    arena->used = 0;
}

#define PUSH_STRUCT(arena, type) (type *)MemoryArenaPushSize(arena, sizeof(type))
#define PUSH_ARRAY(arena, type, count) (type *)MemoryArenaPushSize(arena, (count)*sizeof(type)) 

void *
MemoryArenaPushSize(MemoryArena *arena, size_t size)
{
    ASSERT((arena->used + size) <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    
    return result;
}

void *
MemoryArenaPopSize(MemoryArena *arena, size_t size)
{
    ASSERT(size > 0);
    size_t new_used = arena->used - size;
    ASSERT(new_used >= 0);
    arena->used = new_used;
    void *result = arena->base + new_used;
    return result;
}

//~NOTE(sokus): string

void
ConcatenateStrings(char *str_a, size_t str_a_size,
                   char *str_b, size_t str_b_size,
                   char *dest, size_t dest_size)
{
    size_t dest_size_clamped = CLAMP_BOT(0, dest_size);
    size_t str_a_size_clamped = CLAMP(0, str_a_size, dest_size_clamped);
    size_t size_remaining = dest_size_clamped - str_a_size_clamped;
    size_t str_b_size_clamped = CLAMP(0, str_b_size, size_remaining);
    
    MEMORY_COPY(dest, str_a, str_a_size_clamped);
    MEMORY_COPY(dest+str_a_size_clamped, str_b, str_b_size_clamped);
    
    *(dest+str_a_size_clamped+str_b_size_clamped) = 0;
}

uint
StringLength(char *str)
{
    uint result = 0;
    while(*str++)
    {
        ++result;
    }
    return result;
}

typedef struct String8
{
    U8 *str;
    size_t size;
} String8;

String8 String8FromBuffer(U8 *buffer, size_t size)
{
    String8 result = { buffer, size };
    return result;
}

String8 String8FromRange(U8 *first, U8 *opl)
{
    size_t size = (size_t)(opl - first);
    String8 result = { first, size };
    return result;
}

String8 String8FromCString(char *cstr)
{
    String8 result = { (U8 *)cstr, StringLength(cstr) };
    return result;
}

#define STRING8_FROM_LITERAL(literal) String8FromBuffer((U8 *)(literal), sizeof(literal) - 1)

#define STRING8_EXPAND(string8) ((string8).str), (size_t)((string8).size)

String8 String8Prefix(String8 str, size_t size)
{
    size_t size_clamped = CLAMP(0, size, str.size);
    String8 result = { str.str, size_clamped };
    return result;
}

String8 String8Postfix(String8 str, size_t size)
{
    size_t size_clamped = CLAMP(0, size, str.size);
    size_t start_offset = str.size - size_clamped;
    String8 result = { str.str+start_offset, size_clamped };
    return result;
}

String8 String8Skip(String8 str, size_t size)
{
    size_t size_clamped = CLAMP(0, size, str.size);
    size_t size_remaining = str.size - size_clamped;
    String8 result = { str.str+size_clamped, size_remaining };
    return result;
}

String8 String8Chop(String8 str, size_t size)
{
    size_t size_clamped = CLAMP(0, size, str.size);
    size_t size_remaining = str.size - size_clamped;
    String8 result = { str.str, size_remaining };
    return result;
}

String8 String8Substr(String8 str, size_t first, size_t opl)
{
    size_t first_clamped = CLAMP(0, first, str.size);
    size_t opl_clamped = CLAMP(0, opl, str.size + 1);
    size_t substr_size = opl_clamped - first_clamped - 1;
    String8 result = { str.str+first_clamped, substr_size };
    return result;
}

typedef enum StyleFlags
{
    STYLEFLAG_LINE_END = 1 << 0
} StyleFlags;

typedef struct String8Style
{
    StyleFlags flags;
    
    F32 r;
    F32 g;
    F32 b;
    F32 a;
    
    F32 pos_x;
    F32 pos_y;
} String8Style;

#define STRING8STYLE_DEFAULT (String8Style){ .r=1, .g=1, .b=1, .a=.7f }

typedef struct String8Node
{
    struct String8Node *next;
    String8 string;
    String8Style style;
} String8Node;

typedef struct String8List
{
    String8Node *first;
    String8Node *last;
    size_t node_count;
    size_t total_size;
} String8List;

typedef struct String8Join
{
    String8 pre;
    String8 mid;
    String8 post;
} String8Join;

void String8ListPushExplicit(String8List *list, String8Node *node_memory, String8 string, String8Style *style_optional)
{
    node_memory->string = string;
    node_memory->style = (style_optional != 0) ? *style_optional : STRING8STYLE_DEFAULT;
    SLL_QUEUE_PUSH_BACK(list->first, list->last, node_memory);
    ++list->node_count;
    list->total_size += string.size;
}

void String8ListPush(MemoryArena *arena, String8List *list, String8 string, String8Style *style_optional)
{
    String8Node *node = PUSH_STRUCT(arena, String8Node);
    String8ListPushExplicit(list, node, string, style_optional);
}

String8 String8ListJoin(MemoryArena *arena, String8List *list, String8Join *join_optional)
{
    String8Join dummy_join = {0};
    String8Join *join = join_optional;
    if(join == 0)
    {
        join = &dummy_join;
    }
    
    size_t size = (join->pre.size +
                   join->post.size +
                   join->mid.size * (list->node_count - 1) +
                   list->total_size);
    
    U8 *str = PUSH_ARRAY(arena, U8, size);
    String8 result = { str, size };
    
    U8 *ptr = str;
    
    MEMORY_COPY(ptr, join->pre.str, join->pre.size);
    ptr += join->pre.size;
    
    bool is_mid = false;
    for(String8Node *node = list->first;
        node != 0;
        node = node->next)
    {
        if(is_mid)
        {
            MEMORY_COPY(ptr, join->mid.str, join->mid.size);
            ptr += join->mid.size;
        }
        
        MEMORY_COPY(ptr, node->string.str, node->string.size);
        ptr += node->string.size;
        
        if(node->next != 0)
        {
            is_mid = true;
        }
        else
        {
            is_mid = false;
            
            MEMORY_COPY(ptr, join->post.str, join->post.size);
            ptr += join->post.size;
        }
    }
    
    return result;
}

String8List String8ListSplit(MemoryArena *arena, String8 string, U8 *split_characters, size_t split_characters_count)
{
    String8List result = {0};
    
    U8 *ptr = string.str;
    U8 *word_first = ptr;
    U8 *opl = string.str + string.size;
    for(; ptr < opl; ++ptr)
    {
        U8 byte = *ptr;
        bool is_split_byte = false;
        for(size_t idx = 0;
            idx < split_characters_count;
            ++idx)
        {
            if(byte == split_characters[idx])
            {
                is_split_byte = true;
                break;
            }
        }
        
        if(is_split_byte)
        {
            if(word_first < ptr)
            {
                String8ListPush(arena, &result, String8FromRange(word_first, ptr), 0);
            }
            
            word_first = ptr + 1;
        }
    }
    
    if(word_first < ptr)
    {
        String8ListPush(arena, &result, String8FromRange(word_first, ptr), 0);
    }
    
    return result;
}

String8 String8PushFormatVariadic(MemoryArena *arena, char *format, va_list args)
{
    // in case we need to try a second time
    va_list args2;
    va_copy(args2, args);
    
    // try to build the string in 1024 bytes
    size_t buffer_size = 1024;
    U8 *buffer = PUSH_ARRAY(arena, U8, buffer_size);
    int vsnprintf_outcome = vsnprintf((char *)buffer, buffer_size, format, args);
    ASSERT(vsnprintf_outcome >= 0);
    size_t actual_size = (size_t)vsnprintf_outcome;
    
    String8 result = {0};
    if(actual_size < buffer_size)
    {
        // if first try worked put back what we didn't use and finish
        MemoryArenaPopSize(arena, buffer_size - actual_size);
        result = String8FromBuffer(buffer, actual_size);
    }
    else
    {
        // if first try failed, reset and try again with correct size
        MemoryArenaPopSize(arena, buffer_size);
        U8 *fixed_buffer = PUSH_ARRAY(arena, U8, actual_size + 1);
        vsnprintf_outcome = vsnprintf((char *)fixed_buffer, actual_size + 1, format, args2);
        ASSERT(vsnprintf_outcome >= 0);
        size_t final_size = (size_t)vsnprintf_outcome;
        
        // NOTE(sokus): Get rid of null termination character, we don't use it at the moment.
        MemoryArenaPopSize(arena, 1);
        result = String8FromBuffer(fixed_buffer, final_size);
    }
    
    va_end(args2);
    
    return result;
}

String8 String8PushFormat(MemoryArena *arena, char *format, ...)
{
    va_list args;
    va_start(args, format);
    String8 result = String8PushFormatVariadic(arena, format, args);
    va_end(args);
    return result;
}

void String8ListPushFormat(MemoryArena *arena, String8List *list, char *format, ...)
{
    va_list args;
    va_start(args, format);
    String8 string = String8PushFormatVariadic(arena, format, args);
    va_end(args);
    String8ListPush(arena, list, string, 0);
}

#endif //BASE_H
