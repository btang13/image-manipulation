//imageManip.h

#ifndef IMAGE_MANIP
#define IMAGE_MANIP

#include <stdio.h>
#include "ppm_io.h"

int exposure(Image * img, int ev, FILE *fp);
int blend(Image *img, Image *img2, float alpha, FILE *fp);
int zoom_in(Image *img, FILE *fp);
int zoom_out(Image *img, FILE *fp);
int swirl(Image *img, int xCenter, int yCenter, int distortion, FILE *fp);
Image * pointilism(Image *img);
Image * blur(Image *img, double sigma);

#endif
