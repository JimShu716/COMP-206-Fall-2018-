/* FILE: A5_provided_functions.h
 * 
 * BRIEF: A few functions to help with reading and writing
 *        the bmp data that saves you the ugliest parts
 *        of working with the pixel data.
 *
 * DATE: Nov 27, 2018
 */

 /* FUNCTION: bmp_open
 * 
 * BRIEF:    Attempts to read the header information of bmp_filename to
 *           populate the file information variables. If successful,
 *           allocates sufficient heap memory to store the entire image 
 *           including both the header and the pixel data. Then reads
 *           the image from file into this memory.
 *
 * RETURN:   A pointer to the image data in memory. NULL if any failure.
 * 
 * PARAMS:   bmp_filename   [IN]  The path to the BMP file to be opened
 *           width          [OUT] The width of each image row
 *           height         [OUT] The height of each image column
 *           bits_per_pixel [OUT] The bits per pixel (BPP) of the image data
 *           row_padding    [OUT] Number of padding bytes that are placed at the end of 
 *                                each row to ensure the row size is a multiple of 4.
 *                                This is not stored in the bmp directly, but must be
 *                                computed by your code.
 *           data_size      [OUT] The number of bytes in img_data, includes header
 *           header_size    [OUT] The number of bytes just for the header 
 *                                (also known as "offset", in the image header)
 *           img_data       [OUT] Pointer to the image data on the heap.
 * 
 */ 
unsigned char* 
bmp_open(     char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset  );

/* FUNCTION: bmp_close
 *
 * BRIEF:    frees the memory that was allocated on the heap by any successful bmp_open
 * 
 * PARAMS:   img_data [IN/OUT] Pointer to the image data on heap. Must be free'd and 
 *                             set to NULL. It is up to the calling code to ensure the
 *                             address at *img_data is valid heap memory.
 */
void bmp_close( unsigned char **img_data );

/* FUNCTION: buffer_to_3D_array
 * 
 * BRIEF:    Accpepts the raw information from an image (the full file contents)
 *           as the img_data "buffer". Using the information extracted from the buffer, 
 *           transforms the raw data representation into a 3D array of shape 
 *           [height][width][num_colors].
 *
 * RETURN:   A pointer to the image data as a properly formatted 3D array.
 *           On any failure, the pointer will be NULL.
 * 
 * PARAMS:   img_data       [IN]  A pointer to all of the raw data of a BMP file
 *           header_data    [OUT] The buffer of header data
 *           width          [OUT] The width of each image row
 *           height         [OUT] The height of each image column
 *           num_colors     [OUT] num_colors = bits_per_pixel / 8
 * 
 */ 
unsigned char*** 
buffer_to_3D_array( char *img_data,             unsigned char** header_data, 
                    unsigned int* header_size,  unsigned int *width, 
                    unsigned int* height,       unsigned int* num_colors      );

/* FUNCTION: bmp_to_3D_array
 * 
 * BRIEF:    Calls bmp_open to retrieve the raw information from input_bmp_filename.
 *           Using the information extracted from the file, then transforms the raw
 *           data representation into a 3D array of shape [height][width][num_colors]
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

/* FUNCTION: buffer_from_3D_array
 * 
 * BRIEF:    Accepts the 3D array of pixels in shape [height][width][num_colors] 
 *           as input, converts this to the "raw" pixel data format
 *           that needs to be stored within a bmp file. Returns a pointer to the 
 *           buffer that holds this raw data, suitable for being written to file. 
 *           This function assumes the header 
 *           properly represents the image. That is, it specifies the correct height, 
 *           width, num_colors etc.
 *
 * RETURN:   A pointer to the raw data buffer.
 * 
 * PARAMS:   header_data       [IN]  The buffer of header data
 *           header_size       [IN]  The size of the header, in bytes
 *           pixel_data        [IN]  A pointer to the 3D array of pixel data
 *           width             [IN]  The width of each image row
 *           height            [IN]  The height of each image column
 *           num_colors        [IN]  num_colors = bits_per_pixel / 8
 *           buffer_size       [OUT] Set to the overall buffer size on success
 * 
 */    
unsigned char* 
buffer_from_3D_array( unsigned char* header_data,
                      int header_size,         unsigned char*** pixel_data,
                      int width, int height,   int num_colors,
                      int *buffer_size                  );

