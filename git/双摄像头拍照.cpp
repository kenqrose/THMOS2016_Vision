#include <iostream>
#include <highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

void LeftCallback(int event, int x, int y, int flags, void* param);
void RightCallback(int event, int x, int y, int flags, void* param);
int k = 0;
int i = 0;
char leftc[50];
char rightc[50];
Mat LeftImg;
Mat RightImg;
int main()
{
	
	namedWindow("WebCamLeft");
	namedWindow("WebCamRight");
	Mat Img[2];
	Img[0] = LeftImg;
	Img[1] = RightImg;
	cvSetMouseCallback("WebCamLeft", LeftCallback,(void*)&Img);
	cvSetMouseCallback("WebCamRight", RightCallback,(void*)&Img);
	VideoCapture LeftCapture(1);
	LeftCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	LeftCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	VideoCapture RightCapture(0);
	RightCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	RightCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	while (1)
	{
		LeftCapture >> LeftImg;
		RightCapture >> RightImg;
		Mat left=LeftImg.clone();
		Mat right=RightImg.clone();
		//resize(left, left, Size(640, 480)); 
		//resize(right, right, Size(640, 480));
		imshow("WebCamLeft", left);
		imshow("WebCamRight", right);
		waitKey(33);
		
	}
	return 0;
}

void LeftCallback(int event, int x, int y, int flags, void* param)
{

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		sprintf_s(leftc, "left%d.jpg", k);
		sprintf_s(rightc, "right%d.jpg", k);
		imwrite(leftc, LeftImg);
		imwrite(rightc, RightImg);
		k++;
	}
}

void RightCallback(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		sprintf_s(rightc, "right%d.jpg", i);
		sprintf_s(leftc, "left%d.jpg", i);
		imwrite(rightc, RightImg);
		imwrite(leftc, LeftImg);
		i++;
	}
}