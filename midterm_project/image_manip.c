#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"
#include <ctype.h>
#include <string.h>


////////////////////////////////////////
// Definitions of the functions       //
// declared in image_manip.h go here! //
////////////////////////////////////////

/* Convert a RGB pixel to a single grayscale intensity;
* uses NTSC standard conversion
*/
unsigned char pixel_to_gray( Pixel p ) {
	return (unsigned char)( 0.3 * p.r + 0.59 * p.g + 0.11 * p.b );
}

///______invert______
/* invert the intensity of each channel 
*/
Image invert( const Image in ) {

	// create copy of image
	Image inverted = make_image(in.rows, in.cols);

	// check if memory allocation succeeded
  	if (inverted.data == NULL) {
    	fprintf(stderr, "[ERROR] Invert: Memory allocation failed\n");
    	inverted.rows = 0;
		inverted.cols = 0;
		return inverted; 
    }

	// assign rgb color values by looping through image matrix
	for (int i = 0; i < in.rows * in.cols; i++) {
		//set values for inverted pixels
		inverted.data[i].r = 255 - in.data[i].r;
        inverted.data[i].g = 255 - in.data[i].g;
        inverted.data[i].b = 255 - in.data[i].b;
	}
	return inverted;
}

/*
Function returns a cropped section of the image inputed.
Takes in the input image, and the coordinates for the top left point and the bottom
left point for ht ecrop to be between them.
Returns the cropped image if successful and an empty image if not.
*/
Image crop( const Image in , Point top_left , Point bot_right ){
	
	//find new dimensions of image and then make one that size
	int new_cols = bot_right.x - top_left.x;
	int new_rows = bot_right.y - top_left.y;
	Image cropped = make_image(new_rows, new_cols);

	// check if memory allocation succeeded and return NULL image if failed
  	if (cropped.data == NULL) {
    	fprintf(stderr, "[ERROR] Crop: Memory allocation failed\n");
		cropped.rows = 0;
		cropped.cols = 0;
    	return cropped; 
    }

	//loop through rows and set new image to only be part of the origional image
	for(int row = 0; row < new_rows; row++){
		for(int col = 0; col < new_cols; col++){
			cropped.data[row * new_cols + col] = in.data[ (top_left.y + row) *
			in.cols + (top_left.x + col)];
		}
	}

	return cropped;
}

/* Function to return gaussian matrix,
		used in blur function
*/
double ** gaussian_matrix(double sigma) {

	// find size of array using ceiling
	int N = ceil(10 * sigma);

	// ensure that N is even
	if (N % 2 == 0) {
		N += 1;
	}

	// allocate memory for rows of matrix
	double **gaussian = (double **)calloc(N, sizeof(double *));
	
	// check for memory error
	if (gaussian == NULL) {
		fprintf(stderr, "[ERROR] Memory allocation failed!\n");
		return gaussian;
	}

	// allocate memory for columns of matrix
	for (int i = 0; i < N; i++) {
		gaussian[i] = (double *)calloc(N, sizeof(double));
		if (gaussian[i] == NULL) {
			fprintf(stderr, "[ERROR] Memory allocation failed for row %d\n", i);
			for (int j = 0; j < i; j++) { // Free already allocated rows
        		free(gaussian[j]);
   			 }
    		free(gaussian);
  		  	return NULL;
		}
	}

	// calculate center of matrix
	int center = N / 2;

	// iterate through matrix
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++){
			// calculate dx and dy as offsets of center
			int dx = i - center;
			int dy = j - center;

			double g = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));

			gaussian[i][j] = g;
			
		}
	}

	return gaussian;
	
}
/* Helper function for blur, 
normalizes pixel values
*/
void normalize(double** arr, int N) {
	double sum = 0.0;

	// calculate sum of all elements in arr
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			sum += arr[i][j];
		}
	}

	// divide by sum to normalize
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			arr[i][j] /= sum;
		}
	}
}

