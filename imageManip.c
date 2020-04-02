//imageManip.c

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
#define sqr(a) a*a
#define PI 3.14

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "ppm_io.h" 
#include "imageManip.h"
#include <stdlib.h>

//exposure (DONE)
int exposure(Image * img, int ev, FILE *fp) {

  if (ev < -3 || ev > 3) {
    printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
    return 6;
  }
  
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
  write_ppm(fp, img);
  free(img);
  return 0;
}

//DONE? PROB NOT
int blend(Image *img, Image *img2, float alpha, FILE *fp) {

  if (alpha < 0.0 || alpha > 1.0) {
    printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
    return 6;
  }
  
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
  write_ppm(fp, imgNew);
  free(imgNew->data);
  free(imgNew);
  return 0;
}

//zoom_in (DONE)
int zoom_in(Image *img, FILE *fp) {

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
  write_ppm(fp, imgNew);
  free(imgNew);
  return 0;
}

//zoom_out (DONE)
int zoom_out(Image *img, FILE *fp) {

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
  write_ppm(fp, imgNew);
  free(imgNew->data);
  free(imgNew);
  return 0;
}

//swirl (DONE)
int swirl(Image *img, int xCenter, int yCenter, int distortion, FILE *fp) {

  if (distortion < 1) {
    printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
    return 6;
  }
  
  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = img->rows;
  imgNew->cols = img->cols;
  imgNew->data = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));

  Pixel pix;
  double alpha;
  int xCoord, yCoord;
  
  for (int r = 0; r < imgNew->rows; r++) {
    for (int c = 0; c < imgNew->cols; c++) {
      
      alpha = (sqrt(pow((c - xCenter), 2) + pow((r - yCenter), 2)) / distortion);
      xCoord = ((c - xCenter) * cos(alpha)) - ((r - yCenter) * sin(alpha)) + xCenter;
      yCoord = ((c - xCenter) * sin(alpha)) + ((r - yCenter) * cos(alpha)) + yCenter;

      pix = img->data[yCoord * img->cols + xCoord];
      
      if ( (xCoord < 0 || xCoord > img->cols) || (yCoord < 0 || yCoord > img->rows) ) {
	pix.r = 0;
	pix.g = 0;
	pix.b = 0;
      }
      
      imgNew->data[r * imgNew->cols + c] = pix;
      
    }
  }
  
  write_ppm(fp, imgNew);
  free(imgNew->data);
  free(imgNew);
  return 0;
  
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

int blur(Image *img, double sigma, FILE *fp) {

  //finding dimensions of our matrix
  int dim = (int) (sigma * 10);
  if (dim % 2 == 0) {
    dim++;
  }
  
  double matrix[dim][dim];

  
  //math for the matrix
  int center;
  int dx, dy;

  center = (int) (dim / 2);
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      dx = j - center;
      dy = i - center;
      matrix[i][j] = (1.0 / (2.0 * PI * (sigma * sigma))) * exp( -((dx * dx) + (dy * dy)) / (2 * (sigma * sigma)));
    }
  }

  //declare our output img
  Image *imgNew = malloc(sizeof(Image));
  imgNew->rows = img->rows;
  imgNew->cols = img->cols;

  Pixel *pix;
  pix = malloc(imgNew->rows * imgNew->cols * sizeof(Pixel));


  //bounds for image
  int imgMinRow, imgMinCol;
  imgMinRow = 0;
  imgMinCol = 0;
  int imgMaxRow, imgMaxCol;
  imgMaxRow = (imgNew->rows - 1);
  imgMaxCol = (imgNew->cols - 1);


  //bounds for matrix
  int minRow, minCol;

  double sumMatrix;
  double red, green, blue;
  int x, y, index;

  for (int r = 0; r < img->rows; r++) {
    for (int c = 0; c < img->cols; c++) {

      sumMatrix = 0;
      red = 0;
      green = 0;
      blue = 0;

      //we must initialize the bounds of our matrix
      minRow = r - (dim / 2);
      minCol = c - (dim / 2);

      //find pixel starting position
      x = minCol;
      y = minRow;
      index = (y * imgNew->cols + x);

      for (int i = 0; i < dim; i++) {
	if (y < imgMinRow) {
	  y++;
	  index = (y * imgNew->cols + x);
	  continue;
	}
	if (y > imgMaxRow) {
	  continue;
	}

	for (int j = 0; j < dim; j++) {
	  if (x < imgMinCol) {
	    x++;
	    index = (y * imgNew->cols + x);
	    continue;
	  }
	  if (x > imgMaxCol) {
	    continue;
	  }

	  sumMatrix += matrix[i][j];
	  red += (img->data[index].r * matrix[i][j]);
	  green += (img->data[index].g * matrix[i][j]);
	  blue += (img->data[index].b * matrix[i][j]);

	  x++;
	  index = (y * imgNew->cols + x);
	}

	x = minCol;
	y++;
	index = (y * imgNew->cols + x);

      }

      pix[r * imgNew->cols + c].r = (red / sumMatrix);
      pix[r * imgNew->cols + c].g = (green / sumMatrix);
      pix[r * imgNew->cols + c].b = (blue / sumMatrix);
      

    }
  }

  imgNew->data = pix;
  write_ppm(fp, imgNew);
  free(pix);
  free(imgNew);
  return 0;
}
