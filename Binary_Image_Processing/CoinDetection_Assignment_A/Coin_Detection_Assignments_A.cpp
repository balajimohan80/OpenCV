#if 0
#include<iostream>
#include<vector>

#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

cv::Mat convert_RGB_To_GrayScale(cv::Mat& imageB, cv::Mat& imageG, cv::Mat& imageR) {
	cv::Mat grayScale(imageB.size(), CV_8UC1);
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator B_It =
		imageB.begin<cv::Vec<unsigned char, 1>>();
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator G_It =
		imageG.begin<cv::Vec<unsigned char, 1>>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator R_It =
		imageR.begin<cv::Vec<uchar, 1>>();
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator Gray_It =
		grayScale.begin<cv::Vec<unsigned char, 1>>();
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator Gray_End =
		grayScale.end<cv::Vec<unsigned char, 1>>();
	

	while (Gray_It != Gray_End) {
		float Y = (0.299 * (*R_It++)[0]) +
			(0.587 * (*G_It++)[0]) +
			(0.114 * (*B_It++)[0]);
		(*Gray_It++)[0] = static_cast<unsigned char>(Y);
	}
	return grayScale;
}

#if 0
const std::string g_Threshold_Win_Name = "Thresholded_Image";
int g_Threshold_Pos_Val     = 0;
int g_Threshold_Max_Pix_Val = 1000;

int g_Threshold_Max_Pos_Val = 0;
bool g_Threshold_Bin_Inv = 0;
double g_Threshold_Val        = 0.0;
double g_Threshold_Max_Val    = 0.0;

void apply_Thresholding(void* image) {
	cv::Mat* input_Image = static_cast<cv::Mat*>(image);
	cv::Mat dst;
	if (g_Threshold_Bin_Inv) {
		cv::threshold(*input_Image, dst, g_Threshold_Val, g_Threshold_Max_Val, cv::THRESH_BINARY_INV);
	//	cv::adaptiveThreshold(*input_Image, dst, g_Threshold_Max_Val, 
	//		cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, g_Threshold_Val);
	}
	else {
		cv::threshold(*input_Image, dst, g_Threshold_Val, g_Threshold_Max_Val, cv::THRESH_TRUNC);
	}
	cv::imshow(g_Threshold_Win_Name, dst);
}

void thresholding_Val_Func (int pos, void* userdata) {
	g_Threshold_Val = static_cast<double>(g_Threshold_Pos_Val);
	apply_Thresholding(userdata);
}

void thresholding_Max_Val_Func(int pos, void* userdata) {
	g_Threshold_Max_Val = static_cast<double>(g_Threshold_Max_Pos_Val);
	apply_Thresholding(userdata);
}

void thresholding_binary(int pos, void* userdata) {
	g_Threshold_Bin_Inv = (pos == 0) ? false : true;
	apply_Thresholding(userdata);
}
#endif

int g_Erosion_Kernel_Size        = 3;
int g_Erosion_Iteration          = 1;
int g_Dilation_Kernel_Size       = 3;
int g_Dilation_Iteration         = 1;
int g_MorphShapes = cv::MORPH_ELLIPSE;
bool g_Is_Open = 0;
cv::Mat g_Eroded_Kernel;
cv::Mat g_Dilated_Kernel;
std::string g_Dilated_Win_Name = "Dilation";
std::string g_Eroded_Win_Name = "Erosion";
std::string g_Dilated_Erosion_Win_Name = "Erosion+Dilation";


void dilation_Erosion(void *image) {
	cv::Mat* input_Image = static_cast<cv::Mat*>(image);
	std::cout << "-------------------------------------------------------------\n";
	std::cout << "kr_Sz(Er) : " << g_Erosion_Kernel_Size << " kr_sz(Dil) : " << g_Dilation_Kernel_Size << "\n";
	std::cout << "Iter(Er)  : " << g_Erosion_Iteration << " Iter(Dil)  : " << g_Dilation_Iteration << "\n";
	if (!g_Is_Open) {
		cv::Mat eroded_Image;
		cv::erode(*input_Image, eroded_Image, g_Eroded_Kernel, cv::Point(-1, -1),
			g_Erosion_Iteration);
		cv::imshow("g_Eroded_Win_Name", eroded_Image);
		cv::Mat dilated_Image;
		cv::dilate(eroded_Image, dilated_Image, g_Dilated_Kernel, cv::Point(-1, -1),
			g_Dilation_Iteration);
		
		cv::imshow(g_Dilated_Erosion_Win_Name, dilated_Image);
	}
	else {
		cv::Mat dilated_Image;
		cv::dilate(*input_Image, dilated_Image, g_Dilated_Kernel, cv::Point(-1, -1),
			g_Dilation_Iteration);
		cv::imshow("g_Dilated_Win_Name", dilated_Image);
		cv::Mat eroded_Image;
		cv::erode(dilated_Image, eroded_Image, g_Eroded_Kernel, cv::Point(-1, -1),
			g_Erosion_Iteration);
		
		
		cv::imshow(g_Dilated_Erosion_Win_Name, eroded_Image);
	}
}

