#include "array.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

bool Array_Initialized(DynamicArray* arr)
{
    return arr != NULL && arr->eSize > 0;
}

int Array_New(DynamicArray* arr, size_t capacity, int elementSize)
{
    if(!arr || elementSize <= 0) return -1;

    Array_Free(arr);

    arr->eSize = elementSize;
    Array_SetCapacity(&arr, capacity);
    return 0;
}

void* Array_Get(DynamicArray* arr, size_t index)
{
    if(0 <= index && index < arr->length)
    {
        size_t offset = index*arr->eSize;
        return (char*)arr->p + offset;
    }
    return NULL;
}

int Array_Set(DynamicArray* arr, size_t index, void* pValue)
{
    if(pValue && 0 <= index && index < arr->length)
    {
        size_t offset = index*arr->eSize;
        memcpy((char*)arr->p+offset, pValue, arr->eSize);
        return 0;
    }
    else
    {
        return -1;
    }
}

int Array_Find(DynamicArray* arr, void* val)
{
    if(!Array_Initialized(arr)) return -1;

    for(int i=0; i<arr->length; i++)
    {
        void* el = Array_Get(arr, i);
        if(memcmp(el, val, arr->eSize) == 0) return i;
    }
    return -1;
}

int Array_SetCapacity(DynamicArray* arr, size_t cap)
{
    if(!Array_Initialized(arr)) return -1;
    size_t oldSize = arr->eSize * arr->cap;
    size_t newSize = arr->eSize * cap;
    if(cap <= 0)
    {
        free(arr->p);
        arr->p = NULL;
        arr->length = 0;
        arr->cap = 0;
    }
    else if(cap < arr->cap)
    {
        void* newP = (void*)malloc(newSize);
        memset(newP, 0, newSize);
        if(arr->length > 0) memcpy(newP, arr->p, newSize);
        free(arr->p);
        arr->p = newP;
        arr->length = cap;
        arr->cap = cap;
    }
    else if(cap > arr->cap)
    {
        void* newP = (void*)malloc(newSize);
        memset(newP, 0, newSize);
        if(arr->length > 0) memcpy(newP, arr->p, oldSize);
        free(arr->p);
        arr->p = newP;
        arr->cap = cap;
    }
    return 0;
}

int Array_Remove(DynamicArray* arr, size_t index)
{
    if(!Array_Initialized(arr)) return -1;
    if(arr && 0 <= index && index < arr->length)
    {
        void* atIndex = (char*)arr->p + index*arr->eSize;
        memset(atIndex, 0, arr->eSize);
        size_t tail = (arr->length-1) - index;
        if(tail > 0)
        {
            void* afterIndex = (char*)arr->p + (index+1)*arr->eSize;
            memmove(atIndex, afterIndex, tail*arr->eSize);
        }
        arr->length--;
        return 0;
    }
    else
    {
        return -1;
    }
}

int Array_Append(DynamicArray* arr, void* pValue)
{
    if(!Array_Initialized(arr) || !pValue) return -1;
    arr->length++;
    if(arr->length > arr->cap)
    {
        const float golden = 1.618033f;
        size_t newCap = (arr->cap >= 4) ? (size_t)(arr->cap * golden) : 4;
        Array_SetCapacity(arr, newCap);
    }
    Array_Set(arr, arr->length-1, pValue);
    return 0;
}

int Array_Free(DynamicArray* arr)
{
    if(!arr) return -1;
    
    free(arr->p);
    arr->length = arr->cap = 0;
    return 0;
}

int Array_Copy(DynamicArray* from, DynamicArray* to)
{
    if(!Array_Initialized(from)
        || !Array_Initialized(to)
        || from->eSize != to->eSize) return -1;
    if(from->length == 0) return 0;

    size_t newSize = from->length + to->length;
    if(newSize > to->cap)
    {
        Array_SetCapacity(to, newSize);
    }

    for(size_t i=0; i<from->length; i++)
    {
        void* ptr = Array_Get(from, i);
        Array_Append(to, ptr);
    }
}

static void Swap(void* a, void* b, size_t eSize)
{
    void* p = (void*)malloc(eSize);
    memcpy(p, a, eSize);
    memcpy(a, b, eSize);
    memcpy(b, p, eSize);
    free(p);
}

// Taken from https://www.geeksforgeeks.org/iterative-quick-sort/
static int Partition(void* arr, int l, int h,
                    int (*compare)(void*, void*), size_t eSize, bool desc)
{
    void* x = (char*)arr + h*eSize;
    int i = (l - 1);

    for(int j = l; j <= h-1; j++)
    {
        void* p_j = (char*)arr + j*eSize;
        if( (compare(p_j, x)>0) == desc )
        {
            i++;
            void* p_i = (char*)arr + i*eSize;
            Swap(p_i, p_j, eSize);
        }
    }
    void* p_i = (char*)arr + (i + 1)*eSize;
    Swap(p_i, x, eSize);
    return (i + 1);
}

// Taken from https://www.geeksforgeeks.org/iterative-quick-sort/
void Array_Sort(DynamicArray arr, int (*compare)(void*, void*), bool desc)
{
    if(arr.length < 2) return;

    int l = 0;
    int h = arr.length - 1;

    int* stack = (int*)malloc(sizeof(int)*(h - l + 1));

    int gTop = -1;

    stack[++gTop] = l;
    stack[++gTop] = h;
 
    while (gTop >= 0)
    {
        h = stack[gTop--];
        l = stack[gTop--];
        int p = Partition(arr.p, l, h, compare, arr.eSize, desc);
 
        if (p - 1 > l)
        {
            stack[++gTop] = l;
            stack[++gTop] = p - 1;
        }

        if (p + 1 < h)
        {
            stack[++gTop] = p + 1;
            stack[++gTop] = h;
        }
    }

    free(stack);
}
