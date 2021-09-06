/* date = September 6th 2021 1:04 pm */

#ifndef BASE_H
#define BASE_H

//~NOTE(sokus): compiler flags
/*
* SUMMONED_DEBUG - enable ASSERT, debug functions
  */

//~NOTE(sokus): necessary includes
#include <string.h> // memcpy, memset

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

#define DLL_PUSH_FRONT(f, l, n) DLL_PUSH_BACK_EXPLICIT(l, f, n, prev, next)

#define DLL_REMOVE_EXPLICIT(f, l, n, next, prev) ((f)==(n)?\
((f)=(f)->next,(f)->prev=0):\
((l)==(n)?\
((l)=(l)->prev, (l)->next=0):\
((n)->next->prev=(n)->prev,\
(n)->prev->next=(n)->next)))

#define DLL_REMOVE(f, l, n) DLL_REMOVE_EXPLICIT(f, l, n, next, prev);


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

#define PUSH_STRUCT(arena, type) (type *)PushSize_(arena, sizeof(type))
#define PUSH_ARRAY(arena, type, count) (type *)PushSize_(arena, (count)*sizeof(type)) 

void *
PushSize_(MemoryArena *arena, size_t size)
{
    ASSERT((arena->used + size) <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    
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
    
    memcpy(dest, str_a, str_a_size_clamped);
    memcpy(dest+str_a_size_clamped, str_b, str_b_size_clamped);
    
    *(dest+str_a_size_clamped+str_b_size_clamped) = 0;
}

size_t
StringLength(char *str)
{
    size_t result = 0;
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

String8 String8FromArray(U8 *arr, size_t size)
{
    String8 result = { arr, size };
    return result;
}

String8 String8FromCString(char *cstr)
{
    String8 result = { (U8 *)cstr, StringLength(cstr) };
    return result;
}

#define STRING8_FROM_LITERAL(literal) String8FromArray((U8 *)(literal), sizeof(literal) - 1)

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

String8 String8Substr(String8 str, size_t start, size_t end)
{
    size_t start_clamped = CLAMP(0, start, str.size);
    size_t end_clamped = CLAMP(start_clamped, end, str.size);
    size_t size_remaining = end_clamped - start_clamped;
    String8 result = { str.str+start_clamped, size_remaining };
    return result;
}

typedef struct String8Node
{
    struct String8Node *next;
    String8 string;
    
    F32 r;
    F32 g;
    F32 b;
    F32 a;
    
    int pos_x;
    int pos_y;
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

void String8ListPushExplicit(String8List *list, String8 string, String8Node *node_memory)
{
    node_memory->string = string;
    SLL_QUEUE_PUSH_BACK(list->first, list->last, node_memory);
    list->node_count += 1;
    list->total_size += string.size;
}

void String8ListPush(MemoryArena *arena, String8List *list, String8 string)
{
    String8Node *node = PUSH_STRUCT(arena, String8Node);
    String8ListPushExplicit(list, string, node);
}

#if 0
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
    
    U8 *str = PUSH_ARRAY(arena, U8, size + 1);
    U8 *ptr = str;
    
    
}
#endif

#endif //BASE_H
