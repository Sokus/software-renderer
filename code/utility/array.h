#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdbool.h>

typedef struct DynamicArray
{
    void* p;
    int eSize;
    size_t length;
    size_t cap;
} DynamicArray;

bool Array_Initialized(DynamicArray* arr);

int Array_New(DynamicArray* arr, size_t capacity, int elementSize);
void* Array_Get(DynamicArray* arr, size_t index);
int Array_Set(DynamicArray* arr, size_t index, void* pValue);
int Array_Find(DynamicArray* arr, void* val);
int Array_SetCapacity(DynamicArray* arr, size_t cap);
int Array_Remove(DynamicArray* arr, size_t index);
int Array_Append(DynamicArray* arr, void* pValue);
int Array_Free(DynamicArray* arr);
int Array_Copy(DynamicArray* from, DynamicArray* to);

static void Swap(void* a, void* b, size_t eSize);
static int Partition(void* arr, int l, int h, int (*compare)(void*, void*), size_t eSize, bool desc);
void Array_Sort(DynamicArray arr, int (*compare)(void*, void*), bool desc);

#endif