void erosion_Kernel_Size_Method(int pos, void* Userdata) {

	g_Eroded_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(2*g_Erosion_Kernel_Size+1, 2*g_Erosion_Kernel_Size+1), 
		cv::Point(g_Erosion_Kernel_Size, g_Erosion_Kernel_Size));
	
	dilation_Erosion(Userdata);
}

void erosion_Iteration_Method(int pos, void* Userdata) {
	dilation_Erosion(Userdata);
}

void dilation_Kernel_Size_Method(int pos, void* Userdata) {
	g_Dilated_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(2*g_Dilation_Kernel_Size+1, 2*g_Dilation_Kernel_Size+1), 
		cv::Point(g_Dilation_Kernel_Size, g_Dilation_Kernel_Size));
	dilation_Erosion(Userdata);
}

void dilation_Iteration_Method(int pos, void* Userdata) {
	dilation_Erosion(Userdata);
	
}

void morph_Shapes_Method(int pos, void* Userdata) {
	switch( pos ) {
		case 0:
			g_MorphShapes = cv::MORPH_RECT;
			break;

		case 1:
			g_MorphShapes = cv::MORPH_CROSS;
			break;

		case 2:
			g_MorphShapes = cv::MORPH_ELLIPSE;
			break;
	} 
	g_Dilated_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(g_Dilation_Kernel_Size, g_Dilation_Kernel_Size), cv::Point(-1, -1));
	g_Eroded_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(g_Erosion_Kernel_Size, g_Erosion_Kernel_Size), cv::Point(-1, -1));

	dilation_Erosion(Userdata);
}

#if 1
cv::Mat perform_Morphology(cv::Mat& input_Gray_Image, bool is_Open) {
	cv::Mat Ret;
	std::string win_Name = "Trackbar";
	cv::namedWindow(win_Name, cv::WINDOW_AUTOSIZE);
	g_Is_Open = is_Open;
	cv::createTrackbar("Erode_KSize", win_Name, &g_Erosion_Kernel_Size, 20,
		erosion_Kernel_Size_Method, static_cast<void*>(&input_Gray_Image));
	cv::createTrackbar("Erode_Iteration", win_Name, &g_Erosion_Iteration, 20,
		erosion_Iteration_Method, static_cast<void*>(&input_Gray_Image));

	cv::createTrackbar("Dilate_KSize", win_Name, &g_Dilation_Kernel_Size, 20,
		dilation_Kernel_Size_Method, static_cast<void*>(&input_Gray_Image));
	cv::createTrackbar("Dilate_Iteration", win_Name, &g_Dilation_Iteration, 20,
		dilation_Iteration_Method, static_cast<void*>(&input_Gray_Image));
	cv::createTrackbar("MorphShapes", win_Name, &g_MorphShapes, 2,
		morph_Shapes_Method, static_cast<void*>(&input_Gray_Image));

	cv::waitKey();
	return Ret;
}
#endif


void apply_Erosion_And_Dilation(cv::Mat &image, const int erosion_KSize, const int erosion_Iter, 
	const int dilation_KSize, const int dilation_Iter, bool open) {
	cv::Mat erosion_Kernel  = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erosion_KSize, erosion_KSize));
	cv::Mat dilation_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(dilation_KSize, dilation_KSize));

	if (open) {
		cv::dilate(image, image, dilation_Kernel, cv::Point(-1, -1),
			dilation_Iter);
		cv::erode(image, image, erosion_Kernel, cv::Point(-1, -1),
			erosion_Iter);
	}
	else {
		cv::erode(image, image, erosion_Kernel, cv::Point(-1, -1),
			erosion_Iter);
		cv::dilate(image, image, dilation_Kernel, cv::Point(-1, -1),
			dilation_Iter);
	}
}

cv::Mat displayConnectedComponents(cv::Mat& im)
{
	// Make a copy of the image
	cv::Mat imLabels = im.clone();

	// First let's find the min and max values in imLabels
	cv::Point minLoc, maxLoc;
	double min, max;

	// The following line finds the min and max pixel values
	// and their locations in an image.
	cv::minMaxLoc(imLabels, &min, &max, &minLoc, &maxLoc);

	// Normalize the image so the min value is 0 and max value is 255.
	imLabels = 255 * (imLabels - min) / (max - min);

	// Convert image to 8-bits
	imLabels.convertTo(imLabels, CV_8U);

	// Apply a color map
	cv::Mat imColorMap;
	cv::applyColorMap(imLabels, imColorMap, cv::COLORMAP_JET);

	return imColorMap;
}

