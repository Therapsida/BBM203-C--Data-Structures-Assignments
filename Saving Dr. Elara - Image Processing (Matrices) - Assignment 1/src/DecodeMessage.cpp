// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>
#include <cmath>
#include "EncodeMessage.h"

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}

int DecodeMessage::binaryToInt(std::string string_bit)
{
	//simple binary int function with clipping

	int sum = 0;
	int power_of_two = 0;
	for (int i = string_bit.size() - 1; i >= 0; i--)
	{
		int binary_value = int(string_bit[i] - '0');
		sum += static_cast<int>(pow(2, power_of_two)) * binary_value;
		power_of_two++;
	}
	if (sum > 255) return 255;
	if (sum < 0) return 0;
	return sum;
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {

	// every char of the decoded message. 
	std::vector<std::string> seven_bits_binary;

	// every lsb of the edges.
	std::string decoded_message;


	for ( std::pair<int,int> location : edgePixels)
	{
		
		int value = image.get_data(location.first, location.second);

		int lsb = value % 2;

		if (lsb == 1) 
			decoded_message += '1';
		else
			decoded_message += '0';

	}
	
	while(decoded_message.size() % 7 != 0)
	{
			decoded_message = '0' + decoded_message;
	}


	for (int currentChar=  0; decoded_message.size() > currentChar;  currentChar += 7)
	{
		std::string sub_str = (decoded_message.substr(currentChar, 7));
		seven_bits_binary.push_back(sub_str);
	}

	
	std::string output_message;

	for (std::string bits_binary : seven_bits_binary)
	{
		int asci = binaryToInt(bits_binary);
		if (asci <= 32) 
			asci += 33;
		if (asci >= 127) 
			asci = 126;
		output_message += static_cast<char>(asci) ;
	}
		


	return output_message;
}




