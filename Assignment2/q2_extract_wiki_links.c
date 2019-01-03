#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* args[]){
///void something(char * filename){	
	char *filename=args[1];
	FILE *fp = fopen( filename, "r" );	
	int listIndex=0; 
  	if( !fp ){
  		printf("no such file\n");
  		return 0;
  	} 
  	char *first="<a href=\"/wiki/";
	char *title= "title=\"";
	char *last="</a>";
 	fseek( fp, 0, SEEK_END );
  	int size = ftell( fp );
  	rewind(fp);
  
  	char array[size+1];
	fread( array, size+1, 1, fp );
	

	char *curr=array;
	while(strstr(curr,first)){
		

		curr=strstr(curr,first)+15;
		

		int i=1;
		while(1){
		
			if(*(curr+i)=='"'){
				*(curr+i)='\0';
				break;
			}
			i++;
		}
		char * pageName=curr;
		//printf("%s\n",pageName );
		curr=curr+i+1;

	
		
		if(strstr(curr,last)){
			char*end=strstr(curr,last);
			*(end+4) ='\0';
			
			char * mid;
			if((mid=malloc(strlen(title)+strlen(pageName)+1))!=NULL){
				mid[0]='\n';
				strcpy(mid,title);
				strcat(mid,pageName);
			}else{
				fprintf(stderr, "malloc failed\n" );
				
			}
			
			if(strstr(curr,mid)){
				
				printf("%s\n",pageName );
				
			}

			curr=end+5;
	
		}
	
	}
	
	return 1;
}
