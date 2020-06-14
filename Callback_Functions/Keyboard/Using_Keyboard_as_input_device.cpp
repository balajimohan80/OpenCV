#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/* Highgui Module:
Highgui module was designed to test certain functionalities quickly and provide immediate results.
It provides easy interface to:
1. Create and manipulate windows that can display images and remember their content
2. Add Trackbars to the windows, handle simple mouse events as well as keyboard commands.
3. Read and write Images/video to/from disk
*/
#include <opencv2/highgui.hpp>

#include<string>
#include<iostream>


int main() {
	cv::VideoCapture Camera_Capture(0);
	const std::string E_Key("E is Pressed, Press Z or Esc Key");
	const std::string Z_Key("Z is Pressed, Press E or Esc key");
	cv::Point Point(100, 180);
	cv::Scalar G_Color(0, 255, 0);
	int Key = 0;

	if (!Camera_Capture.isOpened()) {
		std::cout << "Not able to open camera\n";
		return 0;
	}

	/* Displays text when 'e/E' or 'z/Z' is pressed.
	   On pressing ESC, the program terminates and the display close.
	*/

	while (1) {
		cv::Mat Frame;
		Camera_Capture >> Frame;
		if (Frame.empty()) {
			std::cout << "Frame is empty\n";
			break;
		}

		//ESC key is pressed
		if (Key == 27) {
			break;
		}

		if (Key == static_cast<int>('E') || Key == static_cast<int>('e')) {
			cv::putText(Frame, E_Key, Point, cv::FONT_HERSHEY_SIMPLEX, 1, G_Color, 3);
		}
		if (Key == static_cast<int>('Z') || Key == static_cast<int>('z')) {
			cv::putText(Frame, Z_Key, Point, cv::FONT_HERSHEY_SIMPLEX, 1, G_Color, 3);
		}
		cv::imshow("Image", Frame);
		//Waitkey time is increased to see display briefly
		Key = cv::waitKey(10000);
	}

	Camera_Capture.release();
	cv::destroyAllWindows();
	return 0;
}
