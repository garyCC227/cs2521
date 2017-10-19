#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "searchPagerank.h"
#include "invertedIndex.h"
#include <ctype.h>

#define MAX 50
#define strEQ(g,t) (strcmp((g),(t)) == 0)

char **findURLs(Queue searchWords,char **URLs,int numUrl,int N,int *);
int *findFrequency(Queue searchWord, char **URLs, int numURL);
void normalise(char *ch);
void sortByFrequency(char **URL, int *frequency, int numURL);
double getPageRank(char * URL);

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
	int *frequency;
	int FNumURL = 0;
	//if search word > 1, then find the url contain all the search words
	if(queueLength(searchWords) > 1){
		//get the urls contain all the search words
		finalURLs= findURLs(searchWords,URLs,numUrl,N,&FNumURL);
		//find frequency 
		frequency = findFrequency(searchWords,finalURLs,FNumURL);
		//sort the URLs in frequency order, if the frequency is equal, then oreder by pageRank
		sortByFrequency(finalURLs,frequency,FNumURL);
		//write to file
	}else{
	//search word == 1,
	//URLs is uesd
	}
	
	return 0;
}

//if search word > 1, then find the specify url contain all the search words
char **findURLs(Queue searchWords,char **URLs,int numUrl,int numWord, int *num){
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
void sortByFrequency(char **URL, int *frequency, int numURL){
	//support varibles initilize
	int i;
	int tempF;
	char *tempURL;

	//sorting array by bubble sort for frequency
	for(i=1;i<numURL;i++){
		//order by frequency
		if(frequency[i-1] < frequency[i]){
			//swap freqyuency
			tempF = frequency[i-1];
			frequency[i-1] = frequency[i];
			frequency[i] = frequency[i-1];
		
			//swap the URL order to match correctly
			tempURL = URL[i-1];
			URL[i-1] = URL[i];
			URL[i] = URL[i-1];
		}
	}
	
	//another loop to sort the array with pagerank if the frequency is equal
	for(i=0;i<numURL;i++){
		if(frequency[i-1] == frequency[i]){
			//get the pagerank and check if we need to swap order
			if(getPageRank(URL[i-1]) < getPageRank(URL[i])){
				//dont need to swap freqyuency
				//since their frequncy is equal already
				
				//swap the URL order to match correctly
				tempURL = URL[i-1];
				URL[i-1] = URL[i];
				URL[i] = URL[i-1];
			}
		}	
	}
}
//get the pagerank from pagerank.txt file
double getPageRank(char * URL){
	FILE *fp;
	fp = fopen("pagerankList.txt","r");
	char line[MAX];
	int Line_storer;
    int readingLine;
    char *p = NULL;
   	double pageRank;
	
	while(fgets(line,MAX,fp) != NULL){
	        //reset the conditioner
	        Line_storer = 0;
	        readingLine = 0;
	        p = strtok(line,",");
	        while(p != NULL){
	    		//if in the right line, get the pagerank
	    		// check all the conditioner == 1, and the caughtWord(p)!= space and word
	    		if(Line_storer == 1 && readingLine == 1 && !strEQ(p,URL) &&!isspace(*p)){
	    			pagerank = ;
	    		}
	    		//check if word  ==  first word in caught line
	    		if(strEQ(p,URL) && readingLine == 0){
	    			//scan next word in this line
	    			Line_storer = 1;
    				readingLine = 1; 
	    			continue;
	    		}else if(readingLine == 0){
	    			// if the word is not equal to first word in this specific line
	    			// then break and move to next line
	    			break;
	    		}
	    		p = strtok(NULL,",");
	   	    }
	
	
	}
	return pageRank;
}


