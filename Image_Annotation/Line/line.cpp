#include<iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

void Draw_Rectangle_Using_Line(cv::Mat& Image,
	cv::Point Pt1, cv::Point Pt2) {
	cv::Scalar B_Color = { 255, 0, 0 };
	cv::Scalar G_Color = { 0, 255, 0 };
	cv::Scalar R_Color = { 0, 0, 255 };
	cv::Scalar M_Color = { 255, 0, 255 };
	cv::Point Pt_A = Pt1;
	cv::Point Pt_B = { Pt2.x, Pt1.y };
	cv::Point Pt_C = Pt2;
	cv::Point Pt_D = { Pt_A.x, Pt_C.y };
	cv::Point Pt_E = Pt1;

	/* Syntax of Line:
		1st Parameter : Input Image
		2nd Parameter : Starting point Line of XY-Coordinates
		3rd Parameter : End point Line of XY-Coordinates
		4th Parameter : Color of the line
		5th Parameter : Thickness of the line
		6th Parameter : Type of the line 
		                Example: LINE_8 stands for 8 Connected Line
						LINE_AA is Anti-aliased Line
	*/

	cv::line(Image, Pt_A, Pt_B, B_Color, 3, cv::LINE_AA);
	cv::line(Image, Pt_B, Pt_C, G_Color, 3, cv::LINE_AA);
	cv::line(Image, Pt_C, Pt_D, R_Color, 3, cv::LINE_AA);
	cv::line(Image, Pt_D, Pt_E, M_Color, 3, cv::LINE_AA);

	return;
}

int main() {
	const std::string B_Image = "boy.jpg";
	cv::Mat Input_Image = cv::imread(B_Image, cv::IMREAD_COLOR);
	
	Draw_Rectangle_Using_Line(Input_Image, cv::Point{ 200, 80 }, cv::Point{280, 180});
	cv::imshow(B_Image, Input_Image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}