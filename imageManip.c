//imageManip.c

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
#define sqr(a) a*a

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "ppm_io.h" 
#include "imageManip.h"
#include <stdlib.h>

//better to make a new array for each?

//exposure (DONE FUCK YEAH)
Image * exposure(Image * img, int ev) {

  for (int r = 0; r < img->rows; r++) {
    for (int c = 0; c < img->cols; c++) {

      double factor = pow(2.0, ev);

      int red = img->data[r * img->cols + c].r * factor;
      int green = img->data[r * img->cols + c].g * factor;
      int blue = img->data[r * img->cols + c].b * factor;

      //red checker
      if (red > 255) {
	img->data[r * img->cols + c].r = 255;
      }
      else {
	img->data[r * img->cols + c].r = red;
      }

      //green checker
      if (green > 255) {
	img->data[r * img->cols + c].g = 255;
      }
      else {
	img->data[r * img->cols + c].g = green;
      }

      //blue checker
      if (blue > 255) {
	img->data[r * img->cols + c].b = 255;
      }
      else {
	img->data[r * img->cols + c].b = blue;
      }
      
      
    }
  }

  return img;
}

Image * blend(Image *img, Image *img2, int alpha) {

  //figure out which dimensions to use
  int numRows, numCols, numRowsOverlap, numColsOverlap;
  
  //rows
  printf("this is img->rows: %d\n", img->rows);
  printf("this is img2->rows: %d\n", img2->rows);
  
  if (img->rows > img2->rows) {
    numRows = img->rows;
    numRowsOverlap = img2->rows;
  }
  else {
    numRows = img2->rows;
    numRowsOverlap = img->rows;
  }

  printf("this is numRows: %d\n", numRows);
  printf("this is numRows Overlap: %d\n", numRowsOverlap);

  //cols
  printf("this is img->cols: %d\n", img->cols);
  printf("this is img2->cols: %d\n", img2->cols);
  
  if (img->cols > img2->cols) {
    numCols = img->cols;
    numColsOverlap = img2->cols;
  }
  else {
    numCols = img2->cols;
    numColsOverlap = img->cols;
  }

  printf("this is numCols: %d\n", numCols);
  printf("this is numCols Overlap: %d\n", numColsOverlap);

  //declare new image
  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = numRows;
  imgNew->cols = numCols;
  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));

  //first we will take care of overlapping elements
  for (int r = 0; r < numRowsOverlap; r++) {
    for (int c = 0; c < numColsOverlap; c++) {
      imgNew->data[r * numColsOverlap + c].r = alpha * img->data[r * numColsOverlap + c].r + (1 - alpha) * img2->data[r * numColsOverlap + c].r;
      imgNew->data[r * numColsOverlap + c].g = alpha * img->data[r * numColsOverlap + c].g + (1 - alpha) * img2->data[r * numColsOverlap + c].g;
      imgNew->data[r * numColsOverlap + c].b = alpha * img->data[r * numColsOverlap + c].b + (1 - alpha) * img2->data[r * numColsOverlap + c].b;
      
    }
  }

  //ROWS
  //case that image 1 is bigger
  if (img->rows > img2->rows) {
    for (int r = numRowsOverlap; r < imgNew->rows; r++) {
      for (int c = 0; c < numColsOverlap; c++) {
	imgNew->data[r * numColsOverlap + c].r = img->data[r * numColsOverlap + c].r;
	imgNew->data[r * numColsOverlap + c].g = img->data[r * numColsOverlap + c].g;
	imgNew->data[r * numColsOverlap + c].b = img->data[r * numColsOverlap + c].b;
      }
    }
  }

  //case that image 2 is bigger
  if (img2->rows > img->rows) {
    for (int r = numRowsOverlap; r < imgNew->rows; r++) {
      for (int c = 0; c < numColsOverlap; c++) {
	imgNew->data[r * numColsOverlap + c].r = img2->data[r * numColsOverlap + c].r;
	imgNew->data[r * numColsOverlap + c].g = img2->data[r * numColsOverlap + c].g;
	imgNew->data[r * numColsOverlap + c].b = img2->data[r * numColsOverlap + c].b;
      }
    }
  } 


  //COLS
  //case that image 1 is bigger
  if (img->cols > img2->cols) {
    for (int r = 0; r < numRowsOverlap; r++) {
      for (int c = numColsOverlap; c < imgNew->cols; c++) {
	imgNew->data[r * imgNew->cols + c].r = img->data[r * imgNew->cols + c].r;
	imgNew->data[r * imgNew->cols + c].g = img->data[r * imgNew->cols + c].g;
	imgNew->data[r * imgNew->cols + c].b = img->data[r * imgNew->cols + c].b;
      }
    }
  }

  //case that image 2 is bigger
  if (img2->cols > img->cols) {
    for (int r = 0; r < numRowsOverlap; r++) {
      for (int c = numColsOverlap; c < imgNew->cols; c++) {
	imgNew->data[r * imgNew->cols + c].r = img2->data[r * imgNew->cols + c].r;
	imgNew->data[r * imgNew->cols + c].g = img2->data[r * imgNew->cols + c].g;
	imgNew->data[r * imgNew->cols + c].b = img2->data[r * imgNew->cols + c].b;
      }
    }
  }

  //Empty Corner (will be black, or rgb = 0 0 0)
  for (int r = numRowsOverlap; r < imgNew->rows; r++) {
    for (int c = numColsOverlap; c < imgNew->cols; c++) {
      imgNew->data[r * imgNew->cols + c].r = 0;
      imgNew->data[r * imgNew->cols + c].g = 0;
      imgNew->data[r * imgNew->cols + c].b = 0;
    }
  }

  //need to write ppm
  //and then free the thing
  return imgNew;
  
}

