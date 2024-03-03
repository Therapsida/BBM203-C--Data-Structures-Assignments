#include "ImageSharpening.h"

#include <iostream>

// Default constructor
ImageSharpening::ImageSharpening() {
	blurring_kernel = new double*[3];
	for (int i = 0; i < 3; ++i)
	{
		blurring_kernel[i] = new double[3];
		for (int j = 0; j < 3; ++j)
		{
			blurring_kernel[i][j] = (1.0/9.0);
		}
	}
}

ImageSharpening::~ImageSharpening(){
	for (int i = 0; i < 3; ++i)
	{
		delete[] blurring_kernel[i];
	}
	delete[] blurring_kernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {

	Convolution convolution(blurring_kernel,3,3,1,true);

	ImageMatrix image_noisy = convolution.convolve(input_image);

	ImageMatrix image_sharp = input_image + ((input_image - image_noisy )*k);

	for (int i = 0; i < image_sharp.get_height(); ++i)
	{
		for (int j = 0; j < image_sharp.get_width(); ++j)
		{

			if (image_sharp.get_data(i, j) < 0.0)
			{
				image_sharp.set_data(i, j, 0);
			}
			if (image_sharp.get_data(i, j) > 255.0) image_sharp.set_data(i, j, 255);
			
		}
		
		
	}
	
	return image_sharp;
}
