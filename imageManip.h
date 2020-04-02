//imageManip.h

#ifndef IMAGE_MANIP
#define IMAGE_MANIP

#include <stdio.h>
#include "ppm_io.h"

//not done need 2 more functions
Image * exposure(Image * img, int ev);
Image * blend(Image *img, Image *img2, int alpha);
Image * zoom_in(Image *img);
Image * zoom_out(Image *img);
Image * swirl(Image *img, int xCenter, int yCenter, int distortion);
Image * pointilism(Image *img);
Image * blur(Image *img, double sigma);

#endif
