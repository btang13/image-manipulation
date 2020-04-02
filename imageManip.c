//imageManip.c

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
#define sqr(a) pow(a, 2)
#define PI 3.14159265358979323846

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

Image * blend(Image *img, Image *img2, float alpha) {

  //figure out which dimensions to use
  int numRows, numCols, numRowsOverlap, numColsOverlap;
  
  //rows  
  if (img->rows > img2->rows) {
    numRows = img->rows;
    numRowsOverlap = img2->rows;
  }
  else {
    numRows = img2->rows;
    numRowsOverlap = img->rows;
  }

  //cols  
  if (img->cols > img2->cols) {
    numCols = img->cols;
    numColsOverlap = img2->cols;
  }
  else {
    numCols = img2->cols;
    numColsOverlap = img->cols;
  }

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
  if ((img->cols != img2->cols) && (img->rows != img2->rows)) {
    for (int r = numRowsOverlap; r < imgNew->rows; r++) {
      for (int c = numColsOverlap; c < imgNew->cols; c++) {
	imgNew->data[r * imgNew->cols + c].r = 0;
	imgNew->data[r * imgNew->cols + c].g = 0;
	imgNew->data[r * imgNew->cols + c].b = 0;
      }
    }
  }

  return imgNew;
  
}

Image * zoom_in(Image *img) {

  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = (img->rows * 2);
  imgNew->cols = (img->cols * 2);
  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));
  
  for (int r = 0; r < img->rows; r++) {
    for (int c = 0; c < img->cols; c++) {

      int red, green, blue;
      red = img->data[r * img->cols + c].r;
      green = img->data[r * img->cols + c].g;
      blue = img->data[r * img->cols + c].b;

      //2 * row, 2 * col
      imgNew->data[(2 * r) * (imgNew->cols) + (2 * c)].r = red;
      imgNew->data[(2 * r) * (imgNew->cols) + (2 * c)].g = green;
      imgNew->data[(2 * r) * (imgNew->cols) + (2 * c)].b = blue;

      //2 * row + 1, 2 * col
      imgNew->data[(2 * r + 1) * (imgNew->cols) + (2 * c)].r = red;
      imgNew->data[(2 * r + 1) * (imgNew->cols) + (2 * c)].g = green;
      imgNew->data[(2 * r + 1) * (imgNew->cols) + (2 * c)].b = blue;

      //2 * row, 2 * col + 1
      imgNew->data[(2 * r) * (imgNew->cols) + (2 * c + 1)].r = red;
      imgNew->data[(2 * r) * (imgNew->cols) + (2 * c + 1)].g = green;
      imgNew->data[(2 * r) * (imgNew->cols) + (2 * c + 1)].b = blue;

      //2 * row + 1, 2 * col + 1
      imgNew->data[(2 * r + 1) * (imgNew->cols) + (2 * c + 1)].r = red;
      imgNew->data[(2 * r + 1) * (imgNew->cols) + (2 * c + 1)].g = green;
      imgNew->data[(2 * r + 1) * (imgNew->cols) + (2 * c + 1)].b = blue;
    }
    
  }
  return imgNew;
}

