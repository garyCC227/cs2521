#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <bsd/string.h>
#include "invertedIndex.h"
#include <stdbool.h>

#define MAX_L 50
#define strEQ(g,t) (strcmp((g),(t)) == 0)

void invertedIndex(){
	
	int i;
	FILE *fp;
	int numUrl = 0;
	char url[MAX_L];// each url name
	char **urlArray = NULL; // all the urls
	
	//collection 
	fp = fopen("collection.txt","r");
	while(fscanf(fp,"%s",url) != EOF){
        urlArray = realloc(urlArray, (numUrl+1)*sizeof(char*));
        urlArray[numUrl] = malloc(strlen(url)+1);
		strcpy(urlArray[numUrl++], url);
	}
	// sortArray urlArray in alph order
	sortArray(urlArray, numUrl);
	// normalise urlArray
	for(i=0;i<numUrl;i++) {
		normalise(urlArray[i]);	
	}
	
	
	
	// reading each url.txt file to get all the words
	//reset url array to 0
	memset(url,0,sizeof(url));
	int in_sec2;
	//number of words
	int numW = 0;
	//words to store all the caught words
	char **words = NULL;
	//caught word
	char caughtWord[100];
	for(i = 0;i<numUrl;i++){
		//copy path	
		strcpy(url,urlArray[i]);
		strcat(url,".txt");
		fp = fopen(url,"r");	
		//fscanf each words in "url.txt" into urlName array
		//condintioner that whether fp in section-1 or not
		in_sec2 = 0;
		while((fscanf(fp, "%s", caughtWord)) == 1){
			//starting to reading section-2 
			//printf("%s\n", caughtWord);
			if ( strEQ(caughtWord,"Section-2") && in_sec2 == 0){
				in_sec2 =1;
			}else if(strEQ(caughtWord,"Section-2") && in_sec2 == 1){
			//end of section 2, break, get next url file
				break;
			}
			if(in_sec2 == 1){
				//normalise each caught word
				normalise(caughtWord);
				//avoid some unneccessary word
				if(!strEQ(caughtWord,"#end") && !strEQ(caughtWord,"section-2")){
					// avoid duplicate word
					if(NoduplicateWord(caughtWord,words,numW)){
						words = realloc(words, (numW+1)*sizeof(char*));
						words[numW] = malloc(sizeof(caughtWord));
						strcpy(words[numW++], caughtWord);
					}
				}
			}
			memset(caughtWord,0,sizeof(caughtWord));
		}
		fclose(fp);
	}
	
	// sortArray all the words in alph order
	sortArray(words, numW);
	//for(i = 0;i<numW;i++) printf("%s\n",words[i]);
	//write to invetedInde.txt file
	writeToFile(numW,numUrl,urlArray,words);
	
	//free memory 
	freeMemory(urlArray, numUrl);
	freeMemory(words, numW);
	

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

// sortArraying the array in alph order by bubble sortArray
void sortArray(char **array, int maxA){

	int i,j;
	char *temp;
	for(i = 1;i < maxA;i++){
		for(j = 1;j < maxA;j++){
			if(strcmp(array[j-1],array[j])>0){
				temp = array[j - 1];
		 		array[j - 1] = array[j];
            	array[j] = temp;
			}
			
		}		
	}
}

//to check whether there are duplicate word in the wordArray or not;
//not duplicate return 1
//duplicate return 0
int NoduplicateWord(char *word, char **storage,int numW){
	// storage == NULL, then start putting the first word into storage 
	if(storage == NULL){
		return 1;
	}

	int i;
	for(i = 0; i < numW;i++){
		if(strEQ(word,storage[i])) return 0;
	}

	return 1;
}

void writeToFile(int numW, int numUrl, char ** urlArray, char **words){
	
	//scan each word in words to find any url contain this word
	FILE *write = fopen("invertedIndex.txt","w");
	FILE *fp;
	int i,j;
	char url[100];// each url name
	int in_sec2;
	char caughtWord[100];
	
	//for(i = 0;i < numUrl;i++) printf("%s\n",urlArray[i]);

	
	for(i = 0; i < numW; i++){
		//write the word into txt
		fprintf(write, "%s ", words[i]);
		//scan each url.txt file
		for(j = 0; j <numUrl;j++){
			//copy path
			strcpy(url,urlArray[j]);
			strcat(url,".txt");
			fp = fopen(url,"r");
			in_sec2 = 0;
			while((fscanf(fp, "%s", caughtWord)) == 1){
				//starting to reading section-1 
				if ( strEQ(caughtWord,"Section-2") && in_sec2 == 0){
					in_sec2 =1;
				}else if(strEQ(caughtWord,"Section-2") && in_sec2 == 1){
				//end of section 2, break, get next url file
					break;
				}
				if(in_sec2 == 1){
					//normalise each caught word
					normalise(caughtWord);
					//avoid some unneccessary word
					if(!strEQ(caughtWord,"#end") && !strEQ(caughtWord,"section-2")){
						//check caughtWord == words[i] or not?
						if(strEQ(words[i],caughtWord)){
							fprintf(write,"%s ",urlArray[j]);
							break;
						}
					
					}
				}
				memset(caughtWord,0,sizeof(caughtWord));
			}
			fclose(fp);
		}
		// write a new line for each word when scanning each url .txt is finished
		if(i != numW-1){
           	fprintf(write, "\n");
		}
	}
	
}

//free memoery
void freeMemory(char **ptr, int length){
    int i; 

    for(i=0; i<length; i++){
        free(ptr[i]);
    }

    free(ptr);
}