Image * zoom_in(Image *img) {

  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = img->rows * 2;
  imgNew->cols = img->cols * 2;
  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));

  for (int r = 0; r < (imgNew->rows / 2); r++) {
    for (int c = 0; c < (imgNew->cols / 2); c++) {
      //2 * row, 2 * col
      imgNew->data[(2 * r) * (imgNew->cols / 2) + (2 * c)].r = img->data[r * (imgNew->cols / 2) + c].r;
      imgNew->data[(2 * r) * (imgNew->cols / 2) + (2 * c)].g = img->data[r * (imgNew->cols / 2) + c].g;
      imgNew->data[(2 * r) * (imgNew->cols / 2) + (2 * c)].b = img->data[r * (imgNew->cols / 2) + c].b;

      //2 * row + 1, 2 * col
      imgNew->data[(2 * r + 1) * (imgNew->cols / 2) + (2 * c)].r = img->data[r * (imgNew->cols / 2) + c].r;
      imgNew->data[(2 * r + 1) * (imgNew->cols / 2) + (2 * c)].g = img->data[r * (imgNew->cols / 2) + c].g;
      imgNew->data[(2 * r + 1) * (imgNew->cols / 2) + (2 * c)].b = img->data[r * (imgNew->cols / 2) + c].b;

      //2 * row, 2 * col + 1
      imgNew->data[(2 * r) * (imgNew->cols / 2) + (2 * c + 1)].r = img->data[r * (imgNew->cols / 2) + c].r;
      imgNew->data[(2 * r) * (imgNew->cols / 2) + (2 * c + 1)].g = img->data[r * (imgNew->cols / 2) + c].g;
      imgNew->data[(2 * r) * (imgNew->cols / 2) + (2 * c + 1)].b = img->data[r * (imgNew->cols / 2) + c].b;

      //2 * row + 1, 2 * col + 1
      imgNew->data[(2 * r + 1) * (imgNew->cols / 2) + (2 * c + 1)].r = img->data[r * (imgNew->cols / 2) + c].r;
      imgNew->data[(2 * r + 1) * (imgNew->cols / 2) + (2 * c + 1)].g = img->data[r * (imgNew->cols / 2) + c].g;
      imgNew->data[(2 * r + 1) * (imgNew->cols / 2) + (2 * c + 1)].b = img->data[r * (imgNew->cols / 2) + c].b;
    }
  }
  return imgNew;
}

