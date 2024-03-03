#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include "ImageMatrix.h"

class EncodeMessage {
public:
    EncodeMessage();
    ~EncodeMessage();


    std::string static  intToBinary(int value);
    int static binaryToInt(std::string string_bit);
    std::string static charTo7Bits(int c);
    int static fibonacci(int c);
    bool static isPrime(std::vector<int> &prime_numbers, int i);
    ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions);


private:
    // Any private helper functions or variables if necessary

    
};

#endif // ENCODE_MESSAGE_H
