#include<iostream>
#include<string>

#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main() {
	cv::Mat Input_Image = cv::imread("boy.jpg/boy.jpg", 1);
	const std::string Orig_Image = "Original_Image";
	cv::namedWindow(Orig_Image);
	cv::imshow(Orig_Image, Input_Image);
	/* Resizing an Image:
	   Interpolation method choosed here is "INTER_LINEAR"
	   Name of the OCV API is cv::resize
	   I/P Parameters are:
	   src   - Input Image
	   dst   - Output Resized Image
	   dsize - Output Image Size using cv::Size(Width, Height)
	   fx    - Scale Factor along the horizontal AXIS
	   fy    - Scale Factor along the vertical AXIS

	   Using fx & fy parameters to specify the scaling factor for Horizontal & Vertical axis.
	   The O/P size is calculated:
	   Size(round(fx * src.cols), round(fy * sec.rows))
	*/

	const int Resize_Down_Width   = 300;
	const int Resize_Down_Height = 200;
	cv::Mat Resize_Down_Image;
	cv::resize(Input_Image, Resize_Down_Image, cv::Size(Resize_Down_Width, Resize_Down_Height),
		0.0, 0.0, cv::INTER_LINEAR);

	const int Resize_Up_Width = 600;
	const int Resize_Up_Height = 900;
	cv::Mat Resize_Up_Image;
	cv::resize(Input_Image, Resize_Up_Image, cv::Size(Resize_Up_Width, Resize_Up_Height),
		0.0, 0.0, cv::INTER_LINEAR);

	const std::string Scaled_Down = "Scaled_Down";
	const std::string Scaled_Up   = "Scaled_Up";
	cv::namedWindow(Scaled_Down);
	cv::imshow(Scaled_Down, Resize_Down_Image);

	cv::namedWindow(Scaled_Up);
	cv::imshow(Scaled_Up, Resize_Up_Image);


	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
