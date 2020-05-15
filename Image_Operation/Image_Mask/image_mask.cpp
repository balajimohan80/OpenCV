#include<iostream>
#include<string>

#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


//Convert Image Size into String
std::string Image_Size_To_Str(const cv::Mat& Image) {
	std::string Ret;
	Ret += "W: ";
	Ret += std::to_string(Image.size().width);
	Ret += " H: ";
	Ret += std::to_string(Image.size().height);
	return Ret;
}

int main() {
	cv::Mat Image_Original = cv::imread("boy.jpg/boy.jpg", 1);
	const std::string Orig_Name = "I/P Image: " + Image_Size_To_Str(Image_Original);
	cv::namedWindow(Orig_Name);
	cv::imshow(Orig_Name, Image_Original);

	cv::Mat Mask1 = cv::Mat::zeros(Image_Original.size(), Image_Original.type());
	const std::string Image_Name_1 = "Mask1";
	cv::namedWindow(Image_Name_1);
	cv::imshow(Image_Name_1, Mask1);
	
	//Y1, Y2     X1, X2
	Mask1(cv::Range(50, 200), cv::Range(170, 320)).setTo(255);
	const std::string Image_Name_2 = "FaceMask";
	cv::namedWindow(Image_Name_2);
	cv::imshow(Image_Name_2, Mask1);

	//Creating a Mask using Pixel intensity or Color
	//Let try to find out the pixels in RED colors.
	// To Idnetify RED Color pixel in BGR 3 channels
	//Generally RED Pixels have high intensity.
	//RED Channel in the range of (100 to 255)
	//BLUE & GREEN Channel range are in between (0 to 100)
	//EX BGR: 24,25, 200 or 1,10, 180
	cv::Mat Mask2;

	/* cv::inRange produces a binary image (Binary Image: White & Black Pixels)
	   White Pixels corresponds to those pixels in I/P Image which fall in specified range.
	   Black Pixels corresponds to outside of specified range.
		
	   cv::inRange:
	   1. src: Input Image
	   2. lowerb: lower boundary array or scalar
	   3. upperb: upper boundary array or scalar
	   4. dst   : O/P Image
	*/
	//Scalar value for BGR colorspace Range: 0, 0, 150  to 100, 100, 255 
	cv::inRange(Image_Original, cv::Scalar(0, 0, 150), cv::Scalar(100, 100, 255), Mask2);
	const std::string Image_Name_3 = "Masked " + Image_Size_To_Str(Mask2);
	cv::namedWindow(Image_Name_3);
	cv::imshow(Image_Name_3, Mask2);

	//Lets try to mask Skin Color
	//BGR Color Space are in the range 100, 100, 150 to 150, 150, 225
	cv::Mat SkinMask;
	cv::inRange(Image_Original, cv::Scalar(100, 100, 150), cv::Scalar(150, 150, 225), SkinMask);
	const std::string Image_Name_4 = "SkinMask";
	cv::namedWindow(Image_Name_4);
	cv::imshow(Image_Name_4, SkinMask);


	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}