/* FILE: q2_filter_with_history.c
*
* This file is a starter that we have begun for you to complete question 2.
* You are allowed to re-do it in any way that you choose, but in order for it
* to be completed in a sensible amount of time, I think you should heavily consider
* using both this code and the helper functions in A5_provided_functions.h
*
* Recommended to start: read through all this code carefully and all the provided
* functions. Write down a plan, then start coding and run make q2_test frequently.
*
* Date: Nov 27, 2018.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A5_provided_functions.h"



struct IMAGE_FILTER
{
	unsigned char*** old_image; 
	unsigned int width;         
	unsigned int height;        
	unsigned int num_colors;    

	unsigned int filter_size;   
	double * weights;          
	unsigned char*** new_image; 

}typedef filter_struct;

// filter the image using convolution
void filtering(filter_struct * p)
{
	int i, j, h, w, level, filter_radius, weight_index;
	double sum;

	// allocate new space
	p->new_image = (unsigned char***)malloc(sizeof(unsigned char **)*p->height);
	for (h = 0; h < p->height; h++)
	{
		p->new_image[h] = (unsigned char**)malloc(sizeof(unsigned char *) * p->width);
		for (w = 0; w < p->width; w++)
		{
			p->new_image[h][w] = (unsigned char*)malloc(sizeof(unsigned char) * p->num_colors);
			memset(p->new_image[h][w], 0, sizeof(unsigned char) * p->num_colors);
		}
	}

	// calculate the filter radius
	filter_radius = (p->filter_size - 1) / 2;

	for (level = 0; level < p->num_colors; level++)
	{
		for (h = 0; h < p->height; h++)
		{
			for (w = 0; w < p->width; w++)
			{
				// iterate each pixel
				if (0 <= h - filter_radius && h + filter_radius < p->height && 0 <= w - filter_radius && w + filter_radius < p->width)
				{
					// conv the point
					sum = 0;
					weight_index = 0;
					for (i = -filter_radius; i <= filter_radius; i++)
					{
						for (j = -filter_radius; j <= filter_radius; j++)
						{
							sum += (double)p->old_image[h + i][w + j][level] * p->weights[weight_index++];
						}
					}
					// set the sum into certain range 0-255
					sum = sum > 255 ? 255 : sum;
					sum = sum < 0 ? 0 : sum;
					p->new_image[h][w][level] = (unsigned char)sum;
				}
			}
		}
	}
}

// This is our suggested data structure that you might use to keep track of the history
struct IMAGE_HISTORY
{
	int history_length;   // This can hold how many operations have been done, including 1 load and N filters
	int active_image;     // This can represent where we are between 0 and length-1 based on undo/redo
	int image_height;
	int image_width;
	int image_colors;
	int head_size;
	unsigned char * head_data;
	unsigned char** history_array;  // This can be an array of raw image buffers. See the helper functions provided
									// to help you work with these.

} filter_history; // Remember writing this name here means we have one instance of the struct, global variable.

// read the archive file
int readHistory(char *hist_filename)
{
	FILE * fp;
	int n, i, j;
	fp = fopen(hist_filename, "r");
	if (fp == NULL)
	{
		return -1;
	}

	// read image size, header information
	fscanf(fp, "%d %d %d %d %d %d", &filter_history.history_length, &filter_history.active_image, &filter_history.image_height, &filter_history.image_width, &filter_history.image_colors, &filter_history.head_size);
	
	// read image header
	filter_history.head_data = (unsigned char *)malloc(sizeof(unsigned char)*filter_history.head_size);
	for (i = 0; i < filter_history.head_size; i++)
	{
		fscanf(fp, "%d", &n);
		filter_history.head_data[i] = (unsigned char)n;
	}

	// read image content
	filter_history.history_array = (unsigned char**)malloc(sizeof(unsigned char*)*(1 + filter_history.history_length));
	for (i = 0; i < 1 + filter_history.history_length; i++)
	{
		filter_history.history_array[i] = (unsigned char*)malloc(sizeof(unsigned char)*(filter_history.image_height* filter_history.image_width* filter_history.image_colors));
	}
	for (i = 0; i < filter_history.history_length; i++)
	{
		for (j = 0; j < (filter_history.image_height* filter_history.image_width* filter_history.image_colors); j++)
		{
			fscanf(fp, "%d", &n);
			filter_history.history_array[i][j] = (unsigned char)n;
		}
	}
	fclose(fp);
	return 0;
}

// save the archieve file
int writeHistory(char *hist_filename)
{
	FILE * fp;
	int i, j;
	fp = fopen(hist_filename, "w");

	// save the image size, header
	fprintf(fp, "%d %d %d %d %d %d\n", filter_history.history_length, filter_history.active_image, filter_history.image_height, filter_history.image_width, filter_history.image_colors, filter_history.head_size);

	// save the image header
	for (i = 0; i < filter_history.head_size; i++)
	{
		fprintf(fp, "%d ", (int)filter_history.head_data[i]);
	}
	fprintf(fp, "\n");

	// save the image cotent
	for (i = 0; i < filter_history.history_length; i++)
	{
		for (j = 0; j < (filter_history.image_height* filter_history.image_width* filter_history.image_colors); j++)
		{
			fprintf(fp, "%d ", (int)filter_history.history_array[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}

int main(int argc, char* argv[])
{

	if (argc < 2)
	{
		printf("img_filter must be called with at least one command. From the following list:\n");
		printf("$ %s load input_filename\n", argv[0]);
		printf("$ %s filter width [filter_weights](size widthxwidth)\n", argv[0]);
		printf("$ %s undo\n", argv[0]);
		printf("$ %s redo\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *command = argv[1];

	unsigned char*   header_data;
	unsigned int     header_size, width, height, num_colors;
	unsigned char*** pixel_array = NULL;
	int i, h, w;

	// TODO load hist_list

	if (strcmp(command, "load") == 0)
	{
		if (argc != 3)
		{
			printf("img_filter load command requires an image filename to be specified.\n");
			exit(EXIT_FAILURE);
		}
		char * file_name = argv[2];
		pixel_array = bmp_to_3D_array(file_name, &header_data, &header_size, &width, &height, &num_colors);
		if (pixel_array == NULL)
		{
			printf("Error: bmp_to_3D_array failed for file %s.\n", file_name);
			return 0;
		}

		filter_history.active_image = 0;
		filter_history.history_length = 1;
		filter_history.image_height = height;
		filter_history.image_width = width;
		filter_history.image_colors = num_colors;
		filter_history.head_size = header_size;
		filter_history.head_data = header_data;
		filter_history.history_array = (unsigned char**)malloc(sizeof(unsigned char*));
		filter_history.history_array[0] = (unsigned char*)malloc(sizeof(unsigned char)*(height*width*num_colors));

		for (h = 0; h < height; h++)
		{
			for (w = 0; w < width; w++)
			{
				filter_history.history_array[0][(h*width + w) * 3 + 0] = pixel_array[h][w][0];
				filter_history.history_array[0][(h*width + w) * 3 + 1] = pixel_array[h][w][1];
				filter_history.history_array[0][(h*width + w) * 3 + 2] = pixel_array[h][w][2];
			}
		}
		writeHistory("history.dat");
	}
	else if (strcmp(command, "filter") == 0)
	{
		if (argc < 4)
		{
			printf("img_filter filter command requires a filter width and a square filter of size (widthxwidth).\n");
			exit(EXIT_FAILURE);
		}
		int filter_width = atoi(argv[2]);
		double * pfilter_data = (double *)malloc(sizeof(double)*filter_width*filter_width);
		for (i = 0; i < filter_width*filter_width; i++)
		{
			pfilter_data[i] = atof(argv[i + 3]);
		}
		readHistory("history.dat");
		width = filter_history.image_width;
		height = filter_history.image_height;
		num_colors = filter_history.image_colors;
		// allocate new space
		pixel_array = (unsigned char***)malloc(sizeof(unsigned char **)*height);
		for (h = 0; h < height; h++)
		{
			pixel_array[h] = (unsigned char**)malloc(sizeof(unsigned char *) *width);
			for (w = 0; w < width; w++)
			{
				pixel_array[h][w] = (unsigned char*)malloc(sizeof(unsigned char) * num_colors);
				memset(pixel_array[h][w], 0, sizeof(unsigned char) * num_colors);
			}
		}
		for (h = 0; h < height; h++)
		{
			for (w = 0; w < width; w++)
			{
				pixel_array[h][w][0] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 0];
				pixel_array[h][w][1] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 1];
				pixel_array[h][w][2] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 2];
			}
		}

		filter_struct fs;
		fs.old_image = pixel_array;
		fs.width = width;
		fs.height = height;
		fs.num_colors = num_colors;
		fs.filter_size = filter_width;
		fs.weights = pfilter_data;
		fs.new_image = NULL;
		filtering(&fs);

		bmp_from_3D_array("output.bmp", filter_history.head_data, filter_history.head_size, fs.new_image,
			width, height, num_colors);

		// Complete this, read the rest of the filter into some variable, use it to filter the 
		// active image, and update history as required.
		filter_history.active_image++;
		filter_history.history_length = filter_history.active_image + 1;


		for (h = 0; h < height; h++)
		{
			for (w = 0; w < width; w++)
			{
				filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 0] = fs.new_image[h][w][0];
				filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 1] = fs.new_image[h][w][1];
				filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 2] = fs.new_image[h][w][2];
			}
		}
		writeHistory("history.dat");
	}
	else if (strcmp(command, "undo") == 0)
	{
		readHistory("history.dat");		
		if (filter_history.active_image <= 0)
		{
			printf("Cannot undo!\n");
			return 0;
		}

		// undo the step
		filter_history.active_image--;

		// allocate new space
		width = filter_history.image_width;
		height = filter_history.image_height;
		num_colors = filter_history.image_colors;

		pixel_array = (unsigned char***)malloc(sizeof(unsigned char **)*height);
		for (h = 0; h < height; h++)
		{
			pixel_array[h] = (unsigned char**)malloc(sizeof(unsigned char *) *width);
			for (w = 0; w < width; w++)
			{
				pixel_array[h][w] = (unsigned char*)malloc(sizeof(unsigned char) * num_colors);
				memset(pixel_array[h][w], 0, sizeof(unsigned char) * num_colors);
			}
		}

		for (h = 0; h < height; h++)
		{
			for (w = 0; w < width; w++)
			{
				pixel_array[h][w][0] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 0];
				pixel_array[h][w][1] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 1];
				pixel_array[h][w][2] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 2];
			}
		}

		bmp_from_3D_array("output.bmp", filter_history.head_data, filter_history.head_size, pixel_array,
			width, height, num_colors);

		writeHistory("history.dat");
	}
	else if (strcmp(command, "redo") == 0)
	{
		readHistory("history.dat");
		if (filter_history.active_image >= filter_history.history_length - 1)
		{
			printf("Cannot redo!\n");
			return 0;
		}

		// redo the step
		filter_history.active_image++;

		// allocate new space
		width = filter_history.image_width;
		height = filter_history.image_height;
		num_colors = filter_history.image_colors;

		pixel_array = (unsigned char***)malloc(sizeof(unsigned char **)*height);
		for (h = 0; h < height; h++)
		{
			pixel_array[h] = (unsigned char**)malloc(sizeof(unsigned char *) *width);
			for (w = 0; w < width; w++)
			{
				pixel_array[h][w] = (unsigned char*)malloc(sizeof(unsigned char) * num_colors);
				memset(pixel_array[h][w], 0, sizeof(unsigned char) * num_colors);
			}
		}

		for (h = 0; h < height; h++)
		{
			for (w = 0; w < width; w++)
			{
				pixel_array[h][w][0] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 0];
				pixel_array[h][w][1] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 1];
				pixel_array[h][w][2] = filter_history.history_array[filter_history.active_image][(h*width + w) * 3 + 2];
			}
		}
		bmp_from_3D_array("output.bmp", filter_history.head_data, filter_history.head_size, pixel_array,
			width, height, num_colors);
		writeHistory("history.dat");
	}
	else
	{
		printf("Command %s is not valid. Please type one of: load, filer, undo, redo.\n", command);
		return -1;
	}
	return 0;
}
