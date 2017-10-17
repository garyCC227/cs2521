// graph.h ... Interface to Graph of strings
// Written by John Shepherd, September 2015

#ifndef GRAPH_H
#define GRAPH_H

typedef struct GraphRep *Graph;
//typedef unsigned char Num;

typedef struct GraphRep {
	int   nV;
	char  **vertex;// array to store the name of url
	int   **edges; // matrix of boolean
	
} GraphRep;


// Function signatures

Graph newGraph(int numV);
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);
int addVertex(char *str, char **names, int N);

#endif
