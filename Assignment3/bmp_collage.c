/* FILE: bmp_collage.c is provided as part of COMP 206 A3. Do not modify this file.
 *       Its purpose is to test that you have implemented bmp_collage correctly for
 *       Question 3. When you're finished, the output of this program should match
 *       the examples in the assignment document.
 */

#include <stdio.h>
#include <stdlib.h>
#include "A3_solution.h"

int main( int argc, char* argv[] ){

  if( argc != 7 ){
    printf( "bmp_collage must be run with parameters that allow two images to be collaged.\n" );
    printf( "Usage: $ %s <bmp_input1> <bmp_input2> <bmp_result> row_offset col_offset scale\n", argv[0] );
    exit(EXIT_FAILURE);
  }
  
  int row_offset, col_offset;
  row_offset = atoi(argv[4]);
  col_offset = atoi(argv[5]);
  float scale;
  scale = atof(argv[6]);  
  
  int ret_code = bmp_collage( argv[1],    argv[2],    argv[3],
                              row_offset, col_offset, scale    );
                                 
  if( ret_code != 0 ){
    printf( "Error: bmp_collage was not successful.\n" );
    return ret_code;
  }

  return 0;
}