int main() {
	std::string input_Image_Name = "CoinsA.png";
	cv::Mat image = cv::imread(input_Image_Name);
	cv::Mat imageCopy = image.clone();
	cv::imshow(input_Image_Name, image);

	//Convert Image to GrayScale
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

	//Split Cells into Channels
	cv::Mat imageB(image.size(), CV_8UC1);
	cv::Mat imageG(image.size(), CV_8UC1);
	cv::Mat imageR(image.size(), CV_8UC1);

	cv::Mat_<cv::Vec3b>::iterator image_Begin = image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::iterator image_End   = image.end<cv::Vec3b>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator imageB_Begin = imageB.begin<cv::Vec<uchar, 1>>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator imageG_Begin = imageG.begin<cv::Vec<uchar, 1>>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator imageR_Begin = imageR.begin<cv::Vec<uchar, 1>>();

	while (image_Begin != image_End) {
		*imageB_Begin++ = (*image_Begin)[0];
		*imageG_Begin++ = (*image_Begin)[1];
		*imageR_Begin++ = (*image_Begin)[2];
		++image_Begin;
	}

	cv::imshow("B_Coin", imageB);
	cv::imshow("G_Coin", imageG);
	cv::imshow("R_Coin", imageR);
	//Perform Thresholding
#if 0
#if 0
	/* cv::threshold required two values: double thresh, double maxval
	   Use  Trackbar, find an optimal value of thresh & maxval.
	   To do that create two trackbars, one for thresh & other for maxval
	*/
	cv::Mat dst;
	double threshval = 30.0;
	double threshmaxval = 255.0;
	cv::threshold(imageGray, dst, threshval, threshmaxval, cv::THRESH_BINARY_INV);
	cv::imshow("Actual", dst);

	std::string win_Name = "Trackbar";
	cv::namedWindow(win_Name, cv::WINDOW_AUTOSIZE);
	cv::TrackbarCallback thresholding_Val     = thresholding_Val_Func;
	cv::TrackbarCallback thresholding_Max_Val = thresholding_Max_Val_Func;
	cv::TrackbarCallback thresholding_Bin_Inv = thresholding_binary;

	
	cv::createTrackbar("Thresholding", win_Name, &g_Threshold_Pos_Val, g_Threshold_Max_Pix_Val,
		thresholding_Val, static_cast<void*>(&dst));
	
	cv::createTrackbar("Thresholding_Max", win_Name, &g_Threshold_Max_Pos_Val, g_Threshold_Max_Pix_Val,
		thresholding_Max_Val_Func, static_cast<void*>(&dst));

	cv::createTrackbar("Thresholding_Inv", win_Name, nullptr, 1,
		thresholding_Bin_Inv, static_cast<void*>(&dst));

#else
	cv::Mat dst;
	double threshval = 38.0;
	double threshmaxval = 180.0;
	cv::threshold(imageGray, dst, threshval, threshmaxval, cv::THRESH_BINARY_INV);
	cv::imshow("Actual", dst);
	imageGray = dst.clone();
	int k = 0;
	bool is_Max = 0;
	while (1) {
		cv::threshold(dst, dst, threshval, threshmaxval, cv::THRESH_BINARY_INV);
		if (is_Max) {
			threshmaxval = threshmaxval + 0.1;
			std::cout << "threshmaxval: " << threshmaxval << "\n";
		}

		else {
			threshval = threshval + 0.1;
			std::cout << "threshval: " << threshval << "\n";
		}
		cv::imshow("Thresholded_Image", dst);
		k = cv::waitKey() & 0xFF;
		if (k == 27) {
			dst = imageGray.clone();
			is_Max = !is_Max;
		}
	}
#endif
#else
	cv::Mat dst;
	double threshval = 38.0;
//	double threshval = 32.0;
	double threshmaxval = 255.0;
	cv::threshold(imageGray, dst, threshval, threshmaxval, cv::THRESH_BINARY_INV);

	cv::imshow("Actual", dst);

#endif	

#if 1
	cv::Mat morphology_Close=dst.clone();
	const int open_Dilation_Kernel_Size = 18;
	const int dilation_Iteration = 1;
	const int open_Erosion_kernel_Size = 15;
	const int erosion_Iteration = 1;
	apply_Erosion_And_Dilation(morphology_Close, open_Erosion_kernel_Size, erosion_Iteration,
		open_Dilation_Kernel_Size, dilation_Iteration, 1);

	const int close_Dilation_Kernel_Size = 14;
	const int close_Erosion_Kernel_Size = 7;
	apply_Erosion_And_Dilation(morphology_Close, close_Erosion_Kernel_Size, erosion_Iteration,
		close_Dilation_Kernel_Size, dilation_Iteration, 0);

#else	
	const int open_Dilation_Kernel_Size = 18;
	const int dilation_Iteration   = 1;
	cv::Mat open_Dilated_kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(open_Dilation_Kernel_Size, open_Dilation_Kernel_Size), 
		cv::Point(-1, -1));

	const int open_Erosion_kernel_Size = 15;
	const int erosion_Iteration = 1;
	cv::Mat open_Erosion_kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(open_Erosion_kernel_Size, open_Erosion_kernel_Size), cv::Point(-1, -1));
	cv::Mat morphology_Open;
	cv::dilate(dst, morphology_Open, open_Dilated_kernel, cv::Point(-1, -1),
		dilation_Iteration);
	cv::erode(morphology_Open, morphology_Open, open_Erosion_kernel, cv::Point(-1, -1),
		erosion_Iteration);
	cv::imshow("Morphological_Open", morphology_Open);

	const int close_Dilation_Kernel_Size = 14;
	cv::Mat close_Dilation_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
		cv::Size(close_Dilation_Kernel_Size, close_Dilation_Kernel_Size), 
		cv::Point(-1, -1));

	const int close_Erosion_Kernel_Size = 7;
	cv::Mat close_Erosion_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(close_Erosion_Kernel_Size, close_Erosion_Kernel_Size), cv::Point(-1, -1));
	cv::Mat morphology_Close;
	
	cv::erode(morphology_Open, morphology_Close, close_Erosion_Kernel, cv::Point(-1, -1),
		erosion_Iteration);
	cv::dilate(morphology_Close, morphology_Close, close_Dilation_Kernel, cv::Point(-1, -1),
		dilation_Iteration);
	cv::imshow("Morphological Close", morphology_Close);
