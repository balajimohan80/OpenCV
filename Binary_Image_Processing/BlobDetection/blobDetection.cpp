#include<iostream>
#include<vector>
#include<string>

#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>


/* A Blob is a group of connected pixels in an image that share some common property(Ex: Grayscale value ).
   In the image above, the dark connected regions are blobs, and the goal of the blob
   detection is to identify and mark these regions.
*/

int main() {
	std::string input_Image_Name = "blob_detection.jpg";
	cv::Mat input_Image = cv::imread(input_Image_Name, cv::IMREAD_GRAYSCALE);
	cv::Mat clone_Input_Image = input_Image.clone();
	cv::imshow(input_Image_Name, input_Image);

	//Set up detector with default parameters
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();
	std::vector<cv::KeyPoint> keyPoints;
	detector->detect(input_Image, keyPoints);

	// Mark blobs using image annotation concepts
	int x, y;
	int radius;
	int diameter;
	cv::cvtColor(input_Image, input_Image, cv::COLOR_GRAY2BGR);
	for (int i = 0; i < keyPoints.size(); i++) {
		cv::KeyPoint k = keyPoints[i];
		cv::Point keyPt;
		keyPt = k.pt;
		x = (int)keyPt.x;
		y = (int)keyPt.y;
		//Mark Center in BLACK
		cv::circle(input_Image, cv::Point(x, y), 5, cv::Scalar(255, 0, 0), -1);
		//Get Radius of Coin
		diameter = k.size;
		radius = (int)diameter / 2.0;
		//Mark BLOB in GREEN
		cv::circle(input_Image, cv::Point(x, y), radius, cv::Scalar(0, 255, 0), 2);
	}
	cv::imshow("Blob Detected Image", input_Image);

#if 0
	// Setup SimpleBlobDetector parameters.
	cv::SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 1500;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

	detector = cv::SimpleBlobDetector::create(params);
	
	input_Image = clone_Input_Image.clone();
	
	std::vector<cv::KeyPoint> KeyPoints_2;
	detector->detect(input_Image, KeyPoints_2);

	cv::imshow("Blob Detected Image", input_Image);
#endif
	cv::waitKey();
	cv::destroyAllWindows();
	return 0;
}

