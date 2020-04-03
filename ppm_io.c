
// __Add your name and JHED above__
// ppm_io.c
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file

#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>
#include <ctype.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data. Returns NULL if 
 * there is an issue with the file.
 */
Image * read_ppm(FILE *fp) {
  
  // check that fp is not NULL
  assert(fp);
  
  Image *img = malloc(sizeof(Image));
  //Pixel *pix;
  //char header[18];
  int rows, cols;

  char a, ab, abc;

  a = fgetc(fp);
  ab = fgetc(fp);
  abc = fgetc(fp);
    
  if (a != 'P' || ab != '6' || !isspace(abc)) {
    return NULL;
  }
 

  // get the comment
  int c;
  c = getc(fp);
  // printf("2 is %c", c);
  if (c == '#') {
    c = getc(fp);
    // printf("3 is %c", c);
    while (c != '\n') {
      c = getc(fp);
      // printf("%c\n", c);
    } 
  } else {
    ungetc(c, fp);
  }
  
  // not sure how important the whitespace fgetc() is
  //fgetc(fp); // guaranteed whitespace
  fscanf(fp, "%d", &cols);
  // printf("this is # of cols: %d\n", cols);
  fgetc(fp); // guaranteed whitespace
  fscanf(fp, "%d", &rows);
  // printf("this is # of rows: %d\n", rows);
  fgetc(fp); // guaranteed whitespace

  if (cols < 0 || rows < 0) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    exit(3);
  }
  
  int color_check = 0;
  fscanf(fp, "%d", &color_check);
  
  if (color_check != 255) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    exit(3);
  }

  fgetc(fp); // guaranteed whitespace
  
  //pix = malloc(rows * cols * sizeof(Pixel));
  img->rows = rows;
  img->cols = cols;
  img->data = malloc(img->rows * img->cols * sizeof(Pixel));
  //img->data = (Pixel*) malloc(img->rows * img->cols * sizeof(Pixel));
  int count = fread(img->data, sizeof(Pixel), cols * rows, fp);
  //fwrite(img->data, sizeof(Pixel), sizeof(pix), pix);
  
  if (count != (rows * cols)) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    exit(3);
  }

  Pixel * anotherPix = (Pixel*) malloc(sizeof(Pixel));
  if (fread(anotherPix, sizeof(Pixel), 1, fp) == 1) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
    exit(3);
  }
  free(anotherPix);

  return img;
  
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

