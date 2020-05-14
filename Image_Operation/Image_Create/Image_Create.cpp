#include<iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main() {
	std::cout << "Creating an Empty Matrix\n";
	//Scalar value zero means Pure-Black
	cv::Mat EmptyMatrix = cv::Mat(100, 200, CV_8UC3, cv::Scalar(0, 0, 0));
	const std::string W_Name = "EmptyMatrix";
	cv::namedWindow(W_Name, 1);
	cv::imshow(W_Name, EmptyMatrix);
	cv::waitKey(0);
	//Change all Pixels color to pure-white, by setting all pixel value to 255
	EmptyMatrix.setTo(cv::Scalar(255, 255, 255));
	cv::imshow(W_Name, EmptyMatrix);
	cv::waitKey(0);
	//Change all Pixels color to 127
	EmptyMatrix.setTo(cv::Scalar(127, 127, 127));
	cv::imshow(W_Name, EmptyMatrix);
	cv::waitKey(0);
	cv::destroyAllWindows();
	std::cout << "Exit...\n";
	return 0;
}