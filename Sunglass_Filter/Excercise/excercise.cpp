#include<iostream>
#include<vector>

#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>

class Image_Mask {
private:
	template<typename T1>
	void Mask_BGR_With_Alpha(const cv::Mat& Masked_Alpha_Image, cv::Mat& Output_Image, cv::Vec<T1, 3> & Default_Pix_Val) {
		cv::Mat_<cv::Vec<T1, 1>>::const_iterator Masked_Alpha_Begin = Masked_Alpha_Image.begin<cv::Vec<T1, 1>>();
		cv::Mat_<cv::Vec<T1, 1>>::const_iterator Masked_Alpha_End = Masked_Alpha_Image.end<cv::Vec<T1, 1>>();
		cv::Mat_<cv::Vec<T1, 3>>::iterator Output_BGR_Begin = Output_Image.begin<cv::Vec<T1, 3>>();

		while (Masked_Alpha_Begin != Masked_Alpha_End) {
			if ((*Masked_Alpha_Begin)[0]) {
				*Output_BGR_Begin = Default_Pix_Val;
			}
			Output_BGR_Begin++;
			Masked_Alpha_Begin++;
		}
	}

	template<typename T1>
	void Split_Alpha_and_BGR(const cv::Mat& BGRA, cv::Mat& BGR, cv::Mat& Alpha) {
		cv::Mat_<cv::Vec<T1, 4>>::const_iterator BGRA_CH_Begin = BGRA.begin<cv::Vec<T1, 4>>();
		cv::Mat_<cv::Vec<T1, 3>>::iterator BGR_CH_Begin = BGR.begin<cv::Vec<T1, 3>>();
		cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_Begin = Alpha.begin<cv::Vec<T1, 1>>();
		cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_End = Alpha.end<cv::Vec<T1, 1>>();

		while (Alpha_CH_Begin != Alpha_CH_End) {
			(*BGR_CH_Begin)[0] = (*BGRA_CH_Begin)[0];
			(*BGR_CH_Begin)[1] = (*BGRA_CH_Begin)[1];
			(*BGR_CH_Begin++)[2] = (*BGRA_CH_Begin)[2];
			(*Alpha_CH_Begin++)[0] = (*BGRA_CH_Begin++)[3];
		}
	}

	template<typename T1>
	void Multiply(cv::Mat& BGR_Ch, cv::Mat& Alpha_Channel) {
		cv::Mat_<cv::Vec<T1, 3>>::iterator BGR_CH_Begin = BGR_Ch.begin<cv::Vec<T1, 3>>();
		cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_Begin = Alpha_Channel.begin<cv::Vec<T1, 1>>();
		cv::Mat_<cv::Vec<T1, 1>>::iterator Alpha_CH_End = Alpha_Channel.end<cv::Vec<T1, 1>>();

		while (Alpha_CH_Begin != Alpha_CH_End) {
			(*BGR_CH_Begin)[0] *= (*Alpha_CH_Begin)[0];
			(*BGR_CH_Begin)[1] *= (*Alpha_CH_Begin)[0];
			(*BGR_CH_Begin++)[2] *= (*Alpha_CH_Begin++)[0];
		}

	}


public:
	static void Apply_Mask(cv::Mat& In_BGR_Image, cv::Mat& M_BGRA_Image, cv::Range X, cv::Range Y, bool Mask_Not_Apply) {
		static Image_Mask M;
		In_BGR_Image.convertTo(In_BGR_Image, CV_32FC3);
		In_BGR_Image = In_BGR_Image / 255.0;

		M_BGRA_Image.convertTo(M_BGRA_Image, CV_32FC4);
		double Re_Size_X = static_cast<double>(X.end - X.start) / static_cast<double>(M_BGRA_Image.size().width);
		double Re_Size_Y = static_cast<double>(Y.end - Y.start) / static_cast<double>(M_BGRA_Image.size().height);
		cv::resize(M_BGRA_Image, M_BGRA_Image, cv::Size(), Re_Size_X, Re_Size_Y);
		M_BGRA_Image = M_BGRA_Image / 255.0;

	
		//Split BGR & Alpha channel
		cv::Mat M_BGR_Image(M_BGRA_Image.size(), CV_32FC3);
		cv::Mat M_Alpha_Image(M_BGRA_Image.size(), CV_32FC1);
		M.Split_Alpha_and_BGR<float>(M_BGRA_Image, M_BGR_Image, M_Alpha_Image);
		M.Multiply<float>(M_BGR_Image, M_Alpha_Image);
	
		cv::Mat ROI_Image = In_BGR_Image(Y, X);
		if (Mask_Not_Apply)
			cv::add(ROI_Image, M_BGR_Image, ROI_Image);
		else
			M.Mask_BGR_With_Alpha<float>(M_Alpha_Image, ROI_Image, cv::Vec3f { 0.172, 0.172, 0.172 });

		In_BGR_Image = In_BGR_Image * 255;
		In_BGR_Image.convertTo(In_BGR_Image, CV_8UC3);
	}
	
};

int main() {
	const std::string Mustache = "Mustache.png";
	cv::Mat Input_Image = cv::imread("musk.jpg");
	cv::Mat Sunglass_Image = cv::imread("sunglass.png", -1);
	cv::Mat Mustache_Image = cv::imread(Mustache, -1);

	cv::Range X(130, 448);
	cv::Range Y(140, 250);
	Image_Mask::Apply_Mask(Input_Image, Sunglass_Image, X, Y, true);
	cv::Mat Mustache_ROI = Mustache_Image(cv::Range(35, 290), cv::Range(30, 762));
	X.start = 198;
	X.end   = 383;
	Y.start = 255;
	Y.end   = 302;
	Image_Mask::Apply_Mask(Input_Image, Mustache_ROI, X, Y, false);
	cv::imshow("Output_Image", Input_Image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}



