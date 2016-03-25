#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


int main()
{
	VideoCapture CapLeft(1);
	CapLeft.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	CapLeft.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	VideoCapture CapRight(0);
	CapRight.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	CapRight.set(CV_CAP_PROP_FRAME_HEIGHT, 720); 

	Mat intrinsic_matrix_le = (Mat_<double>(3, 3) << 1076.65663744959, 0.0, 617.754164939819, 0.0, 1076.71858774788, 404.033667500091, 0.0, 0.0, 1.0);
	Mat distortion_coeffs_le = (Mat_<double>(4, 1) << 0.00907455289852767, -0.244189836941367, 0.00538240251598171, -0.00197483335132350);
	Mat intrinsic_matrix_ri = (Mat_<double>(3, 3) << 1063.52697026590, 0.0, 658.715451833095, 0.0, 1064.55895387456, 377.678818029156, 0.0, 0.0, 1.0);
	Mat distortion_coeffs_ri = (Mat_<double>(4, 1) << -0.0101556488449985, -0.0818348859823173, 0.00413535779523549, -0.00197260985484142);

	Mat mapx_left;
	Mat mapy_left;
	Mat mapx_right;
	Mat mapy_right;

	Mat R = (Mat_<double>(3, 3) << 0.998574808549094, -0.0508167374280994, 0.0163098414555247, 0.0492451797965485, 0.995123392711047, 0.0854654640545553, -0.0205733808098320, -0.0845404783309065, 0.996207630730479);
	Mat T = (Mat_<double>(3, 1) << -69.1599518496072, 0.475652461529234, 0.271535270957857);


	//cout << "Matrix intrinsic_matrix_le" << endl;
	//int nrr0 = intrinsic_matrix_le.rows; // number of rows  
	//int ncr0 = intrinsic_matrix_le.cols* intrinsic_matrix_le.channels(); // total number of elements per line  
	//for (int j = 0; j<nrr0; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazr0 = intrinsic_matrix_le.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncr0; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazr0[i] << endl;
	//	}
	//}


	Mat R1;
	R1.create(Size(3, 3), CV_64F);
	Mat R2;
	R2.create(Size(3, 3), CV_64F);
	Mat P1;
	P1.create(Size(3, 4), CV_64F);
	Mat P2;
	P2.create(Size(3, 4), CV_64F);
	Mat Q;
	Q.create(Size(4, 4), CV_64F);
	
	stereoRectify(intrinsic_matrix_le, distortion_coeffs_le, intrinsic_matrix_ri, distortion_coeffs_ri, Size(1280, 720), R, T, R1, R2, P1, P2, Q, CV_CALIB_ZERO_DISPARITY,0, Size(1280, 720));

	//cout << "Matrix R" << endl;
	//int nrr0 = R.rows; // number of rows  
	//int ncr0 = R.cols* R.channels(); // total number of elements per line  
	//for (int j = 0; j<nrr0; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazr0 = R.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncr0; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazr0[i] << endl;
	//	}
	//}


	//Mat col = C.colRange(1, 3).clone()
	//Mat intrinsic_matrix_left_new = P1.rowRange(0, 3).clone();
	//Mat intrinsic_matrix_right_new = P2.rowRange(0, 3).clone();
	initUndistortRectifyMap(intrinsic_matrix_le, distortion_coeffs_le, R1, P1, Size(1280, 720), CV_32FC1, mapx_left, mapy_left);
	initUndistortRectifyMap(intrinsic_matrix_ri, distortion_coeffs_ri, R2, P2, Size(1280, 720), CV_32FC1, mapx_right, mapy_right);
	//Mat z = Mat::zeros(R1.rows, R1.cols, CV_16SC2);
	
	//cout << "Matrix Q" << endl;
	//int nrq = Q.rows; // number of rows  
	//int ncq = Q.cols* Q.channels(); // total number of elements per line  
	//for (int j = 0; j<nrq; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazq = Q.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncq; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";"<<datazq[i] << endl;
	//	}
	//}
	//cout << "Matrix R1" << endl;
	//int nrr1 = R1.rows; // number of rows  
	//int ncr1 = R1.cols* R1.channels(); // total number of elements per line  
	//for (int j = 0; j<nrr1; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazr1 = R1.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncr1; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazr1[i] << endl;
	//	}
	//}
	//cout << "Matrix R2" << endl;
	//int nrr2 = R2.rows; // number of rows  
	//int ncr2 = R2.cols* R2.channels(); // total number of elements per line  
	//for (int j = 0; j<nrr2; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazr2 = R2.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncr2; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazr2[i] << endl;
	//	}
	//}
	//cout << "Matrix P1" << endl;
	//int nrp1 = P1.rows; // number of rows  
	//int ncp1 = P1.cols* P1.channels(); // total number of elements per line  
	//for (int j = 0; j<nrp1; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazp1 = P1.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncp1; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazp1[i] << endl;
	//	}
	//}
	//cout << "Matrix P2" << endl;
	//int nrp2 = P2.rows; // number of rows  
	//int ncp2 = P2.cols* P2.channels(); // total number of elements per line  
	//for (int j = 0; j<nrp2; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazp2 = P2.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncp2; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazp2[i] << endl;
	//	}
	//}
	//cout << "mapx_left" << endl;
	//int nrle = mapx_left.rows; // number of rows  
	//int ncle = mapx_left.cols* mapx_left.channels(); // total number of elements per line  
	//for (int j = 0; j<nrle; j++) {
	//	//uchar* data = mapx_left.ptr<uchar>(j);
	//	double* datazle = mapx_left.ptr<double>(j);
	//	//uchar* datac = c.ptr<uchar>(j);
	//	for (int i = 0; i<ncle; i++) {
	//		//datac[i] = data[i]-dataz[i];
	//		cout << i << ";" << datazle[i] << endl;
	//	}
	//}


	//namedWindow("mapx_left");
	////////namedWindow("mapx_right");
	//////Mat c = Mat::zeros(mapx_left.rows, mapx_left.cols, CV_32FC1);
	////int nr = mapx_left.rows; // number of rows  
	////int nc = mapx_left.cols * mapx_left.channels(); // total number of elements per line  
	////for (int j = 0; j<nr; j++) {
	////	//uchar* data = mapx_left.ptr<uchar>(j);
	////	uchar* dataz = z.ptr<uchar>(j);
	////	//uchar* datac = c.ptr<uchar>(j);
	////	for (int i = 0; i<nc; i++) {
	////		//datac[i] = data[i]-dataz[i];
	////		dataz[i] = 0 * dataz[i];
	////	}
	////}
	

	namedWindow("LeftVision");
	namedWindow("RightVision");
	namedWindow("vdisp");
	//namedWindow("dstimage");
	//namedWindow("vdisp_z");
	//namedWindow("initLeftVision");
	//namedWindow("initRightVision");


	//Mat disp = cvCreateMat(imageSize.height,imageSize.width, CV_32F);
	//Mat vdisp = cvCreateMat(imageSize.height,imageSize.width, CV_32F);
	Mat binary_image;
	while (1)
	{
		Mat srcLeft;
		Mat srcRight;
		Mat dstLeft;
		Mat dstRight;
		Mat left_disp;
		Mat right_disp;
		Mat disp;
		Mat disp_z;
		//vector<Point> points;
		//vector<int> hull;

		CapLeft >> srcLeft;
		CapRight >> srcRight;
		//imshow("initLeftVision", srcLeft);
		//imshow("initRightVision", srcRight);

		/*Mat kernela(3, 3, CV_32F, Scalar(0));
		kernela.at<double>(1, 1) = 5.0;
		kernela.at<double>(0, 1) = -1.0;
		kernela.at<double>(2, 1) = -1.0;
		kernela.at<double>(1, 0) = -1.0;
		kernela.at<double>(1, 2) = -1.0;
		filter2D(srcLeft, srcLeft, srcLeft.depth(), kernela);
		filter2D(srcRight, srcRight, srcRight.depth(), kernela);*/

		/*Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		morphologyEx(srcLeft, srcLeft, MORPH_OPEN, element);
		morphologyEx(srcRight, srcRight, MORPH_OPEN, element);*/

		cvtColor(srcLeft, srcLeft, CV_RGB2GRAY);
		cvtColor(srcRight, srcRight, CV_RGB2GRAY);

		medianBlur(srcLeft, srcLeft, 3);
		medianBlur(srcRight, srcRight, 3);
		equalizeHist(srcLeft, srcLeft);
		equalizeHist(srcRight, srcRight);
		remap(srcLeft, dstLeft, mapx_left, mapy_left, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar());
		remap(srcRight, dstRight, mapx_right, mapy_right, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar());
		/*GaussianBlur(dstLeft, dstLeft, Size(5, 5), 0, 0);
		GaussianBlur(dstRight, dstRight, Size(5, 5), 0, 0);*/
		//resize(dstLeft, dstLeft, Size(640, 480));
		//resize(dstRight, dstRight, Size(640, 480));

		cv::StereoSGBM sgbm;
		sgbm.preFilterCap = 63;//1-31
		int SADWindowSize = 15;//5-21-255
		int minDisparity = 0;
		int numberOfDisparities = 64;//%16
		int cn = srcLeft.channels();
		sgbm.SADWindowSize = SADWindowSize > 0 ? SADWindowSize : 3;
		sgbm.P1 = 8 * cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
		sgbm.P2 = 32 * cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
		sgbm.minDisparity = minDisparity;//272
		sgbm.numberOfDisparities = numberOfDisparities;
		sgbm.uniquenessRatio = 15;//5-15
		sgbm.speckleWindowSize = 500;
		sgbm.speckleRange = 32;
		sgbm.disp12MaxDiff = 1;
		sgbm(dstLeft, dstRight, left_disp);
		
		//sgbm(dstGaussianRight, dstGaussianLeft, right_disp);
		normalize(left_disp, disp, 0, 255, CV_MINMAX, CV_8U);
		threshold(disp, binary_image, 50, 255, THRESH_BINARY);
		//Canny(binary_image, binary_image, 3, 9, 3);
		reprojectImageTo3D(disp, disp_z, Q,false,-1);
		//GaussianBlur(disp, disp, Size(11, 11), 0, 0);
		//cvtColor(disp, disp, CV_GRAY2BGR);
		vector<vector<Point>> contours;
		//cv::vector<cv::Vec4i> hierarchy;

		findContours(binary_image, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));
		//int index = 0;
		//for (; index >= 0; index = hierarchy[index][0])
		//{
		//	Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//	drawContours(dstimage, contours, index, color, CV_FILLED, 8, hierarchy);
		//}
		//imshow("dstimage", dstimage);

		vector<vector<Point>> contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		for(unsigned int i=0;i<contours.size();i++)
		{
			approxPolyDP(Mat(contours[i]),contours_poly[i],3,true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
		}

		Mat dstimage=Mat::zeros(disp.size(),CV_8UC3);
		double contarea;
		vector<Moments> mu(contours.size());
		vector<cv::Point> p(contours.size());
		//vector<uchar> world_point(contours.size());
		for(int unsigned i=0;i<contours.size();i++)
		{
			Scalar color(rand() & 255, rand() & 255, rand() & 255);
			drawContours(dstimage,contours_poly,i,color,1,8,vector<Vec4i>(),0,Point());
			contarea=contourArea(contours_poly[i]);
			mu[i]=moments(contours_poly[i],false);
			p[i]=Point(int((boundRect[i].tl().x+boundRect[i].br().x)/2),int((boundRect[i].tl().y+boundRect[i].br().y)/2));
			//world_point[i]=disp_z.at<Vec3f>(p[i]);
			if(contarea>10000)
			{
			rectangle(dstimage,boundRect[i].tl(),boundRect[i].br(),color,2,8,0);
			cout<<"Rectangle Information"<<endl;
			cout << "面积"<<mu[i].m00<<endl;
			//Point p0=Point(100,100);
			//cout<< disp.at<uchar>(p0)<<endl;
			cout << "in world coordinates"<<disp_z.at<Vec3f>(p[i])<<endl;
			//cout<<"x坐标"<<mu[i].m01<<endl;
			//cout<<"y坐标"<<mu[i].m10<<endl;
			}
		}
		imshow("dstimage", dstimage);

		/*convexHull(disp, hull,true);
		int hullcount = (int)hull.size();
		Point point0 = points[hull[hullcount - 1]];

		for (int i = 0; i < hullcount; i++)
		{
			Point point = points[hull[i]];
			line(disp, point0, point, Scalar(255,255,0), 2, 8);
			point0 = point;
		}*/

		imshow("LeftVision", dstLeft);
		imshow("RightVision", dstRight);
		imshow("vdisp", disp);
		//imshow("vdisp_z", disp_z);

		waitKey(33);
	}
	return 0;
}