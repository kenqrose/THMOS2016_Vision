#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>

using namespace std;
using namespace cv;

int main()
{
	char srcname[20];
	Mat matchimg=imread("match.jpg");
	resize(matchimg,matchimg,Size(640,480));
	imwrite("match.jpg",matchimg);
	for (int k=1;k<41;k++)
	{
		sprintf_s(srcname,"0%d.jpg",k);
		Mat src=imread(srcname);
		resize(src,src,Size(640,480));
		imwrite(srcname,src);
	}
    return 0;
}