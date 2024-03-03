#include "ImageMatrix.h"


// Default constructor
ImageMatrix::ImageMatrix() : data(nullptr) ,height(0), width(0)  {


}

// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double* [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
        for(int j = 0 ; j < width;j++)
        {
            data[i][j] = 0;
        }
    }
}

// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}


// Destructor
ImageMatrix::~ImageMatrix() {

	if(data != nullptr)	
    {
		for(int i = 0 ; i < height;i++)
		{
			delete[] data[i];
		}
        delete[] data;
	}
}

// Parameterized constructor - direct initialization with 2D matrix
ImageMatrix::ImageMatrix(const double** inputMatrix, int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth){
    data = new double*[height];
    for(int i = 0 ; i < height; i++)
    {
        data[i] = new double[width];
        for( int j = 0 ; j < width; j++)
        {
            data[i][j] = inputMatrix[i][j];
        }
    }
}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other) : height(other.get_height()), width(other.get_width()) {
    data = new double* [height];
    for (int i = 0; i < height; i++)
    {
        data[i] = new double[width];
        for (int j = 0; j < width; j++)
        {
            data[i][j] = other.get_data(i, j);
        }
    }
}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {
    if(this == &other)
    {
        return *this;
    }

	if(data != nullptr)
    {
	    for(int i = 0 ; i < height; i++)
	    {
            delete[] data[i];
	    }
        delete[] data;
    }

    height = other.height;
    width = other.width;

	data = new double* [height];
    for (int i = 0; i < height; i++)
    {
        data[i] = new double[width];
        for (int j = 0; j < width; j++)
        {
            data[i][j] = other.get_data(i, j);
        }
    }

    for (int i = 0; i < other.get_height(); ++i)
    {
        delete[] other.get_data()[i];
    }
    delete[] other.get_data();


    return *this;
}



// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
	double** resultData = new double* [height];
	for(int i = 0 ; i < height; i++)
    {
        resultData[i] = new double[width];

	    for(int j = 0; j < width; j++)
	    {
            resultData[i][j] =  other.get_data(i, j) + this->data[i][j];
           
	    }
    }
    ImageMatrix output(const_cast<const double**>(resultData), height, width);
    for (int i = 0; i < height; ++i)
    {
        delete[] resultData[i];
    }
    delete[] resultData;
    return  output;
}


// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    double** resultData = new double* [height];
    for (int i = 0; i < height; i++)
    {
        resultData[i] = new double[width];

        for (int j = 0; j < width; j++)
        {
            resultData[i][j] = this->data[i][j] - other.get_data(i,j);

        }
    }
    ImageMatrix output(const_cast<const double**>(resultData), height, width);
    for (int i = 0; i < height; ++i)
    {
        delete[] resultData[i];
    }
    delete[] resultData;
    return  output;
}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {
    double** resultData = new double* [height];
    for (int i = 0; i < height; i++)
    {
        resultData[i] = new double[width];

        for (int j = 0; j < width; j++)
        {
            
        	resultData[i][j] = this->data[i][j] * scalar;
            
            
        }
    }


    ImageMatrix output(const_cast<const double**>(resultData), height, width);

    for (int i = 0; i < height; ++i)
    {
        delete[] resultData[i];
    }
    delete[] resultData;

    return  output;
}

ImageMatrix ImageMatrix::operator*(const ImageMatrix& other) const
{
    double** resultData = new double* [height];
    for (int i = 0; i < height; i++)
    {
        resultData[i] = new double[width];

        for (int j = 0; j < width; j++)
        {
            resultData[i][j] = this->data[i][j] * other.get_data(i, j);
           
        }
    }
    ImageMatrix output(const_cast<const double**>(resultData), height, width);

    for (int i = 0; i < height; ++i)
    {
        delete[] resultData[i];


    }
    delete[] resultData;


    


    return  output;

}


// Getter function to access the data in the matrix
double** ImageMatrix::get_data() const {
    return data;
}

void ImageMatrix::set_data(int i ,int j, int value)
{
    data[i][j] = value;
}

// Getter function to access the data at the index (i, j)
double ImageMatrix::get_data(int i, int j) const {
    return data[i][j];
}

int ImageMatrix::get_height() const
{
    return height;
}
int ImageMatrix::get_width() const
{
    return width;
}

