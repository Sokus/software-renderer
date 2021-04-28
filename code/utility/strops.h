#ifndef STROPS_H
#define STROPS_H

#include <stdlib.h>

int getLength(char *src);
char *skipSpaces(char *src);
int isCapital(char letter);
int isLetter(char letter);
char *copy(char *src); 
void capitalise(char *src);
int compareInsensitive(char a, char b);

#endif