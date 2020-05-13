#include<iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main() {
	cv::Mat Image = cv::imread("boy.jpg/boy.jpg", 1);
	cv::namedWindow("Boy Window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Boy Window", Image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}