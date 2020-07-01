#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>

#include<iostream>
#include<string>


/* Structuring Element used in dilation & erosion operations.
   Structuring Element(Kernels) are used to modify the shape of the blobs.
   It used to scan the image and modify the pixels on the basis of some rule/algorithm

   OpenCV supports 3 types of structuring elements:
   1. Ellipse/Circular Shape
   2. Rectangular Shape.
   3. Cross Shape.
*/
int main() {
	/* Implementing dilation operation using a elliptical structuring element.
	   1. Scan the kernel through the image. When you find a white pixel of overlapping with
	      the center  of the kernel, perform an OR operation of the kernel and the Pixel
		  Neighborhood.

	   2. Scan through the image and superimpose the kernel on the neighbothood of the pixel.
	      Perform an AND operation of the neighborwood with the kernel.
		  Replace the pixel value with the maximum value in the neighborhood given by the kernel.
	*/

	std::string image_Name = "DemoImage";
	//Create an Empty Matrix
	cv::Mat demoImage = cv::Mat::zeros(cv::Size(10, 10), CV_8U);
	std::cout << demoImage << "\n";
	cv::imshow(image_Name, demoImage);

	//Lets add somw white blobs
	demoImage.at<uchar>(0, 1) = 1;
	demoImage.at<uchar>(9, 1) = 1;
	demoImage.at<uchar>(8, 9) = 1;
	demoImage.at<uchar>(2, 2) = 1;
	demoImage(cv::Range(5, 8), cv::Range(5, 8)).setTo(1);
	std::cout << demoImage << "\n";

	//Create an Ellipse structuring Element
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	std::cout << element << "\n";
	const int kSize = element.size().height;

	const int height = element.size().height;
	const int width  = element.size().width;

	//Dilation from scratch: Method-1
	const int  border = kSize / 2;
	cv::Mat padded_Image = cv::Mat::zeros(cv::Size(height+border*2, width+border*2), CV_8UC1);
	cv::copyMakeBorder(padded_Image, padded_Image, border, border, border, border, cv::BORDER_CONSTANT, 0);
	//Compare cloned image with modified padded image (i.e) after executing for loop
	cv::Mat padded_Cloned_Image = padded_Image.clone();
	cv::Mat bitOR;
	for (int h_i = border; h_i < height + border; h_i++) {
		for (int w_i = border; w_i < width + border; w_i++) {
			if (demoImage.at<uchar>(h_i - border, w_i - border)) {
				std::cout << "White Pixel Found @" << h_i << "," << w_i << "\n";
				std::cout << padded_Image(cv::Range(h_i - border, h_i + border + 1),
					cv::Range(w_i - border, w_i + border + 1)) << std::endl;
				std::cout << element << "\n";
				cv::bitwise_or(padded_Image(cv::Range(h_i - border, h_i + border + 1),
					cv::Range(w_i - border, w_i + border + 1)), element, bitOR);
				std::cout << bitOR << "\n";
				std::cout << padded_Image(cv::Range(h_i - border, h_i + border + 1),
					cv::Range(w_i - border, w_i + border + 1)) << "\n";
				bitOR.copyTo(padded_Image(cv::Range(h_i - border, h_i + border + 1),
					cv::Range(w_i - border, w_i + border + 1)));
				std::cout << padded_Image << "\n";
			}
		}
	}
	
	cv::Mat dilatedEllipseKernel;
	cv::dilate(demoImage, dilatedEllipseKernel, element);
	std::cout << dilatedEllipseKernel << "\n";
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}