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


		int minHessian = 400;//SURF�㷨�е�hessian��ֵ
	SurfFeatureDetector detector( minHessian );//����һ��SurfFeatureDetector��SURF�� ������������  
	vector<KeyPoint> keypoints_object, keypoints_scene;//vectorģ���࣬����������͵Ķ�̬����

	//��3������detect��������SURF�����ؼ��㣬������vector������
	detector.detect( src, keypoints_object );
	detector.detect( match, keypoints_scene );

	//��4������������������������
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	extractor.compute( src, keypoints_object, descriptors_object );
	extractor.compute( match, keypoints_scene, descriptors_scene );

	//��5��ʹ��FLANNƥ�����ӽ���ƥ��
	FlannBasedMatcher matcher;
	vector< DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );
	double max_dist = 0; double min_dist = 100;//��С�����������

	//��6��������ؼ���֮���������ֵ����Сֵ
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	printf(">Max dist ������ : %f \n", max_dist );
	printf(">Min dist ��С���� : %f \n", min_dist );

	//��7������ƥ�����С��3*min_dist�ĵ��
	std::vector< DMatch > good_matches;
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
		if( matches[i].distance < 3*min_dist )
		{ 
			good_matches.push_back( matches[i]);
		}
	}

	//���Ƴ�ƥ�䵽�Ĺؼ���
	Mat img_matches;
	drawMatches( src, keypoints_object, match, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//���������ֲ�����
	vector<Point2f> obj;
	vector<Point2f> scene;

	//��ƥ��ɹ���ƥ����л�ȡ�ؼ���
	for( unsigned int i = 0; i < good_matches.size(); i++ )
	{
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography( obj, scene, CV_RANSAC );//����͸�ӱ任 

	//�Ӵ���ͼƬ�л�ȡ�ǵ�
	vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( src.cols, 0 );
	obj_corners[2] = cvPoint( src.cols, src.rows ); obj_corners[3] = cvPoint( 0, src.rows );
	vector<Point2f> scene_corners(4);

	//����͸�ӱ任
	perspectiveTransform( obj_corners, scene_corners, H);

	//���Ƴ��ǵ�֮���ֱ��
	line( img_matches, scene_corners[0] + Point2f( static_cast<float>(src.cols), 0), scene_corners[1] + Point2f( static_cast<float>(src.cols), 0), Scalar(255, 0, 123), 4 );
	line( img_matches, scene_corners[1] + Point2f( static_cast<float>(src.cols), 0), scene_corners[2] + Point2f( static_cast<float>(src.cols), 0), Scalar( 255, 0, 123), 4 );
	line( img_matches, scene_corners[2] + Point2f( static_cast<float>(src.cols), 0), scene_corners[3] + Point2f( static_cast<float>(src.cols), 0), Scalar( 255, 0, 123), 4 );
	line( img_matches, scene_corners[3] + Point2f( static_cast<float>(src.cols), 0), scene_corners[0] + Point2f( static_cast<float>(src.cols), 0), Scalar( 255, 0, 123), 4 );

	//��ʾ���ս��
	imshow( srcname, img_matches );
	waitKey();
	}
	return 0;
}


