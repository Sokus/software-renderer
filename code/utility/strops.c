#include "strops.h"

int getLength(char *src)
{
    int length = 0;
    while(*src != '\0')
    {
        length++;
        src++;
    }
    return length;
}

char *skipSpaces(char *src)
{
    while(*src == ' ')
    {
        src++;
    }
    return src;
}

int isCapital(char letter)
{
    return letter >= 'A' && letter <= 'Z';
}

int isLetter(char letter)
{
    return letter >= 'A' && letter <= 'Z'
        || letter >= 'a' && letter <= 'z';
}

char *copy(char *src)
{
    int length = getLength(src);
    char *new = (char *)malloc(sizeof(char)*(length+1));
    for(int i=0; i<length+1; i++)
    {
        new[i] = src[i];
    }
    return new;
}

void capitalise(char *src)
{
    while(*src != '\0')
    {
        if(isLetter(*src))
        {
            if(*src >= 'a' && *src <= 'z')
            {
                *src -= 32;
                return;
            }
            else
            {
                // First letter is already a capital letter
            }
        }
        src++;
    }
}

int compareInsensitive(char a, char b)
{
    // ASCII:  A(65) - a(97) = 32
    return  a == b    ? 1 :
            a+32 == b ? 1 :
            a == b+32 ? 1 : 0;
}