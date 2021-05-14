#include "strops.hpp"

bool compareCharactersInsensitive(char a, char b)
{
    // ASCII:  A(65) - a(97) = 32
    return  a == b    ? true :
            a+32 == b ? true :
            a == b+32 ? true : false;
}

bool isUpper(char a)
{
    return a >= 'A' && a <= 'Z';
}

std::string capitalise(std::string src)
{
    std::string::iterator it = src.begin();
    while(*it == ' ') it++;
    if(it != src.end()) *it = toupper(*it);
    return src;
}

std::string::iterator skipSpaces(std::string::iterator src)
{
    while(*src==' ' && *src!='\0') ++src;
}

std::string::const_iterator skipSpaces(std::string::const_iterator src)
{
    while(*src==' ' && *src!='\0') ++src;
}