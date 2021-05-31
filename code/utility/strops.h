#ifndef STROPS_H
#define STROPS_H

#include <stdlib.h>
#include <stdbool.h>

bool IsUpper(char letter);
bool IsLetter(char letter);
bool IsNumber(char letter);
bool CompareCharInsensitive(char a, char b);
bool CompareStringInsensitive(char* srcA, char* srcB);
bool StartsWith(char* srcA, char* srcB);
int GetLength(char *src);

char* SkipSpaces(char *src);

char* AddArticle(char* src);
void RemovePadding(char** pSrc);
void RemoveDoubleSpaces(char** pSrc);
char* Capitalise(char* src);

char* Copy(char* src);

char* GetLongestFromArray(char* arr[], int n);

#endif