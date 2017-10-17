

int main (int argc, char *argv[]){
    
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
    char urlArray[size+1][size];  
    //scan into urlArray the url
    for (i = 0; i < size; i++){
        fscanf(fp, "%s", urlArray[i]);
    }
    for (i = 0; i < size; i++){
        printf("%s\n", urlArray[i]);
    }    
    fclose(fp);
    return 0;
}

int get_size_of_array(char * list){
	int i =0;
	
	while(list[i]!='\0') i++;
	
	return i;
}

Graph getGraph( char **list of url name, int numV){
	
	//new graph and pass number of vertexs argument
	Graph g = newGraph(numV);
	
	int i;
	FILE *fp;
	char url[10];	
	//building graph
	for(i = 0;i<numV;i++)
	{
		//copy path	
		strlcpy(url,list[i],sizeof(list[i]));
		strlcat(url,".txt",10*sizeof(char));
		fp = fopen(url,"r");	
		//in section 1
		while(fscanf != NULL){
			buff[first_url] = #;
			for(int j = 0;i<numV;i++)
			{
				//get a url;
				if(g->nV < maxURLs && inGraph(buff,g->vertex)){
					addEdge(g, firstURL, result);
				}
			}
		
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
//		  /    \       /   \	      /   \
//level2:word1 word2 word3 word4 	word5 word6
