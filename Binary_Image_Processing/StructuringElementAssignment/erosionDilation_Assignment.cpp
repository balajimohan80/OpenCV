#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>

#include<iostream>
#include<string>

int main() {
	cv::Mat demoImage = cv::Mat::zeros(cv::Size(10, 10), CV_8U);
	
	//Let's add some white blob
	demoImage.at<uchar>(0, 1) = 1;
	demoImage.at<uchar>(9, 0) = 1;
	demoImage.at<uchar>(8, 9) = 1;
	demoImage.at<uchar>(2, 2) = 1;
	demoImage(cv::Range(5, 8), cv::Range(5, 8)).setTo(1);
	std::cout << demoImage << "\n";

	cv::imshow("BLOB", demoImage * 255);
	
	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	std::cout << element << "\n";
	const int kSize = element.size().height;
	const int height = demoImage.size().height;
	const int width = demoImage.size().width;

	cv::Mat dilatedEllipseKernel;
	cv::dilate(demoImage, dilatedEllipseKernel, element);
	std::cout << dilatedEllipseKernel << "\n";
	
	//Scale UP
	//dilatedEllipseKernel.convertTo(dilatedEllipseKernel, CV_32FC1);
	cv::resize(dilatedEllipseKernel, dilatedEllipseKernel, cv::Size(50, 50));
	//dilatedEllipseKernel.convertTo(dilatedEllipseKernel, CV_8UC3);
	dilatedEllipseKernel = dilatedEllipseKernel * 255;
	cv::cvtColor(dilatedEllipseKernel, dilatedEllipseKernel, cv::COLOR_GRAY2BGR);
	//dilatedEllipseKernel.convertTo(dilatedEllipseKernel, CV_8UC1);
	cv::imshow("DilatedImage", dilatedEllipseKernel);
	//cv::waitKey(0);

	//Create a videoWrite Object 
	const std::string video_Dilate_F_Name("dilationScratch.avi");
	const int fourCC = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
	double frames_Per_Sec = 10;
	cv::Size frame_Size(50, 50);
	cv::VideoWriter v_Dilate_Writer_Obj(video_Dilate_F_Name, fourCC, frames_Per_Sec, frame_Size);
	v_Dilate_Writer_Obj.set(cv::CAP_PROP_FRAME_WIDTH, 320);
	v_Dilate_Writer_Obj.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
	std::cout << "Frame Width  : " << v_Dilate_Writer_Obj.get(cv::CAP_PROP_FRAME_WIDTH) << "\n";
	std::cout << "Frame Height : " << v_Dilate_Writer_Obj.get(cv::CAP_PROP_FRAME_HEIGHT) << "\n";

	const int border = kSize / 2;
	cv::Mat paddedDemoImage = cv::Mat::zeros(cv::Size(height + border * 2, width + border * 2), CV_8UC1);
	copyMakeBorder(demoImage, paddedDemoImage, border, border, border, border, cv::BORDER_CONSTANT, 0);

	cv::Mat paddedDilatedImage = paddedDemoImage.clone();
	cv::Mat mask;
	cv::Mat resizedFrame;
	double minVal, maxVal;
#if 0
	unsigned char Data = 0;
	for (int x = 0; x < paddedDemoImage.size().width; x++) {
		for (int y = 0; y < paddedDemoImage.size().height; y++) {
			paddedDemoImage.at<uchar>(x, y) = Data++;
		}
	}
#endif
	for (int h_i = border; h_i < height + border; h_i++) {
		for (int w_i = border; w_i < width + border; w_i++) {
			resizedFrame = paddedDemoImage(cv::Range(h_i - border, h_i + border + 1), 
				cv::Range(w_i - border, w_i+ border+1));
			cv::bitwise_and(resizedFrame, element, mask);
			bool exit = 0;
			for (int x = 0; x < mask.size().width && !exit; x++) {
				for (int y = 0; y < mask.size().height; y++) {
					if (mask.at<uchar>(x, y)) {
						paddedDilatedImage.at<uchar>(h_i, w_i) = 1;
						exit = 1;
						break;
					}
				}
			}
			cv::Mat cloned_Dilated_Image = paddedDilatedImage.clone();
			cv::resize(cloned_Dilated_Image, cloned_Dilated_Image, cv::Size(50, 50));
			cloned_Dilated_Image = cloned_Dilated_Image * 255;
			cv::cvtColor(cloned_Dilated_Image, cloned_Dilated_Image, cv::COLOR_GRAY2BGR);
			v_Dilate_Writer_Obj << cloned_Dilated_Image;
		}
		//std::cout << "----------------------------------------\n";
	}
	v_Dilate_Writer_Obj.release();
	resizedFrame = paddedDilatedImage(cv::Range(border, height+1), cv::Range(border, width+1));
	std::cout << resizedFrame << "\n";

	//Erosion
	std::cout << "-------------Erosion-----------------\n";
	cv::Mat erodedEllipseKernel;
	cv::erode(demoImage, erodedEllipseKernel, element);
	std::cout << erodedEllipseKernel << "\n";


	paddedDemoImage = cv::Mat::zeros(cv::Size(height + border * 2, width + border * 2), CV_8UC1);
	copyMakeBorder(demoImage, paddedDemoImage, border, border, border, border, cv::BORDER_CONSTANT, 0);

	cv::Mat paddedErodedImage = paddedDemoImage.clone();

	const std::string video_Erode_F_Name("erosionScratch.avi");
	const int fourCC_1 = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
	double frames_Per_Sec_1 = 10;
	cv::Size frame_Size_1(320, 240);
	cv::VideoWriter v_Erosion_Writer_Obj(video_Erode_F_Name, fourCC_1, frames_Per_Sec_1, frame_Size_1);

	for (int h_i = border; h_i < height + border; h_i++) {
		for (int w_i = border; w_i < width + border; w_i++) {
			resizedFrame = paddedDemoImage(cv::Range(h_i - border, h_i + border + 1),
				cv::Range(w_i - border, w_i + border + 1));
			cv::bitwise_or(resizedFrame, element, mask);
			bool exit = 0;
			for (int x = 0; x < mask.size().width && !exit; x++) {
				for (int y = 0; y < mask.size().height; y++) {
					if (!mask.at<uchar>(x, y)) {
						paddedErodedImage.at<uchar>(h_i, w_i) = 0;
						exit = 1;
						break;
					}
				}
			}
			cv::Mat cloned_Eroded_Image = paddedErodedImage.clone();
			cv::resize(cloned_Eroded_Image, cloned_Eroded_Image, cv::Size(50, 50));
			cloned_Eroded_Image = cloned_Eroded_Image * 255;
			cv::cvtColor(cloned_Eroded_Image, cloned_Eroded_Image, cv::COLOR_GRAY2BGR);
			v_Erosion_Writer_Obj << cloned_Eroded_Image;
		}
		//std::cout << "----------------------------------------\n";
	}
	v_Erosion_Writer_Obj.release();

	resizedFrame = paddedErodedImage(cv::Range(border, height + 1), cv::Range(border, width + 1));
	std::cout << resizedFrame << "\n";
 	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}