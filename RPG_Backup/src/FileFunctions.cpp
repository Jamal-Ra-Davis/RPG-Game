/****************************************/
/*                                      */
/* This header includes functions that  */
/* count the number of lines in a file, */
/* and returning the string at a        */
/* specified line number.               */
/*                                      */
/****************************************/
#include "../headers/FileFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int toLower(char* word)
{
	if (word == NULL)
		return 1;
	int i=0;
	while (word[i] != '\0')
	{
		if ((word[i] >= 65)&&(word[i] <= 90))
			word[i] += 32;
		i++;
	}
	return 0;
}
int lineCount(char* fileName)
{
	if (fileName == NULL)
		return -1;
	char *dummy;
	FILE *fp;
	int count = 0;
	dummy = (char*)malloc(100*sizeof(char));
	fp = fopen(fileName, "r"); 
	assert((dummy != NULL)&&(fp != NULL));

	while (fgets(dummy, 100, fp) != NULL)
	{
		count++;
	}

	fclose(fp);
	free(dummy);
	return count;
}
char* getLine(char* fileName, int line)
{
	if (fileName == NULL)
		return NULL;
	char *dummy;
	FILE *fp;
	dummy = (char*)malloc(100*sizeof(char));
	fp = fopen(fileName, "r");
	assert((dummy != NULL)&&(fp != NULL));

	for (int i=0; i<line-1; i++)
	{
		fgets(dummy, 100, fp);
	}
	fscanf(fp, "%s", dummy);
        
	fclose(fp);
	return dummy;
}
char* getWholeLine(char* fileName, int line)
{
	if (fileName == NULL)
		return NULL;
	char *dummy;
	FILE *fp;
	dummy = (char*)malloc(100*sizeof(char));
	fp = fopen(fileName, "r");
	assert((dummy != NULL)&&(fp != NULL));

	for (int i=0; i<line-1; i++)
	{
		fgets(dummy, 100, fp);
	}
	fgets(dummy, 100, fp);

	fclose(fp);
	return dummy;
}

