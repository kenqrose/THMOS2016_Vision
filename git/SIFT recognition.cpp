#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <features2d\features2d.hpp>
#include <calib3d\calib3d.hpp>
#include <nonfree\nonfree.hpp>

#define MaxNumber 30


using namespace std;
using namespace cv;


int main()
{
	char srcname[20];
	for (int k=1;k<MaxNumber+1;k++)
	{
		sprintf_s(srcname,"0%d.jpg",k);
		Mat src=imread(srcname);
		Mat match=imread("match.jpg");


		int minHessian = 400;//SURF算法中的hessian阈值
	SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象  
	vector<KeyPoint> keypoints_object, keypoints_scene;//vector模板类，存放任意类型的动态数组

	//【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
	detector.detect( src, keypoints_object );
	detector.detect( match, keypoints_scene );

	//【4】计算描述符（特征向量）
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	extractor.compute( src, keypoints_object, descriptors_object );
	extractor.compute( match, keypoints_scene, descriptors_scene );

	//【5】使用FLANN匹配算子进行匹配
	FlannBasedMatcher matcher;
	vector< DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );
	double max_dist = 0; double min_dist = 100;//最小距离和最大距离

	//【6】计算出关键点之间距离的最大值和最小值
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	printf(">Max dist 最大距离 : %f \n", max_dist );
	printf(">Min dist 最小距离 : %f \n", min_dist );

	//【7】存下匹配距离小于3*min_dist的点对
	std::vector< DMatch > good_matches;
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
		if( matches[i].distance < 3*min_dist )
		{ 
			good_matches.push_back( matches[i]);
		}
	}

	//绘制出匹配到的关键点
	Mat img_matches;
	drawMatches( src, keypoints_object, match, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//定义两个局部变量
	vector<Point2f> obj;
	vector<Point2f> scene;

	//从匹配成功的匹配对中获取关键点
	for( unsigned int i = 0; i < good_matches.size(); i++ )
	{
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography( obj, scene, CV_RANSAC );//计算透视变换 

	//从待测图片中获取角点
	vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( src.cols, 0 );
	obj_corners[2] = cvPoint( src.cols, src.rows ); obj_corners[3] = cvPoint( 0, src.rows );
	vector<Point2f> scene_corners(4);

	//进行透视变换
	perspectiveTransform( obj_corners, scene_corners, H);

	//绘制出角点之间的直线
	line( img_matches, scene_corners[0] + Point2f( static_cast<float>(src.cols), 0), scene_corners[1] + Point2f( static_cast<float>(src.cols), 0), Scalar(255, 0, 123), 4 );
	line( img_matches, scene_corners[1] + Point2f( static_cast<float>(src.cols), 0), scene_corners[2] + Point2f( static_cast<float>(src.cols), 0), Scalar( 255, 0, 123), 4 );
	line( img_matches, scene_corners[2] + Point2f( static_cast<float>(src.cols), 0), scene_corners[3] + Point2f( static_cast<float>(src.cols), 0), Scalar( 255, 0, 123), 4 );
	line( img_matches, scene_corners[3] + Point2f( static_cast<float>(src.cols), 0), scene_corners[0] + Point2f( static_cast<float>(src.cols), 0), Scalar( 255, 0, 123), 4 );

	//显示最终结果
	imshow( srcname, img_matches );
	waitKey();
	}
	return 0;
}


