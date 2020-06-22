#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

#include<iostream>
#include<string>

void dilation() {
	std::string image_Name = "dilation_example.jpg";
	cv::Mat input_Image = cv::imread(image_Name, cv::IMREAD_COLOR);
	/* This image consists of 4 coins, there are some black spots in the coins.
	   This could be issue to algorithm to detect coins.
	   Remove those black spots and final image consists of 4 white blobs.

	*/
	cv::imshow(image_Name, input_Image);
	/* Apply dilation operation to grow the white region in an Image
	   Two ways of performing dilation:
	   1. Using a single big kernel
	   2. Using muliple iteration of a smaller kernel.
	*/

	//1. Using a single big kernel
	int bigKSize = 7;
	cv::Mat bigkernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(bigKSize, bigKSize));
	//cv::imshow("SingleBigKernel", bigkernel);

	/* dilate:
	   src = Input Image, no of channels are arbitary. But depth should be
			 CV_8U, cv_16U, CV_16S, CV_32F or CV_64F
	   dst = O/P Image of the same size and type as src
	   kernel = Structuring element used for dilation.
	   anchor = Position of the anchor within the element. Def: (-1, -1) means that the anchor is
				at the element center.
	   iterations = number of iteration dilation applied. (Def: 1)
	   borderType = Pixel extrapolation method.
	   borderValue = border value in case of constant border.
	*/
	cv::Mat dilated_Image;
	cv::dilate(input_Image, dilated_Image, bigkernel);
	cv::imshow("DilatedImage-Bigkernel", dilated_Image);

	//2. Using muliple iteration of a smaller kernel.
	int smallKSize = 3;
	cv::Mat smallKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(smallKSize, smallKSize));
	cv::Mat dilatedImage_1, dilatedImage_2;
	cv::dilate(input_Image, dilatedImage_1, smallKernel, cv::Point(-1, -1), 1);
	cv::dilate(input_Image, dilatedImage_2, smallKernel, cv::Point(-1, -1), 2);

	cv::imshow("DilatedImage-Smallkernel-SingleIteration", dilatedImage_1);
	cv::imshow("DilatedImage-Smallkernel-DoubleIteration", dilatedImage_2);


	cv::waitKey(0);
	cv::destroyAllWindows();
}

void erosion() {
	std::string input_Name = "erosion_example.jpg";
	cv::Mat input_Image = cv::imread(input_Name, cv::IMREAD_COLOR);
	/* Input image has coins, but it has some white spots.
	   We need to remove this white spots in the coin and convert fully black coins.
	   Erosion operation can be used to grow the black region in an image.
	*/
	cv::imshow(input_Name, input_Image);
	/* Apply erosion operation to grow the black region in an Image
	   Two ways of performing dilation:
	   1. Using a single big kernel
	   2. Using muliple iteration of a smaller kernel.
	*/
	//1. Using a single big kernel
	int bigKSize = 7;
	cv::Mat bigkernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(bigKSize, bigKSize));
	//cv::imshow("SingleBigKernel", bigkernel);

	/* dilate:
	   src = Input Image, no of channels are arbitary. But depth should be
			 CV_8U, cv_16U, CV_16S, CV_32F or CV_64F
	   dst = O/P Image of the same size and type as src
	   kernel = Structuring element used for dilation.
	   anchor = Position of the anchor within the element. Def: (-1, -1) means that the anchor is
				at the element center.
	   iterations = number of iteration dilation applied. (Def: 1)
	   borderType = Pixel extrapolation method.
	   borderValue = border value in case of constant border.
	*/
	cv::Mat erosion_Image;
	cv::erode(input_Image, erosion_Image, bigkernel);
	cv::imshow("ErosionImage-Bigkernel", erosion_Image);

	//2. Using muliple iteration of a smaller kernel.
	int smallKSize = 3;
	cv::Mat smallKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(smallKSize, smallKSize));
	cv::Mat erosionImage_1, erosionImage_2;
	cv::erode(input_Image, erosionImage_1, smallKernel, cv::Point(-1, -1), 1);
	cv::erode(input_Image, erosionImage_2, smallKernel, cv::Point(-1, -1), 2);

	cv::imshow("ErosionImage-Smallkernel-SingleIteration", erosionImage_1);
	cv::imshow("ErosionImage-Smallkernel-DoubleIteration", erosionImage_2);

	cv::waitKey(0);
	cv::destroyAllWindows();
}


int main() {
	//dilation();
	erosion();
	return 0;
}