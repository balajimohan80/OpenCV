#include<iostream>
#include<vector>
#include<string>

#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>


int g_Erosion_Kernel_Size = 3;
int g_Erosion_Iteration = 1;
int g_Dilation_Kernel_Size = 3;
int g_Dilation_Iteration = 1;
int g_MorphShapes = cv::MORPH_ELLIPSE;
bool g_Is_Open = 0;
cv::Mat g_Eroded_Kernel;
cv::Mat g_Dilated_Kernel;
std::string g_Dilated_Win_Name = "Dilation";
std::string g_Eroded_Win_Name = "Erosion";
std::string g_Dilated_Erosion_Win_Name = "Erosion+Dilation";
std::string g_Eroded_Dilated_Win_Name = "Dilation+Erosion";

void dilation_Erosion(void* image) {
	cv::Mat* input_Image = static_cast<cv::Mat*>(image);
	std::cout << "-------------------------------------------------------------\n";
	std::cout << "kr_Sz(Er) : " << g_Erosion_Kernel_Size << " kr_sz(Dil) : " << g_Dilation_Kernel_Size << "\n";
	std::cout << "Iter(Er)  : " << g_Erosion_Iteration << " Iter(Dil)  : " << g_Dilation_Iteration << "\n";
	if (g_Is_Open) {
		cv::Mat eroded_Image;
		cv::erode(*input_Image, eroded_Image, g_Eroded_Kernel, cv::Point(-1, -1),
			g_Erosion_Iteration);
		cv::imshow("g_Eroded_Win_Name", eroded_Image);
#if 0	
		cv::Mat dilated_Image;
		cv::dilate(eroded_Image, dilated_Image, g_Dilated_Kernel, cv::Point(-1, -1),
			g_Dilation_Iteration);

		cv::imshow(g_Dilated_Erosion_Win_Name, dilated_Image);
#endif	
	}
	else {
		cv::Mat dilated_Image;
		cv::dilate(*input_Image, dilated_Image, g_Dilated_Kernel, cv::Point(-1, -1),
			g_Dilation_Iteration);
		cv::imshow("g_Dilated_Win_Name", dilated_Image);
#if 0		
		cv::Mat eroded_Image;
		cv::erode(dilated_Image, eroded_Image, g_Eroded_Kernel, cv::Point(-1, -1),
			g_Erosion_Iteration);


		cv::imshow(g_Eroded_Dilated_Win_Name, eroded_Image);
#endif	
	}
}

void erosion_Kernel_Size_Method(int pos, void* Userdata) {
	
	g_Eroded_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(2 * g_Erosion_Kernel_Size + 1, 2 * g_Erosion_Kernel_Size + 1),
		cv::Point(g_Erosion_Kernel_Size, g_Erosion_Kernel_Size));

	dilation_Erosion(Userdata);
}

void erosion_Iteration_Method(int pos, void* Userdata) {
	dilation_Erosion(Userdata);
}

void dilation_Kernel_Size_Method(int pos, void* Userdata) {
	g_Dilated_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(2 * g_Dilation_Kernel_Size + 1, 2 * g_Dilation_Kernel_Size + 1),
		cv::Point(g_Dilation_Kernel_Size, g_Dilation_Kernel_Size));
	dilation_Erosion(Userdata);
}

void dilation_Iteration_Method(int pos, void* Userdata) {
	dilation_Erosion(Userdata);

}

