/* FILE: A3_provided_functions.h
 * 
 * BRIEF: A few functions to help with reading and writing
 *        the bmp data that saves you the ugliest parts
 *        of working with the pixel data.
 *
 * DATE: Oct 22, 2018
 */


/* FUNCTION: bmp_to_3D_array
 * 
 * BRIEF:    Calls bmp_open to retrieve the raw information from input_bmp_filename.
 *           Using the information extracted from the file, then transforms the raw
 *           data representation into a 3D array of shape [height][width][num_colors]
 *           Returns a pointer to the 3D array as the return value
 *
 * RETURN:   A pointer to the image data as a properly formatted 3D array.
 *           On any failure, the pointer will be NULL.
 * 
 * PARAMS:   bmp_filename   [IN]  The path to the BMP file to be opened
 *           header_data    [OUT] The buffer of header data
 *           width          [OUT] The width of each image row
 *           height         [OUT] The height of each image column
 *           num_colors     [OUT] num_colors = bits_per_pixel / 8
 * 
 */ 
unsigned char*** 
bmp_to_3D_array(  char *bmp_filename,         unsigned char** header_data, 
                  unsigned int* header_size,  unsigned int *width, 
                  unsigned int* height,       unsigned int* num_colors          
);
                           
/* FUNCTION: 3D_array_to_bmp
 * 
 * BRIEF:    Accepts the 3D array of pixels in shape [height][width][num_colors] 
 *           as input, converts this to the "raw" pixel data format
 *           that needs to be stored within a bmp file. Writes the header
 *           and all pixel data, to output_filename. This function assumes the header 
 *           properly represents the image. That is, it specifies the correct height, 
 *           width, num_colors etc.
 *
 * RETURN:   An integer indicating success (0) or failure (-1) of writing the image.
 * 
 * PARAMS:   output_filename   [IN] The path for the new BMP file to be created
 *           header_data       [IN] The buffer of header data
 *           header_size       [IN] The size of the header, in bytes
 *           pixel_data        [IN] A pointer to the 3D array of pixel data
 *           width             [IN] The width of each image row
 *           height            [IN] The height of each image column
 *           num_colors        [IN] num_colors = bits_per_pixel / 8
 * 
 */                            
int 
bmp_from_3D_array( char* output_filename,   unsigned char* header_data,
                   int header_size,         unsigned char*** pixel_data,
                   int width, int height, int num_colors              
);