#endif
	cv::SimpleBlobDetector::Params params;
	params.blobColor = 0;
	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;

	// Set up detector with params
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
	std::vector<cv::KeyPoint> keyPoint;
	detector->detect(morphology_Close, keyPoint);

	cv::Mat image_Circle = image.clone();
	for (const auto& kp : keyPoint) {
		
		std::cout << "x: " << kp.pt.x << " y: " << kp.pt.y << " angle: ";
		std::cout << kp.angle << " Sz: " << kp.size << "\n";
		cv::circle(image_Circle, kp.pt, 1, cv::Scalar(255, 0, 0), 3);
		cv::circle(image_Circle, kp.pt, static_cast<int>(kp.size / 2.0), cv::Scalar(0, 255, 0));
	}
	cv::imshow("CircledImage", image_Circle);
	cv::Mat connected_Image;
	cv::bitwise_not(morphology_Close, connected_Image);
	
	int nComponenets = cv::connectedComponents(connected_Image, connected_Image);
	cv::Mat colorMap = displayConnectedComponents(connected_Image);
	cv::imshow("Colormap", colorMap);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(morphology_Close, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	std::cout << "-----------------------------------------\n";
	std::cout << "pt: " << contours.size();
#if 0	
	for (auto contour_point : contours) {
		for (auto pt : contour_point) {
			std::cout << "X: " << pt.x << " Y: " << pt.y << "\n";
		}
	}
#endif
	cv::Mat contour_Image = image.clone();
	cv::drawContours(contour_Image, contours, -1, cv::Scalar(0, 0, 0), 3);
	cv::Moments M;
	int x, y;
	for (size_t i = 0; i < contours.size(); i++) {
		//We will use the contour moments to find the centeroid
		M = cv::moments(contours[i]);
		x = int(M.m10 / double(M.m00));
		y = int(M.m01 / double(M.m00));
		//Mark the center
		cv::circle(contour_Image, cv::Point(x, y), 1, cv::Scalar(0, 0, 255), 3);
	}
	cv::imshow("Centroid Image", contour_Image);
	std::vector<std::vector<cv::Point>> contour_Ext;
	cv::findContours(morphology_Close, contour_Ext, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat contour_Outer_Image = image.clone();
	cv::drawContours(contour_Outer_Image, contour_Ext, -1, cv::Scalar(0, 255, 0), 10);

	//Find area & perimeter
	double area;
	double perimeter;
	std::vector<std::pair<double, int>> contour_Moments_Container(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		area = cv::contourArea(contours[i]);
		perimeter = cv::arcLength(contours[i], true);
		std::cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << "\n";
		contour_Moments_Container[i] = { area, i };
	}
	std::sort(contour_Moments_Container.begin(), contour_Moments_Container.end(), []
	(const std::pair<double, double>& a, const std::pair<double, double>& b) {
			return a.first < b.first;
		});
	std::cout << "Maximum area of Contour: " << contour_Moments_Container[contour_Moments_Container.size() - 1].first << "\n";
	const int index_Not_To_Print = contour_Moments_Container[contour_Moments_Container.size() - 1].second;
	cv::Mat contour_Individual_Plot_Image = image.clone();
	for (size_t i = 0; i < contours.size(); i++) {
		if (i != index_Not_To_Print) {
			cv::drawContours(contour_Individual_Plot_Image, contours, i, cv::Scalar(0, 255, 0), 5);
		}
	}
	cv::imshow("contour_Individual_Plot", contour_Individual_Plot_Image);
	
	for (size_t i = 0; i < contours.size(); i++) {
		if (i != index_Not_To_Print) {
			cv::Point2f point;
			float radius;
			cv::minEnclosingCircle(contours[i], point, radius);
			cv::circle(image, point, 1, cv::Scalar(0, 0, 255), 3);
			cv::circle(image, point, radius, cv::Scalar(255, 0, 0), 3);
		}
	}
	cv::imshow("Final_Image", image);
	cv::waitKey();
	cv::destroyAllWindows();
	return 0;
}
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


void displayImage(Mat image) {
	imshow("Image", image);
	waitKey();
	destroyAllWindows();
}

void apply_Erosion_And_Dilation(cv::Mat& image, const int erosion_KSize, const int erosion_Iter,
	const int dilation_KSize, const int dilation_Iter, bool open) {
	cv::Mat erosion_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erosion_KSize, erosion_KSize));
	cv::Mat dilation_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(dilation_KSize, dilation_KSize));

	if (open) {
		cv::dilate(image, image, dilation_Kernel, cv::Point(-1, -1), dilation_Iter);
		cv::erode(image, image, erosion_Kernel, cv::Point(-1, -1), erosion_Iter);
	}
	else {
		cv::erode(image, image, erosion_Kernel, cv::Point(-1, -1), erosion_Iter);
		cv::dilate(image, image, dilation_Kernel, cv::Point(-1, -1), dilation_Iter);
	}
}