void morph_Shapes_Method(int pos, void* Userdata) {
	switch (pos) {
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
		cv::Size(2 * g_Dilation_Kernel_Size + 1, 2 * g_Dilation_Kernel_Size + 1), cv::Point(-1, -1));
	g_Eroded_Kernel = cv::getStructuringElement(g_MorphShapes,
		cv::Size(2 * g_Erosion_Kernel_Size + 1, 2 * g_Erosion_Kernel_Size + 1), cv::Point(-1, -1));

	dilation_Erosion(Userdata);
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

void thresholding_experiment(const cv::Mat& image) {
	double min, max;
	cv::minMaxLoc(image, &min, &max);
	double threshold = min + ((max - min) / 2);
	double max_val = 255;
	cv::Mat dst;

	while (1) {
		threshold = threshold + 0.2;
		cv::threshold(image, dst, threshold, max_val, cv::THRESH_BINARY);
	}
}

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

void perform_Erosion(const cv::Mat& image, bool Erosion) {
	int e_KSize = 1;
	int iteration = 1;
	int k;
	bool i_flag=0;
	while (1) {
		cv::Mat k_Sz = cv::getStructuringElement(cv::MORPH_ELLIPSE,
			cv::Size(e_KSize, e_KSize));
		cv::Mat out;
		if (i_flag)
			iteration++;
		else
			e_KSize++;
		std::cout << "Iteration: " << iteration << " KSize: " << e_KSize << "\n";
		if (Erosion)
			cv::erode(image, out, k_Sz, cv::Point(-1, -1), iteration);
		else
			cv::dilate(image, out, k_Sz, cv::Point(-1, -1), iteration);

		cv::imshow("Erorsion", out);
		k = cv::waitKey() & 0xFF;
		if (k == 27) {
			i_flag = !i_flag;
		}
	}
}

void apply_Erosion_And_Dilation(cv::Mat& image, const int erosion_KSize, const int erosion_Iter,
	const int dilation_KSize, const int dilation_Iter, bool open) {
	cv::Mat erosion_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(erosion_KSize, erosion_KSize));
	cv::Mat dilation_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(dilation_KSize, dilation_KSize));

	if (open) {
		cv::erode(image, image, erosion_Kernel, cv::Point(-1, -1),
			erosion_Iter);
		cv::dilate(image, image, dilation_Kernel, cv::Point(-1, -1),
			dilation_Iter);
	}
	else {
		cv::dilate(image, image, dilation_Kernel, cv::Point(-1, -1),
			dilation_Iter);
		cv::erode(image, image, erosion_Kernel, cv::Point(-1, -1),
			erosion_Iter);
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
	std::string image_Name = "CoinsB.png";
	cv::Mat image = cv::imread(image_Name);
//	cv::Size scale_Down_Size(image.size().width / 3, image.size().height / 3);
//	cv::resize(image, image,scale_Down_Size, 0.0, 0.0, cv::INTER_LINEAR);
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
	cv::imshow("imageGray", imageGray);
#if 0
	cv::Mat imageB(cv::Size(image.size()), CV_8UC1);
	cv::Mat imageG(cv::Size(image.size()), CV_8UC1);
	cv::Mat imageR(cv::Size(image.size()), CV_8UC1);
	convert_BGR_To_Single_Channel(image, imageB, imageG, imageR);
	cv::imshow("imageB", imageB);
	cv::imshow("imageG", imageG);
	cv::imshow("imageR", imageR);
#endif

	//Perform Thresholding
	//thresholding_experiment(imageGray);
	cv::Mat dst;
	cv::threshold(imageGray, dst, 160.0, 255, cv::THRESH_BINARY);
	cv::imshow("Thresholded", dst);

	//Removing white noises from coin.
	//To do that applying Erosion, where it removes white pixels in coins.
	#if 1
	cv::Mat pltImg;

	//First remove black 
	int d_KSize = 2;
	int d_Iteration = 3;
	cv::Mat d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(dst, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);
	cv::Mat pltImg_1 = pltImg.clone();

	int e_KSize = 2;
	int e_Iteration = 2;
	cv::Mat e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(pltImg, pltImg, e_Kernel, cv::Point(-1, -1), e_Iteration);

	d_KSize = 3;
	d_Iteration = 2;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);
	

	e_Iteration = 5;
	e_KSize = 5;
	e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(pltImg, pltImg, e_Kernel, cv::Point(-1, -1), e_Iteration);
	
	
	d_KSize = 5;
	d_Iteration = 5;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);
	
	//added now
	d_KSize = 3;
	d_Iteration = 3;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);

	e_KSize = 4;
	e_Iteration = 18;
	e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(pltImg, pltImg, e_Kernel, cv::Point(-1, -1), e_Iteration);

	d_KSize = 5;
	d_Iteration = 14;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);

	
	//perform_Morphology(pltImg, 1);

	
	
	
	
