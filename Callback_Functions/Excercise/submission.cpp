#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <math.h>

const std::string Win_Name = "Face Annotation";

void Mouse_CallBack_Events(int event, int x, int y, int flags, void* userdata) {
	static cv::Point PT1;
	static cv::Point PT2;

	if (event == cv::MouseEventTypes::EVENT_LBUTTONUP) {
		PT2 = cv::Point(x, y);
		const std::string Face("Face.png");
		cv::Rect ROI(PT1, PT2);
		const cv::Scalar G_Color(0, 255, 0);
		cv::Mat *Image = static_cast<cv::Mat*>(userdata);
		cv::Mat Image_To_Write(*Image, ROI);
		cv::imwrite(Face, Image_To_Write);
		cv::rectangle(*Image, ROI, G_Color, 1, cv::LINE_AA);
	}
	else if (event == cv::MouseEventTypes::EVENT_LBUTTONDOWN) {
		PT1 = cv::Point(x, y);
	}

}

int main() {
	const std::string Image_Name("sample.jpg");
	const std::string Text("Choose top left corner, and drug.?(c->cancel, Esc->Quit)");
	const cv::Scalar W_Color(255, 255, 255);
	const cv::HersheyFonts Font = cv::FONT_HERSHEY_SIMPLEX;
	const cv::Point Text_PT(5, 15);
	cv::Mat Source = cv::imread(Image_Name);
	cv::Mat Clone_Image = Source.clone();
	cv::namedWindow(Win_Name, 1);
	cv::MouseCallback CallBack_Func = Mouse_CallBack_Events;
	void* User_Data = static_cast<void*>(&Source);
	cv::setMouseCallback(Win_Name, CallBack_Func, User_Data);
	int K = 0;
	cv::imshow(Win_Name, Source);
	cv::putText(Source, Text, Text_PT,
		Font, 0.4, W_Color, 1, cv::LINE_AA);
	while (K != 27) {
		//Press Escape to cancel
		cv::imshow(Win_Name, Source);

		K = cv::waitKey(20) & 0xFF;
		if (K == 99) {
			Clone_Image.copyTo(Source);
			cv::putText(Source, Text, Text_PT,
				Font, 0.4, W_Color, 1, cv::LINE_AA);
		}
	}
	cv::destroyAllWindows();
	return 0;
}