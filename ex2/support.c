// graph.c ... Graph of strings (adjacency matrix)
// Written by linchen chen, 19/10/2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "support.h"
#include <ctype.h>



#define strEQ(g,t) (strcmp((g),(t)) == 0)
#define MAX 50


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
	names[N] = malloc(strlen(str)+1);
	strcpy(names[N], str);
	return N;
}
Graph getGraph( char **urlArray, int numV){	
	//new graph and pass number of vertexs argument
	Graph g = newGraph(numV);	
	int i,j,in_sec1;
	FILE *fp;
	char url[100];
	char urlName[100];	

	//add all url into graph vertex
	for(i = 0;i<numV;i++) {
		addVertex(urlArray[i],g->vertex,i);
		g->nV++;
		//printf("vertex is %s\n",g->vertex[i]);
	}
	//building graph
	for(i = 0;i<numV;i++){
		//copy path	
		strcpy(url,urlArray[i]);
		strcat(url,".txt");
		//printf("%s\n", url);
		fp = fopen(url,"r");	
		//in section 1
		//scan into the file , get the outlinks url name
		
		//fscanf each words in "url.txt" into urlName array
		//condintioner that whether fp in section-1 or not
		in_sec1 = 0;
		while((fscanf(fp, "%s", urlName)) == 1){
			//printf("%s\n", urlName);
			//starting to reading section-1 
			if ( strEQ(urlName,"Section-1") && in_sec1 == 0){
				in_sec1 =1;
			}else if(strEQ(urlName,"Section-1") && in_sec1 == 1){
			//end of section 1, break, get next url file
				break;
			}
			if(in_sec1 == 1){
			//printf("%s\n", urlName);
				for(j = 0;j<numV;j++){
					//if one url contain the outlink of another, build edge 
					if(inGraph(urlName,g->vertex,g->nV)){
						addEdge(g, urlArray[i], urlName);
						
					}
				}
			}
			memset(urlName,0,sizeof(urlName));
		}
		fclose(fp);
	}
	
	return g;
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

bool inGraph(char *str, char **names,int numV){
	int i = 0;
	while(i < numV){
		if(strcmp(str,names[i]) == 0) return 1;
		
		i++;
	}
	// if not in graph, return 0
	return 0;
}

//--------------------------------------


char **getUrlArray(int *numv){
    
    char ch;
    int i, size = 0, in_word = 0;
    FILE *fp = fopen("collection.txt", "r");
    //get size of file, number of words in file
    while ((ch=getc(fp)) != EOF) {
        if (ch == ' ' ||  ch == '\t' || ch == '\n'){
            in_word = 0;            
        } else if (in_word == 0){
            size++;
            in_word = 1; //checks word amount
        }
    }
    //printf("%d\n", size);
    *numv = size;
    rewind(fp); //go back to start of file
    char **urlArray;
    urlArray = malloc(size * sizeof(char *));
    for(i = 0; i < size; i++){
    	urlArray[i] = malloc(size * sizeof(int));
    	if(urlArray[i] == NULL){
    		fprintf(stderr,"out of memory\n");
    		return 0;
    	}
    }  
    //scan into urlArray the url
    for (i = 0; i < size; i++){
        fscanf(fp, "%s", urlArray[i]);
    }
    //for (i = 0; i < size; i++){
    //    printf("%s\n", urlArray[i]);
    //}    
    fclose(fp);
    
    return urlArray;
}


//---------------------------------
// Queue.c ... list implementation of a queue



#define MAX 50


typedef struct QueueNode *Link;

struct QueueNode {
   Item item;
   Link next;
};

struct QueueRep {
   Link head;
   Link tail;
   int  size;
};

// private function for creating list nodes
static Link createNode(Item item)
{
   Link n = malloc(sizeof(struct QueueNode));
   assert(n != NULL);
   n->item = malloc(sizeof(char*));
   n->item = item;
   n->next = NULL;
   return n;
}

// create an initially empty Queue
Queue createQueue(void)
{
   Queue q = malloc(sizeof(struct QueueRep));
   assert(q != NULL);
   q->head = NULL;
   q->tail = NULL;
   q->size = 0;
   return q;
}

// free all memory used by the Queue
void dropQueue(Queue q)
{
   Link curr;
   Link next;
   assert(q != NULL);
   curr = q->head;
   while (curr != NULL) {
      next = curr->next;
      curr = next;
   }
   free(q);
}


// add new Item to the tail of the Queue
void enterQueue (Queue q, Item it)
{
   assert(q != NULL);
   Link n = createNode(it);
   if (q->head == NULL) {
      q->head = n;
   }else{
	q->tail->next = n;
   }
   q->tail = n;
   q->size++;
}

// remove Item from head of Queue; return it
Item leaveQueue (Queue q)
{
   assert(q != NULL);
   Item it = q->head->item;
   Link delNode = q->head;
   q->head = q->head->next;
   free(delNode);
   q->size--;
   return it;
}

// return count of Items in Queue
int queueLength(Queue q)
{
   assert(q != NULL);
   return q->size;
}

// display Queue as list of 2-digit numbers
void showQueue(Queue q)
{
   Link curr;
   curr = q->head;
   while (curr != NULL) {
      printf(" %s\n", curr->item);
      curr =curr->next;
   }
}
//check duplicate item in this queue,
//return -1, not seen
//return other positive integer for id(position in queue)
int isSeen(Queue q, Item it){
	if(q == NULL){
		return -1;
	}

	Link curr;
	int i = 0;
	curr = q->head;
	while (curr != NULL) {
		if(strEQ(curr->item,it)){
			// id
			return i;
		}
		curr =curr->next;
		i++;
	}
	return -1;
}
// get id to find the url in queue
Item getURL(Queue q,int id){
	Link curr;
	int i = 0;
	curr = q->head;
	while (curr != NULL) {
		if( i == id){
			// id
			return curr->item;
		}
		curr =curr->next;
		i++;
	}
	return NULL;
}

//if search word > 1, then find the specify url contain all the search words
char **findURLs(Queue searchWords,char **URLs,int numUrl,int numWord, int *num){
	assert(searchWords != NULL);


	//supprt varibles
	int i,id = 0; // id for the order of urls store in queue e.g url1(id= 0),url2(id=1)
	int foundid;
	Queue seen = createQueue();
	int appear[numUrl];
	// to find times of duplicate URLs in array
	// times == amount, then that is the url contain all the searchWords;
	for(i = 0; i < numUrl;i++){
		if(isSeen(seen,URLs[i]) == -1){
			enterQueue(seen, URLs[i]);
			appear[id++] = 1;
		}else{
			foundid = isSeen(seen,URLs[i]);
			appear[foundid]++;
		}
	}

	//get id to find the urls
	char **finalURLs = NULL;
	char *ptr;
	int N = 0;
	for(i = 0;i<id;i++){
		if(appear[i] == numWord){
			ptr = getURL(seen,i);
			finalURLs = realloc(finalURLs,(N+1)*sizeof(char*));
			finalURLs[N] = malloc(strlen(ptr)+1);
			strcpy(finalURLs[N++], ptr);
		}
	}
	*num = N;
	free(ptr);
	return finalURLs;
}

int *findFrequency(Queue searchWord, char **URLs, int numURL){
	FILE *fp;
	char url[100];
	char caughtWord[100];
	int in_sec2;
	int frequency[numURL];
	int *f;
	int i;


	// find the frequency of the words that each url contains them
	//e.g frequency[1] for URLs[1]

	//main lopp for scanning .txt file
	for(i = 0;i<numURL;i++){
		//copy path
		strcpy(url,URLs[i]);
		strcat(url,".txt");
		fp = fopen(url,"r");
		//in section 2
		//fscanf each words in "url.txt" into urlName array
		//condintioner that whether fp in section-1 or not
		in_sec2 = 0;

		//initilize frequency == 0, avoid error
		frequency[i] = 0;
		while((fscanf(fp, "%s", caughtWord)) == 1){
			//printf("%s\n", urlName);
			//starting to reading section-1
			if ( strEQ(caughtWord,"Section-2") && in_sec2 == 0){
				in_sec2 =1;
			}else if(strEQ(caughtWord,"Section-2") && in_sec2 == 1){
			//end of section 1, break, get next url file
				break;
			}


			//find the frequency
			if(in_sec2 == 1){
				//normalise each caught word
				normalise(caughtWord);
				// if the caughtWord from .txt file == search word
				if(!strEQ(caughtWord,"#end") && !strEQ(caughtWord,"section-2")){
					if(isSeen(searchWord, caughtWord) != -1){
						//frequency for this URL ++;
						frequency[i]++;
						//printf("1\n");
					}
					//printf("i am caught word %s\n",caughtWord);
				}
			}

			//reset caughtWord
			memset(caughtWord,0,sizeof(caughtWord));
		}
		fclose(fp);
	}

	//set f = frequency
	f = frequency;

	return f;
}
//lowcase the array characters
void normalise(char *ch){
	int i;
    char *curr, *next;

    //convert each character in this array to lowercase
    for(i=0; i<strlen(ch); i++){
        ch[i] = tolower(ch[i]);
    }

    //remove ".", ",", "?", ";" characters
    curr = next = ch;
    while(*curr){
        *next = *curr++;
        if((*next != '.' && *next != ','  && *next != ';'  && *next != '?' && *next != ' ' )){
            next++;
        }
    }
    *next = '\0';
}

//order by frequency
//if the frequency is equal,then order by pagerank
void sortByFrequency(char **URL, int *frequency, int numURL, Queue searchWords){
	//support varibles initilize
	int i;
	int tempF;
	char *tempURL;
	double tempTF;
	
	double Tfidf[numURL];
	getTdidf(URL,searchWords,numURL,Tfidf);
	for(i=0;i<numURL;i++){
			//printf("%s ",URL[i]);
			//printf("%.6f\n",Tfidf[i]);
			
	}
	
	//sorting array by bubble sort for frequency
	for(i=1;i<numURL;i++){
		//order by frequency
		if(frequency[i-1] < frequency[i]){
			//swap freqyuency
			tempF = frequency[i-1];
			frequency[i-1] = frequency[i];
			frequency[i] = tempF;

			//swap the URL order to match correctly
			tempURL = URL[i-1];
			URL[i-1] = URL[i];
			URL[i] = tempURL;
			
			//swap tfidf
			tempTF = Tfidf[i-1];
			Tfidf[i-1] = Tfidf[i];
			Tfidf[i] = tempTF;
		}
	}

	//another loop to sort the array with pagerank if the frequency is equal
	for(i=1;i<numURL;i++){
		if(frequency[i-1] == frequency[i]){
			//get the pagerank and check if we need to swap order
			if(Tfidf[i-1] < Tfidf[i]){
				//dont need to swap freqyuency
				//since their frequncy is equal already

				//swap tfidf
				tempTF = Tfidf[i-1];
				Tfidf[i-1] = Tfidf[i];
				Tfidf[i] = tempTF;
				
				//swap the URL order to match correctly
				tempURL = URL[i-1];
				URL[i-1] = URL[i];
				URL[i] = tempURL;
			}
		}
	}
	
	//write to .txt file
	FILE *writer;
	writer = fopen("searchTfidf.txt","w");
	for(i=0;i<numURL;i++){
			fprintf(writer,"%s ",URL[i]);
			fprintf(writer,"%.6f\n",Tfidf[i]);
	}

}
//get the tfidf for each url that we need to output and store in array
void getTdidf(char **URL, Queue searchWords, int numURL, double Tfidf[]){
	//supportive intialize
	FILE *fp;
	FILE *inveIndex;
	char url[MAX];
	char caughtWord[MAX];
	int i;
	Link curr;
	int f,N;// (f is frequency of term t in d) ,N is (number of words in d)
	double tf,idf;//term frequency

  //varibles scan collection.txt file
  char **urlArray = NULL;
	int totalNumURL = 0;

	//varibles for reading invertedIndex;
	char line[MAX];
	int URL_storer,insec2;
	int readingURL;
	int document;//number of file contain this word;
	double temp;
	double temptfidf;

	//id
	int id = 0;
	char *p = NULL;

	//reading collection txt;
	fp = fopen("collection.txt","r");
	while(fscanf(fp,"%s",url) != EOF){
		urlArray = realloc(urlArray, (totalNumURL+1)*sizeof(char*));
		urlArray[totalNumURL] = malloc(strlen(url)+1);
		strcpy(urlArray[totalNumURL++], url);
	}
	memset(url,0,sizeof(url));


	//scan through each url file to find frequency for each searchWord in this url file
	for(i=0;i<numURL;i++){
		//copy path and open file
		strcpy(url,URL[i]);
		strcat(url,".txt");
		fp = fopen(url,"r");
		//main loop to find tf-idf for this url
		//reset the pointer to the head of queue every time open a new url
		curr = searchWords->head;
		temp = 0;
		temptfidf = 0;
		while(curr!=NULL){
			//curr->item;
			//reset insec2 conditioner and tf and N
			insec2 = 0;
			f = 0;
			N = 0;


			//counting tf loop;
			while(fscanf(fp,"%s",caughtWord)){
				//check if come to section 2
				if (strEQ(caughtWord,"Section-2") && insec2 == 0){
					insec2 =1;
				}else if(strEQ(caughtWord,"#end") && insec2 == 1){
				//end of section 2, break, get next url file
				//end of section 2, calculate tf;
					tf = (double) f/N;
					break;
				}

				//now we are in section 2
				if(insec2 == 1){
					//normalise each caught word
					normalise(caughtWord);

					//avoid unneccessary word
					if(!strEQ(caughtWord,"section-2")){
						if(strEQ(caughtWord,curr->item)){
							f++;
						}

						//increase N when read a word in section2;
						N++;
					}
				}
			}

			//to evaulate idf
			//go into invertedIndex file to find the numbers of url contains this word
			inveIndex = fopen("invertedIndex.txt","r");
			document = 0;
			// get each line of invertedIndex.txt file
			while(fgets(line,MAX,inveIndex) != NULL){
				//compare the caught word with search word
				//reset the conditioner
				URL_storer = 0;
				readingURL = 0;
				p = strtok(line," ");
				while(p != NULL){
					//if in the right line, count the number of urls
					// check all the conditioner == 1, and the caughtWord(p)!= space and word
					if(URL_storer == 1 && readingURL == 1 && !strEQ(p,curr->item) &&!isspace(*p)){
						document++;
					}

					//check if searchWord  ==  first word in this line
					if(strEQ(p,curr->item) && readingURL == 0){
						//scan next word in this line
						URL_storer = 1;
						readingURL = 1;
						continue;
					}else if(!strEQ(p,curr->item) && readingURL == 0){
						// if the word is not equal to first word in this specific line
						// then break and move to next line
						break;
					}
					p = strtok(NULL," ");
				}

				//if document != 0, mean we got into right line
				//and we can stop readding the rest of lines
				if(document!=0){
					idf = (double)log(totalNumURL/document);
					break;
				}
			}
			fclose(inveIndex);

			//store tfidf, and sum of tfidf for each searchWord;
			temp = (double)idf * tf;
			temptfidf += temp;
			//go the start of file to server next word
			rewind(fp);
			//get next word
			curr = curr->next;
		}
		fclose(fp);


    //store tdidf for URL
    //Tfidf = realloc(Tfidf,(id+1)*sizeof(double));
    //Tfidf[id] = malloc(sizeof(double));
    Tfidf[id++] = temptfidf;
	}

}





