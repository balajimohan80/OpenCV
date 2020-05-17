#include<iostream>
#include<vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main() {
	//Brightness is a measure of Light Falling on the scene.
	//In RGB color Space, it can be thought of as the arthimetic mean of 
	//R,G & B Color Values. 
	//To make an Image brighter, the intensity values should be 
	//increased by offset(X) and viceversa

	// Out_Image = Image_Input + X
	int Brightness_Offset = 50;
	cv::Mat Input_Image = cv::imread("boy.jpg/boy.jpg");
	const std::string Name_1 = "Input_Image";
	cv::namedWindow(Name_1);
	cv::imshow(Name_1, Input_Image);

	//std::vector<cv::Mat> Bright_Image(3);
	int i = 0;
	while (i < 10) {
		cv::Mat Bright_Image[3];
		cv::split(Input_Image, Bright_Image);

		for (auto& Ch : Bright_Image) {
			cv::add(Ch, Brightness_Offset, Ch);
		}
		cv::Mat Out = Input_Image.clone();
		cv::merge(Bright_Image, 3, Out);
		const std::string Name_2 = "Bright_Image";
		cv::namedWindow(Name_2);
		cv::imshow(Name_2, Out);

		cv::waitKey(0);
		Brightness_Offset = Brightness_Offset + 50;
		i++;
	}
	cv::destroyAllWindows();
	return 0;
}