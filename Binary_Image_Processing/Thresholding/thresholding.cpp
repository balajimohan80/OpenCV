#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>

#include<iostream>
#include<string>

#include "Timer.hpp"

template<typename Type, int C>
void thresholdUsingForLoop(const cv::Mat& src, cv::Mat& dest, int threshold, int maxval) {
#if 0
	cv::Mat_<cv::Vec<Type, C>>::const_iterator src_Begin
		= src.begin<cv::Vec<Type, C>>();
	cv::Mat_<cv::Vec<Type, C>>::const_iterator src_End
		= src.end<cv::Vec<Type, 1>>();
	cv::Mat_<cv::Vec<Type, C>>::iterator dest_Begin
		= dest.begin<cv::Vec<Type, C>>();

	while (src_Begin != src_End) {
		(*dest_Begin++)[0] = ((*src_Begin++)[0] > static_cast<Type>(threshold)) ? 
			static_cast<Type>(maxval) : 0;
	}
#else
	cv::Mat_<cv::Vec<Type, C>>::iterator dest_Begin
		= dest.begin<cv::Vec<Type, C>>();
	cv::Mat_<cv::Vec<Type, C>>::iterator dest_End
		= dest.end<cv::Vec<Type, 1>>();

	while (dest_Begin != dest_End) {
		(*dest_Begin++)[0] = ((*dest_Begin)[0] > static_cast<Type>(threshold)) ?
			static_cast<Type>(maxval) : 0;
	}
#endif
}
void thresholdingUsingForLoop(cv::Mat src, cv::Mat dst, int thresh, int maxValue) {
	int height = src.size().height;
	int width = src.size().width;

	// Loop over rows
	for (int i = 0; i < height; i++) {
		// Loop over columns
		for (int j = 0; j < width; j++) {
			if (src.at<uchar>(i, j) > thresh)
				dst.at<uchar>(i, j) = maxValue;
			else
				dst.at<uchar>(i, j) = 0;
		}
	}
}

std::string Get_Win_Name(const std::string Txt, const int Thres, const int Max) {
	std::string Ret = Txt;
	Ret += " Thrsh: " + std::to_string(Thres);
	Ret += " Max: " + std::to_string(Max);
	return Ret;
}

int main() {
	const std::string input_Image_Name = "threshold.png";
	cv::Mat src = cv::imread(input_Image_Name, cv::IMREAD_GRAYSCALE);
	cv::Mat dest = src.clone();
	const int Thresh_Val = 100;
	const int Max_Val = 255;
	cv::imshow(input_Image_Name, src);
	{
		Timer T("TimeTaken to apply Threshold using at<>");
		thresholdingUsingForLoop(src, dest, Thresh_Val, Max_Val);
	}
	cv::imshow("DestImage", dest);
	dest = src.clone();
	{
		Timer T("TimeTaken to apply Threshold using Iterator");
		thresholdUsingForLoop<unsigned char, 1>(src, dest, Thresh_Val, Max_Val);
	}
	dest = src.clone();
	{
		Timer T("TimeTaken to apply Threshold using cv::Threshold");
		cv::Mat dest_local;
		std::string Win_Name = Get_Win_Name("THRESH_BINARY", Thresh_Val, Max_Val);
		/* Threshold value set to 100 and Max Value set to 225.
		   Threshold Binary set to 255, if any pixel value greater than 100
		   It sets to Zero, if any pixel value lesser than 100
		   It acts like a High Pass filter, means any value greater than 100 will set to 255
		*/
		cv::threshold(src, dest_local, Thresh_Val, Max_Val, cv::THRESH_BINARY);
		cv::imshow(Win_Name, dest_local);
	}
	
	cv::Mat Inverse_Threshold;
	std::string Win_Inv_Thrs = Get_Win_Name("THRESH_BINARY_INV", Thresh_Val, Max_Val);
	/* Threshold binary inverse will set to 255 only if i/p pixel value is lesserthan 100
	   It set to Zero, if i/p pixel value is greater than 100.
	   It acts like a Low Pass filter, means any value lesser-than 100 will set to 255
	*/
	cv::threshold(src, Inverse_Threshold, Thresh_Val, Max_Val, cv::THRESH_BINARY_INV);
	cv::imshow(Win_Inv_Thrs, Inverse_Threshold);
	
	cv::Mat Truncate_Threshold;
	std::string Win_Trunc_Thrs = Get_Win_Name("THRESH_TRUNC", Thresh_Val, Max_Val);
	/* Threshold Truncate will set to 100 only if i/p pixel val is greater than 100.
	   Any I/P pixel value lesser than 100 will pass it.
	   Basically it saturates a pixel value greater than 100.
	*/
	cv::threshold(src, Truncate_Threshold, Thresh_Val, Max_Val, cv::THRESH_TRUNC);
	cv::imshow(Win_Trunc_Thrs, Truncate_Threshold);

	cv::Mat Zero_Threshold;
	std::string Win_Zero_Thrs = Get_Win_Name("THRESH_TOZERO", Thresh_Val, Max_Val);
	/* Threshold Zero will set to zero, only if I/P Pix val lesser than 100.
	   It set the same I/P Pix val, only if I/P PIX val is greater than 100.
	   Basically it act as a Band-Pass filter, where any I/P pix value is greater than
	   100 will pass and I/P Pix val is lesserthan 100 set to zero.
	*/
	cv::threshold(src, Zero_Threshold, Thresh_Val, Max_Val, cv::THRESH_TOZERO);
	cv::imshow(Win_Zero_Thrs, Zero_Threshold);

	cv::Mat Inv_Zero_Threshold;
	std::string Win_Inv_Zero_Thrs = Get_Win_Name("THRESH_TOZERO_INV", Thresh_Val, Max_Val);
	/* Inv Threshold zero will pass I/P Pix val is lesser than 100.
	   It set to zero, any I/P Pix val is greater than 100.
	*/
	cv::threshold(src, Inv_Zero_Threshold, Thresh_Val, Max_Val, cv::THRESH_TOZERO_INV);
	cv::imshow(Win_Inv_Zero_Thrs, Inv_Zero_Threshold);


	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}