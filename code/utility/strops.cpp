#include "strops.hpp"

bool CompareCharInsensitive(char a, char b)
{
    // ASCII:  A(65) - a(97) = 32
    return  a == b    ? true :
            a+32 == b ? true :
            a == b+32 ? true : false;
}

bool IsUpper(char a)
{
    return a >= 'A' && a <= 'Z';
}

std::string Capitalise(std::string src)
{
    std::string::iterator it = src.begin();
    while(*it == ' ') it++;
    if(it != src.end()) *it = toupper(*it);
    return src;
}