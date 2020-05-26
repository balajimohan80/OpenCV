#include<iostream>
#include<string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


void Resize_Image(cv::Mat& Image, double Scaling_Ratio) {
	Image.convertTo(Image, CV_32FC3);
	cv::resize(Image, Image, cv::Size(), Scaling_Ratio, Scaling_Ratio, 1);
	Image.convertTo(Image, CV_8UC3);
}



cv::Mat Add_Text_Darshan_Image(const std::string& Image_Name, 
	const std::string &Text) {
	cv::Mat Image = cv::imread(Image_Name, cv::IMREAD_ANYCOLOR);
	Resize_Image(Image, 0.25);
	cv::Point Org(0, Image.size().height-20);
	int Font_Face = cv::FONT_HERSHEY_COMPLEX;
	double FontScale = 1.5;
	cv::Scalar G_Color( 0, 255, 0 );
	int Thickness = 2;
	int Line_Type = cv::LINE_AA;

	//Adjust FontHeight based on PixelHeight
	int Pixel_Height = 25;
	std::cout << "Actual FontScale: " << FontScale << " ";
	FontScale = cv::getFontScaleFromHeight(Font_Face, Pixel_Height, Thickness);
	std::cout << "Adjusted FontScale: " << FontScale << "\n";
	
	//Calculate width & height of the Text
	//To calculate Text size, following Parameters are needed
	//1. Text
	//2. Font type(Font_Face)
	//3. Font Scale(FontScale)
	//4. Thickness
	//5. baseLine
	int BaseLine = 0;
	cv::Size Text_Size = cv::getTextSize(Text, Font_Face, FontScale, Thickness, &BaseLine);
	std::cout << "Text Size: " << Text_Size << "\n";

	//Now we know Text Size.
	//Now we calculate X & Y Co-ordinates where Text wants to display in the Image.
	// We want Text to print in Center of the Image at bottom of the Image
	Org.x = (Image.size().width - Text_Size.width) / 2;
	Org.y = Image.size().height - BaseLine - 10;
	
	//Lets create a Rectangular Canvas 
	cv::Point Top_Rect_XY(Org.x, Image.size().height - Text_Size.height - 20);
	cv::Point Bottom_Rect_XY(Org.x + Text_Size.width, Org.y+10);
	cv::Scalar B_Color(0, 0, 0);
	cv::rectangle(Image, Top_Rect_XY, Bottom_Rect_XY, B_Color, -1, cv::LINE_AA);

	//Now Draw a line
	//To Draw a line, we need two XY Co-ordinates
	cv::Point Line_Start_XY_Coordinates(Org.x, Org.y);
	cv::Point Line_End_XY_Coordinates(Org.x+Text_Size.width, Org.y);
	cv::Scalar M_Line_Color(255, 0, 255);
	cv::line(Image, Line_Start_XY_Coordinates, Line_End_XY_Coordinates,
		M_Line_Color, 2, cv::LINE_AA);

	cv::putText(Image, Text, Org, Font_Face, FontScale, G_Color, Thickness, Line_Type);
	return Image;
}


