#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include<iostream>

/* Opening refers Erosion followed by Dilation and these operations is used for clearing WHITE blobs
   Closing refers Dilation followed by Erosion and these operations is used for clearing Black holes
   In OpenCV, the opening & closing operations are implemented using MorphologyEx function.

   OpenCV Function:
   Opening:
   void morphologyEx(Mat initialImage, Mat imageMorphOpened, MORPH_OPEN, Mat structuringElement)

   Closing:
   void morphologyEx(Mat initialImage, Mat imageMorphClosed, MORPH_CLOSE, Mat structuringElement)

   Parameters:
   src     - Source Image, channels are arbitary. But depth should be one of CV_8U, CV_16U, CV_16S, CV_32F or CV_64F
   dst     - Destination image of the same size and type as source image.
   op      - Type of Morphological operation
   kernel  - Structuring element. It can be created using getStructuringElement.
   anchor  - Anchor position with the kernel. Negative values mean that the anchor is at the kernel center.
   iterations - Number of times erosion and dilation are applied.
   borderType - Pixel extrapolation method.
   borderValue - Border value in case of a constant border. The default value has a special meaning.
 
   We will discuss different methods for performing Opening and Closing operations :
   1. Using Dilation and Erosion Operations
   2. Using MorphologyEx function

 */


int main() {
	std::string image_Name = "opening.png";
	cv::Mat input_Image = cv::imread(image_Name, cv::IMREAD_GRAYSCALE);
	cv::imshow(image_Name, input_Image);
	
	//Opening : Method 1 - Using combination of Erosion and Dilation
	int kernelSize = 10;
	cv::Mat Element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		cv::Point(kernelSize, kernelSize));
	cv::Mat imEroded;
	cv::erode(input_Image, imEroded, Element, cv::Point(-1, -1), 1);
	
	cv::Mat imOpen;
	cv::dilate(imEroded, imOpen, Element, cv::Point(-1, -1), 1);
	cv::imshow("Image_Manual_Open",imOpen);
	//Opening : Method 2 - Using MorphologyEx function 
	//Get structuring Element/kernel which will be used for opening operation
	int openingSize = 3;

	Element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * openingSize + 1, 2 * openingSize + 1),
		cv::Point(openingSize, openingSize));
	//Applying erosion followed by dilation on image by using morphologyEx function and specifying MORPH_OPEN.
	cv::Mat imageMorphOpened;
	cv::morphologyEx(input_Image, imageMorphOpened, cv::MORPH_OPEN, Element, cv::Point(-1, -1), 3);
	cv::imshow("Image_Morph_Opened", imageMorphOpened);

	//------------------------------Closing Operation---------------------
	std::string closing_Image_Name = "closing.png";
	cv::Mat Closing_Image = cv::imread(closing_Image_Name, cv::IMREAD_GRAYSCALE);
	cv::imshow(closing_Image_Name, Closing_Image);
	
	//Closing: Method 1 - Using combination of Dilation and Erosion
	Element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		cv::Point(kernelSize, kernelSize));
	cv::Mat imDilated;
	cv::dilate(Closing_Image, imDilated, Element);
	
	cv::Mat imClose;
	cv::erode(imDilated, imClose, Element);
	cv::imshow("Image_Manula_Close", imClose);

	//Closing : Method 2 - Using MorphologyEx function
	int closingSize = 10;
	// Selecting a elliptical kernel and storing in Mat element
	Element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * closingSize + 1, 2 * closingSize + 1),
		cv::Point(closingSize, closingSize));
	cv::Mat imageMorphClosed;
	cv::morphologyEx(Closing_Image, imageMorphClosed, cv::MORPH_CLOSE, Element);
	cv::imshow("Image_Morph_Closed", imageMorphClosed);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}



