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
  

  //NOTE: THESE ARE NOT COMPLETE AND WE PROBABLY NEED TO ADD MORE RIGOROUS CHECKS BUT I JUST DID THIS FOR NOW
  //NOTE: THESE ARE REALLY NOT COMPLETE
  //open the input image
  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");
  
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
    printf("penis\n");
    printf("Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    return 3;
  }
  
  //ERROR 4 CHECKER
  if ( (strcmp(argv[3], "exposure") != 0) && (strcmp(argv[3], "blend") != 0) && (strcmp(argv[3], "zoom_in") != 0) && (strcmp(argv[3], "zoom_out") != 0) && (strcmp(argv[3], "pointilism") != 0) && (strcmp(argv[3], "swirl") != 0) && (strcmp(argv[3], "blur") != 0) ) {
    printf("No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }

  //ERROR 5 CHECKER
  //need to add code to check for kinds of arguments (for literally all of these)
  if ( (strcmp(argv[3], "exposure") == 0 && argc != 5) && (strcmp(argv[3], "blend") == 0 && argc != 6) && (strcmp(argv[3], "zoom_in") == 0 && argc != 4) && (strcmp(argv[3], "zoom_out") == 0 && argc != 4) && (strcmp(argv[3], "pointilism") == 0 && argc != 4) && (strcmp(argv[3], "swirl") == 0 && argc != 7) && (strcmp(argv[3], "blur") == 0 && argc != 5) ) {
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

  /* OK SO HERE'S WHERE THE ERRORS SIGNIFICANTLY BEGIN
   * basically nothing here is working properly i don't know even if the
   * inputImg = read_ppm(input) line you see right above is working or not but when
   * I do that and then I try to print img->rows & img->cols in ppm_io.c, it keeps printing
   * wrong values. Don't know if this is an error on your end coding the ppm_io.c or an error
   * from me not passing the file correctly. If you can please look over this because I'm really
   * iffy on passing the file over correctly and I'm pretty sure it's my fault lmao. If we can nail
   * this we shouldn't have too too too many problems left but we also gotta make sure that we pass
   * valgrind and no memory leaks. I kind of don't know what to do about memory leaks because when I
   * coded the imageManip.c file, I was gonna pass a dynamically allocated array back to project.c. Don't
   * know if this is a good idea anymore because not sure when we can free arrays then (or maybe it's easy
   * and I"m just being retarded, as in like we can just go like a* = exposure(blah, blah) and then after doing
   * write_ppm(a, blah) we can just free(a) okay yea that might be better. Anyways I have 0 clue how to fix this
   * so if you can please fix the stuff happening here (stuff I was talking about in like the first half). 
   * I have no clue how we're gonna finish by thursday night anymore honestly cause we still haven't finishehd coding
   * the thing and we have to debug.
   * yeeeaaa I was hoping I could see if my functions that I coded as of now are even working but I can't so
   * I still have 0 clue if I'm doing stuff right or not. Tomorrow will either be shit or hella shit
   */

  //exposure
  if (strcmp(argv[3], "exposure") == 0) {

    //ok this line doesnt work --> leads to core dump
    outputImg = exposure(inputImg, atoi(argv[4]));

    //num_pixels_written = write_ppm(output, outputImg);
    //printf("%d", num_pixels_written);
  }

}