Image * zoom_out(Image *img) {

  Image *imgNew = malloc(sizeof(Image));

  if (img->rows % 2 == 0 && img->cols % 2 == 0) {
    imgNew->rows = img->rows / 2;
    imgNew->cols = img->cols / 2;
  }
  else {
    if (img->rows % 2 == 1) {
      imgNew->rows = (img->rows / 2) - 1;
    }
    if (img->cols % 2 == 1) {
      imgNew->cols = (img->cols / 2) - 1;
    }
  }

  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));

  Pixel average;
  
  for (int r = 0; r < imgNew->rows; r++) {
    for (int c = 0; c < imgNew->cols; c++) {
      
      //red (2x2, 2+1x2, 2x2+1, 2+1x2+1)
      average.r = ( img->data[(2 * r) * img->cols + (2 * c)].r + img->data[(2 * r + 1) * img->cols + (2 * c)].r + img->data[(2 * r) * img->cols + (2 * c + 1)].r + img->data[(2 * r + 1) * img->cols + (2 * c + 1)].r ) / 4;

      //green (2x2, 2+1x2, 2x2+1, 2+1x2+1)
      average.g = ( img->data[(2 * r) * img->cols + (2 * c)].g + img->data[(2 * r + 1) * img->cols + (2 * c)].g + img->data[(2 * r) * img->cols + (2 * c + 1)].g + img->data[(2 * r + 1) * img->cols + (2 * c + 1)].g ) / 4;

      //blue (2x2, 2+1x2, 2x2+1, 2+1x2+1)
      average.b = ( img->data[(2 * r) * img->cols + (2 * c)].b + img->data[(2 * r + 1) * img->cols + (2 * c)].b + img->data[(2 * r) * img->cols + (2 * c + 1)].b + img->data[(2 * r + 1) * img->cols + (2 * c + 1)].b ) / 4;

      
      imgNew->data[r * imgNew->cols + c] = average;
    }
  }
  
  return imgNew;
}

//NEED TO FIX SWIRL ASAP
Image * swirl(Image *img, int xCenter, int yCenter, int distortion) {

  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = img->rows;
  imgNew->cols = img->cols;
  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));

  double alpha;
  int xCoord, yCoord;
  Pixel pix;
  
  for (int r = 0; r < imgNew->rows; r++) {
    for (int c = 0; c < imgNew->cols; c++) {
      
      alpha = sqrt( pow((r - xCenter), 2) + pow((c - yCenter), 2)) / distortion;
      //xCoord is column #
      //yCoord is row #

      xCoord = (r - xCenter) * cos(alpha) - (c - yCenter) * sin(alpha) + xCenter;
      yCoord = (r - xCenter) * sin(alpha) + (c - yCenter) * cos(alpha) + yCenter;

      pix = img->data[yCoord * img->cols + xCoord];

      imgNew->data[c * imgNew->cols + r] = pix;
    }
  }
  
  
  return imgNew;
}

Image * pointilism(Image *img) {
  
  unsigned int totalPixels = img->rows * img->cols;
  //unsigned int *pixels = malloc(totalPixels * sizeof(unsigned int));
  printf("Total pixels: %u, pixels affected: %u", totalPixels, totalPixels * 3 / 100);
    
  for (unsigned int i = 0; i < totalPixels * 3 / 100; i++) { // loop for 3% of pixels

    int row = rand() % img->rows;
    int col = rand() % img->cols;
    int rad = (rand() % 5) + 1;
    Pixel color = img->data[row * img->cols + col];
    
    // idk how to actually draw the circle so im making a box around the pixel
    // and figuring out which pixels are in the radius

    int start_row = max(row - rad, 0);
    int start_col = max(col - rad, 0);
    
    for (int k = start_col; k <= start_col + 2 * rad; k++) {
      for (int j = start_row; j <= start_row + 2 * rad; j++) {
	if (sqrt(sqr(j - row) + sqr(k - col)) <= (double) rad) { // distance formula
	  img->data[j * img->cols + k] = color;
	}
      }
    }
  }

  return img;
  
}

void gaussian_distr(double **g, double sigma, int N) {
  
  int mid = (N / 2) + 1;

  printf("\n%d\n%ld %ld\n\n", N, sizeof(g), sizeof(g[0]));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      double dx = (double)i - mid;
      double dy = (double)j - mid;
      g[j][i] = (1.0 / (2.0 * PI * sqr(sigma))) * exp( -(sqr(dx) + sqr(dy)) / (2 * sqr(sigma)));

      printf("%0.6f ", g[j][i]);

      //printf("%d %d\n", i, j);
      
    }
    printf("\n");
  }
  
}

Image * blur(Image *img, double sigma) {
  
  int N = ((int)(10.0 * sigma));
  N = N % 2 == 1 ? N : N + 1;
  
  double **g;
  g = (double **)malloc(N * sizeof(double *));
  for (int i = 0; i < N; i++) {
    g[i] = (double *)malloc(N * sizeof(double));
  }
  
  gaussian_distr(g, sigma, N);
  return img; // replace

}
