#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

const std::string Window_Name   = "Resize_Image";
const std::string TrackbarValue = "Scale";
const std::string TrackbarType  = "Type: \n 0: Scale Up \n 1: Scale Down";
int Max_ScaleUp  = 100;
int Scale_Factor = 2;
int Scale_Type   = 0;
int Max_Type     = 1;
bool Scale_Up    = 1;

void Scale_Up_Down(void* user_Data) {
	double scaleFactorDouble;
	if (Scale_Up)
		scaleFactorDouble = 1 + Scale_Factor / 100.0;
	else
		scaleFactorDouble = 1 - Scale_Factor / 100.0;

	if (scaleFactorDouble == 0) scaleFactorDouble = 1;
	cv::Mat scaledImage;
	cv::Mat* inImage = static_cast<cv::Mat*>(user_Data);
	cv::resize(*inImage, scaledImage, cv::Size(), scaleFactorDouble,
		scaleFactorDouble, cv::INTER_LINEAR);
	cv::imshow(Window_Name, scaledImage);
}

void trackBar_ScaleImage(int pos, void* userData) {
	Scale_Up_Down(userData);
}

void trackBar_Type(int pos, void* userData) {
	Scale_Up = (pos == 1) ? 0 : 1;
	Scale_Up_Down(userData);
}

int main() {
	const std::string Image_Name = "Truth.png";
	cv::Mat Input_Image = cv::imread(Image_Name);
	cv::namedWindow(Window_Name, cv::WINDOW_AUTOSIZE);

	cv::TrackbarCallback track_Callback_ScaleUp = trackBar_ScaleImage;
	cv::TrackbarCallback track_Callback_Type = trackBar_Type;
	void* user_Data = static_cast<void*>(&Input_Image);

	cv::createTrackbar(TrackbarValue, Window_Name, &Scale_Factor, Max_ScaleUp, 
		track_Callback_ScaleUp, user_Data);

	cv::createTrackbar(TrackbarType, Window_Name, &Scale_Type, Max_Type, 
		trackBar_Type, user_Data);

	trackBar_ScaleImage(25, user_Data);
	while (true) {
		int c;
		c = cv::waitKey(20);
		if (static_cast<char>(c) == 27) break;
	}
	cv::destroyAllWindows();
	return 0;
}