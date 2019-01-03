#include <stdio.h>
#include <stdlib.h>

int determinePower(int posint)/* this is a helper function to determine whether the h/2 is a power of 2*/
{
    int test = 1;
    int n = 0;
    while(test <= posint){
        if(test != posint){
            test = test*2;
        }
        else{
            n = 1;
            break;
        }
    }
    return n;
}

void drawTriangle(char **diamond, int r, int c, int h)

{
    int i = 1; 
    int j = 0; 
    while(i <= h ){
        for (j = 0; j < 2*i - 1; j++){
            diamond[r-i+1][c-i+1+j] = ' '; 
        }

        i++;
    }
}




void divide1(int h, int l, char **diamond,int rl,int cl,int rr,int cr,int ru,int cu){/*note that h is the height of the diamond, r,c are the coordinates of the points on the first triangle  */
    int triHeight = (h+1)/2;

    if(l == 1){
        return;
    }else{
        drawTriangle(diamond,(rl+rr)/2,(cl+cr)/2,triHeight/2);
        l--;
        divide1((h-1)/2,l,diamond,rl,cl,(rl+rr)/2,(cl+cr)/2-1,(rl+ru+1)/2,(cl+cu-1)/2);
        divide1((h-1)/2,l,diamond,(rl+rr)/2,(cl+cr)/2+1,rr,cr,(rr+ru+1)/2,(cr+cu+1)/2);
        divide1((h-1)/2,l,diamond,(rl+ru-1)/2,(cl+cu+1)/2,(rr+ru-1)/2,(cr+cu-1)/2,ru,cu);
        }   
}

int power(int x, int y){
    int result=1;
    for(int j=1; j<=y; j++){
        result = result *x;
    }
    return result;
}



void clearMidTri(char **diamond, int h){
            /*store the information in a 2D array */
    int i = 1;/* i means the number of asterisks in each row*/
        int j = 1;/* j means for the index of printing in the loop*/



        for(i = 1; i <= h; i+=2) 
        {  
            for(j = 1; j <= h; j++){
            
                if( j <= (h -i) / 2 || j > ( h + i ) / 2 ){
                    diamond[j-1][(i+1)/2-1] = ' ';
                }else{
                    diamond[j-1][(i+1)/2-1] = '*';
                }
            }
        }

        j=1;

        for(i = h-2; i >= 1; i-=2)
        {
            for(j = 1; j <= h; j++){
            
                if( j <= (h -i) / 2 || j > ( h + i ) / 2 ){
                    diamond[j-1][(2*h+1-i)/2-1] = ' ';
                }else{
                    diamond[j-1][(2*h+1-i)/2-1] = '*';
                }
            } 
        }
}


void main(int argc, char* argv[] ){
    int height = atoi(argv[1]);
    int l = atoi(argv[2]);
    int triHeight = (height+1)/2;

    if(!(argc ==3))
    {
        printf("ERROR: Wrong number of arguments. Two required.");
    }else if(determinePower(triHeight) == 0 | triHeight < power(2, l-1)){
        printf("ERROR: Height does not allow evenly dividing requested number of levels.");
    }else if(height % 2 == 0 || height <= 0){
        printf("ERROR: Bad argument. Height must be positive odd integer.");
    }else{

        char **diamond;
        int m=0,n=0;
        int row = height, col =height;
        diamond = (char **)malloc(row*sizeof(char *));
        for(m=0;m<height;m++){

            *(diamond+m)=(char *)malloc(col*sizeof(char));
        }




        for(m=0;m<5;m++){
            for(n=0;n<height;n++){
                diamond[m][n]=' ';
            }
        }

        clearMidTri(diamond,height);

            

        divide1(height,l,diamond,(height+1)/2-1,0,(height+1)/2-1,height-1,0,(height+1)/2-1);
        
        for(int i=0;i<=(height/2-1);i++){

        	diamond[height/2+1+i]=diamond[height/2-i-1];
        }
        int r = 0;
   		int c = 0;
    	for(r=0; r < height; r++){
        	for(c=0; c<height;c++){
            printf("%c",diamond[r][c]);
        	}
        	printf("\n");
    	}
     }
}	