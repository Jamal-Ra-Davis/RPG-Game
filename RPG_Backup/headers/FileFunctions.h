/****************************************/
/*                                      */
/* This header includes functions that  */
/* count the number of lines in a file, */
/* and returning the string at a        */
/* specified line number.               */
/*                                      */
/****************************************/
#pragma once
#include <stdio.h>
#include <stdlib.h>

int toLower(char* word);
int lineCount(char* fileName);
char* getLine(char* fileName, int line);
char* getWholeLine(char* fileName, int line);
