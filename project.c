//project.c

#include <stdio.h>
#include "imageManip.h"
#include "ppm_io.h"
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

  // argc is the number of command-line arguments
  // argv contains the arguments
  // for example:
  //  ./project trees.ppm trees-exp-half.ppm exposure 0.5
  //  argc is 4
  //  argv[0] is "trees.ppm"
  //  argv[1] is "trees-exp..."
  //  argv[2] is "exposure"
  //  argv[3] is "0.5"
  //THIS IS WRONG ARGV[0] IS ./PROJECT 

  printf("%s %s %s %s\n", argv[1], argv[2], argv[3], argv[4]);
  
  //open the input image
  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");
  
  //ERROR 1 CHECKER
  //if there are less than 2 args (so either no input or no output)
  if (argc <= 2) {
    printf("Failed to supply input filename or output filename, or both\n");
    return 1;
  }

  //ERROR 2 CHECKER
  //if can't open input image
  if (input == NULL) {
    printf("Specified input file could not be opened\n");
    return 2;
  }

  //ERROR 3 CHECKER
  if (read_ppm(input) == NULL) {
    printf("Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    return 3;
  }

  //ERROR 4 CHECKER
  if ( (strcmp(argv[3], "exposure") != 0) || (strcmp(argv[3], "blend") != 0) || (strcmp(argv[3], "zoom_in") != 0) || (strcmp(argv[3], "zoom_out") != 0) || (strcmp(argv[3], "pointilism") != 0) || (strcmp(argv[3], "swirl") != 0) || (strcmp(argv[3], "blur") != 0) ) {
    printf("No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }

  //ERROR 5 CHECKER
  //need to add code to check for kinds of arguments (for literally all of these)
  if ( (strcmp(argv[3], "exposure") == 0 && argc != 4) || (strcmp(argv[3], "blend") == 0 && argc != 5) || (strcmp(argv[3], "zoom_in") == 0 && argc != 3) || (strcmp(argv[3], "zoom_out") == 0 && argc != 3) || (strcmp(argv[3], "pointilism") == 0 && argc != 3) || (strcmp(argv[3], "swirl") == 0 && argc != 6) || (strcmp(argv[3], "blur") == 0 && 4) ) {
      printf("Incorrect number of arguments or kind of arguments specified operation\n");
      return 5;
  }

  //need to start running tests here
  //debating whether imageManip.c should do the printing or whether it should return the modified image
  //think it should return modified image, that way we can take care of the printing here

  
  //getting the input image

  Image * inputImg = read_ppm(input);
  Image * outputImg;
  
  int num_pixels_written;

  //exposure
  if (strcmp(argv[2], "exposure") == 0) {
    outputImg = exposure(inputImg, atoi(argv[3]));
    num_pixels_written = write_ppm(output, outputImg);
    printf("%d", num_pixels_written);
  }

  

  
  //if (write_ppm(argv[1], 
 
}
