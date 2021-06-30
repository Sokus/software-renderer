#include "strops.h"

#include <stdlib.h>

bool IsUpper(char c)
{
    return c >= 'A' && c <= 'Z';
}

bool IsLetter(char c)
{
    return c >= 'A' && c <= 'Z'
        || c >= 'a' && c <= 'z';
}

bool IsNumber(char c)
{
    return c >= '0' && c <= '9';
}

int CompareCharInsensitive(char a, char b)
{
    if(a == b) return 0;

    // ASCII: A(65) - a(97) = 32
    if(IsUpper(a)) a += 32;
    if(IsUpper(b)) b += 32;
    return a < b ? -1 : 1;
}

int CompareStringInsensitive(char* srcA, char* srcB)
{
    if(srcA == srcB) return 0;
    if(!srcA || !srcB) return 0;

    int testValue = 0;
    while(1)
    {
        testValue = CompareCharInsensitive(*srcA, *srcB);
        if(testValue) return testValue;
        srcA++, srcB++;
        bool aEnded = (*srcA == '\0');
        bool bEnded = (*srcB == '\0');
        if(aEnded && bEnded) return 0;
    }
}

bool StartsWith(char* srcA, char* srcB)
{
    if(srcA == srcB) return true;
    if(!srcA || !srcB) return false;

    while(*srcB != '\0')
    {
        bool charactersMatch = CompareCharInsensitive(*srcA, *srcB);
        bool sourceEnded = (*srcA == '\0');
        if( !charactersMatch || sourceEnded ) return false;

        srcA++;
        srcB++;
    }
    return true;
}

int GetLength(char* src)
{
    if(!src) return 0;
    int length = 0;
    while(*src != '\0')
    {
        length++;
        src++;
    }
    return length;
}

char* SkipSpaces(char* src)
{
    while(*src == ' ') src++;
    return src;
}

void AddArticle(char** pSrc)
{   
    if(!pSrc) return;
    char* src = *pSrc;
    if(!src || !IsLetter(*src)) return;
    //                     a  b  c  d  e  f  g  j  i  j  k  l  m  n  o  p  q  r  s  t  u  w  x  y  z
    bool whereToPutAn[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0};
    int arrSize = sizeof(whereToPutAn)/sizeof(bool);
    bool putAn = false;
    for(int i=0; i<arrSize; i++)
    {
        if(CompareCharInsensitive((char)(i+65), *src))
        {
            putAn = whereToPutAn[i];
            break; 
        }
    }

    int oldLength = GetLength(src);
    int articleSize = putAn ? 3 : 2;
    int newLength = articleSize + oldLength;
    char* newSrc = (char*)malloc(sizeof(char)*(newLength+1));
    if(putAn) newSrc[0] = 'a', newSrc[1] = 'n', newSrc[2] = ' ';
    else newSrc[0] = 'a', newSrc[1] = ' ';
    for(int i=0; i<oldLength; i++)
    {
        newSrc[i+articleSize] = src[i];
    }
    newSrc[newLength] = '\0';
    free(src);
    *pSrc = newSrc;
}

void RemovePadding(char** pSrc)
{
    if(!pSrc) return;
    char* src = *pSrc;
    if(!src) return;

    bool anyLettersFound = false;
    int index=0, start=0, end=0;
    while(*(src+index) != '\0')
    {
        if(*(src+index) != ' ')
        {
            if(!anyLettersFound)
            {
                anyLettersFound = true;
                start = index;
            }
            end = index;
        }
        index++;
    }

    int newLength = end-start+1;
    if(newLength < index-1)
    {
        char* newSrc = (char*)malloc(sizeof(char)*(newLength+1));
        for(int i=0; i<newLength; i++)
        {
            newSrc[i] = src[start+i];
        }
        newSrc[newLength] = '\0';
        free(src);
        *pSrc = newSrc;
    }
}

void RemoveDoubleSpaces(char** pSrc)
{
    if(!pSrc) return;
    char* src = *pSrc;
    if(!src) return;
    char* other = src;
    while(*other != '\0')
    {
        *src = *other;
        if(*other == ' ') other = SkipSpaces(other);
        else other++;
        src++;
    }

    int newLength = src - *pSrc;
    char* newSrc = (char*)malloc(sizeof(char)*(newLength+1));
    for(int i=0; i<newLength; i++)
    {
        newSrc[i] = (*pSrc)[i];
    }
    newSrc[newLength] = '\0';
    free(*pSrc);
    *pSrc = newSrc;
}

void Capitalise(char** pSrc)
{
    if(!pSrc) return;
    char* src = *pSrc;
    if(!src) return;
    src = SkipSpaces(src);
    if(IsLetter(*src) && !IsUpper(*src)) *src -= 32;
}

char* Copy(char* src)
{
    int length = GetLength(src);
    char *new = (char *)malloc(sizeof(char)*(length+1));
    for(int i=0; i<length; i++)
    {
        new[i] = src[i];
    }
    new[length] = '\0';
    return new;
}

char* GetLongestFromArray(char* arr[], int n)
{
    if(!arr) return NULL;
    char* longestTag = NULL;
    int maxLength = 0;
    for(int i=0; i<n; i++)
    {
        int length = GetLength(arr[i]);
        if(length > maxLength)
        {
            longestTag = arr[i];
            maxLength = length;
        }
    }
    return longestTag;
}