int main() {
	const std::string Image_1 = "boy.jpg";
	cv::Mat Image_Input=cv::imread(Image_1, cv::IMREAD_COLOR);
	/* Text string to be drawn */
	std::string Text = "I am studying";
	/* Bottom-left Corner of the text string in the Image */
	cv::Point Org{20, 350};
	/* Font Types: 
	   FONT_HERSHEY_SIMPLEX
	   FONT_HERSHEY_PLAIN
	   FONT_HERSHEY_DUPLEX
	   FONT_HERSHEY_COMPLEX
	   FONT_HERSHEY_TRIPLEX
	   FONT_HERSHEY_COMPLEX_SMALL
	   FONT_HERHEY_SCRIPT_SIMPLEX
	   FONT_HERHEY_SCRIPT_COMPLEX
	 */
	int FontFace = cv::FONT_HERSHEY_COMPLEX; /* Normal Size Serif Font*/
	/*Font Scale factor that is multiplied by the font-specific base size*/
	double FontScale = 1.5;
	cv::Scalar B_Color{ 255, 0, 0 };
	int Font_Thickness = 1;
	int Line_Type = cv::LINE_AA;
	/* When true, the Image data origin is at the bottom-left corner.
	   Otherwise, it is at the top-left corner.
	*/
	bool Bottom_Left_Origin = false;
	cv::namedWindow(Image_1);
	cv::Mat Texted_Image = Image_Input.clone();
	cv::putText(Texted_Image, Text, Org, FontFace, FontScale, B_Color,
		Font_Thickness, Line_Type, Bottom_Left_Origin);
	cv::imshow(Image_1, Texted_Image);
	
	/* Following problems are given below for above image:
	1. Text looks pretty big. (If the image is large, font-scale 1.5
	   can look too small and vice-versa.
	2. Bottom part of the text "Studying" is slightly clipped.
	   This is because not sure how big the text will look on the Image.
	 */
	/* Get Font size based on text's Pixel height:
	   cv::getFontScaleFromHeight which calculates the fontscale with
	   the given font height and font type.
	*/
	// Instead of FontScale, use Pixelheight to determine FontScale
	int PixelHeight = 20;
	FontScale = cv::getFontScaleFromHeight(FontFace, PixelHeight, Font_Thickness);
	std::cout << "Computed FontScale: " << FontScale << "\n";

	cv::Mat Get_Font_Size_Image = Image_Input.clone();
	cv::putText(Get_Font_Size_Image, Text, Org,
		FontFace, FontScale, B_Color, Font_Thickness, Line_Type);
	cv::imshow("Adjusted_FontScale", Get_Font_Size_Image);

	/* In Cloned_Input_Image_1, we printed the text at the bottom.
	   To print text at the center of the image, we must know 
	   what is the area of text size it takes on the Image.
	   Also, its difficult to write text when the background is busy.
	   In this particular problem, we can create a Rectangular
	   Canvas and write the text on top of that.
	   cv::getTextSize will determine the hight and width of the text box.
	*/
	cv::Mat Image_Get_Text_Size = Image_Input.clone();
	const int Image_Height = Image_Get_Text_Size.size().height;
	const int Image_Width  = Image_Get_Text_Size.size().width;

	//Compute text box height & width & baseline
	int Base_Line = 0;
	cv::Size TextSize = cv::getTextSize(Text, FontFace, FontScale, 
		Font_Thickness, &Base_Line);
	const int Text_Width  = TextSize.width;
	const int Text_Height = TextSize.height;
	std::cout << "Text_Width  : " << Text_Width << "\n";
	std::cout << "Text_Height : " << Text_Height << "\n";
	std::cout << "Base_Line   : " << Base_Line << "\n";
	
	//Calculate the X-Y Co-ordinates of Text box Bottom Left corner
	//X-Co-ordinates will be such that text is centered.
	int X_Cordinates = (Image_Width - Text_Width) / 2;
	//Y Coordiantes will be such that entire box is just 10 Pixels 
	//above the bottom of the image
	int Y_Cordinates = (Image_Height - Base_Line - 10);
	std::cout << "TextBox Bottom Left: (" << X_Cordinates << ", " << Y_Cordinates << ")" << "\n";
	
	//Draw the canvas using filled rectangle
	cv::Scalar W_CanvasColor{ 255, 255, 255 };
	cv::Point Canvas_Bottom_Left(X_Cordinates, Y_Cordinates + Base_Line);
	cv::Point Cancas_Top_Right(X_Cordinates + Text_Width, Y_Cordinates - Text_Height);
	std::cout << "Canvas_Bottom_Left: " << Canvas_Bottom_Left << " Cancas_Top_Right: " << Cancas_Top_Right << "\n";
	cv::rectangle(Image_Get_Text_Size, Canvas_Bottom_Left, Cancas_Top_Right,
		W_CanvasColor, -1);
	cv::imshow("Image_Get_Text_Size", Image_Get_Text_Size);
	//Now Draw the baseline
	int Line_Thickness = 2;
	cv::Point Line_Left(X_Cordinates, Y_Cordinates);
	cv::Point Line_Right(X_Cordinates + Text_Width, Y_Cordinates);
	cv::Scalar G_Line_Color(0, 255, 0);
	cv::line(Image_Get_Text_Size, Line_Left, Line_Right, G_Line_Color,
		Line_Thickness, cv::LINE_AA);
	//Now Draw the text
	cv::putText(Image_Get_Text_Size, Text, cv::Point(X_Cordinates, Y_Cordinates),
		FontFace, FontScale, B_Color, Font_Thickness, cv::LINE_AA);
	cv::imshow("Final_Image", Image_Get_Text_Size);

	const std::string Image_Name_1 = "Darshan.jpg";
	const std::string Text_1 = "My name is Darshan";
	cv::Mat Out_Image = Add_Text_Darshan_Image(Image_Name_1, Text_1);
	cv::imshow(Image_Name_1, Out_Image);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}