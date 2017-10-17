#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>
#include "readData.h"
#include <stdbool.h>

int main(int argc, char *argv[]){
	
	//if (argc != 4)
		//fprintf(stderr, "Usage: %s <filename> #num #num\n", argv[0]);
	
	// ptr pointer to the array store all the vertexs
	int numV = 0;
	char **urlArray = getUrlArray(&numV);
	// get the amount of vettexs
	printf("%d\n", numV);
	// get a graph
	Graph g = getGraph(urlArray,numV);
	showGraph(g,1);
	//pageRank
	
	return 0;	
}
