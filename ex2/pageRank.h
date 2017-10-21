//h library for pageRank.c
//written by linchen chen, 19/10/2017

#ifndef PAGERANK_H
#define PAGERANK_H




void sortPageRank(double *pageRank, char **urlArray, int *outDeg, int numUrl);
void writePR(double *pageR, char **urlArray, int *outD, int numV);

#endif
