#include "strops.h"

bool IsUpper(char letter)
{
    return letter >= 'A' && letter <= 'Z';
}

bool IsLetter(char letter)
{
    return letter >= 'A' && letter <= 'Z'
        || letter >= 'a' && letter <= 'z';
}

bool IsNumber(char letter)
{
    return letter >= '0' && letter <= '9';
}

bool CompareCharInsensitive(char a, char b)
{
    // ASCII:  A(65) - a(97) = 32
    return  a == b    ? 1 :
            a+32 == b ? 1 :
            a == b+32 ? 1 : 0;
}

bool CompareStringInsensitive(char* srcA, char* srcB)
{
    if(srcA == srcB) return true;
    if(srcA || srcB) return false;

    while(true)
    {
        if(!CompareCharInsensitive(*srcA, *srcB)) return false;
        srcA++;
        srcB++;
        bool aEnded = *srcA == '\0';
        bool bEnded = *srcB == '\0';
        if(aEnded != bEnded) return false;
        if(aEnded && bEnded) return true;
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
    while(*src == ' ')
    {
        src++;
    }
    return (char*)src;
}

char* AddArticle(char* src)
{   
    if(!src || !IsLetter(*src)) return src;
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
    return newSrc;
}

void RemovePadding(char** pSrc)
{
    bool anyLettersFound = false;
    int index=0, start=0, end=0;
    while(*(*pSrc+index) != '\0')
    {
        if(*(*pSrc+index) != ' ')
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
            newSrc[i] = (*pSrc)[start+i];
        }
        newSrc[newLength] = '\0';
        free(*pSrc);
        *pSrc = newSrc;
    }
}

void RemoveDoubleSpaces(char** pSrc)
{
    char* src = *pSrc;
    char* other = *pSrc;
    while(*other != '\0')
    {
        *src = *other;
        if(*other == ' ')
        {
            other = SkipSpaces(other);
        }
        else
        {
            other++;
        }
        src++;
    }
    *src = '\0';
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

char* Capitalise(char* src)
{
    src = SkipSpaces(src);
    if(IsLetter(*src) && !IsUpper(*src))
    {
        *src -= 32;
    }
    return src;
}

char* Copy(char* src)
{
    int length = GetLength(src);
    char *new = (char *)malloc(sizeof(char)*(length+1));
    for(int i=0; i<length+1; i++)
    {
        new[i] = src[i];
    }
    return new;
}

char* GetLongestFromArray(char* arr[], int n)
{
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