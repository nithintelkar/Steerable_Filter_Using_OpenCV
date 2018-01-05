#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "cvaux.h"
#include "cxcore.h"
#include "ml.h"
#include "cv.h"



int main( int argc, char** argv ) 
{
	double pi=3.141593, theta;
	int i=0, j=0;;
	int t;


	double pi_Gxx, pi_Gyy, pi_Gxy, t_Gxx, t_Gyy, t_Gxy,SUM;


	printf("Enter The Value For Theta\n");
	scanf("%d",&t);
	theta = (-t*(pi/180));


	IplImage* img = cvLoadImage( argv[1] );
	IplImage* Gxx = cvCreateImage(cvGetSize(img),img->depth, 1);
	IplImage* Gyy = cvCreateImage(cvGetSize(img),img->depth, 1);
	IplImage* Gxy = cvCreateImage(cvGetSize(img),img->depth, 1);
	cvCvtColor(img,Gxx, CV_BGR2GRAY);
	cvCvtColor(img,Gyy, CV_BGR2GRAY);
	cvCvtColor(img,Gxy, CV_BGR2GRAY);


	IplImage* Gxx_16S = cvCreateImage(cvGetSize(Gxx),IPL_DEPTH_16S,1);
	cvSobel(Gxx,Gxx_16S,2,0,3);
	cvConvertScale(Gxx_16S,Gxx);  //Working 8U on converted image. 
	//Should hav worked on 16S. change for loop from Gxx to Gxx_16S for 16S


	IplImage* Gyy_16S = cvCreateImage(cvGetSize(Gyy),IPL_DEPTH_16S,1);
	cvSobel(Gyy,Gyy_16S,0,2,3);
	cvConvertScale(Gyy_16S,Gyy); //Working 8U on converted image. 
	//Should hav worked on 16S. change for loop from Gyy to Gyy_16S for 16S


	IplImage* Gxy_16S = cvCreateImage(cvGetSize(Gxy),IPL_DEPTH_16S,1);
	cvSobel(Gxy,Gxy_16S,2,2,3);
	cvConvertScale(Gxy_16S,Gxy); //Working 8U on converted image. 
	//Should hav worked on 16S. change for loop from Gxx to Gxy_16S for 16S

	IplImage* output_16S = cvCreateImage(cvGetSize(Gxy),IPL_DEPTH_16S,1);
	IplImage* output_8U = cvCreateImage(cvGetSize(Gxy),IPL_DEPTH_8U,1);

	for (int i=0 ; i<(Gxx->height); i++)
	{
		for ( int j=0 ; j<Gxx->width ; j++)
		{
			pi_Gxx =  cvGetReal2D (Gxx,i,j); 
			t_Gxx = pi_Gxx * cos(theta) * cos(theta);

			pi_Gyy =  cvGetReal2D (Gyy,i,j); 
			t_Gyy = pi_Gyy * sin(theta) * sin(theta);

			pi_Gxy =  cvGetReal2D (Gxy,i,j); 
			t_Gxy = pi_Gxy * cos(theta) * sin(theta);
			SUM = abs(t_Gxx) + abs(t_Gyy) + abs(t_Gxy);
			cvSetReal2D(output_16S,i,j,SUM);
		
			/*theta min and theta max calculation. to be done on 16S image. 
			process halted for the further clarification
			
			float A = sqrt((pi_Gxx * pi_Gxx) - (2 *  pi_Gxx * pi_Gyy) + (pi_Gyy * pi_Gyy) + (4 * pi_Gxy));

			double Tmin = atan((pi_Gxx-pi_Gyy-A)/(2*pi_Gxy));
			printf("\nTmin = %f\n",Tmin);
			double Tmin = atan((pi_Gxx-pi_Gyy+A)/(2*pi_Gxy));
			printf("\nTmax = %f\n",Tmax);
			*/
		}
	}

	cvConvertScale(output_16S,output_8U,1,0);

	
	cvThreshold(output_8U,output_8U,15,255,CV_THRESH_OTSU);
	cvShowImage("output",output_8U);
	
	cvWaitKey(0);
	cvReleaseImage( &img );
	cvReleaseImage( &Gxx );
	cvReleaseImage( &Gyy );
	cvReleaseImage( &Gxy );
	cvReleaseImage( &Gxx_16S );
	cvReleaseImage( &Gyy_16S );
	cvReleaseImage( &Gxy_16S );
	cvReleaseImage( &output_16S );
	cvReleaseImage( &output_8U );
}