#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readData.h"
#include "graph.h"
#include <bsd/string.h>
#include <stdbool.h>

char **getUrlArray(){
    
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
    printf("%d\n", size);
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
    for (i = 0; i < size; i++){
        printf("%s\n", urlArray[i]);
    }    
    fclose(fp);
    
    return urlArray;
}

int get_size_of_array(char **urlArray){
	
	int i =0;
	while(urlArray[i]!='\0') i++;
	fprintf(stderr,"how many URLs: %d\n", i);
	return i;
}

Graph getGraph( char **urlArray, int numV){	
	//new graph and pass number of vertexs argument
	Graph g = newGraph(numV);	
	int i;
	FILE *fp;
	char url[100];
	char urlName[100];	
	//building graph
	for(i = 0;i<numV;i++){
		//copy path	
		strlcpy(url,urlArray[i],sizeof(urlArray[i]));
		strlcat(url,".txt",10*sizeof(char));
		fp = fopen(url,"r");	
		//in section 1
		//scan into the file , get the outlinks url name
		
		//fscanf each words in "url.txt" into urlName array
		while(!fscanf(fp, "%s", urlName)){
			for(int j = 0;j<numV;j++){
				//get a url;
				if(g->nV < numV && inGraph(urlName,g->vertex)){
					addEdge(g, urlName, urlArray[j]);
				}
			}
			memset(urlName,0,sizeof(urlName));
		}
	}
	
	return g;
}

bool inGraph(char *str, char **names){
	int i = 0;
	while(names[i] != '\0'){
		if(strcmp(str,names[i]) == 0) return 1;
		i++;
	}
	// if not in graph, return 0
	return 0;
}


//invertedList(BST tree);
//e.g
//level1:url11------->url22--------->url33
//		  
//level2:word1 word2 word3 word4 	word5 word6
