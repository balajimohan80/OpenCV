#include<iostream>
#include<vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

std::string Get_Image_Size(const cv::Mat& Image) {
	std::string Ret = "H: ";
	Ret += std::to_string(Image.size().height);
	Ret += " W: ";
	Ret += std::to_string(Image.size().width);
	Ret += " CH: ";
	Ret += std::to_string(Image.channels());
	return Ret;
}

int main() {
	//Load the Musk Image
	cv::Mat Musk_Image = cv::imread("musk.jpg");
	Musk_Image.convertTo(Musk_Image, CV_32FC3);
	Musk_Image = Musk_Image / 255.0;

	//Load a Sunglass
	//Read a Sunglass image with Alpha Channel.
	//Therefor Total no of Channels are 4.
	cv::Mat SunGlass_Image = cv::imread("sunglass.png", -1);
	SunGlass_Image.convertTo(SunGlass_Image, CV_32FC4);
	SunGlass_Image = SunGlass_Image / 255.0;
	cv::resize(SunGlass_Image, SunGlass_Image, cv::Size(), 0.5, 0.5);
	
	const std::string Name_1 = "musk " + Get_Image_Size(Musk_Image);
	const std::string Name_2 = "sunglass "+ Get_Image_Size(SunGlass_Image);

	cv::namedWindow(Name_1);
	cv::imshow(Name_1, Musk_Image);
	cv::namedWindow(Name_2);
	cv::imshow(Name_2, SunGlass_Image);

	
	std::vector<cv::Mat> Glass_RGBA_Ch(4); /* B, G, R & A Channels */
	std::vector<cv::Mat> Glass_RGB_Ch(3); /* Alpha Channel */
	cv::split(SunGlass_Image, Glass_RGBA_Ch);

	std::string Name_3 = "RGB";
	//Store only B, G, R channels alone
	cv::namedWindow(Name_3);
	for (int i = 0; i < 3; i++) {
		Glass_RGB_Ch[i] = Glass_RGBA_Ch[i];
	}

	cv::Mat Glass_BGR(SunGlass_Image.size(), CV_32FC3);
	cv::Mat GLass_Mask1(SunGlass_Image.size(), CV_32FC1);

	//Split & Copy B, G & R Channels
	cv::Mat_<cv::Vec3f>::iterator BGR_Begin = Glass_BGR.begin<cv::Vec3f>();
	cv::Mat_<cv::Vec3f>::iterator BGR_End   = Glass_BGR.end<cv::Vec3f>();
	cv::Mat_<cv::Vec<float, 1>>::iterator B_Itr =
		Glass_RGB_Ch[0].begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::iterator G_Itr =
		Glass_RGB_Ch[1].begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::iterator R_Itr =
		Glass_RGB_Ch[2].begin<cv::Vec<float, 1>>();

	int i = 0;
	//Copying B, G & R channel Pixel values
	while (BGR_Begin != BGR_End) {
		(*BGR_Begin)[0] = (*B_Itr)[0];
		(*BGR_Begin)[1] = (*G_Itr)[0];
		(*BGR_Begin)[2] = (*R_Itr)[0];
		++BGR_Begin;
		++B_Itr;
		++G_Itr;
		++R_Itr;
	}


	std::string Name_4("BGR_GLASS");
	cv::imshow(Name_4, Glass_BGR);

	//4th channel is ALPHA Channel
	cv::Mat_<cv::Vec<float, 1>>::iterator Alpha_Begin =
		GLass_Mask1.begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::iterator Alpha_End =
		GLass_Mask1.end<cv::Vec<float, 1>>();

	cv::Mat_<cv::Vec<float, 1>>::iterator A_Itr =
		Glass_RGBA_Ch[3].begin<cv::Vec<float, 1>>();
	//Copying Alpha Channel pixels
	while (Alpha_Begin != Alpha_End) {
		(*Alpha_Begin)[0] = (*A_Itr)[0];
		++Alpha_Begin;
		++A_Itr;
	}

	std::string Name_5("GLass_Mask");
	cv::imshow(Name_5, GLass_Mask1);
	//Clone a Musk face
	cv::Mat Face_With_Glass_Naive = Musk_Image.clone();
#if 0
	cv::Mat Face_ROI = Face_With_Glass_Naive(cv::Range(150, 250), cv::Range(140, 440));
	Glass_BGR.copyTo(Face_ROI);
#else
	std::string Name_6("Face_ROI");
	cv::imshow(Name_6, Face_With_Glass_Naive);
	cv::waitKey(0);
	//Create a Musk's Face ROI and copy a Glass image
	//But this is Naive
	cv::Mat Face_ROI = Face_With_Glass_Naive(cv::Range(150, 150 + Glass_BGR.size().height),
		cv::Range(140, 140 + Glass_BGR.size().width));
	Glass_BGR.copyTo(Face_ROI);
#endif
	cv::imshow(Name_6, Face_With_Glass_Naive);


	cv::waitKey(0);

	cv::destroyAllWindows();
	return 0;
}