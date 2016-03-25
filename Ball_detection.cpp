#include <iostream>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <highgui.h>
#include <cxcore.h>
#include <cv.h>

#define PARTICLE_NUMBER 50
#define DETECT_RADIUS 5
#define COLBOUNDARY 10
#define ROWBOUNDARY 10

#define THRESHOLD 0.5
#define DETECT_RADIUS_DISTANT 5
#define DETECT_RADIUS_NEARBY 10
#define ROW_SEARCH_RANGE 50
#define COL_SEARCH_RANGE 50

using namespace std;
using namespace cv;

struct img
{
	int x[600][425];
	int y[600][425];
	int z[600][425];
} imagetable;

int main()
{
	//set color range
	int ball_white_value[3][2];
	int gray_value[3][2];
	int line_white_value[3][2];
	int detect_radius[3];
	detect_radius[0]=DETECT_RADIUS_NEARBY;
	detect_radius[1]=DETECT_RADIUS_DISTANT;
	detect_radius[2]=int((DETECT_RADIUS_NEARBY+DETECT_RADIUS_DISTANT)/2);


	for(int i=0;i<3;i++)
	{
		ball_white_value[i][0]=244;
		ball_white_value[i][1]=255;
		gray_value[i][0]=159;
		gray_value[i][1]=206;
		line_white_value[i][0]=244;
		line_white_value[i][1]=255;
	}
	int green_value[3][2];
	green_value[0][0]=12;
	green_value[0][1]=73;
	green_value[1][0]=91;
	green_value[1][1]=154;
	green_value[2][0]=16;
	green_value[2][1]=77;

	//detected_ball_position[2] contains the x,y position of previous detected ball position
	int detected_ball_position[2];

	//p is the pointer of yuyv image
	/*int *p;
	for(int i=0;i<src.row;i++)
	{
		for(int j=0;j<src.col;j++)
		{
			imagetable[i][j][0]=*p++;
			imagetable[i][j][1]=*p++;
			p++;
			imagetable[i][j][2]=*p++;
		}
	}*/
	//imagetable contains pixels of the image in yuv422 format.

	//Particle Filter
	int ball_detected=1;
	//if this varible has value 1, it means that it's the first time for detection, so we set particles randomly
	//if this varible is -1,it means that it's not the first time for detection, so we set particlese around the previous ball location

	//assign boundary of search domain
	int row_boundary=ROWBOUNDARY;
	int col_boundary=COLBOUNDARY;

	//set particles
	srand(unsigned(time(0)));
	int particle_position[PARTICLE_NUMBER][2];
	int search_points[PARTICLE_NUMBER][100][2];
	//set degree of coordinates
	double Doc[PARTICLE_NUMBER];
	double Doc_max;
	int Doc_max_particle_number;
	int row_search_range=ROW_SEARCH_RANGE;
	int col_search_range=COL_SEARCH_RANGE;
	for(int i=0;i<PARTICLE_NUMBER;i++)
	{
		particle_position[i][0]=rand()%(2*row_search_range)+detected_ball_position[0]-row_search_range;
		particle_position[i][1]=rand()%(2*col_search_range)+detected_ball_position[1]-col_search_range;
	}
	int detection_radius=DETECT_RADIUS;
	for(int i=0;i<PARTICLE_NUMBER;i++)
	{
		//set 200 points inside a rectangle around the particles
		for(int j=0;j<201;j++)
		{
			search_points[i][j][0]=rand()%(4*DETECT_RADIUS)+particle_position[i][0]-2*DETECT_RADIUS;
			search_points[i][j][1]=rand()%(4*DETECT_RADIUS)+particle_position[i][1]-2*DETECT_RADIUS;
		}
		int n_ball_inside=0;
		int n_ball_outside=0;
		int n_not_ball_inside=0;
		int n_not_ball_outside=0;
		int field_or_line_outside=0;
		for(int j=0;j<201;j++)
		{
			int points_temp[3];
			for(int l=0;l<3;l++)
			{
				points_temp[0]=imagetable.x[i][j];
				points_temp[1]=imagetable.y[i][j];
				points_temp[2]=imagetable.z[i][j];
			}
			int distance_to_circle=int(sqrt((search_points[i][j][0]-particle_position[i][0])^2+(search_points[i][j][1]-particle_position[i][1])^2));
			if(((ball_white_value[0][0]<=points_temp[0]||points_temp[0]<=ball_white_value[0][1]) &&
			   (ball_white_value[1][0]<=points_temp[1]||points_temp[1]<=ball_white_value[1][0]) &&
			   (ball_white_value[2][0]<=points_temp[2]||points_temp[2]<=ball_white_value[2][1])) ||
			   ((gray_value[0][0]<=points_temp[0]||points_temp[0]<=gray_value[0][1]) &&
			   (gray_value[1][0]<=points_temp[1]||points_temp[1]<=gray_value[1][1]) &&
			   (gray_value[2][0]<=points_temp[2]||points_temp[2]<=gray_value[2][1])))
			{
				if(distance_to_circle<=detection_radius)
				{
					n_ball_inside=n_ball_inside+1;
				}
				else
				{
					n_ball_outside=n_ball_outside+1;
				}
			}
			else
			{
				if(distance_to_circle>detection_radius)
				{
					n_not_ball_outside=n_not_ball_outside+1;
				}
				else
				{
					n_not_ball_inside=n_not_ball_inside+1;
				}
			}
		}
		int n_inside=n_ball_inside+n_not_ball_inside;
		int n_outside=n_ball_outside+n_not_ball_outside;
		if(n_inside==0)
		{
			n_inside=1;
		}
		else if(n_outside==0)
		{
			n_outside=1;
		}

		Doc[i]=(n_ball_inside-n_ball_outside)*n_not_ball_outside/(n_inside*n_outside);
	}
	Doc_max_particle_number=-1;
	Doc_max=Doc[0];
	for(int i=0;i<PARTICLE_NUMBER;i++)
	{
		if(Doc[i]>THRESHOLD && Doc[i]>Doc_max)
		{
			Doc_max_particle_number=i;
			Doc_max=Doc[i];
		}
	}
	if(Doc_max_particle_number==-1)
	{
		cout<<"Detect failed"<<endl;
		ball_detected=0;
	}
	else
	{

		cout<<particle_position[Doc_max_particle_number][0]<<";"<<particle_position[Doc_max_particle_number][1]<<endl;
		ball_detected=1;
	}

	return 0;
	}

	