Mat displayConnectedComponents(Mat& im)
{
	// Make a copy of the image
	Mat imLabels = im.clone();

	// First let's find the min and max values in imLabels
	Point minLoc, maxLoc;
	double min, max;

	// The following line finds the min and max pixel values
	// and their locations in an image.
	minMaxLoc(imLabels, &min, &max, &minLoc, &maxLoc);

	// Normalize the image so the min value is 0 and max value is 255.
	imLabels = 255 * (imLabels - min) / (max - min);

	// Convert image to 8-bits
	imLabels.convertTo(imLabels, CV_8U);

	// Apply a color map
	Mat imColorMap;
	applyColorMap(imLabels, imColorMap, COLORMAP_JET);

	return imColorMap;
}

void convert_BGR_To_Single_Channel(const cv::Mat& image, cv::Mat& b, cv::Mat& g, cv::Mat& r) {
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator b_Iterator =
		b.begin<cv::Vec<unsigned char, 1>>();
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator g_Iterator =
		g.begin<cv::Vec<unsigned char, 1>>();
	cv::Mat_<cv::Vec<unsigned char, 1>>::iterator r_Iterator =
		r.begin<cv::Vec<unsigned char, 1>>();
	cv::Mat_<cv::Vec<unsigned char, 3>>::const_iterator bgr_Iterator_Begin =
		image.begin<cv::Vec<unsigned char, 3>>();
	cv::Mat_<cv::Vec<unsigned char, 3>>::const_iterator bgr_Iterator_End =
		image.end<cv::Vec<unsigned char, 3>>();

	while (bgr_Iterator_Begin != bgr_Iterator_End) {
		*b_Iterator++ = (*bgr_Iterator_Begin)[0];
		*g_Iterator++ = (*bgr_Iterator_Begin)[1];
		*r_Iterator++ = (*bgr_Iterator_Begin)[2];
		bgr_Iterator_Begin++;
	}

}


