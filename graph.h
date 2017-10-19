// graph.h ... Interface to Graph of strings
// Written by linchen chen, 19/10/2017

#ifndef GRAPH_H
#define GRAPH_H
typedef struct GraphRep *Graph;

typedef struct GraphRep {
	int   nV;
	int   numV;
	char  **vertex;// array to store the name of url
	int   **edges; // matrix of boolean
	
} GraphRep;


// Function signatures
int isConnectedIn(Graph g, char *src, char *dest);
Graph newGraph(int numV);
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnectedOut(Graph, char *, char *);
void  showGraph(Graph,int);
int addVertex(char *str, char **names, int N);
int vertexID(char *, char **, int);
int isConnectedByID(Graph g, int v, int w);
#endif

