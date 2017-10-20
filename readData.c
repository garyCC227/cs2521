#include "readData.h"
#include <stdio.h>
#include <stdlib.h>



#define strEQ(g,t) (strcmp((g),(t)) == 0)

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

bool inGraph(char *str, char **names,int numV){
	int i = 0;
	while(i < numV){
		if(strcmp(str,names[i]) == 0) return 1;
		
		i++;
	}
	// if not in graph, return 0
	return 0;
}



