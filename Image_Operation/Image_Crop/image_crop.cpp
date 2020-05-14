#include<iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main() {
	cv::Mat Image = cv::imread("boy.jpg/boy.jpg", 1);
	cv::namedWindow("Boy Window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Boy Window", Image);
	// Now Crop an Image.
	// Boy Face: 
	// X Co-Ordinates = 180 to 310
	// Y Co-Ordinates = 40 to 200
	/*
	Y1(40)           
	--------------------
	|                  |
	|                  |
    |                  |
	|                  |
    |                  |
	--------------------
    Y2(200)
	X1(180)---------X2(310)
	*/
	const int X1 = 180;
	const int X2 = 310;
	const int Y1 = 40;
	const int Y2 = 200;
	// Cropping also be called Region Of Interest(ROI)
	cv::Mat Crop = Image(cv::Range(Y1, Y2), cv::Range(X1, X2)); 
	const std::string Image_Name = "Cropped Image";
	cv::namedWindow(Image_Name);
	cv::imshow(Image_Name, Crop);
	cv::waitKey(0);
	//After Cropping an Image, copying a Cropped image into 
	// an another Image or Paste it to another Image by
	// Choosing where you want to paste it.
	cv::Mat Image_Clone = Image.clone(); //copying from Original Image
	//Find Height and Width of ROI
	const int ROI_Height = Crop.size().height;
	const int ROI_Width  = Crop.size().width;
	//Copy to Left of Face
	Crop.copyTo(Image_Clone(cv::Range(40, 40 + ROI_Height), cv::Range(10, 10 + ROI_Width)));
	//Copy to Right of Face
	Crop.copyTo(Image_Clone(cv::Range(40, 40 + ROI_Height), cv::Range(330, 330 + ROI_Width)));
	const std::string Image_Copy = "Image_Copy";
	cv::namedWindow(Image_Copy);
	cv::imshow(Image_Copy, Image_Clone);
	cv::waitKey(0);

	cv::destroyAllWindows();
	return 0;
}