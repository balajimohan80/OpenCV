#include<iostream>
#include<string>
#include<vector>

#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

/* Contours are simply the boundaries of an object or part of object in an image. 
   They are useful in shape analysis and object Detection/Recognition using traditional 
   Computer Vision algorithms
*/

int main() {
	std::string input_Image_Name = "Contour.png";
	cv::Mat image_Input = cv::imread(input_Image_Name);
	cv::imshow(input_Image_Name, image_Input);
	
	cv::Mat imageCopy = image_Input.clone();
	cv::Mat imageGray;
	cv::cvtColor(image_Input, imageGray, cv::COLOR_BGR2GRAY);
	cv::imshow("GrayImage", imageGray);
	/* 
	void cv::findContours    (    InputArray     image,
			OutputArrayOfArrays     contours,
			OutputArray     hierarchy,
			int     mode,
			int     method,
			Point     offset = Point()
	)
	Where,

	image - input image (8-bit single-channel). Non-zero pixels are treated as 1's. Zero pixels remain 0's, 
	        so the image is treated as binary . 
			You can use compare, inRange, threshold , adaptiveThreshold, Canny, and others to create 
			a binary image out of a grayscale or color one.
	contours  - Detected contours. Each contour is stored as a vector of points.
	hierarchy - Optional output vector containing information about the image topology. 
				It has been described in detail in the video above.
	mode - Contour retrieval mode, ( RETR_EXTERNAL, RETR_LIST, RETR_CCOMP, RETR_TREE )
	method - Contour approximation method. ( CHAIN_APPROX_NONE, CHAIN_APPROX_SIMPLE, CHAIN_APPROX_TC89_L1 etc )
	offset - Optional offset by which every contour point is shifted. 
			  This is useful if the contours are extracted from the image ROI and 
			  then they should be analyzed in the whole image context.
	*/
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	std::cout << "Number of Contours Found: " << contours.size() << "\n";

	//Contours are simply an array of pixel locations. Lets draw the contour on the boundary of objects
	cv::drawContours(image_Input, contours, -1, cv::Scalar(255, 0, 0), 6);
	cv::imshow("Inner & Outer Contours", image_Input);

	//RETR_LIST - Detected the inner and outer contours are detected
	//RETR_EXTERNAL - Detect only outer contours
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	std::cout << "Number of Contours Found: " << contours.size() << "\n";
	image_Input = imageCopy.clone();
	cv::drawContours(image_Input, contours, -1, cv::Scalar(0, 255, 0), 6);
	cv::imshow("Outer Contours", image_Input);

	//Draw all detected Contours
	for (int i = 0; i < contours.size(); i++) {
		image_Input = imageCopy.clone();
		cv::drawContours(image_Input, contours, i, cv::Scalar(0, 0, 255), 6);
		std::string Str = "Contour-" + std::to_string(i);
		cv::imshow(Str, image_Input);
	}

	//Once you have detected the contours from an image, 
	//it becomes very easy to do further analysis on the basis of various properties of contours. 
	//Let us discuss a few useful properties of contours.

	//Center of Mass or Centroid
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	// Draw all the contours
	cv::drawContours(image_Input, contours, -1, cv::Scalar(0, 255, 0), 3);

	cv::Moments M;
	int x, y;
	for (size_t i = 0; i < contours.size(); i++) {
		//We will use the contour moments to find the centeroid
		M = cv::moments(contours[i]);
		x = int(M.m10 / double(M.m00));
		y = int(M.m01 / double(M.m00));
		//Mark the center
		cv::circle(image_Input, cv::Point(x, y), 10, cv::Scalar(255, 0, 0), -1);
	}
	cv::imshow("Centroid Image", image_Input);
	
	//Find area & perimeter
	double area;
	double perimeter;
	for (size_t i = 0; i < contours.size(); i++) {
		area = cv::contourArea(contours[i]);
		perimeter = cv::arcLength(contours[i], true);
		std::cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << "\n";
	}

	//Bounding Boxes
	//There are 2 type of bounding boxes we can create around a contour :
	//1. A vertical rectangle
	//2. A rotated rectangle - This is the bounding box with the minimum area
	image_Input = imageCopy.clone();
	cv::Rect rect;
	for (size_t i = 0; i < contours.size(); i++) {
		// Vertical rectangle
		rect = cv::boundingRect(contours[i]);
		cv::rectangle(image_Input, rect, cv::Scalar(255, 0, 255), 2);
	}
	cv::imshow("Vertical rectangle", image_Input);

	image_Input = imageCopy.clone();
	cv::RotatedRect rotrect;
	cv::Point2f rect_points[4];
	cv::Mat boxPoints2f, boxPointsCov;
	for (size_t i = 0; i < contours.size(); i++) {
		// Rotated rectangle
		rotrect = cv::minAreaRect(contours[i]);
		cv::boxPoints(rotrect, boxPoints2f);
		boxPoints2f.assignTo(boxPointsCov, CV_32S);
		cv::polylines(image_Input, boxPointsCov, true, cv::Scalar(0, 255, 255), 2);
	}
	cv::imshow("Rotated rectangle", image_Input);

	cv::waitKey();
	cv::destroyAllWindows();
	return 0;
}