#include<iostream>
#include<string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main() {
	const std::string Image_Name("boy.jpg");
	cv::Mat Image = cv::imread(Image_Name, cv::IMREAD_COLOR);
	cv::Point Center{250, 125};
	cv::Size Axes{ 100, 50 }; /* Generally half of the size of center*/
	double Angle_1 = 0; /* Rotation angle in degress */
	double Angle_2 = 90;
	double Start_Angle = 0;
	double End_Angle = 90;
	cv::Scalar B_Color = { 255, 0, 0 };
	cv::Scalar G_Color = { 0, 255, 0 };

	int Thickness = 1;

	cv::namedWindow(Image_Name);
	for (int i = 0; i < 4; i++) {
		cv::ellipse(Image, Center, Axes, Angle_1, Start_Angle, End_Angle,
			B_Color, 3, cv::LINE_AA);
		cv::ellipse(Image, Center, Axes, Angle_2, Start_Angle, End_Angle,
			G_Color, 3, cv::LINE_AA);

		End_Angle += 90;
		cv::imshow(Image_Name, Image);
		cv::waitKey(0);
	}
	cv::destroyAllWindows();
	return 0;
}