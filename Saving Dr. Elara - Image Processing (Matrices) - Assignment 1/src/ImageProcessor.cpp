#include "ImageProcessor.h"

#include <iostream>


ImageProcessor::ImageProcessor() : sharpening_const(2) {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {

	DecodeMessage decode_message;

	EdgeDetector edge_detector;

	ImageSharpening image_sharpening;

	ImageMatrix sharpenedImage = image_sharpening.sharpen(img, sharpening_const);

	std::vector<std::pair<int,int>> edge_pixels = edge_detector.detectEdges(sharpenedImage);

	
	return decode_message.decodeFromImage(sharpenedImage, edge_pixels);

}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {

	EncodeMessage encode_message;

	ImageSharpening image_sharpening;

	ImageMatrix sharpened_image = image_sharpening.sharpen(img, sharpening_const);

	EdgeDetector edge_detector;

	std::vector<std::pair<int,int>> edge_pixels = edge_detector.detectEdges(sharpened_image);

	

	return ImageMatrix(encode_message.encodeMessageToImage(img, message, edge_pixels)); // img or sharpened img ?);
}
