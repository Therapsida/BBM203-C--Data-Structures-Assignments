#include "EncodeMessage.h"
#include <cmath>
#include <iostream>


// Default Constructor
EncodeMessage::EncodeMessage() {


}

// Destructor
EncodeMessage::~EncodeMessage() {



}


std::string EncodeMessage::intToBinary(int value)
{
	std::string binaryValue;
	while (value >= 1)
	{
		int reminder = value % 2;

		value = value / 2;

		if (reminder == 1)
			binaryValue = '1' + binaryValue;
		else
			binaryValue = '0' + binaryValue;
	}
	while (binaryValue.size() < 7)
	{
		binaryValue = '0' + binaryValue;
	}


	return binaryValue;
}

int EncodeMessage::binaryToInt(std::string string_bit)
{
	int sum = 0;
	int power_of_two = 0;
	for(int i = string_bit.size() -1; i >= 0; i--)
	{
		int binary_value = int(string_bit[i] - '0');
		sum += static_cast<int>(pow(2, power_of_two)) * binary_value ;
		power_of_two++;
	}
	if (sum > 255) return 255;
	if (sum < 0) return 0;
	return sum;
}

std::string EncodeMessage::charTo7Bits(int c)
{
	std::string sevenBitString;
	while (c >= 1)
	{
		sevenBitString =  char(c % 2 == 1 ? '1' : '0' )+ sevenBitString;
		c = c / 2;
	}
	while (sevenBitString.size() < 7)
	{
		sevenBitString = '0' + sevenBitString;
	}

	return sevenBitString;

}



int EncodeMessage::fibonacci(int n)
{
	if (n == 1 || n == 2) return 1;
	if (n == 0) return 0;
	int current = 1;
	int prev_one = 1;
	for (int i = 1; i < n-1; ++i)
	{
		int temp = current;
		current += prev_one;
		prev_one = temp;
	}

	return current;
}

bool EncodeMessage::isPrime(std::vector<int> &prime_numbers , int i)
{
	if (i == 0 || i == 1) return false;
	for (int prime_number : prime_numbers)
	{
		if (i % prime_number == 0) return false;
	}
	prime_numbers.push_back(i);
	return true;
}





// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {

	std::string output_message = message;

	std::vector<int> primenumbers ;


	for (int i = 0; i < output_message.size(); ++i)
	{
		if ((isPrime(primenumbers, i)))
		{
			int c = fibonacci(i);

			int fibonacci_char =  output_message[i] + c;

			if (fibonacci_char <= 32)
				fibonacci_char += 33;
			if (fibonacci_char > 126)
				fibonacci_char = 126;

			output_message[i] = static_cast<char>(fibonacci_char);
		}

		
	}


	int right_shift = static_cast<int>(output_message.size())/2;

	output_message = output_message.substr(output_message.size() - right_shift, right_shift) + output_message.substr(0, output_message.size() - right_shift);


	std::string bits_string;

	for (char charx : output_message)
	{
		bits_string += charTo7Bits(charx);

	}

	
	double** output_matrix = new double*[img.get_height()];
	for (int i = 0; i < img.get_height(); ++i)
	{
		output_matrix[i] = new double[img.get_width()];
		for (int j = 0; j < img.get_width(); ++j)
		{
			output_matrix[i][j] = img.get_data(i, j);
		}
	}



	for (int i = 0; i < positions.size(); ++i)
	{
		if (bits_string.size() <= i) break;

		std::string new_bit = intToBinary(static_cast<int>(img.get_data(positions[i].first, positions[i].second)));

		new_bit[new_bit.size()-1] = bits_string[i];
		output_matrix[positions[i].first][positions[i].second] = binaryToInt(new_bit);
	

	}

	
			


	

	ImageMatrix output(const_cast<const double**>(output_matrix), img.get_height(), img.get_width());
	for (int i = 0; i < img.get_height(); ++i)
	{
		delete[] output_matrix[i];
	}
	delete[] output_matrix;


	return  output;


}
