#include <iostream>
#include <highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

int main()
{
	VideoCapture CapLeft(0);
	CapLeft.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	CapLeft.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	VideoCapture CapRight(1);
	CapRight.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	CapRight.set(CV_CAP_PROP_FRAME_HEIGHT, 720);


	Mat intrinsic_matrix_le = (Mat_<double>(3, 3) << 1090.50174204408, 0.0, 631.370137684007, 0.0, 1089.69032903236, 392.869465626791, 0.0, 0.0, 1.0);
	Mat distortion_coeffs_le = (Mat_<double>(4, 1) << -0.0109963788129695, -0.161351186069308, 0.00192931157417567, -0.000460444740042993);
	Mat intrinsic_matrix_ri = (Mat_<double>(3, 3) << 1075.16083743406, 0.0, 657.106196572848, 0.0, 1073.60888608308, 369.436311742215, 0.0, 0.0, 1.0);
	Mat distortion_coeffs_ri = (Mat_<double>(4, 1) << -0.0153699518279344, -0.106773298391025, 0.00178490744870522, -0.000494760726251003);
	
	Mat mapx_left;
	Mat mapy_left;
	Mat mapx_right;
	Mat mapy_right;

	Mat R = (Mat_<double>(3, 3) << 0.998785699292624, -0.0490437579866268, 0.00467297454444615, 0.0490049676903026, 0.998765832397063, 0.00808239926471775, -0.00506359854414415, -0.00784358583500511, 0.999956418115826);
	Mat T = (Mat_<double>(3, 1) << -68.4772267671313, 1.67531203478809, -0.986250244907829);

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

	stereoRectify(intrinsic_matrix_le, distortion_coeffs_le, intrinsic_matrix_ri, distortion_coeffs_ri, Size(1280, 720), R, T, R1, R2, P1, P2, Q, CV_CALIB_ZERO_DISPARITY,
		0);

	initUndistortRectifyMap(intrinsic_matrix_le, distortion_coeffs_le, R1, P1, Size(1280, 720), CV_32FC1, mapx_left, mapy_left);
	initUndistortRectifyMap(intrinsic_matrix_ri, distortion_coeffs_ri, R2, P2, Size(1280, 720), CV_32FC1, mapx_right, mapy_right);

	Mat srcLeft;
	Mat srcRight;
	Mat dstLeft;
	Mat dstRight;


	namedWindow("LeftVision");
	namedWindow("RightVision");
	namedWindow("vdisp");
	//namedWindow("vdisp_z");

	Mat left_disp;
	Mat right_disp;
	Mat disp;
	Mat dispv;
	Mat disp_z;


	StereoBM stereo;
	stereo.state->preFilterType = 1;
	stereo.state->preFilterSize = 15;
	stereo.state->preFilterCap = 63;
	stereo.state->SADWindowSize = 15;
	stereo.state->minDisparity =0;
	stereo.state->numberOfDisparities = 32;
	stereo.state->textureThreshold = 10;
	stereo.state->uniquenessRatio = 15;
	stereo.state->speckleWindowSize = 100;
	stereo.state->speckleRange = 16;
	stereo.state->disp12MaxDiff = 1;


	while (1)
	{
		CapLeft >> srcLeft;
		CapRight >> srcRight;
		//imshow("initLeftVision", srcLeft);
		//imshow("initRightVision", srcRight);
		cvtColor(srcLeft, srcLeft, CV_RGB2GRAY);
		cvtColor(srcRight, srcRight, CV_RGB2GRAY);

		remap(srcLeft, dstLeft, mapx_left, mapy_left, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar());
		remap(srcRight, dstRight, mapx_right, mapy_right, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar());
		//GaussianBlur(dstLeft, dstGaussianLeft, Size(3, 3), 0, 0);
		//GaussianBlur(dstRight, dstGaussianRight, Size(3, 3), 0, 0);
		//medianBlur(dstLeft, dstLeft, 5);
		//medianBlur(dstRight, dstRight, 5);

		stereo(dstLeft, dstRight, disp);
		normalize(disp, dispv, 0, 255, CV_MINMAX, CV_8U);
		//reprojectImageTo3D(disp, disp_z, Q, false,-1);
		

		imshow("LeftVision", dstLeft);
		imshow("RightVision", dstRight);
		imshow("vdisp", dispv);
		//imshow("vdisp_z", disp_z);
		waitKey(33);
	}
	return 0;
}