/* Blur function, takes in an image and a sigma
value that determines how blurry the outputted
image will be
*/
Image blur( const Image in , double sigma ) {

	int N = ceil(10 * sigma);

	if (N % 2 == 0) {
        N++;
    }

	int half_N = N / 2;

	double **arr = gaussian_matrix(sigma);

	normalize(arr, N);

	Image blurred = make_image(in.rows, in.cols);

	// make sure memory was allocated properly
	if (blurred.data == NULL) {
    	fprintf(stderr, "[ERROR] Invert: Memory allocation failed\n");
    	blurred.rows = 0;
		blurred.cols = 0;
		return blurred; 
    }

	// 
	double red_sum, green_sum, blue_sum, weight_sum;
	// loop through pixels in input image
	for (int x = 0; x < in.cols; x++) {
		for (int y = 0; y < in.rows; y++) {
			// initialize sums for each pixel to 0
			red_sum = 0.0;
			green_sum = 0.0;
			blue_sum = 0.0;
			weight_sum = 0.0;

			// loop through window centered around current pixel (x, y)
			for (int i = -half_N; i <= half_N; i++) {
				for (int j = -half_N; j <= half_N; j++) {
					// calculate coordinates of neighboring pixel
					int xi = x + i;
					int yj = y + j;

					// check if neighboring pixel is in image boundaries
					if (xi >= 0 && xi < in.cols && yj >= 0 && yj < in.rows) {
						double kernel_value = arr[i + half_N][j + half_N];

						// access pixel
						Pixel p = in.data[yj * in.cols + xi];

						// get weighted sums
						red_sum += p.r * kernel_value;
       		 			green_sum += p.g * kernel_value;
        				blue_sum += p.b * kernel_value;

						// get total weight
        				weight_sum += kernel_value;
					}
				}
			}
			// calculate average color values
			Pixel *out_pixel = &blurred.data[y * in.cols + x];
            out_pixel->r = (unsigned char)(red_sum / weight_sum);
            out_pixel->g = (unsigned char)(green_sum / weight_sum);
            out_pixel->b = (unsigned char)(blue_sum / weight_sum);
		}
	}
	// free memory
	for (int i = 0; i < N; i++) {
        free(arr[i]);
    }
    free(arr);

    return blurred;

}


/*
Function takes an image and then applys pointilism to it.
Takes in an input image and a seed for the random number generator.
Returns the image after the transformation.
*/
Image pointilism( const Image in , unsigned int seed ){

	Image pointed = make_image(in.rows, in.cols);

	// check if memory allocation succeeded and return NULL image if failed
  	if (pointed.data == NULL) {
    	fprintf(stderr, "[ERROR] Pointilism: Memory allocation failed\n");
		pointed.rows = 0;
		pointed.cols = 0;
    	return pointed; 
    }

	//initialize whole image to black 
	for (int i = 0; i < pointed.rows * pointed.cols; i++ ){
		pointed.data[i].r = 0;
		pointed.data[i].g = 0;
		pointed.data[i].b = 0;
	}

	//find how many pixels and points there should be and set seed
	int num_pixels = in.rows * in.cols;
	int num_points = (int) (num_pixels * 0.03);
	srand(seed);

	//iterate through the random number of points
	for ( int i = 0; i < num_points; i ++){
		
		//find random radius, x and y position, and color
		int radius = (rand() % 5) + 1;
		int column_center = rand() % pointed.cols;
		int row_center = rand() % pointed.rows;
		Pixel color = in.data[(row_center * in.cols) + column_center];

		//draw the circle
		draw_circle(&pointed, column_center, row_center, radius, color);

	}

	return pointed;
}

