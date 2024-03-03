#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : kernelHeight(3), kernelWidth(3), stride(1), padding(true) {
	kernel = new double* [3];
	for (int i = 0 ; i < 3 ; i++)
	{
		kernel[i] = new double[3];
		for (int j = 0; j < 3; j++)
		{
			kernel[i][j] = 0;
		}
	}
	
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) : kernelHeight(kh),kernelWidth(kw),stride(stride_val),padding(pad) {
	kernel = new double* [kernelHeight];
	for (int i = 0; i < kernelHeight; i++)
	{
		kernel[i] = new double[kernelWidth];
		for (int j = 0; j < kernelWidth; j++)
		{
			kernel[i][j] = customKernel[i][j];
		}
	}

}

// Destructor
Convolution::~Convolution() {
	for (int i = 0; i < kernelHeight; ++i)
	{
		delete[] kernel[i];
	}
	delete[] kernel;
}

// Copy constructor
Convolution::Convolution(const Convolution &other) : kernelHeight(other.kernelHeight) , kernelWidth(other.kernelWidth) , stride(other.stride) , padding(other.padding){
	kernel = new double* [kernelHeight];
	for (int i = 0; i < kernelHeight; i++)
	{
		kernel[i] = new double[kernelWidth];
		for (int j = 0; j < kernelWidth; j++)
		{
			kernel[i][j] = other.kernel[i][j];
		}
	}
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
	if(this == &other)
	{
		return *this;
	}
	if(this->kernel != nullptr) // even though kernel can not be nullptr.
	{
		for (int i = 0; i < kernelHeight; ++i)
		{
			delete[] kernel[i];
		}
		delete[] kernel;
	}

	kernelHeight = other.kernelHeight;
	kernelWidth = other.kernelWidth;
	stride = other.stride;
	padding = other.padding;

	kernel = new double* [kernelHeight];
	for (int i = 0; i < kernelHeight; i++)
	{
		kernel[i] = new double[kernelWidth];
		for (int j = 0; j < kernelWidth; j++)
		{
			kernel[i][j] = other.kernel[i][j];
		}
	}
	return *this;

}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {

	int input_image_height = input_image.get_height();
	int input_image_width = input_image.get_width();

	double** input_data;

	const int output_height = (int)((input_image_height - kernelHeight + 2 * padding) / stride) + 1;

	const int output_width = (int)((input_image_width - kernelWidth + 2 * padding) / stride) + 1;

	if (padding)
	{
		input_image_height += 2;
		input_image_width += 2;
		input_data = new double* [input_image_height];
		for (int i = 0; i < input_image_height; ++i)
		{
			input_data[i] = new double[input_image_width];
			for (int j = 0; j < input_image_width; ++j)
			{
				if (i == 0 || j == 0 || i == input_image_height-1 || j == input_image_width-1)
					input_data[i][j] = 0;
				else
					input_data[i][j] = input_image.get_data(i - 1, j - 1);
			}
		}



	}
	else
	{
		input_data = input_image.get_data();
	}


	


	

	double** output_data = new double*[output_height];

	
	for (int i = 0; i <= input_image_height - kernelHeight; i += stride)
	{
		if (i + kernelHeight > input_image_height) break;

		output_data[i/stride] = new double[output_width];

		for (int j = 0; j <= input_image_width - kernelWidth; j += stride)
		{
			if( j+kernelWidth > input_image_width) break;

			double sum = 0;
			for (int k = 0; k < kernelHeight; k++)
			{
				for (int l = 0; l < kernelWidth; ++l)
				{
					sum += input_data[i+k][j+l] * kernel[k][l];
					
					
				}
			}
			output_data[i/stride][j/stride] = sum;
		}
		
	}

	if(padding){
	for (int i = 0; i < input_image_height; ++i)
		{
		delete[] input_data[i];
		}
	delete[] input_data;
	}

	ImageMatrix output_image(const_cast<const double**>(output_data), output_height, output_width);

	for (int i = 0; i < output_image.get_height(); ++i)
	{
		delete[] output_data[i];
	}
	delete[] output_data;

	return  output_image;

}
