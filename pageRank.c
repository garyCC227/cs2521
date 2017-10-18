#include "pageRank.h"
#define MAX 3 // for array(collect in/out degree) max to 3;
//function signature 
void calculatePageRank(Graph g,double d, double diffPR,int maxT);

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
	in_out_degree(g);
	
	
	return 0;	
}

void in_out_degree(Graph g){
	int i,j;
	
	//array to indegree ,outdegree
	int *inDeg = malloc(g->nV * sizeof(int));
	int *outDeg = malloc(g->nV * sizeof(int));
	for(i = 0; i < g->nV; i++){
		inDeg[i] = 0;
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
	
	//array[id][indegree]
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
	
	double pageR[g->nV];
	//calculation of pageRank
	for(i = 0; i < g->nV; i++){
		pageR[i] = pageRank(g,d,diffPR,maxT,inDeg[i],outDeg[i]);
	}
	
}

double pageRank(Graph g, double d, double diffPR,int maxT, int *inDeg, int *outDeg){
	//initialize PR = pageRank
	double PR = 1/(g->nV);
	
	int t = 0;// curr iteration
	double diff = diffPR;
	int N = g->nV;
	
	while(t < maxT && diff >= diffPR){
		PR = (1-d)/N + d *sumPR(inDeg,outDeg);
		diff = sumDiff();
		t++;
	}

	return pR;
}

double sumPR( nt *inDeg, int *outDeg){
	double sum = 0;
	
	return sum;
}

