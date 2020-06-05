#include<opencv2/opencv.hpp>

#include<iostream>
#include<string>

int main() {
	cv::VideoCapture Web_Cam_Cap(0);
	if (!Web_Cam_Cap.isOpened()) {
		Web_Cam_Cap.release();
		std::cout << "Web Camera is not opened...\n";
	}

	/* FOURCC - It is a short for "Four Character Code" 
	   - An Identifier for Video Codec.
	   - Compressior Format
	   - Color or Pixel format used in MEDIA files
	   For Example, fourcc('P','I', 'M', 1') is MPEG-1 codec
	   fourCC('M', 'J', 'P', 'G') is Motion-JPEG code.
	   To get more information, please refer following link:
	   http://fourcc.org/fourcc.php
	*/
	const std::string Video_File_Name("Output_Camera.mp4");
	int FourCC = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
	double Frame_Per_Sec = 10;
	cv::Size Frame_Size(Web_Cam_Cap.get(cv::CAP_PROP_FRAME_WIDTH), 
		Web_Cam_Cap.get(cv::CAP_PROP_FRAME_HEIGHT));

	cv::VideoWriter Video_Write(Video_File_Name, FourCC, Frame_Per_Sec, Frame_Size);
	for (int i = 0; i < 20; i++) {
		cv::Mat Frame;
		Web_Cam_Cap >> Frame;
		if (Frame.empty()) break;
		Video_Write.write(Frame);
		cv::imshow("Camera Frame", Frame);
		cv::waitKey(100);
	}
	Web_Cam_Cap.release();
	Video_Write.release();

	//Let's read a file what we have captured on last write
	cv::VideoCapture Read_Captured_Video(Video_File_Name);
	if (!Read_Captured_Video.isOpened()) {
		std::cout << "Not able to open Captured Frame";
		Read_Captured_Video.release();
		return 0;
	}

	while (1) {
		cv::Mat Frame;
		Read_Captured_Video >> Frame;
		if (Frame.empty()) {
			std::cout << "Frame becomes End...";
			break;
		}
		cv::imshow("Recorded Frame", Frame);
		cv::waitKey(1000);
	}
	Read_Captured_Video.release();
	return 0;
}