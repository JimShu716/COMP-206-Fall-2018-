

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

int main(int argc, char* argv[])
{

	if (argc <= 4)
	{
		printf("Fromat: <infile> <outfile> <filterSize> <para...>\n");
		return 0;
	}

	char * infile_path = argv[1];
	char * outfile_path = argv[2];

	unsigned char*   header_data;
	unsigned int     header_size, width, height, num_colors;
	unsigned char*** pixel_array = NULL;
	int filter_width;
	double * pfilter_data;
	int i;
	filter_struct fs;

	pixel_array = bmp_to_3D_array(infile_path, &header_data,
		&header_size, &width, &height, &num_colors);

	if (pixel_array == NULL)
	{
		printf("Error: bmp_to_3D_array failed for file %s.\n", infile_path);
		return 0;
	}

	filter_width = atoi(argv[3]);
	pfilter_data = (double *)malloc(sizeof(double)*filter_width*filter_width);
	for (i = 0; i < filter_width*filter_width; i++)
	{
		pfilter_data[i] = atof(argv[i + 4]);
	}


	fs.old_image = pixel_array;
	fs.width = width;
	fs.height = height;
	fs.num_colors = num_colors;
	fs.filter_size = filter_width;
	fs.weights = pfilter_data;
	fs.new_image = NULL;
	filtering(&fs);

	
	bmp_from_3D_array(outfile_path, header_data, header_size, fs.new_image,
		width, height, num_colors);

	free(fs.old_image);
	free(fs.weights);
	free(fs.new_image);

	return 0;

}

