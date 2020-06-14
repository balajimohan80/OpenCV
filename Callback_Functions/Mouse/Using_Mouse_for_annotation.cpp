#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<vector>
#include<iostream>
#include<string>
#include<math.h>

cv::Point Center, Circumference;
cv::Mat Source;
const std::string S = "Window";
std::string Str = S;


int Copy_Src_To_Dest_Size(cv::Mat& Src, cv::Mat& Dest, cv::Rect ROI, bool Adjust_Src=1) {
	//if (Dest.size() != ROI.size()) {
	//	return -1;
	//}

	cv::Mat_<cv::Vec<unsigned char, 3>>::iterator Src_Begin =
		Src.begin<cv::Vec<unsigned char, 3>>();
	
	cv::Mat_<cv::Vec<unsigned char, 3>>::iterator Dest_Begin =
		Dest.begin<cv::Vec<unsigned char, 3>>();
	cv::Mat_<cv::Vec<unsigned char, 3>>::iterator Dest_End =
		Dest.end<cv::Vec<unsigned char, 3>>();
	if (Adjust_Src) {
		Src_Begin += ((Src.size().width * ROI.x) + (Src.size().height * ROI.y));
	}
	else {
		Dest_Begin += ((Dest.size().width * ROI.x) + (Dest.size().height * ROI.y));
	}
	while (Dest_Begin != Dest_End) {
		*Dest_Begin++ = *Src_Begin++;
	}
	return 0;
}
/*
@param event one of the cv::MouseEventTypes constants.
@param x The x - coordinate of the mouse event.
@param y The y - coordinate of the mouse event.
@param flags one of the cv::MouseEventFlags constants.
@param userdata The optional parameter.
*/
void Draw_Circle(int event, int x, int y, int flags, void* userdata) {
	if (event == cv::MouseEventTypes::EVENT_MOUSEMOVE) {
		std::string Coordiantes = "X: " + std::to_string(x);
		Coordiantes += " Y: ";
		Coordiantes += std::to_string(y);
		cv::Point PT1(0, Source.size().height-50);
		cv::Rect Text_ROI(PT1, cv::Size (Source.size().width, 50));
		cv::Mat *Clone_Data = (cv::Mat *)userdata;
		cv::Mat Copy_Data_From_Clone(cv::Size(Source.size().width, 50), CV_8UC3);
		Copy_Src_To_Dest_Size(*Clone_Data, Copy_Data_From_Clone, Text_ROI);
		cv::putText(Copy_Data_From_Clone, Coordiantes, cv::Point(0, 25), cv::FONT_HERSHEY_SIMPLEX, 1,
			cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
		Copy_Src_To_Dest_Size(Copy_Data_From_Clone, Source, Text_ROI, 0);
	
	}
	else if (event == cv::MouseEventTypes::EVENT_LBUTTONDOWN) {
		Center = cv::Point(x, y);
		const cv::Scalar G_DOT(255, 255, 0);
		const int Radius = 1;
		cv::circle(Source, Center, Radius, G_DOT, 2, cv::LINE_AA);
	} 
	else if (event == cv::MouseEventTypes::EVENT_LBUTTONUP ) {
		Circumference = cv::Point(x, y);
		//Compute Radius of Circle
		auto Pow_x = std::pow(Center.x - Circumference.x, 2);
		auto Pow_y = std::pow(Center.y - Circumference.y, 2);
		std::cout << "X: " << Pow_x << " Y: " << Pow_y << "\n";
		const int Radius = std::sqrt(Pow_x + Pow_y);
		std::cout << "Rad: " << Radius << "\n";
		const cv::Scalar G_Color(0, 255, 0);
		cv::circle(Source, Center, Radius, G_Color, 2, cv::LINE_AA);
		cv::imshow(Str, Source);
	}
}

int main() {
	cv::MouseCallback CallBack_Func = Draw_Circle;
	Source = cv::imread("sample.jpg", 1);
	cv::Mat Image_Clone = Source.clone();
	cv::namedWindow(Str);
	void* User_Data = static_cast<void*>(&Image_Clone);
	cv::setMouseCallback(Str, CallBack_Func, User_Data);
	int K = 0;
	while (K != 27) {
		const std::string Text("Choose Center, and dragm Press ESC to exit and C to Clear");
		const cv::Point Pt(10, 30);
		const cv::HersheyFonts Font = cv::FONT_HERSHEY_SIMPLEX;
		const float Font_Size = 0.7;
		const cv::Scalar W_Color(255, 255, 255);
		cv::imshow(Str, Source);
		cv::putText(Source, Text, Pt, Font, 0.7, W_Color, 2, cv::LINE_AA);
		K = cv::waitKey(20) & 0xFF;
	
		if (K == 99)
			Image_Clone.copyTo(Source);
	}

	return 0;
}