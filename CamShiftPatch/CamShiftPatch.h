#pragma once

#include "cv.h" 
#include "highgui.h" 

class CamShiftPatch
{
public:
	CvRect track_window;
	IplImage* originImage;
	CvHistogram *histogram;

	CamShiftPatch(IplImage* image);
	CamShiftPatch();
	~CamShiftPatch();

	
	void setImage(IplImage* image);

	//CvScalar maskRange   val[0]=Vmin ; val[1]=Vmax ; val[2]=Smin
	IplImage* drawHistImg(CvRect selectROI, CvScalar maskRange);

	//CvScalar maskRange   val[0]=Vmin ; val[1]=Vmax ; val[2]=Smin
	IplImage* getInRangeMask(CvScalar maskRange, IplImage* &hue);

	//CvScalar maskRange   val[0]=Vmin ; val[1]=Vmax ; val[2]=Smin
	CvBox2D getTrackBox(CvScalar maskRange, CvHistogram *hist);

};