#else
	cv::Mat pltImg;
	int e_KSize    = 3;
	int e_Iteration = 1;
	cv::Mat e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE ,
		cv::Size(e_KSize, e_KSize));
	cv::erode(dst,pltImg, e_Kernel, cv::Point(-1, -1), e_Iteration);
	cv::imshow("Eroded", pltImg);

	//By removing white noises on coins, it increased black pixels.
	//To remove black pixels, perfrom dilation.
	
	int d_KSize = 4;
	int d_Iteration = 1;
	cv::Mat d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(d_KSize, d_KSize));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);
	cv::imshow("Dilated", pltImg);

	//Removing all white noises on coins. 
	
	e_Iteration = 2;
	e_KSize = 4;
	e_Kernel = cv::getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(pltImg, pltImg, e_Kernel, cv::Point(-1, -1), e_Iteration);
	cv::imshow("Eroded-1", pltImg);

	//Removing black pixels on background
	
	d_KSize = 5;
	d_Iteration = 2;
	d_KSize += 3;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);
	cv::imshow("Dilated-1", pltImg);

	e_Iteration = 3;
	e_KSize = 7;
	e_KSize += 3;
	e_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * e_KSize + 1, 2 * e_KSize + 1));
	cv::erode(pltImg, pltImg, e_Kernel, cv::Point(-1, -1), e_Iteration);
	cv::imshow("Eroded-2", pltImg);
	   
	d_KSize = 9;
	d_Iteration = 3;
	d_KSize += 3;
	d_Kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * d_KSize + 1, 2 * d_KSize + 1));
	cv::dilate(pltImg, pltImg, d_Kernel, cv::Point(-1, -1), d_Iteration);
	cv::imshow("Dilated-2", pltImg);
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

	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
	std::vector<cv::KeyPoint> keyPoint;
	detector->detect(pltImg, keyPoint);
	std::cout << "Number of Coins Detected: " << keyPoint.size() << "\n";

	cv::Mat coins_Detected_Image = image.clone();
	for (cv::KeyPoint point : keyPoint) {
		cv::circle(coins_Detected_Image, point.pt, 1, cv::Scalar(0, 0, 0), 3);
		cv::circle(coins_Detected_Image, point.pt, point.size/2.0, cv::Scalar(0, 0, 255), 3);
	}
	cv::imshow("Coins Detected", coins_Detected_Image);

	cv::Mat connected_Image;
	cv::bitwise_not(pltImg, connected_Image);

	int nComponenets = cv::connectedComponents(connected_Image, connected_Image);
	std::cout << "Number of connected components detected = " << nComponenets << "\n";
	cv::Mat colorMap = displayConnectedComponents(connected_Image);
	cv::imshow("Colormap", colorMap);
	
	std::vector<std::vector<cv::Point>> contours_2;
	std::vector<cv::Vec4i> hierarchy_2;

	cv::findContours(pltImg, contours_2, hierarchy_2, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	std::cout << "-----------------------------------------\n";
	std::cout << "Number of contours found = " << contours_2.size() << "\n";

	cv::Mat contour_Image_2 = image.clone();
	cv::drawContours(contour_Image_2, contours_2, -1, cv::Scalar(0, 0, 0), 1);
	cv::imshow("contour_image", contour_Image_2);

	std::vector<std::vector<cv::Point>> contour_Ext_2;
	cv::findContours(pltImg, contour_Ext_2, hierarchy_2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat contour_Ext_Image_2 = image.clone();
	cv::drawContours(contour_Ext_Image_2, contour_Ext_2, -1, cv::Scalar(0, 255, 0), 1);
	cv::imshow("contour_external_image", contour_Ext_Image_2);

	std::vector<std::pair<double, int>> contours_Container(contours_2.size());
	size_t count = 0;
	for (const std::vector<cv::Point> &contour_Pts : contours_2) {
		double area = cv::contourArea(contour_Pts);
		double perimeter = cv::arcLength(contour_Pts, true);
		contours_Container[count] = { area,  count };
		++count;
		std::cout << "Contour #" << count << " has area = " << area << " and perimeter = " << perimeter << "\n";
	}
	std::sort(contours_Container.begin(), contours_Container.end(),
		[](std::pair<double, int>& a, 
			std::pair<double, int>& b) {
			return a.first < b.first;
		});

	std::cout << "Maximum area of contour = " << contours_Container[count - 1].first << "\n";
	contours_Container.erase(contours_Container.end() - 1, contours_Container.end());
	cv::Mat cloned_Image = image.clone();
	for (const auto& local_ref : contours_Container) {
		cv::drawContours(cloned_Image, contours_2, local_ref.second, 
			cv::Scalar(0, 255, 0), 3);
	}
	cv::imshow("Max_area_removed",cloned_Image);

	count = 0;
	for (const auto& local_ref : contours_Container) {
		double perimeter = cv::arcLength(contours_2[local_ref.second], true);
		++count;
		std::cout << "Contour #" << count << " has area = " << local_ref.first << " and perimeter = " << perimeter << "\n";
	}
	contours_Container.erase(contours_Container.begin(), contours_Container.begin()+2);
	cloned_Image = image.clone();
	count = 0;
	std::vector<std::pair<cv::Point2f, float>> circle_Container(contours_Container.size());
	for (const auto& local_ref : contours_Container) {
		cv::drawContours(cloned_Image, contours_2, local_ref.second,
			cv::Scalar(255, 0, 0), 3);
		cv::Point2f point;
		float radius;
		
		cv::minEnclosingCircle(contours_2[local_ref.second], point, radius);
		circle_Container[count] = {point, radius};
		cv::circle(cloned_Image, point, 2, cv::Scalar(255, 0, 0), 3);
		const std::string text_Num = std::to_string(++count);
		cv::putText(cloned_Image, text_Num, point, cv::FONT_HERSHEY_SIMPLEX, 1,
			cv::Scalar(0, 0, 255), 3);
	}
	cv::imshow("number of coins", cloned_Image);

	cloned_Image = image.clone();

	for (const auto& local_Circle : circle_Container) {
		cv::circle(cloned_Image, local_Circle.first, 2, cv::Scalar(0, 0, 255), 3);
		cv::circle(cloned_Image, local_Circle.first, local_Circle.second, cv::Scalar(255, 0, 0), 3);
	}
	cv::imshow("Final Image", cloned_Image);

	cv::waitKey();
	cv::destroyAllWindows();
	return 0;
}
