//project.c

#include <stdio.h>
#include "imageManip.h"
#include "ppm_io.h"
#include <string.h>
#include <stdlib.h>

int test (int argc, char *argv[]) {

  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");
  
  //ERROR 1 CHECKER
  if (argc <= 3) {
    printf("Failed to supply input filename or output filename, or both\n");
    return 1;
  }

  //ERROR 2 CHECKER
  if (input == NULL) {
    printf("Specified input file could not be opened\n");
    return 2;
  }

  Image * inputImg = read_ppm(input);

  //ERROR 3 CHECKER
  if (inputImg == NULL) {
    printf("Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    return 3;
  }
  
  //ERROR 4 CHECKER
  if ( (strcmp(argv[3], "exposure") != 0) && (strcmp(argv[3], "blend") != 0) && (strcmp(argv[3], "zoom_in") != 0) && (strcmp(argv[3], "zoom_out") != 0) && (strcmp(argv[3], "pointilism") != 0) && (strcmp(argv[3], "swirl") != 0) && (strcmp(argv[3], "blur") != 0) ) {
    printf("No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }

  //ERROR 5 CHECKER
  if ( (strcmp(argv[3], "exposure") == 0 && argc != 5) && (strcmp(argv[3], "blend") == 0 && argc != 6) && (strcmp(argv[3], "zoom_in") == 0 && argc != 4) && (strcmp(argv[3], "zoom_out") == 0 && argc != 4) && (strcmp(argv[3], "pointilism") == 0 && argc != 4) && (strcmp(argv[3], "swirl") == 0 && argc != 7) && (strcmp(argv[3], "blur") == 0 && argc != 5) ) {
      printf("Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
  }
  
  //exposure (WORKING)
  if (strcmp(argv[3], "exposure") == 0) {
    return (exposure(inputImg, atoi(argv[4]), output));
  }
  
  //blend (WORKING)
  Image *inputImg2;
  if (strcmp(argv[3], "blend") == 0) {
    FILE *input2 = fopen(argv[4], "r");
    inputImg2 = read_ppm(input2);
    return (blend(inputImg, inputImg2, atof(argv[5]), output));
  }

  //zoom_inc (WORKING)
  if (strcmp(argv[3], "zoom_in") == 0) {
    return (zoom_in(inputImg, output));
  }

  //zoom_out (WORKING)
  if (strcmp(argv[3], "zoom_out") == 0) {
    return (zoom_out(inputImg, output));
  }
  
  //swirl (NEED TO FIX)
  if (strcmp(argv[3], "swirl") == 0) {
    return (swirl(inputImg, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), output));
  }

  /*
  //pointilism
  if (strcmp(argv[3], "pointilism") == 0) {
    outputImg = pointilism(inputImg);
    num_pixels_written = write_ppm(output, outputImg);
    printf("%d pixels were written.\n", num_pixels_written);
  }
  */

  return 0;
}

int main (int argc, char *argv[]) {

  return (test(argc, argv));
  
}
