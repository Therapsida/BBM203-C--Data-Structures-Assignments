// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>
#include <iostream>


// Default constructor
EdgeDetector::EdgeDetector() {

	sobol_operator_x = new double*[3]{
        new double[3] {-1.0, 0.0, 1.0},
        new double[3] {-2.0, 0.0, 2.0},
        new double[3] {-1.0, 0.0, 1.0}
	};

    sobol_operator_y = new double*[3]{
        new double[3] {-1.0, -2.0, -1.0},
        new double[3] {0.0, 0.0, 0.0},
        new double[3] {1.0, 2.0, 1.0}
    };
}

// Destructor
EdgeDetector::~EdgeDetector() {
	for (int i = 0; i < 3; ++i)
	{
        delete[] sobol_operator_x[i];
        delete[] sobol_operator_y[i];
	}
    delete[] sobol_operator_x;
    delete[] sobol_operator_y;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

	

	std::vector<std::pair<int, int>> gradient_vector;

    Convolution sobol_x(sobol_operator_x, 3, 3, 1, true);
    Convolution sobol_y(sobol_operator_y, 3, 3, 1, true);

	ImageMatrix gradient_x = sobol_x.convolve(input_image);
	ImageMatrix gradient_y = sobol_y.convolve(input_image);


	std::vector<std::vector<double>> all_gradients(input_image.get_height(), std::vector<double>(input_image.get_width(),0));
	double sum = 0;
	for (int i = 0; i < input_image.get_height() ; i++)
	{
		for (int j = 0; j < input_image.get_width(); j++)
		{
			double number = sqrt(pow(gradient_x.get_data(i, j), 2) + pow(gradient_y.get_data(i, j), 2));
			sum += number;
			all_gradients[i][j] = number;
		}
	}
	double average_sum = sum / (gradient_x.get_height() * gradient_x.get_width());

	for (int i = 0; i < gradient_x.get_height(); i++)
	{
		for (int j = 0; j < gradient_x.get_width(); j++)
		{
			if (all_gradients[i][j] >= average_sum) {

				gradient_vector.push_back(std::pair<int, int>(i, j));
			}


		}
	}
		
	return gradient_vector;
}

