#include<opencv2/opencv.hpp>

#include<iostream>
#include<string>

void Display_Frame_Param(const cv::VideoCapture& Cap) {
	std::cout << "Width of the Frame       : " << Cap.get(cv::CAP_PROP_FRAME_WIDTH) << "\n";
	std::cout << "Height of the Frame      : " << Cap.get(cv::CAP_PROP_FRAME_HEIGHT) << "\n";
	std::cout << "FPS of the Video         : " << Cap.get(cv::CAP_PROP_FPS) << "\n";
	std::cout << "Four Character of CODECS : " << Cap.get(cv::CAP_PROP_FOURCC) << "\n";
}

int main() {
	//std::string Video_File_Name = {"chaplin.mp4"};
	//cv::VideoCapture Cap(Video_File_Name);
	
	//Video Camera rather than recording camera
	cv::VideoCapture Cap(0);
	if (!Cap.isOpened()) {
		std::cout << "Video file is not opened, please check filename" << "\n";
		return 0;
	}
	//Getting the Video Properties
	int Width_Of_The_Frame  = Cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int Height_Of_The_Frame = Cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	Display_Frame_Param(Cap);
	while (Cap.isOpened()) {
		cv::Mat Frame; 
		if (!Cap.read(Frame)) break;
		cv::imshow("Frame", Frame);
		cv::waitKey(100);
		//Let increase the Height & Width size
		// First I have tried to increase the frame size, it exit the because frame 
		// is empty
		/*
		Width_Of_The_Frame += 10;
		Height_Of_The_Frame += 10;
		*/

		/*
		We tried to decrease the frame size, but that also not working
		Width_Of_The_Frame -= 10;
		Height_Of_The_Frame -= 10;
		*/
		Cap.set(cv::CAP_PROP_FRAME_WIDTH, Width_Of_The_Frame);
		Cap.set(cv::CAP_PROP_FRAME_WIDTH, Height_Of_The_Frame);
		std::cout << "--------------------------------------------------------\n";
		Display_Frame_Param(Cap);
	}
	cv::destroyAllWindows();
	return 0;
}