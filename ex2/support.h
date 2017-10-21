//written by linchen chen, 19/10/2017

#ifndef SP_H
#define SP_H


typedef struct GraphRep *Graph;

#include <stdbool.h>
#include <bsd/string.h>

typedef char *Item;
typedef struct QueueNode *Link;
// Queues has a hidden representation
typedef struct QueueRep *Queue; 



//---------------
typedef struct GraphRep {
	int   nV;
	int   numV;
	char  **vertex;// array to store the name of url
	int   **edges; // matrix of boolean
	
} GraphRep;
//scan text file into url array
char **getUrlArray();
//gets size of url array
//int get_size_of_array(char **);
//get a graph for URL connection
Graph getGraph( char **, int);
//checking whether the url = str in the graph or not
bool inGraph(char *, char **,int);



//--------------------------------------------------
// Queue.h ... Queue ADT
#define strEQ(g,t) (strcmp((g),(t)) == 0)
// Queue items are positive integers


// create an initially empty Queue
Queue createQueue(void); 

// free all memory used by the Queue
void dropQueue(Queue); 

// add new Item to the tail of the Queue
void enterQueue(Queue, Item); 

// remove Item from head of Queue; return it
Item leaveQueue(Queue); 

// return count of Items in Queue
int  queueLength(Queue); 

// display Queue as list of 2-digit numbers
void showQueue(Queue);
// 
int isSeen(Queue q, Item it);
// get id to find the url in queue
Item getURL(Queue q,int id);

char **findURLs(Queue searchWords,char **URLs,int numUrl,int N,int *);
int *findFrequency(Queue searchWord, char **URLs, int numURL);
void normalise(char *ch);
void sortByFrequency(char **URL, int *frequency, int numURL, Queue searchWords);
void getTdidf(char **URL, Queue searchWords, int numURL, double *Tfidf);


//--------------------------------------------



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

double *pageRank(Graph g, double d, double diffPR,int maxT);
int  *outDegree(Graph g);
int  *inDegree(Graph g);
double WeightedInPR(Graph g,int *inDegree, int IDv, int IDu);
double WeightedOutPR(Graph g,int *outDegree, int IDv, int IDu);
double sumInDegree(Graph g,int id,int *InDegree);
double sumOutDegree(Graph g,int id,int *outDegree);
Graph getGraph( char **urlArray, int numV);
bool inGraph(char *str, char **names,int numV);

//---------------------------------------------------



#endif
