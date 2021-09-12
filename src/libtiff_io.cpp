#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;

#include "libtiff_io.h"

// Global variables
Mat tifFile;

float * read_tiff_image(const char* inputFile, unsigned int *widthOut,
                        unsigned int *heightOut, unsigned int *nChannels)
{

    tifFile = imread(inputFile, IMREAD_UNCHANGED);
    if(tifFile.empty())
    {
        std::cout << "Error, could not read the input image." << std::endl;
        std::cout << "File name : " << inputFile << std::endl;
        return(NULL);
    }

    //get height and width of image
    *widthOut = tifFile.cols;
    *heightOut = tifFile.rows;
    *nChannels = tifFile.channels();
    *nChannels = (int)fmax((float)*nChannels,(float)1.0);
    *nChannels = (int)fmin((float)*nChannels,(float)MAX_CHANNELS);

    std::cout << "Image size : " << *widthOut << " x " <<
                 *heightOut << " x " << *nChannels << std::endl;
    unsigned int width = *widthOut;
    unsigned int height = *heightOut;

    //reserve temporary space for the image
    unsigned int npixels=(unsigned int)(width*height);
    //uint32* raster=(uint32*) _TIFFmalloc(npixels *sizeof(uint32));

    float *outputImg = new float[npixels*(*nChannels)];
    //copy image information into the matrix
    for (unsigned int i=0; i<height; i++)
        for (unsigned int j=0; j<width; j++)
            for (unsigned int c=0; c<(*nChannels); c++)
            {
                Vec3w pixel = tifFile.at<Vec3w>(i, j);
                //note, the libtiff stores the image from the bottom left as the origin
                switch(c)
                {
                case 0: // Red
                    outputImg[j + i*width] = (float)pixel.val[2];
                    break;
                case 1: // Green
                    outputImg[j + i*width + width*height] = (float)pixel.val[1];
                    break;
                case 2: // Blue
                    outputImg[j + i*width + 2*width*height] = (float)pixel.val[0];
                    break;
                default:
                    std::cout << "Error in reading the tiff file, too many channels." << std::endl;
                    break;
                }
            }

    return(outputImg);
}


int write_tiff_image(float* inputImg, unsigned int n, unsigned int m,
                     unsigned int nChannels, const char* outputFile)
{
    unsigned int width,height;
    width = (unsigned int)n;
    height = (unsigned int)m;

    //Now writing image to the file one strip at a time
    for (unsigned int i = 0; i < height; i++)
    {
        //copy the image information into a temporary buffer
        for (unsigned int j = 0; j < width; j++)
        {	//the tiff stores the image info in the following order : RGB
            for (unsigned int c = 0; c < nChannels; c++)
            {
                // Vec3w pixel = tifFile.at<Vec3w>(i, j);
                switch(c)
                {
                case 0: // Red
                    tifFile.at<Vec3w>(i, j).val[2] = (unsigned short)(round( inputImg[j + i*width + c*width*height]) );
                    break;
                case 1: // Green
                    tifFile.at<Vec3w>(i, j).val[1] = (unsigned short)(round( inputImg[j + i*width + c*width*height]) );
                    break;
                case 2: // Blue
                    tifFile.at<Vec3w>(i, j).val[0] = (unsigned short)(round( inputImg[j + i*width + c*width*height]) );
                    break;
                default:
                    std::cout << "Error in reading the tiff file, too many channels." << std::endl;
                    break;
                }
            }
        }
    }

    imwrite(outputFile, tifFile);
    return(0);
}