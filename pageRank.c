#include "pageRank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#define MAX 3 // for array(collect in/out degree) max to 3;

//function signature 
double *pageRank(Graph g, double d, double diffPR,int maxT);
int  *outDegree(Graph g);
int  *inDegree(Graph g);
double WeightedInPR(Graph g,int *inDegree, int IDv, int IDu);
double WeightedOutPR(Graph g,int *outDegree, int IDv, int IDu);
double sumInDegree(Graph g,int id,int *InDegree);
double sumOutDegree(Graph g,int id,int *outDegree);
void sortPageRank(double *pageRank, char **urlArray, int *outDeg, int numUrl);
void writePR(double *pageR, char **urlArray, int *outD, int numV);

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

	//write to .txt file
	writePR(pageR,urlArray,outD,g->nV);

	//free memory
	free(urlArray);
	free(outD);
	free(pageR);

	return 0;	
}

int  *outDegree(Graph g){
	//supprt varible
	int i,j;
	
	//array to store outdegree
	int *outDeg = malloc(g->nV * sizeof(int));
	for(i = 0; i < g->nV; i++){
		outDeg[i] = 0;
	}
	
	//array[id][outdegree]
	//to find outdegree
	for(i=0;i < g->nV;i++){
		for(j = 0; j < g->nV; j++){
			//avoid self-loop
			if(i != j){
				if(isConnectedOut(g,g->vertex[i],g->vertex[j])){
					outDeg[i]++;
				}
			}
		}
	}
	
	return outDeg;
}

int *inDegree(Graph g){	
	
	// initialisation 
	int *inDeg = malloc(g->nV * sizeof(int));
	int i,j;
	for(i = 0; i < g->nV; i++){
		inDeg[i] = 0;
	}
	
	//to find indegree
	for(i=0;i < g->nV;i++){
		for(j = 0; j < g->nV; j++){
			//avoid self-loop
			if(i != j){
				if(isConnectedIn(g,g->vertex[j],g->vertex[i])){
					inDeg[i]++;
				}
			}
		}
	}
	return inDeg;
}
//get weightedInPR
double WeightedInPR(Graph g,int *inDegree, int IDv, int IDu){
	double weightIn;
	weightIn = (double)inDegree[IDu]/sumInDegree(g,IDv,inDegree);	
	return weightIn;
}
//get weightedOutPR
double WeightedOutPR(Graph g,int *outDegree, int IDv, int IDu){
	double weightOut;
	
	if(outDegree[IDu]== 0){
		weightOut = (double)0.5/sumOutDegree(g,IDv,outDegree);
		return weightOut;
	}
	weightOut = (double)outDegree[IDu]/sumOutDegree(g,IDv,outDegree);
	return weightOut;
}
//sum all indegree
double sumInDegree(Graph g,int id,int *InDegree){
	int i;
	int sum = 0;
	for(i = 0; i < g->nV; i ++){
		if(g->edges[id][i] == 1 && i != id ){
			// if vertex has 0 outdegree, then outdegree set to 0.5
			if(InDegree[i] != 0){
				sum = (double)InDegree[i] + sum;
			}else{
				sum = 0.5 + sum;
			}
		}
	}
	return (double)sum;
}
//sum all outdegree 
double sumOutDegree(Graph g,int id,int *outDegree){
	int i;
	double sum = 0;
	for(i = 0; i < g->nV; i ++){
		if(g->edges[id][i] == 1 && i != id){
			// if vertex has 0 outdegree, then outdegree set to 0.5
			if(outDegree[i] != 0){
				sum = (double)outDegree[i] + sum;
			}else{
				sum = 0.5 + sum;
			}
		}
	}
	return sum;
}

double *pageRank(Graph g, double d, double diffPR,int maxT){
	
	//initialize varibles and pagerank array
	double N = (double)g->nV;
	double *pageRank = malloc(sizeof(double)*(g->nV));
	double oldPR[g->nV];
	
	//initialisation variables for algorithm of pageRank evaluation
	int t = 0;// curr iteration
	double diff = diffPR;
	double sigma = 0;
	double wIn = 0;
	double wOut = 0;
	double diffSum = 0;
	int i,j;

	//array to store in/out indegree for each URL

	int *inD;
	int *outD;
	//get in/out indegree
	outD = outDegree(g);
	inD = inDegree(g);

	//set the initial pageRank of each URL to 1/N
	for(i = 0; i < g->nV;i++) pageRank[i] = 1/N;
	
	//calculate a fantastic pageRank
	while(t < maxT && diff >= diffPR){
		//next iteration;
		t++;
		//rest diffSum to 0
		diffSum = 0;
		//updata old pageRank array
		for(i = 0; i < g->nV;i++) oldPR[i] = pageRank[i];

		//upda pageRank for each URL in each iteration
		for(i = 0; i < g->nV; i++){
			//reset sigma value
			sigma = 0;
			for(j=0;j<g->nV;j++){
				//avoid self-loop
				if(i == j) continue;
				//find all the url that have outlink to URL i
				if(isConnectedByID(g, j, i)){
					wIn = WeightedInPR(g,inD, j, i);
					wOut = WeightedOutPR(g,outD, j, i);
					sigma = sigma + (oldPR[j]*wIn*wOut);
				}	
			}
			//updata pageRank for URL i;
			pageRank[i] = ((1-d)/N)+(d*sigma);
		}

		//update value of diffSum 
		for(i=0; i<g->nV; i++){
			diffSum = diffSum + fabs(pageRank[i] - oldPR[i]);
		}
		//updata diff
		diff = diffSum;
	}

	//free memory
	free(inD);
	free(outD);

	return pageRank;
}

// sorting pageRank in descending order 
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
