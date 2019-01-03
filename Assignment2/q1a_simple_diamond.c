#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *args[]){
	int height= atoi(args[1]);
	 if (argc !=2){
		
                printf("ERROR: Wrong number of arguments.One required.");
		return 0;
	}
	
         if ((height%2)!=1){
		
                  printf("ERROR: Bad argument.Height must be positive odd integer.");
		  return 0;
	}

	for (int a=1; a<=((height+1)/2);a++){
		
		for(int i=0;i<(((height-1)/2)-a+1);i++){
		   	char blank=' ';
            printf("%c",blank);
		}
	
		for(int i=0;i<(2*a-1);i++){
         		char star='*';
        		printf("%c",star);
 		}
		for(int i=0;i<(((height-1)/2)-a+1);i++){
		   	char blank=' ';
            printf("%c",blank);
		}
		printf("\n");
	}
	

	for (int j=1;j<=((height-1)/2);j++){
		for(int i=0;i<j;i++){
		   	char blank=' ';
            printf("%c",blank);
		}
		//printAsterisk;
		for(int i=0;i<(height-2*j);i++){
         		char star='*';
        		printf("%c",star);
 		}
		//printBlankspace(j);
		for(int i=0;i<j;i++){
		   	char blank=' ';
            printf("%c",blank);
		}
		printf("\n");
	}	
	return 1;
       
}
