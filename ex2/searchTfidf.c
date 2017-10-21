#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "searchTfidf.h"
#include "support.h"
#include <ctype.h>

#define MAX  50  



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
	Queue searchWords1 = createQueue();
	int i;
      
    // store words loop into queue
    for (i = 1;i < argc;i++){
        enterQueue(q, argv[i]);
		enterQueue(searchWords, argv[i]); 
		enterQueue(searchWords1, argv[i]);
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
	int *frequency;
	int FNumURL = 0;
	//if search word > 1, then find the url contain all the search words
	if(queueLength(searchWords) > 1){
		//get the urls contain all the search words
		finalURLs= findURLs(searchWords,URLs,numUrl,N,&FNumURL);
		
		//find frequency 
		frequency = findFrequency(searchWords,finalURLs,FNumURL);

		//sort the URLs in frequency order, if the frequency is equal, then oreder by pageRank
		// write to file
		sortByFrequency(finalURLs,frequency,FNumURL,searchWords1);

	}else{
	//search word == 1,
		
		//find frequency 
		frequency = findFrequency(searchWords,URLs,numUrl);

		//sort the URLs in frequency order, if the frequency is equal, then oreder by pageRank
		//write to file
		sortByFrequency(URLs,frequency,numUrl,searchWords1);
		//print out final result;
	}
	
	return 0;
}







