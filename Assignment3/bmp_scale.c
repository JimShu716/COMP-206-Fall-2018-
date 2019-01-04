/* FILE: bmp_scale.c is provided as part of COMP 206 A3. Do not modify this file.
 *       Its purpose is to test that you have implemented bmp_scale correctly for
 *       Question 2. When you're finished, the output of this program should match
 *       the examples in the assignment document.
 */

#include <stdio.h>
#include <stdlib.h>
#include "A3_solution.h"
#include "A3_provided_functions.h"

int main( int argc, char* argv[] ){

  if( argc != 4 ){
    printf( "bmp_mask must be run with parameters specifying an image scaling.\n");
    printf( "Usage: $ %s <bmp_input_file> <bmp_output_file> scale\n", argv[0] );
    exit(EXIT_FAILURE);
  }
  
  double scale = atof(argv[3]);
  
  if( scale < 0.1 || scale > 10.0 ){
    printf( "Error: Scale argument must be a floating point between 0.1 and 10.0. You entered %f.\n", scale );
    return 0;
  }
  
  unsigned char*   header_data;
  unsigned int     header_size, width, height, num_colors;
  unsigned char*** pixel_array = NULL;
  
  pixel_array = bmp_to_3D_array( argv[1],       &header_data, 
                                 &header_size,  &width, 
                                 &height,       &num_colors   );
                                 
  if( pixel_array == NULL ){
    printf( "Error: bmp_to_3D_array failed for file %s.\n", argv[1] );
    return 0;
  }
                                 
  unsigned char*** scaled_pixel_array = bmp_scale( pixel_array, header_data, header_size,
                                                   &width, &height, num_colors,  scale   );
                                                  
  if( scaled_pixel_array == NULL ){
    printf( "Error: Call to bmp_scale failed!\n" );
    return 0;
  }                                          

  bmp_from_3D_array( argv[2], header_data, header_size, scaled_pixel_array,
                     width,   height,      num_colors                       );
    
  return 0;
}

