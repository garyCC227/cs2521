#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <bsd/string.h>
#include "readData.h"
#include <stdbool.h>
#define MAX_L 50

#define strEQ(g,t) (strcmp((g),(t)) == 0)
//function signature
void sort(char **array, int maxA);
void normalise(char *ch);
int NoduplicateWord(char *word, char **storage,int numW);
void writeToFile(int numW, int numUrl, char ** urlArray, char **words);
