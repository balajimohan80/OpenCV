#include<iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main() {
	cv::Mat Orig_Image = cv::imread("boy.jpg/boy.jpg");
	const std::string Name_1 = "Original_Image";
	cv::namedWindow(Name_1);
	cv::imshow(Name_1, Orig_Image);

	double Scaling_Factor = static_cast<double>(1 / 255.0);
	double Shift = 0;

	//Converting From Unsigned Char to Float(32bit)
	// The Scaling Factor reduce the range of 0-255 to 0-1.
	//Hence all the floating point numbers are between 0-1.
	cv::Mat Float_Image;
	Orig_Image.convertTo(Float_Image, CV_32FC3, Scaling_Factor, Shift);
	const std::string Name_2 = "Floating Point";
	cv::namedWindow(Name_2);
	cv::imshow(Name_2, Float_Image);
	
	//Converting Float to U_int8 
	double Reciprocal = static_cast<double>(1.0 / Scaling_Factor);
	cv::Mat U_INT8_Image;
	Float_Image.convertTo(U_INT8_Image, CV_8UC3, Reciprocal, Shift);
	std::string Name_3 = "Float to CV_8UC3";
	cv::namedWindow(Name_3);
	cv::imshow(Name_3, U_INT8_Image);
	
	//Contrast is the difference in Intensity between brightest and darkest regions
	//Higher the difference, higher is the contrast
	//Maximum Contrast of an Image is known as Dynamic Range.

	//To improve contrast of an image is Intensity Scaling
	//Scale Factor(K) is multiplied with Intensity Values of all Pixels.
	//Intensity Scaling is represented by:
	// I = K.I
	double ConstrastPercentage = 30.0;
	int i = 0;
	while (i < 10) {
		cv::Mat Contrast_High;
		double Scaling_Factor_1 = (1 + ConstrastPercentage / 100.0);
		Orig_Image.convertTo(Contrast_High, CV_32FC3, 1.0, Shift);
		std::string Name_4 = "Contrast_High";
		Name_4 += std::to_string(ConstrastPercentage);
		cv::namedWindow(Name_4);
		cv::imshow(Name_4, Contrast_High);
		cv::waitKey(0);
		ConstrastPercentage = ConstrastPercentage + 1.0;
		i++;
	}
	cv::destroyAllWindows();

	return 0;
}