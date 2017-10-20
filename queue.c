// Queue.c ... list implementation of a queue

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "queue.h"
#include <ctype.h>


#define strEQ(g,t) (strcmp((g),(t)) == 0)

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

