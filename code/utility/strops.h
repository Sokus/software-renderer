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
bool CompareCharInsenitive(char a, char b);

#endif