Image * zoom_out(Image *img) {

  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = (img->rows / 2);
  imgNew->cols = (img->cols / 2);
  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));

  for (int r = 0; r < imgNew->rows; r++) {
    for (int c = 0; c < imgNew->cols; c++) {

      //red (2x2, 2+1x2, 2x2+1, 2+1x2+1)
      imgNew->data[r * imgNew->cols + c].r = ( img->data[(2 * r) * imgNew->cols + (2 * c)].r + img->data[(2 * r + 1) * imgNew->cols + (2 * c)].r + img->data[(2 * r) * imgNew->cols + (2 * c + 1)].r + img->data[(2 * r + 1) * imgNew->cols + (2 * c + 1)].r ) / 4;

      //green (2x2, 2+1x2, 2x2+1, 2+1x2+1)
      imgNew->data[r * imgNew->cols + c].g = ( img->data[(2 * r) * imgNew->cols + (2 * c)].g + img->data[(2 * r + 1) * imgNew->cols + (2 * c)].g + img->data[(2 * r) * imgNew->cols + (2 * c + 1)].g + img->data[(2 * r + 1) * imgNew->cols + (2 * c + 1)].g ) / 4;

      //blue (2x2, 2+1x2, 2x2+1, 2+1x2+1)
      imgNew->data[r * imgNew->cols + c].b = ( img->data[(2 * r) * imgNew->cols + (2 * c)].g + img->data[(2 * r + 1) * imgNew->cols + (2 * c)].g + img->data[(2 * r) * imgNew->cols + (2 * c + 1)].g + img->data[(2 * r + 1) * imgNew->cols + (2 * c + 1)].g ) / 4;
    }
  }

  //write ppm
  //free the imgNew
  return imgNew;
}

Image * swirl(Image *img, int xCenter, int yCenter, int distortion) {

  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = img->rows;
  imgNew->cols = img->cols;
  imgNew->data = malloc(imgNew->rows * imgNew-> cols * sizeof(Pixel));

  int alpha;
  int xCoord, yCoord;
  
  for (int r = 0; r < imgNew->rows; r++) {
    for (int c = 0; c < imgNew->cols; c++) {
      alpha = sqrt( pow((r - xCenter), 2) + pow((c - yCenter), 2)) / distortion;

      xCoord = (r - xCenter) * cos(alpha) - (c - yCenter) * sin(alpha) + xCenter;
      yCoord = (r - xCenter) * sin(alpha) - (c - yCenter) * cos(alpha) + yCenter;

      imgNew->data[r * imgNew->cols + c] = img->data[xCoord * imgNew->cols + yCoord];
    }
  }

  //write ppm
  //free imgNew
  
  return imgNew;
}

Image * pointilism(Image *img) {
  
  unsigned int totalPixels = img->rows * img->cols;
  //unsigned int *pixels = malloc(totalPixels * sizeof(unsigned int));
  for (unsigned int i = 0; i < totalPixels * 3 / 100; i++) { // loop for 3% of pixels

    int row = rand() % img->rows;
    int col = rand() % img->rows;
    int rad = (rand() % 5) + 1;
    Pixel color = img->data[row * img->rows + col];

    // idk how to actually draw the circle so im making a box around the pixel
    // and figuring out which pixels are in the radius
    
    int start_row = min(row - rad, 0);
    int start_col = min(col - rad, 0);

    for (int j = start_row; j < start_row + 2 * rad; j++) {
      for (int k = start_col; k < start_col + 2 * rad; k++) {
	if (sqrt(sqr(j - row) + sqr(k - col)) <= rad) { // distance formula
	  img->data[j * img->rows + k] = color;
	}
      }
    }
  }

  return img;
  
}

Image * blur(Image *img, double sigma) {

  sigma++; //replace
  return img; // replace

}
