#include <stddef.h>

#define OFFSETOF(member, type) ((size_t) &((type*)0)->member)

#define CONTAINEROF(ptr, type, member) (type*)((char*)ptr - OFFSETOF(member, type))

#define SWAP(a, b, type) do { type SWAP = x; x = y; y = SWAP; } while (0)