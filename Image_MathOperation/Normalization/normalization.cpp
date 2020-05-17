#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main() {
	unsigned char Data[] = { 100, 110, 120, 130 };
	cv::Mat Input_Image(2, 2, CV_8UC1, Data);
	std::cout << Input_Image << "\n";
	//Observe that a[1,1] should added to 160 (130 + 130).
	//But it rather set to 255.
	std::cout << Input_Image + 130 << "\n";

	//This can be resolved by
	int dataInt[] = { 100,110,120,130 };
	cv::Mat b(2, 2, CV_32SC1, dataInt);
	b = b + 130;
	std::cout << b << "\n";
	//bring it back to uint8 in the final stage
	b.convertTo(b, CV_8UC1);
	std::cout << b << "\n";

	// Convert to normalized float32/float64

	//It is a good practice to convert the uint8 to float and normalize the range to 
	//[0,1] and change it back to [0,255] after doing all arithmetic operations
	
	cv::Mat a_float32;
	Input_Image.convertTo(a_float32, CV_32F, 1.0 / 255.0, 0);
	a_float32 = a_float32 + 130.0 / 255.0;
	std::cout << a_float32 << "\n";

	cv::Mat c;
	a_float32.convertTo(c, CV_32F, 255.0, 0.0);
	std::cout << c << "\n";

	// Clipped output
	cv::Mat b_uint8;
	c.convertTo(b_uint8, CV_8UC1);
	std::cout << b_uint8;

	return 0;
}