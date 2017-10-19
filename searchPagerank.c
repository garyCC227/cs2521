#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "searchPagerank.h"
#include <ctype.h>

#define MAX 50
#define strEQ(g,t) (strcmp((g),(t)) == 0)

char **findURLs(Queue searchWords,char **URLs,int numUrl,int N);

int main(int argc, char *argv[]){
	// checking the commanline is a right input
	if(argc == 1){
		fprintf(stderr,"Usage: %s #word1 #word2 #word3 ...\n",argv[0]);
		exit(1);
	}
    
    
    //get all the search words and store them into queue
    // create a new queue and initilize support varibles
    Queue q = createQueue();
    Queue searchWords = createQueue();
    int i;
      
    // store words loop
    for (i = 1;i < argc;i++){
        enterQueue(q, argv[i]);
        enterQueue(searchWords, argv[i]); 
    }
    //number of word need to search
    int N = argc - 1;
    
	//support varible initialize
	char *word;
	//char caughtWord[MAX];
	char line[MAX];
	FILE *fp;
	fp = fopen("invertedIndex.txt","r");
	
	//initilize varibles for helping loop
	char **URLs = NULL;
	int URL_storer,readingURL;
	int numUrl = 0;
	char *p = NULL;
	
	// starting scan via URL file 
	while(queueLength(q)!= 0){
	    // get search word out from queue
	    word = leaveQueue(q);
	    // get each line of invertedIndex.txt file
	    while(fgets(line,MAX,fp) != NULL){
	        //compare the caught word with search word
	        //reset the conditioner
	        URL_storer = 0;
	        readingURL = 0;
	        p = strtok(line," ");
	        while(p != NULL){
	    		//if in the right line, store the url
	    		//in to an array, this array contain all the urls for all the words
	    		// check all the conditioner == 1, and the caughtWord(p)!= space and word
	    		if(URL_storer == 1 && readingURL == 1 && !strEQ(p,word) &&!isspace(*p)){
					URLs = realloc(URLs,(numUrl+1)*sizeof(char*));
					URLs[numUrl] = malloc(strlen(p)+1);
					strcpy(URLs[numUrl++], p);
	    			//printf("%s\n",p);
	    		}
	    		
	    		//check if word  ==  first word in caught line
	    		if(strEQ(p,word) && readingURL == 0){
	    			//scan next word in this line
	    			URL_storer = 1;
	    			readingURL = 1;
	    			continue;
	    		}else if(readingURL == 0){
	    			// if the word is not equal to first word in this specific line
	    			// then break and move to next line
	    			break;
	    		}
	    		p = strtok(NULL," ");
	   	    }
	    }
	    rewind(fp);
	}
	char **finalURLs;
	//if search word > 1, then find the url contain all the search words
	if(queueLength(searchWords) > 1){
		finalURLs= findURLs(searchWords,URLs,numUrl,N);
	}else{
	//search word == 1,
	//URLs is uesd
	}
	
	return 0;
}

//if search word > 1, then find the specify url contain all the search words
char **findURLs(Queue searchWords,char **URLs,int numUrl,int numWord){
	assert(searchWords != NULL);
	
	//the amount of search words == the times of appearance of url in URLs array
	int amount;
	amount = queueLength(searchWords);
	
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
	int num = 0;
	char *ptr;
	for(i = 0;i<id;i++){
		if(appear[i] == numWord){
			ptr = getURL(seen,i);
			finalURLs = realloc(finalURLs,(num+1)*sizeof(char*));
			finalURLs[num] = malloc(strlen(ptr)+1);
			strcpy(finalURLs[num++], ptr);
		}
	}
	
	free(ptr);
	return finalURLs;
}



