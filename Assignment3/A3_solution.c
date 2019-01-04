/* FILE: A3_solutions.c is where you will code your answers for Assignment 3.
 * 
 * Each of the functions below can be considered a start for you. They have 
 * the correct specification and are set up correctly with the header file to
 * be run by the tester programs.  
 *
 * You should leave all of the code as is, especially making sure not to change
 * any return types, function name, or argument lists, as this will break
 * the automated testing. 
 *
 * Your code should only go within the sections surrounded by
 * comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A3_solutions.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A3_provided_functions.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size, 
          unsigned int *data_offset                                  )
{
    unsigned char *img_data=NULL;
    FILE *bmpfile = fopen( bmp_filename, "rb" );
    char b, m;
    fread (&b,1,1,bmpfile);
    fread (&m,1,1,bmpfile);
    unsigned int overallFileSize;
    fread( &overallFileSize, 1, sizeof(unsigned int), bmpfile );
    rewind(bmpfile);
    char imageData[overallFileSize];
    fread( imageData, 1, overallFileSize, bmpfile );
    unsigned int* wp = (unsigned int*)(imageData+18);
    *width = *wp;
    unsigned int* hp =(unsigned int*)(imageData+22);
    *height= *hp;
    unsigned short int* bppp =(unsigned short int*)(imageData+28);
    *bits_per_pixel = *bppp;
    *data_size=overallFileSize;
    unsigned int* offp = (unsigned int*)(imageData+10);
    *data_offset=* offp;
    *padding=(4-((*width)*(*bits_per_pixel/8))%4)%4;
    rewind(bmpfile);
    img_data = (unsigned char*)malloc( sizeof(unsigned char*) * (overallFileSize));
    fread( img_data, 1, overallFileSize, bmpfile );
    fclose(bmpfile);

  return img_data;  
}


void 
bmp_close( unsigned char **img_data )
{
 
    free(*img_data);

}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
  unsigned char*** new_pixel_array = NULL; 

    if(scale>1){
        unsigned int newwidth=(unsigned int)((*width) * scale);
        unsigned int newheight=(unsigned int)((*height) * scale);
        
        new_pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (newheight));
        
        for( int row=0; row<newheight; row++ ){
            new_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (newwidth) );
            for( int col=0; col<newwidth; col++ ){
                new_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (num_colors) );
            }
        }  
        
        for(int row=0;row<newheight;row++){
            for (int col=0;col<newwidth;col++){
                for(int color=0;color<num_colors;color++){
                    new_pixel_array[row][col][color]=pixel_array[(unsigned int)(row/scale)][(unsigned int)(col/scale)][color];
                }
            }
        }
        *height=newheight;
        *width=newwidth;
        
        unsigned int *widthp=(unsigned int*)(header_data+18);
        *widthp=newwidth;
        
        unsigned int *heightp=(unsigned int*)(header_data+22);
        *heightp=newheight;
        
        unsigned int newsize = header_size+newheight*newwidth*4;
        unsigned int *newoverallsize = (unsigned int*)(header_data+34);
        *newoverallsize = newsize;
     }
    
    
    else if(scale<=1.0){
        unsigned int newwidth=(unsigned int)((*width)*scale);
        unsigned int newheight=(unsigned int)((*height)*scale);
        
        new_pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (newheight));
        
        for( int row=0; row < newheight; row++ ){
            new_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (newwidth) );
            for( int col=0; col < newwidth; col++ ){
                new_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (num_colors) );
            }
        }
        
        int ratio=(int)(1/scale);
        
        for(int row=0;row<newheight;row++){
            for (int col=0;col<newwidth;col++){
                for(int color=0;color<num_colors;color++){
                    new_pixel_array[row][col][color]=pixel_array[(int)(row*ratio)][(int)(col*ratio)][color];
                }
            }
        }
        *height=newheight;
        *width=newwidth;
        unsigned int *widthp=(unsigned int*)(header_data+18);
        *widthp=newwidth;
        unsigned int *heightp=(unsigned int*)(header_data+22);
        *heightp=newheight;
        unsigned int newsize = header_size + newheight*newwidth*4;
        unsigned int *newoverallsize = (unsigned int*)(header_data+34);
        *newoverallsize = newsize;
    }
 
  return new_pixel_array;
}         

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )
{

    unsigned char*   header_data1;
    unsigned char*   header_data2;
    
    unsigned int     header_size1, width1, height1, num_colors1;
    unsigned int     header_size2, width2, height2, num_colors2;
    
    unsigned char*** pixel_array1 = NULL;
    unsigned char*** pixel_array2 = NULL;
    
    pixel_array1 = bmp_to_3D_array( background_image_filename,       &header_data1,
                                  &header_size1,  &width1,
                                  &height1,       &num_colors1   );
    
    pixel_array2 = bmp_to_3D_array( foreground_image_filename,       &header_data2,
                                   &header_size2,  &width2,
                                   &height2,       &num_colors2   );
    
    unsigned char*** scaled_pixel_array = bmp_scale( pixel_array2, header_data2, header_size2,
                                                    &width2, &height2, num_colors2,  scale   );
    for(int row=0;row<height2;row++){
        for (int col=0;col<width2;col++){
            if(scaled_pixel_array[row][col][0]!=0){
                for(int color=1;color<num_colors1;color++){
                    pixel_array1[row+row_offset][col+col_offset][color]=scaled_pixel_array[row][col][color];
                    
                }
            }
        }
    }
    bmp_from_3D_array( output_collage_image_filename, header_data1, header_size1, pixel_array1,
                      width1,   height1,      num_colors1                      );
 
  return 0;
}              

