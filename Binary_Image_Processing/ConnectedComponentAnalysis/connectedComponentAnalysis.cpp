#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>

#include<iostream>
#include<string>
/* Connected components used to count the number of blobs(also called connected components)
   in a binary image. 
*/

int main() {
	std::string image_Name = "truth.png";
	cv::Mat image = cv::imread(image_Name, cv::IMREAD_GRAYSCALE);
	//A binary Image with 5 blobs corresponding to letters T,R,U,T,H
	//CCA allows us to apply a label to every blob.
	//CCA create a mask where all the pixels corresponding to the background are zero.
	//All pixels corresponding to the 1st blob T->1, R->2, U->3...
	
	//Threshold Image 
	cv::Mat binary_Thresh;
	cv::threshold(image, binary_Thresh, 127, 255, cv::THRESH_BINARY);

	//Find Connected Components
	cv::Mat imLabels;
	
	int nComponents = cv::connectedComponents(binary_Thresh, imLabels);
	std::cout << "nComponents:  " << nComponents << "\n";

	cv::Mat imLabelsCopy = imLabels.clone();

	cv::Point minLoc, maxLoc;
	double minVal, maxVal;

	//Find Min and Max Pixel values and their location
	cv::minMaxLoc(imLabels, &minVal, &maxVal, &minLoc, &maxLoc);

	std::cout << "MinPixelVal: " << minVal << " Location: " << minLoc << "\n";
	std::cout << "MaxPixelVal: " << maxVal << " Location: " << maxLoc << "\n";

	//Normalize the image so the min and max value is between 0 to 255
	imLabels = 255 * (imLabels - minVal) / (maxVal - minVal);

	imLabels.convertTo(imLabels, CV_8U);
	cv::imshow("CCI_Normalized", imLabels);
	
	imLabels = imLabelsCopy.clone();
	cv::imshow("imLabels==0", imLabels == 0);
	cv::imshow("imLabels==1", imLabels == 1);
	cv::imshow("imLabels==2", imLabels == 2);
	cv::imshow("imLabels==3", imLabels == 3);
	cv::imshow("imLabels==4", imLabels == 4);
	cv::imshow("imLabels==5", imLabels == 5);

	//Since it is a bit difficult to visualize the difference in intensity value in grayscale images, 
	//we apply a colormap so that grayscale values are converted to color for the purpose of display.

	//First, we normalize the image pixel values to 0 to 255. 
	//To achieve this we first find the min and max values in the image, 
	//and then normalize the image by subtracting the minand dividing by(max - min).
	//This normalizes the image to be between 0 and 1.
	//Finally 255 is multiplied to we get an image with values between 0 and 255. 
	//Finally, we apply a colormap on the labelled image

	/* What is a colormap ?
	   Let’s say we want to show the temperature in different parts of the United States on a map. 
	   We could overlay temperature data on top of a US map as a grayscale image — darker regions representing 
	   cooler temperatures, and brighter regions representing hotter regions. 
	   Not only is such a representation incredibly boring, it is a bad representation for two big reasons.

	   First, the human visual system is not optimized to measure small changes in grayscale intensities. 
	   We are way better at perceiving changes in color.
	   Second, we associate different meanings with different colors. 
	   It is much more meaningful to represent cooler temperatures using blue and warmer temperatures using red.
	   Temperature data is just one example, but there are several other cases 
	   where the data is single valued (grayscale), 
	   but it makes sense to convert it to color data for visualization. 
	   Other examples of data that are better visualized by pseudo-coloring are height, pressure, density, 
	   humidity so on and so forth.

	   OpenCV defines 12 colormaps that can be applied to a grayscale image using the function 
	   applyColorMap to produce a pseudocolored image. We will use COLORMAP_JET for our example.
	*/
	imLabels = imLabelsCopy.clone();

	cv::Point minLoc2, maxLoc2;
	double minVal2, maxVal2;
	cv::minMaxLoc(imLabels, &minVal2, &maxVal2, &minLoc2, &maxLoc2);

	//Normalize the image so the min value is 0 and max value is 255;
	imLabels = 255 * (imLabels - minVal2) / (maxVal2 - minVal2);
	imLabels.convertTo(imLabels, CV_8U);

	//Apply a color Map
	cv::Mat imColorMap_Image;
	cv::applyColorMap(imLabels, imColorMap_Image, cv::COLORMAP_JET);
	cv::imshow("ColorMapImage", imColorMap_Image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}