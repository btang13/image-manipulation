
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
  
  if (fgetc(fp) != 'P' || fgetc(fp) != '6') {
    return NULL;
  }

  fgetc(fp); //guaranteed whitespace after P6

  //i added this; this is to check for comment lines in the image
  //before this, code wasn't picking up the rows/cols correctly
  int c;
  c = getc(fp);

  if (c == '#') {
    while (getc(fp) != '\n') {
      c = getc(fp);
      printf("%c\n", c);
    }
    ungetc(c, fp);
  }
  ungetc(c, fp);
  ungetc(c, fp);
  //note: i have no clue if this code works properly
  //works for building.ppm & puppy.ppm but i dont have the other pics
  //so idk if it works for those
  
  // not sure how important the whitespace fgetc() is
  fgetc(fp); // guaranteed whitespace
  fscanf(fp, "%d", &cols);
  printf("this is # of cols: %d\n", cols);
  fgetc(fp); // guaranteed whitespace
  fscanf(fp, "%d", &rows);
  printf("this is # of rows: %d\n", rows);
  fgetc(fp); // guaranteed whitespace

  unsigned char color_check = 0;
  fscanf(fp, "%hhu", &color_check);
  
  if (color_check != 255) {
    printf("%u\n", color_check);
    return NULL;
  }

  fgetc(fp); //guaranteed whitespace

  //pix = malloc(rows * cols * sizeof(Pixel));
  img->data = malloc(rows * cols * sizeof(Pixel));
  img->rows = rows;
  img->cols = cols;
  fread(img->data, sizeof(Pixel), cols * rows, fp);
  //fwrite(img->data, sizeof(Pixel), sizeof(pix), pix);
  
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

