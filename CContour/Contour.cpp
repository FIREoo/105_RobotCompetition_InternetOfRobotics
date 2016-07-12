#include "stdafx.h"
#include "Contour.h"


CContour::CContour() : pGrayImg(nullptr), pSobelImg_16S(nullptr), pSobelImg_8U(nullptr) 
{
}


CContour::~CContour()
{
	cvReleaseImage(&pGrayImg);
	cvReleaseImage(&dst);
	delete[] CenterPoint;
	delete[] ClosestPoint;
	cvReleaseImage(&pSobelImg_8U);
	cvReleaseMemStorage(&storage);
}
CvPoint CContour ::GetClosestPoint(IplImage *src, CvPoint center)
{
	CvPoint ClosestP = cvPoint(0, 0);
	long double length = 160000;
	long double lengthTemp;
	CvScalar pixel;
	for (int i = 0; i < src->width; i++) {
		for (int j = 0; j < src->height; j++) {
			pixel = cvGet2D(src, j, i);
			if (pixel.val[0] > 0){
				lengthTemp = pow(i - center.x, 2) + pow(j - center.y, 2);
				if (length>lengthTemp){
					length = lengthTemp;
					if (length == lengthTemp){
						ClosestP.x = i;
						ClosestP.y = j;
					}
				}
			}
		}
	}

	return ClosestP;
}
CvPoint CContour::GetCenterPoint(IplImage *src)
{

	int x0 = 0, y0 = 0, sum = 0;
	CvPoint center;
	CvScalar pixel;
	for (int i = 0; i < src->width; i++) {
		for (int j = 0; j < src->height; j++) {
			pixel = cvGet2D(src, j, i);
			if (pixel.val[0] > 0)
			{
				x0 = x0 + i;
				y0 = y0 + j;
				sum = sum + 1;
			}
		}
	}
	if (sum == 0) {
		center.x = 0;
		center.y = 0;
		return center;
	}
	center.x = x0 / sum;
	center.y = y0 / sum;
	return center;
}

void CContour::GetCornerPoint(IplImage *src)
{
	CvPoint MinP = cvPoint(320, 240);
	CvPoint MaxP = cvPoint(0, 0);
	CvScalar pixel;
	for (int i = 0; i < src->width; i++) {
		for (int j = 0; j < src->height; j++) {
			pixel = cvGet2D(src, j, i);
			if (pixel.val[0] > 0)
			{
				if (j < MinP.y){		
					MinP.y = j;
				}
				if (j > MaxP.y){				
					MaxP.y = j;
				}
				if (i < MinP.x){			
					MinP.x = i;
				}
				if (i > MaxP.x){				
					MaxP.x = i;
				}
			}
		}
	}
	LTPoint.x = MinP.x;
	LTPoint.y = MinP.y;
	RBPoint.x = MaxP.x;
	RBPoint.y = MaxP.y;
}

IplImage* CContour::DoContour(IplImage* pDImage)
{
	IplImage* pImage = (IplImage *)cvClone(pDImage);
	 pGrayImg = cvCreateImage(cvGetSize(pImage), pImage->depth, 1);
	pSobelImg_16S = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_16S, 1);
	 pSobelImg_8U = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_8U, 1);
	IplImage* pSobelImg_16 = cvCreateImage(cvGetSize(pImage), IPL_DEPTH_16S, 1);
	
	cvCvtColor(pImage, pGrayImg, CV_BGR2GRAY);
	cvReleaseImage(&pImage);

	IplImage* dst = cvCreateImage(cvSize(pGrayImg->width, pGrayImg->height), pGrayImg->depth, 3);
	IplImage* First = cvCreateImage(cvSize(pGrayImg->width, pGrayImg->height), pGrayImg->depth, 3);
	IplImage* Sec = cvCreateImage(cvSize(pGrayImg->width, pGrayImg->height), pGrayImg->depth, 3);

	cvAdaptiveThreshold(pGrayImg, pGrayImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 15);
	cvSobel(pGrayImg, pSobelImg_16, 1, 0, 3);
	cvSobel(pGrayImg, pSobelImg_16S, 0, 1, 3);
	cvOr(pSobelImg_16, pSobelImg_16S, pSobelImg_16S);
	cvConvertScaleAbs(pSobelImg_16S, pSobelImg_8U, 1, 0);

	cvReleaseImage(&pSobelImg_16);
	cvReleaseImage(&pSobelImg_16S);

	cvDilate(pSobelImg_8U, pSobelImg_8U, 0, 1);
	cvErode(pSobelImg_8U, pSobelImg_8U, 0, 1);
	storage = cvCreateMemStorage(0);

	cvFindContours(pSobelImg_8U, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP,
		CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	for (; contours != 0; contours = contours->h_next)
	{
		ext_color = CV_RGB(rand() & 255, rand() & 255, rand() & 255);//設定每一CONTOUR都有一個顏色表示

		cvDrawContours(dst, contours, ext_color, CV_RGB(0, 0, 0),	-1, CV_FILLED, 8, cvPoint(0, 0));
		Sec = (IplImage *)cvClone(dst);

		cvAbsDiff(Sec, First, Sec);
		*(CenterPoint + ObjectQuantity) = GetCenterPoint(Sec);
		*(ClosestPoint + ObjectQuantity) = GetClosestPoint(Sec, *(CenterPoint + ObjectQuantity));
		
		ObjectQuantity++;

		First = (IplImage *)cvClone(dst);
		//將CONTOUR以顏色塗滿
	}
	cvReleaseImage(&First);
	cvReleaseImage(&Sec);
	return dst;
}