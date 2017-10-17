#include <stdio.h>
#include <stdlib.h>
#include <bsd/string.h>

int main(int argc, char *argv[]){
	
	if (argc != 4){
		fprintf(stderr, "Usage: %s <filename> #num #num\n", argv[0]);
	}
	
	// ptr pointer to the array store all the vertexs
	char *ptr = get_collection();
	// get the amount of vettexs
	int numV = get_size_of_array(ptr);
	// get a graph
	graph g = getGraph(ptr,numV);
	
	//pageRank
	
	return 0;	
}
