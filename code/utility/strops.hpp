#ifndef STROPS_H
#define STROPS_H

#include <string>

bool compareCharactersInsensitive(char a, char b);
bool isUpper(char a);

std::string capitalise(std::string src);

std::string::iterator skipSpaces(std::string::iterator src);
std::string::const_iterator skipSpaces(std::string::const_iterator src);

#endif