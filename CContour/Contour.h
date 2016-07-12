#pragma once
#include "cvaux.h"
class CContour
{
public:
	CContour();
	~CContour();
	
	IplImage *pGrayImg;
	IplImage *pSobelImg_16S;
	IplImage *pSobelImg_8U;
	IplImage *dst;

	IplImage* DoContour(IplImage* pDImage);
	CvPoint GetCenterPoint(IplImage *src);
	void GetCornerPoint(IplImage *src);
	CvPoint GetClosestPoint(IplImage *src, CvPoint center);

	CvPoint LTPoint;
	CvPoint RBPoint;
	CvPoint *CenterPoint = new CvPoint[100];
	CvPoint *ClosestPoint = new CvPoint[100];
	int ObjectQuantity = 0;
	CvMemStorage *storage;
	CvSeq *contours;
	CvScalar(ext_color);
};

