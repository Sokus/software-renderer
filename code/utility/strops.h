#ifndef STROPS_H
#define STROPS_H

#include <stdbool.h>

bool IsUpper(char c);
bool IsLetter(char c);
bool IsNumber(char c);
int CompareCharInsensitive(char a, char b);
int CompareStringInsensitive(char* srcA, char* srcB);
bool StartsWith(char* srcA, char* srcB);
int GetLength(char *src);

char* SkipSpaces(char *src);

void AddArticle(char** pSrc);
void RemovePadding(char** pSrc);
void RemoveDoubleSpaces(char** pSrc);
void Capitalise(char** pSrc);

char* Copy(char* src);

char* GetLongestFromArray(char* arr[], int n);

#endif