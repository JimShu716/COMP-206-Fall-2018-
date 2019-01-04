#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A5_provided_functions.h"

/* FUNCTION: bmp_open */
unsigned char*
bmp_open(     char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset  ){

  unsigned char *img_data=NULL;

  FILE *fp;
  if( ( fp = fopen( bmp_filename, "rb" ) ) == NULL ){
      printf( "Failed to open file %s.\n", bmp_filename );
      return NULL;
  }

  char b, m;
  fread( &b, 1, 1, fp );
  fread( &m, 1, 1, fp );

  fread( data_size, 1, sizeof(int), fp );

  rewind(fp);

  img_data = (unsigned char*)malloc(*data_size);
  
  if( img_data == NULL ){
    printf( "Error: Failed to malloc space for image of size %d.\n", *data_size );
    return img_data;
  }

  fread( img_data, 1, *data_size, fp );

  *data_offset = *((unsigned int*)(img_data+10));
  *width = *((unsigned int*)(img_data+18));
  *height = *((unsigned int*)(img_data+22));
  *bits_per_pixel = *( (unsigned short int*)(img_data+28));

  *padding = ( 4 - (*width * (*bits_per_pixel/8)) % 4) % 4;
  
  return img_data;  
}

/* FUNCTION: bmp_close */
void bmp_close( unsigned char **img_data ){

  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
}

/* FUNCTION: buffer_to_3D_array */
unsigned char*** 
buffer_to_3D_array( char *img_data,             unsigned char** header_data, 
                    unsigned int* header_size,  unsigned int *width, 
                    unsigned int* height,       unsigned int* num_colors      ){      
  
  *header_size = *((unsigned int*)(img_data+10));
  *width = *((unsigned int*)(img_data+18));
  *height = *((unsigned int*)(img_data+22));
  unsigned short int bits_per_pixel = *( (unsigned short int*)(img_data+28));

  *num_colors = bits_per_pixel/8;

  unsigned char*** pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (*height));
  if( pixel_array == NULL ){
    printf( "Error: bmp_to_3D_array failed to allocate memory for image of height %d.\n", (*height) );
    return NULL;
  }
  
  for( int row=0; row<*height; row++ ){
    pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (*width) );
    for( int col=0; col<*width; col++ ){
      pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (*num_colors) );
    }
  }

  unsigned int padding = ( 4 - (*width * (*num_colors)) % 4) % 4;
  unsigned char *pixel_data_ptr = img_data + (*header_size);
  int row_bytes = (*num_colors) * (*width) + padding;

  for( int row=0; row<*height; row++ )
    for( int col=0; col<*width; col++ )
      for( int color=0; color<*num_colors; color++ )
        pixel_array[*height-row-1][col][color] = pixel_data_ptr[row*row_bytes + col*(*num_colors) + color ];
  
 
  *header_data = (unsigned char*)malloc( *header_size );
  memcpy( *header_data, img_data, *header_size );

  return pixel_array;
}               

/* FUNCTION: bmp_to_3D_array */
unsigned char*** 
bmp_to_3D_array( char *input_bmp_filename,   unsigned char** header_data, 
                 unsigned int* header_size,  unsigned int *width, 
                 unsigned int* height,       unsigned int* num_colors      ){

  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned char* img_data    = NULL;
  img_data = bmp_open( input_bmp_filename, width, height, &bits_per_pixel, &padding, &data_size, header_size ); 

  if( img_data == NULL ){ 
    printf( "Error: bmp_open returned NULL. Returning from bmp_to_3D_array without attempting changes.\n" ); 
    return NULL; 
  }

  unsigned char *** pixel_array = buffer_to_3D_array( img_data, header_data, header_size, width, 
                             height,   num_colors                          );

  bmp_close( &img_data );

  return pixel_array;
}

/* FUNCTION: buffer_from_3D_array */
unsigned char* 
buffer_from_3D_array( unsigned char* header_data,
                      int header_size,         unsigned char*** pixel_data,
                      int width, int height,   int num_colors,
                      int *buffer_size                  )
{
  int padding = ( 4 - (width * num_colors) % 4) % 4;
  int row_size = width*num_colors+padding;
  int raw_pixel_size = height*row_size;
  unsigned char* img_data = (unsigned char*)malloc( header_size + raw_pixel_size );

  memcpy( img_data, header_data, header_size );
  char* raw_pixels = img_data + header_size;
  
  for( int row=0; row<height; row++ ){
    for( int col=0; col<width; col++ ){
      for( int color=0; color<num_colors; color++ ){
        raw_pixels[row*row_size + col*num_colors + color] = pixel_data[height-row-1][col][color];
      }
    }
  }

  *buffer_size = header_size + raw_pixel_size;

  return img_data;
}

/* FUNCTION: bmp_from_3D_array */
int 
bmp_from_3D_array( char* output_filename,   unsigned char* header_data,
                   int header_size,         unsigned char*** pixel_data,
                   int width, int height,   int num_colors                  )
{
  int buffer_size = 0;
  char *img_data = buffer_from_3D_array( header_data, header_size, pixel_data, width, height, num_colors, &buffer_size );

  FILE *out_fp = fopen( output_filename, "wb" );
  if( out_fp == NULL ){
    printf( "Error: bmp_from_3D_array could not open file %s for writing.\n", output_filename );
    return -1;
  }
  
  fwrite( img_data, 1, buffer_size, out_fp );
  fclose( out_fp );
  
  return 0; 
} 
