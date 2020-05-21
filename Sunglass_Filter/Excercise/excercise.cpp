#include<iostream>
#include<vector>

#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>

template<typename T1>
void Split_3_Channels(const cv::Mat& BGR_Image, std::vector<cv::Mat>& BGR_Channel) {
	cv::Mat_<cv::Vec<T1, 3>>::const_iterator BGR_Image_Begin = BGR_Image.begin<cv::Vec<T1, 3>>();
	cv::Mat_<cv::Vec<T1, 3>>::const_iterator BGR_Image_End   = BGR_Image.end<cv::Vec<T1, 3>>();

	cv::Mat_<cv::Vec<T1, 1>>::iterator B_Channel_Begin = BGR_Channel[0].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator G_Channel_Begin = BGR_Channel[1].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator R_Channel_Begin = BGR_Channel[2].begin<cv::Vec<T1, 1>>();

	while (BGR_Image_Begin != BGR_Image_End) {
		(*B_Channel_Begin++)[0] = (*BGR_Image_Begin)[0];
		(*G_Channel_Begin++)[0] = (*BGR_Image_Begin)[1];
		(*R_Channel_Begin++)[0] = (*BGR_Image_Begin)[2];
		++BGR_Image_Begin;
	}
}

template<typename T1>
void Merge_3_Channels(const std::vector<cv::Mat>& BGR_Channel, cv::Mat& BGR_Image) {
	cv::Mat_<cv::Vec<T1, 3>>::iterator BGR_Image_Begin = BGR_Image.begin<cv::Vec<T1, 3>>();
	cv::Mat_<cv::Vec<T1, 3>>::iterator BGR_Image_End = BGR_Image.end<cv::Vec<T1, 3>>();

	cv::Mat_<cv::Vec<T1, 1>>::const_iterator B_Channel_Begin = BGR_Channel[0].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::const_iterator G_Channel_Begin = BGR_Channel[1].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::const_iterator R_Channel_Begin = BGR_Channel[2].begin<cv::Vec<T1, 1>>();

	while (BGR_Image_Begin != BGR_Image_End) {
		(*BGR_Image_Begin)[0] = (*B_Channel_Begin++)[0];
		(*BGR_Image_Begin)[1] = (*G_Channel_Begin++)[0];
		(*BGR_Image_Begin)[2] = (*R_Channel_Begin++)[0];
		++BGR_Image_Begin;
	}
}

template<typename T1>
void Split_Alpha_And_BGR_Channel(const cv::Mat& Image, std::vector<cv::Mat>& BGR_Ch, cv::Mat& Alpha_Ch) {
	cv::Mat_<cv::Vec<T1, 1>>::iterator B_CH_Begin = BGR_Ch[0].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator G_CH_Begin = BGR_Ch[1].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator R_CH_Begin = BGR_Ch[2].begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_Begin = Alpha_Ch.begin<cv::Vec<T1, 1>>();

	cv::Mat_<cv::Vec<T1, 4>>::const_iterator Image_Begin = Image.begin<cv::Vec<T1, 4>>();
	cv::Mat_<cv::Vec<T1, 4>>::const_iterator Image_End   = Image.end<cv::Vec<T1, 4>>();

	while (Image_Begin != Image_End) {
		(*B_CH_Begin)[0]      = (*Image_Begin)[0];
		(*G_CH_Begin)[0]      = (*Image_Begin)[1];
		(*R_CH_Begin)[0]      = (*Image_Begin)[2];
		(*Alpha_CH_Begin)[0]  = (*Image_Begin)[3];
		++B_CH_Begin;
		++G_CH_Begin;
		++R_CH_Begin;
		++Alpha_CH_Begin;
		++Image_Begin;
	}

}

template<typename T1>
void Multiply(cv::Mat &BGR_Ch, cv::Mat& Alpha_Channel) {
	cv::Mat_<cv::Vec<T1, 3>>::iterator BGR_CH_Begin = BGR_Ch.begin<cv::Vec<T1, 3>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_Begin = Alpha_Channel.begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_End = Alpha_Channel.end<cv::Vec<T1, 1>>();

	while (Alpha_CH_Begin != Alpha_CH_End) {
		(*BGR_CH_Begin)[0] *= (*Alpha_CH_Begin)[0];
		(*BGR_CH_Begin)[1] *= (*Alpha_CH_Begin)[0];
		(*BGR_CH_Begin++)[2] *= (*Alpha_CH_Begin++)[0];
	}

}

template<typename T1>
void Split_Alpha_and_BGR(const cv::Mat& BGRA, cv::Mat& BGR, cv::Mat& Alpha) {
	cv::Mat_<cv::Vec<T1, 4>>::const_iterator BGRA_CH_Begin  = BGRA.begin<cv::Vec<T1, 4>>();
	cv::Mat_<cv::Vec<T1, 3>>::iterator BGR_CH_Begin   = BGR.begin<cv::Vec<T1, 3>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_Begin = Alpha.begin<cv::Vec<T1, 1>>();
	cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_End   = Alpha.end<cv::Vec<T1, 1>>();

	while (Alpha_CH_Begin != Alpha_CH_End) {
		(*BGR_CH_Begin)[0]   = (*BGRA_CH_Begin)[0];
		(*BGR_CH_Begin)[1]   = (*BGRA_CH_Begin)[1];
		(*BGR_CH_Begin++)[2]   = (*BGRA_CH_Begin)[2];
		(*Alpha_CH_Begin++)[0] = (*BGRA_CH_Begin++)[3];
	}
}

int main() {
	cv::Mat Input_Image = cv::imread("musk.jpg");
	Input_Image.convertTo(Input_Image, CV_32FC3);
	Input_Image = Input_Image / 255.0;

	cv::Mat Sunglass_Image  = cv::imread("sunglass.png", -1);
	Sunglass_Image.convertTo(Sunglass_Image, CV_32FC4);
	cv::resize(Sunglass_Image, Sunglass_Image, cv::Size(), 0.5, 0.5);
	Sunglass_Image = Sunglass_Image / 255.0;

	//Sunglass is a 4 Channels. 3 channels are BGR and 4th channel is ALPHA
	//Split BGR and Alpha Channel
	cv::Mat Sun_Glass_BGR(Sunglass_Image.size(), CV_32FC3);
	cv::Mat Sun_Glass_Alpha(Sunglass_Image.size(), CV_32FC1);
	Split_Alpha_and_BGR<float>(Sunglass_Image, Sun_Glass_BGR, Sun_Glass_Alpha);
	Multiply<float>(Sun_Glass_BGR, Sun_Glass_Alpha);


	const int Y1 = 140;
	const int Y2 = Y1 + Sunglass_Image.size().height;
	const int X1 = 130;
	const int X2 = X1 + Sunglass_Image.size().width;

	cv::Mat Face_ROI = Input_Image(cv::Range(Y1, Y2), cv::Range(X1, X2));
	cv::add(Face_ROI, Sun_Glass_BGR, Face_ROI);
	cv::imshow("Ouput_Image", Input_Image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}