int main() {

	// Image path
	//string imagePath = DATA_PATH + "images/CoinsA.png";
	std::string imagePath = "CoinsA.png";
	// Read image
	// Store it in the variable image
	///
	/// YOUR CODE HERE
	///
	cv::Mat image = cv::imread(imagePath);
	Mat imageCopy = image.clone();

	displayImage(image);

	// Convert image to grayscale
	// Store it in the variable imageGray
	///
	/// YOUR CODE HERE
	///
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
	displayImage(imageGray);

	// Split cell into channels
	// Store them in variables imageB, imageG, imageR
	///
	/// YOUR CODE HERE
	///
	cv::Mat imageB(image.size(), CV_8UC1);
	cv::Mat imageG(image.size(), CV_8UC1);
	cv::Mat imageR(image.size(), CV_8UC1);
	cv::Mat_<cv::Vec3b>::iterator image_Begin = image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::iterator image_End = image.end<cv::Vec3b>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator imageB_Begin = imageB.begin<cv::Vec<uchar, 1>>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator imageG_Begin = imageG.begin<cv::Vec<uchar, 1>>();
	cv::Mat_<cv::Vec<uchar, 1>>::iterator imageR_Begin = imageR.begin<cv::Vec<uchar, 1>>();

	while (image_Begin != image_End) {
		*imageB_Begin++ = (*image_Begin)[0];
		*imageG_Begin++ = (*image_Begin)[1];
		*imageR_Begin++ = (*image_Begin)[2];
		++image_Begin;
	}
	displayImage(imageB);
	displayImage(imageG);
	displayImage(imageR);

	// Perform thresholding
	///
	/// YOUR CODE HERE
	///
	cv::Mat dst;
	double threshval = 38.0;
	double threshmaxval = 255.0;
	cv::threshold(imageGray, dst, threshval, threshmaxval, cv::THRESH_BINARY_INV);
	// Modify as required
	displayImage(dst);

	// Perform morphological operations
	///
	/// YOUR CODE HERE
	///
	cv::Mat imageDilated = dst.clone();
	const int open_Dilation_Kernel_Size = 18;
	const int dilation_Iteration = 1;
	const int open_Erosion_kernel_Size = 15;
	const int erosion_Iteration = 1;
	apply_Erosion_And_Dilation(imageDilated, open_Erosion_kernel_Size, erosion_Iteration,
		open_Dilation_Kernel_Size, dilation_Iteration, 1);
	displayImage(imageDilated);

	// Get structuring element/kernel which will be used for dilation
	///
	/// YOUR CODE HERE
	///
	cv::Mat imageEroded = imageDilated.clone();
	const int close_Dilation_Kernel_Size = 14;
	const int close_Erosion_Kernel_Size = 7;
	apply_Erosion_And_Dilation(imageEroded, close_Erosion_Kernel_Size, erosion_Iteration,
		close_Dilation_Kernel_Size, dilation_Iteration, 0);

	displayImage(imageEroded);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	params.blobColor = 0;

	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs
	///
	/// YOUR CODE HERE
	///
	std::vector<cv::KeyPoint> keyPoint;
	detector->detect(imageEroded, keyPoint);
	// Print number of coins detected
	///
	/// YOUR CODE HERE
	///
	std::cout << "Number of coins detected = " << keyPoint.size() << "\n";
	// Mark coins using image annotation concepts we have studied so far
	/// YOUR CODE HERE
	///
	for (const auto& kp : keyPoint) {
		cv::circle(image, kp.pt, 1, cv::Scalar(255, 0, 0), 3);
		cv::circle(image, kp.pt, static_cast<int>(kp.size / 2.0), cv::Scalar(0, 255, 0));
	}
	displayImage(image);

	// Find connected components
	// Use displayConnectedComponents function provided above
	///
	/// YOUR CODE HERE
	///
	cv::Mat connected_Image;
	cv::bitwise_not(imageEroded, connected_Image);
	int nComponenets = cv::connectedComponents(connected_Image, connected_Image);
	cv::Mat colorMap = displayConnectedComponents(connected_Image);
	displayImage(colorMap);

	// Find all contours in the image
	///
	/// YOUR CODE HERE
	///
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(imageEroded, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	// Print the number of contours found
	///
	/// YOUR CODE HERE
	///
	std::cout << "Number of contours found = " << contours.size() << "\n";
	// Draw all contours
	///
	/// YOUR CODE HERE
	///
	image = imageCopy.clone();
	cv::drawContours(image, contours, -1, cv::Scalar(0, 0, 0), 3);
	cv::Moments M;
	int x, y;
	for (size_t i = 0; i < contours.size(); i++) {
		//We will use the contour moments to find the centeroid
		M = cv::moments(contours[i]);
		x = int(M.m10 / double(M.m00));
		y = int(M.m01 / double(M.m00));
		//Mark the center
		cv::circle(image, cv::Point(x, y), 1, cv::Scalar(0, 0, 255), 3);
	}
	displayImage(image);
	// Remove the inner contours
	// Display the result
	///
	/// YOUR CODE HERE
	///

	std::vector<std::vector<cv::Point>> contour_Ext;
	cv::findContours(imageEroded, contour_Ext, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	image = imageCopy.clone();
	cv::drawContours(image, contour_Ext, -1, cv::Scalar(0, 255, 0), 10);
	displayImage(image);

	// Print area and perimeter of all contours
	///
	/// YOUR CODE HERE
	///
	double area;
	double perimeter;
	std::vector<std::pair<double, int>> contour_Moments_Container(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		area = cv::contourArea(contours[i]);
		perimeter = cv::arcLength(contours[i], true);
		std::cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << "\n";
		contour_Moments_Container[i] = { area, i };
	}
	// Print maximum area of contour
	// This will be the box that we want to remove
	///
	/// YOUR CODE HERE
	///
	std::sort(contour_Moments_Container.begin(), contour_Moments_Container.end(), []
	(const std::pair<double, double>& a, const std::pair<double, double>& b) {
			return a.first < b.first;
		});
	std::cout << "Maximum area of Contour: " << contour_Moments_Container[contour_Moments_Container.size() - 1].first << "\n";
	// Remove this contour and plot others
	///
	/// YOUR CODE HERE
	///
	const int index_Not_To_Print = contour_Moments_Container[contour_Moments_Container.size() - 1].second;
	image = imageCopy.clone();
	for (size_t i = 0; i < contours.size(); i++) {
		if (i != index_Not_To_Print) {
			cv::drawContours(image, contours, i, cv::Scalar(0, 255, 0), 5);
		}
	}
	displayImage(image);
	// Fit circles on coins
	///
	/// YOUR CODE HERE
	///
	image = imageCopy.clone();
	for (size_t i = 0; i < contours.size(); i++) {
		if (i != index_Not_To_Print) {
			cv::Point2f point;
			float radius;
			cv::minEnclosingCircle(contours[i], point, radius);
			cv::circle(image, point, 1, cv::Scalar(0, 0, 255), 3);
			cv::circle(image, point, radius, cv::Scalar(255, 0, 0), 3);
		}
	}
	displayImage(image);

	imagePath = "CoinsB.png";
	// Read image
	// Store it in variable image
	///
	/// YOUR CODE HERE
	///
	image = cv::imread(imagePath);
	imageCopy = image.clone();

	// Convert image to grayscale
	// Store it in the variable imageGray
	///
	/// YOUR CODE HERE
	///
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
	displayImage(imageGray);

	// Split cell into channels
	// Store them in variables imageB, imageG, imageR
	///
	/// YOUR CODE HERE
	///
	imageB.create(image.size(), CV_8UC1);
	imageG.create(image.size(), CV_8UC1);
	imageR.create(image.size(), CV_8UC1);

	convert_BGR_To_Single_Channel(image, imageB, imageG, imageR);
	displayImage(imageB);
	displayImage(imageG);
	displayImage(imageR);

	// Perform thresholding
	///
	/// YOUR CODE HERE
	///
	cv::threshold(imageGray, dst, 160.0, 255, cv::THRESH_BINARY);
	displayImage(dst);

	// Perform morphological operations
	///
	/// YOUR CODE HERE
	///
	cv::Mat morphological_Image;

	//First remove black 
	int d_KSize = 2;
	int d_Iteration = 3;
	cv::Mat d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(dst, morphological_Image, d_Kernel, cv::Point(-1, -1), d_Iteration);

	int e_KSize = 2;
	int e_Iteration = 2;
	cv::Mat e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(morphological_Image, morphological_Image, e_Kernel, cv::Point(-1, -1), e_Iteration);

	d_KSize = 3;
	d_Iteration = 2;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(morphological_Image, morphological_Image, d_Kernel, cv::Point(-1, -1), d_Iteration);


	e_Iteration = 5;
	e_KSize = 5;
	e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(morphological_Image, morphological_Image, e_Kernel, cv::Point(-1, -1), e_Iteration);


	d_KSize = 5;
	d_Iteration = 5;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(morphological_Image, morphological_Image, d_Kernel, cv::Point(-1, -1), d_Iteration);

	//added now
	d_KSize = 3;
	d_Iteration = 3;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(morphological_Image, morphological_Image, d_Kernel, cv::Point(-1, -1), d_Iteration);

	e_KSize = 4;
	e_Iteration = 18;
	e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(morphological_Image, morphological_Image, e_Kernel, cv::Point(-1, -1), e_Iteration);

	d_KSize = 5;
	d_Iteration = 14;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(morphological_Image, morphological_Image, d_Kernel, cv::Point(-1, -1), d_Iteration);
	displayImage(morphological_Image);
	// Setup SimpleBlobDetector parameters.

	params.blobColor = 0;

	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;

	// Set up detector with params
	detector = SimpleBlobDetector::create(params);

	// Detect blobs
	///
	/// YOUR CODE HERE
	///
	detector->detect(morphological_Image, keyPoint);
	// Print number of coins detected
	///
	/// YOUR CODE HERE
	///
	std::cout << "Number of Coins Detected: " << keyPoint.size() << "\n";

	// Mark coins using image annotation concepts we have studied so far
	///
	/// YOUR CODE HERE
	///
	cv::Mat coins_Detected_Image = image.clone();
	for (cv::KeyPoint point : keyPoint) {
		cv::circle(coins_Detected_Image, point.pt, 3, cv::Scalar(0, 0, 0), 3);
		cv::circle(coins_Detected_Image, point.pt, point.size / 2.0, cv::Scalar(0, 0, 255), 3);
	}
	displayImage(coins_Detected_Image);
	// Find connected components
	///
	/// YOUR CODE HERE
	///
	cv::bitwise_not(morphological_Image, connected_Image);

	nComponenets = cv::connectedComponents(connected_Image, connected_Image);
	std::cout << "Number of connected components detected = " << nComponenets << "\n";
	colorMap = displayConnectedComponents(connected_Image);
	displayImage(colorMap);

	// Find all contours in the image
	///
	/// YOUR CODE HERE
	///
	std::vector<std::vector<cv::Point>> contours_2;
	std::vector<cv::Vec4i> hierarchy_2;

	cv::findContours(morphological_Image, contours_2, hierarchy_2, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	// Print the number of contours found
	///
	/// YOUR CODE HERE
	///
	std::cout << "Number of contours found = " << contours_2.size() << "\n";
	// Draw all contours
	///
	/// YOUR CODE HERE
	///
	cv::Mat contour_Image_2 = image.clone();
	cv::drawContours(contour_Image_2, contours_2, -1, cv::Scalar(0, 0, 0), 3);
	displayImage(contour_Image_2);
	// Remove the inner contours
	// Display the result
	///
	/// YOUR CODE HERE
	///
	std::vector<std::vector<cv::Point>> contour_Ext_2;
	cv::findContours(morphological_Image, contour_Ext_2, hierarchy_2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat contour_Ext_Image_2 = image.clone();
	cv::drawContours(contour_Ext_Image_2, contour_Ext_2, -1, cv::Scalar(0, 255, 0), 5);
	displayImage(contour_Ext_Image_2);

	// Print area and perimeter of all contours
	///
	/// YOUR CODE HERE
	///
	std::vector<std::pair<double, int>> contours_Container(contours_2.size());
	size_t count = 0;
	for (const std::vector<cv::Point>& contour_Pts : contours_2) {
		double area = cv::contourArea(contour_Pts);
		double perimeter = cv::arcLength(contour_Pts, true);
		contours_Container[count] = { area,  count };
		++count;
		std::cout << "Contour #" << count << " has area = " << area << " and perimeter = " << perimeter << "\n";
	}
	// Print maximum area of contour
	// This will be the box that we want to remove
	///
	/// YOUR CODE HERE
	///
	std::sort(contours_Container.begin(), contours_Container.end(),
		[](std::pair<double, int>& a,
			std::pair<double, int>& b) {
				return a.first < b.first;
		});

	std::cout << "Maximum area of contour = " << contours_Container[count - 1].first << "\n";

	// Remove this contour and plot others
	///
	/// YOUR CODE HERE
	///
	contours_Container.erase(contours_Container.end() - 1, contours_Container.end());
	cv::Mat cloned_Image = image.clone();
	for (const auto& local_ref : contours_Container) {
		cv::drawContours(cloned_Image, contours_2, local_ref.second,
			cv::Scalar(0, 255, 0), 3);
	}
	displayImage(cloned_Image);
	// Print area and perimeter of all contours
	///
	/// YOUR CODE HERE
	///
	count = 0;
	for (const auto& local_ref : contours_Container) {
		double perimeter = cv::arcLength(contours_2[local_ref.second], true);
		++count;
		std::cout << "Contour #" << count << " has area = " << local_ref.first << " and perimeter = " << perimeter << "\n";
	}
	// Remove contours
	///
	/// YOUR CODE HERE
	///
	contours_Container.erase(contours_Container.begin(), contours_Container.begin() + 2);

	// Draw revised contours
	///
	/// YOUR CODE HERE
	///
	cloned_Image = image.clone();
	count = 0;
	std::vector<std::pair<cv::Point2f, float>> circle_Container(contours_Container.size());
	for (const auto& local_ref : contours_Container) {
		cv::drawContours(cloned_Image, contours_2, local_ref.second,
			cv::Scalar(255, 0, 0), 3);
		cv::Point2f point;
		float radius;

		cv::minEnclosingCircle(contours_2[local_ref.second], point, radius);
		circle_Container[count] = { point, radius };
		cv::circle(cloned_Image, point, 2, cv::Scalar(255, 0, 0), 3);
		const std::string text_Num = std::to_string(++count);
		cv::putText(cloned_Image, text_Num, point, cv::FONT_HERSHEY_SIMPLEX, 1,
			cv::Scalar(0, 0, 255), 3);
	}
	displayImage(cloned_Image);
	// Fit circles on coins
	///
	/// YOUR CODE HERE
	///
	cloned_Image = image.clone();

	for (const auto& local_Circle : circle_Container) {
		cv::circle(cloned_Image, local_Circle.first, 2, cv::Scalar(0, 0, 255), 3);
		cv::circle(cloned_Image, local_Circle.first, local_Circle.second, cv::Scalar(255, 0, 0), 3);
	}
	displayImage(cloned_Image);

	return 0;
}
