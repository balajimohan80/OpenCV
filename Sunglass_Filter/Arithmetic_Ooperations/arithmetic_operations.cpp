#include<iostream>
#include<vector>

#include<opencv2/opencv.hpp>
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
	cv::Mat Musk_Image = cv::imread("musk.jpg");
	Musk_Image.convertTo(Musk_Image, CV_32FC3);
	Musk_Image = Musk_Image / 255.0;

	cv::Mat Sun_Glass = cv::imread("sunglass.png", -1);
	Sun_Glass.convertTo(Sun_Glass, CV_32FC4);
	Sun_Glass = Sun_Glass / 255.0;
	cv::resize(Sun_Glass, Sun_Glass, cv::Size(), 0.5, 0.5);
	const std::string Name_1 = "Musk_Image";
	cv::imshow(Name_1, Musk_Image);

	const std::string Name_2 = "Sun_Glass";
	cv::imshow(Name_2, Sun_Glass);

	//Split Alpha & BGR channels
	cv::Mat Glass_Alpha_Channel(Sun_Glass.size(), CV_32FC1);
	cv::Mat Glass_BGR_Channels(Sun_Glass.size(), CV_32FC3);

	cv::Mat_<cv::Vec<float, 3>>::iterator Glass_BGR_Begin =
		Glass_BGR_Channels.begin<cv::Vec<float, 3>>();
	cv::Mat_<cv::Vec<float, 1>>::iterator Glass_Alpha_Begin =
		Glass_Alpha_Channel.begin<cv::Vec<float, 1>>();

	cv::Mat_<cv::Vec<float, 4>>::const_iterator Sun_Glass_Begin =
		Sun_Glass.begin < cv::Vec<float, 4>>();
	cv::Mat_<cv::Vec<float, 4>>::const_iterator Sun_Glass_End =
		Sun_Glass.end < cv::Vec<float, 4>>();

	while (Sun_Glass_Begin != Sun_Glass_End) {
		(*Glass_BGR_Begin)[0]   = (*Sun_Glass_Begin)[0];
		(*Glass_BGR_Begin)[1]   = (*Sun_Glass_Begin)[1];
		(*Glass_BGR_Begin)[2]   = (*Sun_Glass_Begin)[2];
		(*Glass_Alpha_Begin)[0] = (*Sun_Glass_Begin)[3];
		++Sun_Glass_Begin;
		++Glass_BGR_Begin;
		++Glass_Alpha_Begin;
	}
	cv::imshow("Glass_BGR_Channels", Glass_BGR_Channels);
	cv::imshow("Glass_Alpha_Channel", Glass_Alpha_Channel);
	//Make the dimension of the mask same as the Input Image.
	//Input Image is a 3-channels(BGR) image. so we create 3 Sun-Glass Channels masks
	//Create Mask for each channels(BGR)
	cv::Mat Glass_3CH_Mask(Glass_Alpha_Channel.size(), CV_32FC3);
	std::vector<cv::Mat> Mask_Vec(3, Glass_Alpha_Channel.clone());
	
	cv::Mat_<cv::Vec<float, 3>>::iterator Glass_3CH_Begin =
		Glass_3CH_Mask.begin<cv::Vec<float, 3>>();
	cv::Mat_<cv::Vec<float, 3>>::iterator Glass_3CH_End =
		Glass_3CH_Mask.end<cv::Vec<float, 3>>();
	Glass_Alpha_Begin = Glass_Alpha_Channel.begin<cv::Vec<float, 1>>();
	//Copy the Alpha pixels to BGR channels
	while (Glass_3CH_Begin != Glass_3CH_End) {
		(*Glass_3CH_Begin)[0] = (*Glass_Alpha_Begin)[0];
		(*Glass_3CH_Begin)[1] = (*Glass_Alpha_Begin)[0];
		(*Glass_3CH_Begin)[2] = (*Glass_Alpha_Begin)[0];

		++Glass_3CH_Begin;
		++Glass_Alpha_Begin;
	}
	
	//Clone an Input Image
	cv::Mat Face_With_Glasss = Musk_Image.clone();
	//Take ROI(Only Eye part) of Input Image
	const int Y1 = 140;
	const int Y2 = Y1 + Sun_Glass.size().height;
	const int X1 = 130;
	const int X2 = X1 + Sun_Glass.size().width;
	cv::Mat Face_ROI_Eye = Face_With_Glasss(cv::Range(Y1, Y2), cv::Range(X1, X2));
	
	//Split a ROI(Eye part) Region
	std::vector<cv::Mat> Eye_ROI_BGR_Channels(3);
	Eye_ROI_BGR_Channels[0] = cv::Mat(Face_ROI_Eye.size(), CV_32FC1);
	Eye_ROI_BGR_Channels[1] = cv::Mat(Face_ROI_Eye.size(), CV_32FC1);
	Eye_ROI_BGR_Channels[2] = cv::Mat(Face_ROI_Eye.size(), CV_32FC1);

	cv::Mat_<cv::Vec<float, 3>>::const_iterator Face_ROI_Eye_Begin =
		Face_ROI_Eye.begin<cv::Vec<float, 3>>();
	cv::Mat_<cv::Vec<float, 3>>::const_iterator Face_ROI_Eye_End =
		Face_ROI_Eye.end<cv::Vec<float, 3>>();

	cv::Mat_<cv::Vec<float, 1>>::iterator Eye_ROI_B_Begin =
		Eye_ROI_BGR_Channels[0].begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::iterator Eye_ROI_G_Begin =
		Eye_ROI_BGR_Channels[1].begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::iterator Eye_ROI_R_Begin =
		Eye_ROI_BGR_Channels[2].begin<cv::Vec<float, 1>>();

	while (Face_ROI_Eye_Begin != Face_ROI_Eye_End) {
		(*Eye_ROI_B_Begin)[0] = (*Face_ROI_Eye_Begin)[0];
		(*Eye_ROI_G_Begin)[0] = (*Face_ROI_Eye_Begin)[1];
		(*Eye_ROI_R_Begin)[0] = (*Face_ROI_Eye_Begin)[2];
		++Eye_ROI_B_Begin;
		++Eye_ROI_G_Begin;
		++Eye_ROI_R_Begin;
		++Face_ROI_Eye_Begin;
	}
	
	
	std::vector<cv::Mat> Masked_Eye_Channels(3);
	for (int i = 0; i < 3; i++) {
		cv::multiply(Eye_ROI_BGR_Channels[i], (1 - Mask_Vec[i]), 
			Masked_Eye_Channels[i]);
	}

	//Merge Masked_Eye_Channel to BGR Chnanel
	cv::Mat Masked_Eye_BGR_Channel(Face_ROI_Eye.size(), CV_32FC3);
	
	cv::Mat_<cv::Vec<float, 3>>::iterator Masked_Eye_BGR_Begin =
		Masked_Eye_BGR_Channel.begin<cv::Vec<float, 3>>();
	cv::Mat_<cv::Vec<float, 3>>::iterator Masked_Eye_BGR_End =
		Masked_Eye_BGR_Channel.end<cv::Vec<float, 3>>();

	cv::Mat_<cv::Vec<float, 1>>::const_iterator Masked_Eye_B_Begin =
		Masked_Eye_Channels[0].begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::const_iterator Masked_Eye_G_Begin =
		Masked_Eye_Channels[1].begin<cv::Vec<float, 1>>();
	cv::Mat_<cv::Vec<float, 1>>::const_iterator Masked_Eye_R_Begin =
		Masked_Eye_Channels[2].begin<cv::Vec<float, 1>>();

	while (Masked_Eye_BGR_Begin != Masked_Eye_BGR_End) {
		(*Masked_Eye_BGR_Begin)[0] = (*Masked_Eye_B_Begin++)[0];
		(*Masked_Eye_BGR_Begin)[1] = (*Masked_Eye_G_Begin++)[0];
		(*Masked_Eye_BGR_Begin)[2] = (*Masked_Eye_R_Begin++)[0];
		++Masked_Eye_BGR_Begin;
	}

	cv::imshow("BGR", Masked_Eye_BGR_Channel);

	cv::Mat MaskedGlass;
	cv::multiply(Glass_BGR_Channels, Glass_3CH_Mask, MaskedGlass);
	cv::imshow("MaskGlass", MaskedGlass);
	
	cv::Mat EyeRoiFinal;
	cv::add(Masked_Eye_BGR_Channel, MaskedGlass, EyeRoiFinal);
	cv::imshow("EyeRoiFinal", EyeRoiFinal);

	EyeRoiFinal.copyTo(Face_ROI_Eye);
	cv::imshow("Face_With_Glass", Face_With_Glasss);
	cv::waitKey(0);

	cv::destroyAllWindows();
	return 0;
}