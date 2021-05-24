#ifndef STROPS_H
#define STROPS_H

#include <stdlib.h>
#include <stdbool.h>

int GetLength(const char *src);
char* SkipSpaces(const char *src);
bool IsUpper(char letter);
bool IsLetter(char letter);
char* Copy(char* src); 
void Capitalise(char* src);
bool CompareCharInsensitive(char a, char b);
bool CompareStringInsensitive(const char* srcA, const char* srcB);

#endif