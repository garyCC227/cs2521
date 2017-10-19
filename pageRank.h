//h library for pageRank.c
//written by linchen chen, 19/10/2017

#ifndef PAGERANK_H
#define PAGERANK_H

#include "readData.h"

double *pageRank(Graph g, double d, double diffPR,int maxT);
int  *outDegree(Graph g);
int  *inDegree(Graph g);
double WeightedInPR(Graph g,int *inDegree, int IDv, int IDu);
double WeightedOutPR(Graph g,int *outDegree, int IDv, int IDu);
double sumInDegree(Graph g,int id,int *InDegree);
double sumOutDegree(Graph g,int id,int *outDegree);
void sortPageRank(double *pageRank, char **urlArray, int *outDeg, int numUrl);
void writePR(double *pageR, char **urlArray, int *outD, int numV);

#endif
