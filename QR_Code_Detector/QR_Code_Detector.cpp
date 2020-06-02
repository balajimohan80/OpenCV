#include<iostream>
#include<string>

#include<opencv2/opencv.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

void Add_Line(cv::Mat& QR_Quadrangle_Vertex, cv::Mat& BGR_Input_Image) {
	int n = QR_Quadrangle_Vertex.rows;
	cv::Scalar B_Color(255, 0, 0);
	
	for (int i = 0; i < n; i++)
	{
		//Convert QR_Quadrangle_Vertex Co-ordinates from float to Integer.
		cv::Point Pt1(static_cast<int>(QR_Quadrangle_Vertex.at<float>(i,0)),
			static_cast<int>(QR_Quadrangle_Vertex.at<float>(i, 1)));

		cv::Point Pt2(static_cast<int>(QR_Quadrangle_Vertex.at<float>((i+1)%n, 0)),
			static_cast<int>(QR_Quadrangle_Vertex.at<float>((i + 1) % n, 1)));
		cv::line(BGR_Input_Image, Pt1, Pt2, B_Color, 3, cv::LINE_AA);
	}
}

int main() {
	const std::string Image_Name("IDCard-Satya.png");
	//const std::string Image_Name("IDCard-Template.png");
	cv::Mat Input_Image = cv::imread(Image_Name, -1);
	cv::Mat BGR_Input_Image;
	cv::Mat Alpha_Input_Image;
	cv::QRCodeDetector QRDetector;
	
	//Image to QRdector.detect function only accepts GRAY or BGR image
	if (Input_Image.channels() > 3) {
		BGR_Input_Image.create(Input_Image.size(), CV_8UC3);
		Alpha_Input_Image.create(Input_Image.size(), CV_8UC1);
		//Now we need split BGR & Alpha channels from Input_Image.
		//Create a Iterator for 4-Channels Input Image
		cv::Mat_<cv::Vec<unsigned char, 4>>::const_iterator Image_Begin =
			Input_Image.begin<cv::Vec<unsigned char, 4>>();
		cv::Mat_<cv::Vec<unsigned char, 4>>::const_iterator Image_End =
			Input_Image.end<cv::Vec<unsigned char, 4>>();
		//Create a Iterator for 3-channels(BGR) Seperate Image;
		cv::Mat_<cv::Vec<unsigned char, 3>>::iterator Image_BGR_Begin =
			BGR_Input_Image.begin<cv::Vec<unsigned char, 3>>();
		//Create a Iterator for 1-Channel Alpha Image
		cv::Mat_<cv::Vec<unsigned char, 1>>::iterator Image_Alpha_Begin =
			Alpha_Input_Image.begin<cv::Vec<unsigned char, 1>>();

		
		//Split BGR & Alpha Channels
		while (Image_Begin != Image_End) {
			(*Image_BGR_Begin)[0] = (*Image_Begin)[0];
			(*Image_BGR_Begin)[1] = (*Image_Begin)[1];
			(*Image_BGR_Begin++)[2] = (*Image_Begin)[2];
			(*Image_Alpha_Begin++)[0] = (*Image_Begin++)[3];
		}


	}
	else {
		BGR_Input_Image = Input_Image.clone();
	}

	/* If QR decoder detects, it produces a output which has where exactly
	   QR code starts. 
	   A------B
	   |      |
	   |      |
	   C------D
	   Quadrangle Vertex
	   But Quadrangle Vertex XY Co-ordinates is in the form of cv::Mat and floating point
	 */
	cv::Mat QR_Quadrangle_Points;
	cv::Mat QR_Binarized_Image;
	std::string Ret = QRDetector.detectAndDecode(BGR_Input_Image, QR_Quadrangle_Points, QR_Binarized_Image);
	if (Ret.length() > 0) {
		std::cout << "QR Code O/P: " << Ret << "\n";
		Add_Line(QR_Quadrangle_Points, BGR_Input_Image);
	} else {
		std::cout << "QR has no code\n";
	}

	cv::imshow(Image_Name, BGR_Input_Image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}