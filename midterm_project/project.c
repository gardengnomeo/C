#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "image_manip.h"

// Return (exit) codes
#define RC_SUCCESS            0
#define RC_MISSING_FILENAME   1
#define RC_OPEN_FAILED        2
#define RC_INVALID_PPM        3
#define RC_INVALID_OPERATION  4
#define RC_INVALID_OP_ARGS    5
#define RC_OP_ARGS_RANGE_ERR  6
#define RC_WRITE_FAILED       7
#define RC_UNSPECIFIED_ERR    8


void print_usage(void);

int main (int argc, char* argv[]) {

  //checks to make sure enough arguments inputed
  if (argc < 4) {
    fprintf(stderr, "Error: Not enough arguments provided\n");
    print_usage();
    return RC_MISSING_FILENAME;
  }
  
  //splits argumetns up for easier use and intitalizes variables
  const char *input_filename = argv[1];
  const char *output_filename = argv[2];
  const char *operation = argv[3];
  Image output_image = {NULL, 0, 0};
  Image im = {NULL, 0, 0};

  //reads input file and returns error if can't
  FILE *input_file = fopen(input_filename, "rb");
  if (!input_file) {
    fprintf(stderr, "Error: could not open file %s for reading.\n", input_filename);
    print_usage();
    return RC_OPEN_FAILED;
  }

  //checks if input file can be read as ppm and returns error if not
  im = read_ppm(input_file);
  fclose(input_file);
  if(im.data == NULL || im.cols == 0 || im.rows == 0){
    fprintf(stderr, "Error: %s can not be read as PPM file\n", input_filename);
    free_image(&im);
    print_usage();
    return RC_INVALID_PPM;
  }

  //checks which operation was inputed
  if (strcmp(operation, "invert") == 0){
    //checks num arguments correct
    if (argc != 4){
      fprintf(stderr, "Error: %d is not the correct number of arguments for %s\n",
      argc, operation);
      free_image(&im);
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    //applies invert operation
    output_image = invert(im);


  } else if (strcmp(operation, "crop") == 0){
    //checks num arguments correct
    if (argc != 8){
      fprintf(stderr, "Error: %d is not the correct number of arguments for %s\n",
      argc, operation);
      free_image(&im);
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    //create points for crop
    Point top_left = {atoi(argv[4]), atoi(argv[5])};
    Point bot_right = {atoi(argv[6]), atoi(argv[7])};

	  //check to see if points are valid and return invalid image if not
    if (top_left.x < 0 || top_left.y < 0 || bot_right.x > im.cols || 
      bot_right.y > im.rows || top_left.x >= bot_right.x || 
      top_left.y >= bot_right.y){
      fprintf(stderr, "Error: Invalid arguments for %s\n", operation);
      free_image(&im);
      print_usage();
      return RC_OP_ARGS_RANGE_ERR;
    }

    //call crop 
    output_image = crop(im, top_left, bot_right);


  } else if (strcmp(operation, "zoom_out") == 0){
    //checks num arguments correct
    if (argc != 4){
      fprintf(stderr, "Error: %d is not the correct number of arguments for %s\n",
      argc, operation);
      free_image(&im);
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    //call zoom_out
    output_image = zoom_out(im);


  } else if (strcmp(operation, "binarize") == 0){
    //checks num arguments correct
    if (argc != 5){
      fprintf(stderr, "Error: %d is not the correct number of arguments for %s\n",
      argc, operation);
      free_image(&im);
      print_usage();
      return RC_INVALID_OP_ARGS;
    }
   // store parameter
   char* arg = argv[4];
   //checks if threshold is in range
   if (!is_valid_threshold(argv[4])) {
    fprintf(stderr, "[ERROR] Invalid threshold\n");
    free_image(&im);
    print_usage();
    return RC_OP_ARGS_RANGE_ERR;
   }
   // convert string to int
   int threshold = atoi(arg);

   // call binarize
   output_image = binarize(im, threshold);


  } else if (strcmp(operation, "pointilism") == 0){
    //checks num arguments correct
    if (argc != 4){
      fprintf(stderr, "Error: %d is not the correct number of arguments for %s\n",
      argc, operation);
      free_image(&im);
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    //call pointilism
    output_image = pointilism(im, 1);
    

  } else if (strcmp(operation, "blur") == 0){
    //checks num arguments correct
    if (argc != 5){
      fprintf(stderr, "Error: %d is not the correct number of arguments for %s\n",
      argc, operation);
      free_image(&im);
      print_usage();
      return RC_INVALID_OP_ARGS;
    }

    // convert 5th argument to double
    double sigma = strtod(argv[4], NULL);

    // make sure sigma is a positive number
    if (sigma <= 0) {
      fprintf(stderr, "Error: Invalid sigma value. Sigma must be a positive number.\n");
      free_image(&im);
      print_usage();
      return RC_OP_ARGS_RANGE_ERR;
    }
    // call blur
    output_image = blur(im, sigma);

  
  //invalid operation inputed
  } else {
    fprintf(stderr, "Error: %s is not a valid operation\n", operation);
    free_image(&im);
    print_usage();
    return RC_INVALID_OPERATION;
  }

  //frees im as it is no longer needed
  free_image(&im);

  //checks to see if the operation produced an empty image
  if (output_image.data == NULL || output_image.cols == 0 || output_image.rows == 0) {
    fprintf(stderr, "Error: Output image is empty. Cannot write to file.\n");
    free_image(&output_image);
    print_usage();
    return RC_WRITE_FAILED;
  }

  //creates output file and returns error and closes input if can't
  FILE *output_file = fopen(output_filename,"wb");
  if(!output_file){
    fprintf(stderr, "Error: could not open file %s for writing.\n", output_filename);
    free_image(&output_image);
    print_usage();
    return RC_WRITE_FAILED;
  }
  
  //check to see if the writing to the file was successful
  if( write_ppm( output_file, output_image) != 0 ){
    fprintf(stderr, "Error: Failed to write output image %s\n", output_filename);
    fclose(output_file);
    free_image(&output_image);
    print_usage();
    return RC_WRITE_FAILED;
  }

  //closes files and images to ensure no memory lost and returns code that there was no error
  fclose(output_file);
  free_image(&output_image);
  return RC_SUCCESS;
}

void print_usage(void) {
  printf("USAGE: ./project <input-image> <output-image> <command-name> <command-args>\n");
  printf("SUPPORTED COMMANDS:\n");
  printf("   invert\n" );
  printf("   zoom-out\n" );
  printf("   binarize <threshold>\n" );
  printf("   pointilism\n" );
  printf("   blur <sigma>\n" );
  printf("   crop <top left (x,y)> <bottom right (x,y)>\n" );
}
