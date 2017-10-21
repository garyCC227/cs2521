#include "pageRank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "support.h"



#define MAX 3 // for array(collect in/out degree) max to 3;

int main(int argc, char *argv[]){

	if (argc != 4){
		fprintf(stderr, "Usage: %s <filename> #num #num #num\n", argv[0]);
		exit(1);
	}
	//get argument for pageRank function
	double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxT = atoi(argv[3]);
	// ptr pointer to the array store all the vertexs
	int numV = 0;
	//get url name
	char **urlArray = getUrlArray(&numV);

	// get a graph
	Graph g = getGraph(urlArray,numV);
	//showGraph(g,1);
	
	//get outdegree
	int *outD;
	outD = outDegree(g);

	//get pageRank
	double *pageR = pageRank(g,d,diffPR,maxT);
	
	//sorting page rank,urlArray and outdegree in a descending order
	sortPageRank(pageR, urlArray, outD, g->nV);

	//write pagerank to .txt file
	writePR(pageR,urlArray,outD,g->nV);




	//write invertedIndex.txt




	//free memory
	free(urlArray);
	free(outD);
	free(pageR);

	return 0;	
}

void sortPageRank(double *pageRank, char **urlArray, int *outDeg, int numUrl){
	// support varibles initilisation
	int i,j;
	double tempPR;
	char *tempUrl;
	int tempDeg;

	//bubble sort to sort pageRank
	for(i=1;i<numUrl;i++){
		for(j=1;j<numUrl;j++){
			//avoid self-loop
			if(i == j)continue;

			//sorting pageRank
			if(pageRank[j-1] < pageRank[j]){
				tempPR = pageRank[j-1];
				pageRank[j-1] = pageRank[j];
				pageRank[j] = tempPR;

				//make urlArray match their pageRank;
				tempUrl = urlArray[j-1];
				urlArray[j-1] = urlArray[j];
				urlArray[j] = tempUrl;

				//make outdegree match their pageRank;
				tempDeg = outDeg[j-1];
				outDeg[j-1] = outDeg[j];
				outDeg[j] = tempDeg;
			}
		}
	}
}

void writePR(double *pageR, char **urlArray, int *outD, int numV){
	//open file
	FILE *fp;
	fp = fopen("pagerankList.txt","w");

	//support varible
	int i;

	//loop write into file
	for(i=0; i < numV;i++){
		fprintf(fp,"%s,",urlArray[i]);
		fprintf(fp,"%d,",outD[i]);
		fprintf(fp,"%.7f\n",pageR[i]);
	}

	fclose(fp);
}
