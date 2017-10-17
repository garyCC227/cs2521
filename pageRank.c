#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>
#include "readData.h"
#include "graph.h"
#include <stdbool.h>

int main(int argc, char *argv[]){
	
	//if (argc != 4)
		//fprintf(stderr, "Usage: %s <filename> #num #num\n", argv[0]);
	
	// ptr pointer to the array store all the vertexs
	char **urlArray = (getUrlArray());
	// get the amount of vettexs
	int numV = get_size_of_array(urlArray);
	//printf("%d\n", numV);
	// get a graph
	graph g = getGraph(urlArray,numV);
	
	//pageRank
	
	return 0;	
}