/*
Function takes in an image and draws a circle around a given point
Takes in the image to be manipulated, the point at the center and radius as well 
as the color to paint the circle. Returns void as it directly changes the input image
*/
void draw_circle(Image * pointed, int column_center, int row_center, int radius, Pixel color){

	//find the min and max for x and y and make sure that it is within bounds of image
	int min_col = fmax(column_center - radius, 0);
	int max_col = fmin(column_center + radius, pointed->cols - 1);
	int min_row = fmax(row_center - radius, 0);
	int max_row = fmin(row_center + radius, pointed->rows - 1);

	//iterate through columns and rows
	for(int col = min_col; col <= max_col; col++){
		for(int row = min_row; row <= max_row; row++){
			
			//finds the radius of current point
			int dx = column_center - col;
			int dy = row_center - row;
			int current_radius = dx * dx + dy *dy;
			
			//check if it is within range of radius and color it in
			if(current_radius <= (radius * radius) ){
				pointed->data[row * pointed->cols + col] = color;
			}
		}
	}
}

/* Binarize helper function to determine
if the given parameted is valid 
*/
int is_valid_threshold(const char *str){
	// check to make sure string isnt empty
	if (str == NULL || strlen(str) == 0) {
		return 0;
	}
	// iterate through the string, check if every value is a digit
	for (int i = 0; str[i] != '\0'; i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}
	// convert string to integer
	int value = atoi(str);
	// make sure value is within range
	if (value < 0 || value > 255) {
		return 0;
	}
	return 1;
}

/* Function to binarize image
according to given threshold
*/
Image binarize( const Image in , const int threshold ) {
	// create image
	Image binarized = make_image(in.rows, in.cols);

	// check if make_image succeeded
	if (binarized.data == NULL) {
    	fprintf(stderr, "[ERROR] Binarize: Memory allocation failed\n");
		binarized.rows = 0;
		binarized.cols = 0;
    	return binarized; 
    }
	// iterate through pixels in input image 
	for (int i = 0; i < in.rows * in.cols; i++) {
		// convert pixel to gray scale
		int value = pixel_to_gray(in.data[i]);
		// compare value to threshold and set pixel values
		if (value < threshold) {
			binarized.data[i].r = 0;
			binarized.data[i].g = 0;
			binarized.data[i].b = 0;
		} else {
			binarized.data[i].r = 255;
			binarized.data[i].g = 255;
			binarized.data[i].b = 255;
		}
	}
	return binarized;
}

/*
This funciton takes in an image and outputs an image that is hald the dimensions.
It takes in an image and returns a new image.
Averages the surrounding pixels
*/
Image zoom_out( const Image in ){

	//create new image with correct dimensions and error check
	int new_rows = in.rows / 2;
	int new_cols = in.cols / 2;
	
	Image zoomed = make_image(new_rows, new_cols);
	if( zoomed.data == NULL ){
		fprintf(stderr, "Error: zoom_out memory allocation failed\n");
		zoomed.rows = 0;
		zoomed.cols = 0;
		return zoomed;
	}

	//iterate through rows and columns of new image
	for(int row = 0; row < new_rows; row++){
		for(int col = 0; col < new_cols; col++){
			//find original rows and columns to average
			int orig_row = row * 2;
			int orig_col = col * 2;

			//get four pixels that will be averaged
			Pixel top_left = in.data[orig_row * in.cols + orig_col];
			Pixel top_right = in.data[orig_row * in.cols + (orig_col + 1)];
			Pixel bot_left = in.data[(orig_row +1) * in.cols + orig_col];
			Pixel bot_right = in.data[(orig_row +1) * in.cols + (orig_col+1)];

			//average the four pixels and set new pixel to that color
			zoomed.data[row * new_cols + col].r = (top_left.r + top_right.r +
				bot_left.r + bot_right.r) / 4;
			zoomed.data[row * new_cols + col].g = (top_left.g + top_right.g +
				bot_left.g + bot_right.g) / 4;
			zoomed.data[row * new_cols + col].b = (top_left.b + top_right.b +
				bot_left.b + bot_right.b) / 4;
		}
	}

	return zoomed;
}
