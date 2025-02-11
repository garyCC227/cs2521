// graph.c ... Graph of strings (adjacency matrix)
// Written by linchen chen, 19/10/2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "readData.h"

#define strEQ(g,t) (strcmp((g),(t)) == 0)



// newGraph()
// - create an initially empty Graph
Graph newGraph(int numV)
{
	Graph new = malloc(sizeof(GraphRep));
	assert(new != NULL);
	int i, j;
	new->nV = 0;
	new->numV = numV;
	new->vertex = malloc(numV*sizeof(char *));
	assert(new->vertex != NULL);
	new->edges = malloc(numV*sizeof(int *));
	assert(new->edges != NULL);
	for (i = 0; i < numV; i++) {
		new->vertex[i] = NULL;
		new->edges[i] = malloc(numV*sizeof(int));
		assert(new->edges[i] != NULL);
		for (j = 0; j < numV; j++)
			new->edges[i][j] = 0;
	}
	return new;
}

// disposeGraph(Graph)
// - clean up memory associated with Graph
void disposeGraph(Graph g)
{
	if (g == NULL) return;
	int i;
	for (i = 0; i < g->nV; i++) {
		free(g->vertex[i]);
	}
	for (i = 0; i < g->numV; i++) {
		free(g->edges[i]);
	}
	free(g->edges);
}

// addEdge(Graph,Src,Dest)
// - add an edge from Src to Dest
// - returns 1 if edge successfully added
// - returns 0 if unable to add edge
//   (usually because nV exceeds maxV)
int addEdge(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(src,g->vertex,g->nV);
	if (v < 0) {
		fprintf(stderr,"So such a url is not in graph %s\n",src);
		return 0;
	}
	int w = vertexID(dest,g->vertex,g->nV);
	if (w < 0) {
		fprintf(stderr,"So such a url is not in graph %s\n",dest);
		return 0;
	}
	g->edges[v][w] = 1;
	return 1;
}

// isConnectedout(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest for outlink
int isConnectedOut(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(src,g->vertex,g->nV);
	int w = vertexID(dest,g->vertex,g->nV);
	if (v < 0 || w < 0)
		return 0;
	else
		return g->edges[v][w];
}
//check if v vertex link to u vertex by input id
int isConnectedByID(Graph g, int v, int w)
{
	assert(g != NULL);
	return g->edges[v][w];
}

// isConnectedint(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest for inlink
int isConnectedIn(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(src,g->vertex,g->nV);
	int w = vertexID(dest,g->vertex,g->nV);
	if (v < 0 || w < 0)
		return 0;
	else
		return g->edges[v][w];
}
// nVertices(Graph)
// - return # vertices currently in Graph
int nVertices(Graph g)
{
	assert(g != NULL);
	return (g->nV);
}

// showGraph(Graph)
// - display Graph
void showGraph(Graph g, int mode)
{
	assert(g != NULL);
	if (g->nV == 0)
		printf("Graph is empty\n");
	else {
		printf("Graph has %d vertices:\n",g->nV);
		int i, j;
		if (mode == 1) {
			for (i = 0; i < g->nV; i++) {
				for (j = 0; j < g->nV; j++)
					printf("%d",g->edges[i][j]);
				putchar('\n');
			}
		}
		else {
			for (i = 0; i < g->nV; i++) {
				printf("Vertex: %s\n", g->vertex[i]);
				printf("connects to\n");
				for (j = 0; j < g->nV; j++) {
					if (g->edges[i][j])
						printf("   %s\n",g->vertex[j]);
				}
			}
		}
	}
}

// Helper functions

// vertexID(Str,Names,N)
// - searches for Str in array of Names[N]
// - returns index of Str if found, -1 if not
int vertexID(char *str, char **names, int N)
{
	int i;
	for (i = 0; i < N; i++)
		if (strEQ(str,names[i])) return i;
	return -1;
}

// addVertex(Str,Names,N)
// - add Str at end of Names
int addVertex(char *str, char **names, int N)
{
	strcpy(names[N], str);
	return N;
}

