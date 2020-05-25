#include<iostream>
#include<string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



int main() {
	const std::string Image_1("boy.jpg");
	cv::Mat Input_Image = cv::imread(Image_1, cv::IMREAD_COLOR);
	cv::Point Center{ 250, 125 };
	int Radius = 50;
	cv::Scalar B_Color{ 255, 0, 0 };
	cv::Scalar G_Color{ 0, 255, 0 };
	cv::Scalar R_Color{ 0, 0, 255 };
	cv::Scalar M_Color{ 255, 0, 255 };

	cv::circle(Input_Image, Center, Radius, B_Color, 5, cv::LINE_AA);
	cv::circle(Input_Image, Center, Radius+50, G_Color, 5, cv::LINE_AA);
	cv::circle(Input_Image, Center, Radius+100, R_Color, 5, cv::LINE_AA);
	cv::circle(Input_Image, Center, Radius+150, M_Color, 5, cv::LINE_AA);

	cv::imshow(Image_1, Input_Image);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}