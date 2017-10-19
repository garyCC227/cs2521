#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "searchPagerank.h"

#define MAX 50;
#define strEQ(g,t) (strcmp((g),(t)) == 0)

int main(int argc, char *argv[]){
	// checking the commanline is a right input
	if(argc == 1){
		fprintf("Usage: %s #word1 #word2 #word3 ...",argv[0]);
		exit(1);
	}
    
    
    //get all the search words and store them into queue
    // create a new queue and initilize support varibles
    Queue q = createQueue();
    int i;
      
    // store words loop
    for (i = 1;i < argc;i++){
        enterQueue(q, argv[i]); 
    }
	
	//support varible initialize
	char *word;
	char caughtWord[MAX];
	char line[MAX];
	FILE *fp;
	fp = fopen("invertedIndex.txt","r");
	// starting scan via URL file 
	while(queueLength(q)!= 0){
	    // get search word out from queue
	    word = leaveQueue(q);
	    
	    // get each line of invertedIndex.txt file
	    while(fgets(line,MAX,fp) != NULL){
	        //compare the caught word with search word
	        while(sscanf(line,"%s",caughtWord))
	    
	    
	    
	    }
	}
	
	
	
	
	return 0;
}
