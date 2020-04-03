//project.c

#include <stdio.h>
#include "imageManip.h"
#include "ppm_io.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int test (int argc, char *argv[]) {
  
  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");
  
  //ERROR 1 CHECKER
  if (argc < 3) {
    fprintf(stderr, "Failed to supply input filename or output filename, or both\n");
    return 1;
  }

  //ERROR 2 CHECKER
  if (input == NULL) {
    fprintf(stderr, "Specified input file could not be opened\n");
    return 2;
  }

  Image * inputImg = read_ppm(input);

  //ERROR 3 CHECKER
  if (inputImg == NULL) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    printf("PENIS");
    printf("WHAT THE FUCK\n");
    return 3;
  }

  
  //ERROR 4 CHECKER
  if ( (strcmp(argv[3], "exposure") != 0) && (strcmp(argv[3], "blend") != 0) && (strcmp(argv[3], "zoom_in") != 0) && (strcmp(argv[3], "zoom_out") != 0) && (strcmp(argv[3], "pointilism") != 0) && (strcmp(argv[3], "swirl") != 0) && (strcmp(argv[3], "blur") != 0) ) {
    fprintf(stderr, "No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }

  if (argc == 3) {
    fprintf(stderr, "No operation name was specified, or operation name specified was invalid\n");
    return 4;
  }
  

  int answer;
  
  //exposure (WORKING)
  if (strcmp(argv[3], "exposure") == 0) {
    if (argc != 5) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }
      
      char* lastptr;
      errno = 0;
      double exposureFactor = strtod(argv[4], &lastptr);
      if (errno != 0 || *lastptr != '\0') {
	fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
	return 6;
      }
      
      answer = exposure(inputImg, exposureFactor, output);
      fclose(input);
      fclose(output);
      return answer;
    }
  }
  
  //blend (WORKING i think? works like the example on the site)
  if (strcmp(argv[3], "blend") == 0) {
    if (argc != 6) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }
      Image *inputImg2;
      FILE *input2 = fopen(argv[4], "r");

      if (input2 == NULL) {
	fprintf(stderr, "Specified input file could not be opened\n");
	return 2;
      }
      
      inputImg2 = read_ppm(input2);

      if (inputImg2 == NULL) {
	fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
	return 3;
      }
      
      char* lastptr;
      errno = 0;
      double blendFactor = strtod(argv[4], &lastptr);
      if (errno != 0 || *lastptr != '\0') {
        fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
        return 6;
      } 
      
      answer = blend(inputImg, inputImg2, blendFactor, output);
      free(inputImg2);
      fclose(input);
      fclose(input2);
      fclose(output);
      return answer;
    }
  }

  //zoom_inc (WORKING)
  if (strcmp(argv[3], "zoom_in") == 0) {
    if (argc != 4) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }
      answer = zoom_in(inputImg, output);
      fclose(input);
      fclose(output);
      return answer;
    }
  }

  //zoom_out (WORKING)
  if (strcmp(argv[3], "zoom_out") == 0) {
    if (argc != 4) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }
      answer = zoom_out(inputImg, output);
      fclose(input);
      fclose(output);
      return answer;
    }
  }
  
  //swirl (WORKING)
  if (strcmp(argv[3], "swirl") == 0) {
    if (argc != 7) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }
      //check if argv[6] is an int
      double intTest = strtod(argv[6], NULL);
      if ((int) intTest != intTest) {
	fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
	return 6;
      }
      
      answer = swirl(inputImg, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), output);
      fclose(input);
      fclose(output);
      return answer;
    }
  }

  //pointilism (WORKING)
  if (strcmp(argv[3], "pointilism") == 0) {
    if (argc != 4) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }
      answer = pointilism(inputImg, output);
      fclose(input);
      fclose(output);
      return answer;
    }
  }

  //blur (WORKING)
  if (strcmp(argv[3], "blur") == 0) {
    if (argc != 5) {
      fprintf(stderr, "Incorrect number of arguments or kinds of arguments specified for the specified operation\n");
      return 5;
    }
    else {
      if (output == NULL) {
	fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails\n");
	return 7;
      }

      char* lastptr;
      errno = 0;
      double blurFactor = strtod(argv[4], &lastptr);
      if (errno != 0 || *lastptr != '\0') {
        fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
        return 6;
      } 
      
      answer = blur(inputImg, blurFactor, output);
      fclose(input);
      fclose(output);
      return answer;
    }
  }
  
  return 0;
}

int main (int argc, char *argv[]) {

  int final = test(argc, argv);
  return final;
  
}
