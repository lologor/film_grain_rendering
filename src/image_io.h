
#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <iostream>
#include <cmath>
#include <cstring>
#include "matrix.h"

#define MAX_CHANNELS 3

float* read_image(const char* inputFile, unsigned int *width,
                       unsigned int *height, unsigned int *nChannels);

int write_image(float* inputImg, unsigned int n, unsigned int m,
                     unsigned int nChannels, const char* outputFile);
#endif
