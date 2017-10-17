#include <stdbool.h>
#include "graph.h"
#include <bsd/string.h>

//scan text file into url array
char **getUrlArray();
//gets size of url array
//int get_size_of_array(char **);
//get a graph for URL connection
Graph getGraph( char **, int);
//checking whether the url = str in the graph or not
bool inGraph(char *, char **);
