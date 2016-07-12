#include "CamShiftPatch.h"

CvScalar hsv2rgb(float hue)
{
	int rgb[3], p, sector;
	static const int sector_data[][3] =
	{ { 0, 2, 1 }, { 1, 2, 0 }, { 1, 0, 2 }, { 2, 0, 1 }, { 2, 1, 0 }, { 0, 1, 2 } };
	hue *= 0.033333333333333333333333333333333f;
	sector = cvFloor(hue);  //捨去小數點
	//std::cout<<"sector:" << sector << std::endl;
	p = cvRound(255 * (hue - sector));//取最近整數
	printf("\np before:%d\n", p);
	printf("sector:%d\n", sector);
	p ^= sector ? 255 : 0;    //當sector!=0 則 p^=255  ===> 即p=p^11111111(做XOR運算)，相當於減法;    當sector=0則p^=0  ===> 即p與0做XOR運算，相當於不改變值
	printf("p after:%d\n", p);
	rgb[sector_data[sector][0]] = 255;
	rgb[sector_data[sector][1]] = 0;
	rgb[sector_data[sector][2]] = p;
	return cvScalar(rgb[2], rgb[1], rgb[0], 0);
}

CamShiftPatch::CamShiftPatch(IplImage* image)
{
	histogram = 0;
	originImage = 0;
	originImage = cvCreateImage(cvGetSize(image), 8, 3);
	cvCopy(image, originImage, 0);
}
CamShiftPatch::CamShiftPatch()
{
	histogram = 0;
	originImage = 0;
}
CamShiftPatch::~CamShiftPatch()
{
}

void CamShiftPatch::setImage(IplImage* image)
{
	originImage = cvCreateImage(cvGetSize(image), 8, 3);
	cvCopy(image, originImage, 0);
}
IplImage* CamShiftPatch::getInRangeMask(CvScalar maskRange, IplImage* &hue)
{
	IplImage *mask = 0;
	mask = cvCreateImage(cvGetSize(originImage), 8, 1);//遮罩空間，單通道

	IplImage *hsv = 0;
	hue = cvCreateImage(cvGetSize(originImage), 8, 1); //1 channel
	hsv = cvCreateImage(cvGetSize(originImage), 8, 3);
	cvCvtColor(originImage, hsv, CV_BGR2HSV); //彩色空間轉換BGR to HSV 


	//cvShowImage("hsv", hsv);
	cvInRangeS(//cvInRangeS 功能為 => 檢查數組元素是否在兩個數量之間
		hsv, //第一個原數組
		cvScalar(0, maskRange.val[2], MIN(maskRange.val[0], maskRange.val[1]), 0),  //包括進的下邊界
		cvScalar(180, 256, MAX(maskRange.val[0], maskRange.val[1]), 0), //不包括進的上邊界
		mask);
	//得到二值的MASK  
	cvSplit(hsv, hue, 0, 0, 0); //只提取HUE分量  
	
	//cvShowImage("mask", mask);

	IplImage* returnImg = nullptr;
	returnImg = cvCloneImage(mask);

	cvReleaseImage(&mask);
	cvReleaseImage(&hsv);

	return returnImg;
}

IplImage* CamShiftPatch::drawHistImg(CvRect selectROI, CvScalar maskRange)
{
	IplImage* hue = 0;
	hue = cvCreateImage(cvGetSize(originImage), 8, 1);
	IplImage *mask = getInRangeMask(maskRange, hue);//CvScalar 0->Vmin  1->Vmax  2->Smin

	//---設定ROI和畫出直方圖---

	float max_val = 0.f;
	cvSetImageROI(hue, selectROI);
	cvSetImageROI(mask, selectROI);

	
	int hdims = 48; //劃分HIST的個數，越高越精確 
	float hranges_arr[] = { 0, 180 }; //直方圖範圍
	float* hranges = hranges_arr;//指向值方圖範圍
	histogram = cvCreateHist(1, &hdims, CV_HIST_ARRAY, &hranges, 1); //設定直方圖的格式
	cvCalcHist(&hue, histogram, 0, mask); //以hue資訊 得到直方圖 (只有ROI部分的資訊)
	cvGetMinMaxHistValue(histogram, 0, &max_val, 0, 0); //只找最大值 
	cvConvertScale(histogram->bins, histogram->bins, max_val ? 255. / max_val : 0., 0); //縮放bin到區間[0,255]  
	//input      output        尺度放大或縮小         全部顏色的增減       
	cvResetImageROI(hue); // remove ROI  
	cvResetImageROI(mask);
	//track_window = selectROI; //搜索窗即一開始ROI框

	//----畫直方圖hist to histimg------------------

	IplImage* histimg = cvCreateImage(cvSize(320, 200), 8, 3);  //直方圖顯示空間，三通道
	cvZero(histimg); //置背景為黑色

	int bin_w = 0;
	bin_w = histimg->width / hdims; // hdims:條的個數，則bin_w為條的寬度  

	for (int i = 0; i < hdims; i++)
	{
		int val = cvRound(cvGetReal1D(histogram->bins, i)*histimg->height / 255);  //cvGetReal1D(直方條高度，第幾條) 
		CvScalar color = hsv2rgb(i*180.f / hdims); //在RGB空間上，直方圖每條的顏色計算   180/hdims表示每一格多寬 ， i*180/hdims代表到橫軸的哪個位置了 由那個位置的顏色資訊轉換成RGB  
		cvRectangle(histimg, cvPoint(i*bin_w, histimg->height), cvPoint((i + 1)*bin_w, histimg->height - val), color, -1, 8, 0);//畫出不同顏色的直方長條來
	}
	//---設定ROI和畫出直方圖 end---
	IplImage* returnImg = nullptr;
	returnImg = cvCloneImage(histimg);

	cvReleaseImage(&hue);
	cvReleaseImage(&mask);
	cvReleaseImage(&histimg);

	return returnImg;
}

CvBox2D CamShiftPatch::getTrackBox(CvScalar maskRange, CvHistogram *hist)
{
	IplImage* backproject = cvCreateImage(cvGetSize(originImage), 8, 1);//反投影空間，單通道
	IplImage* hue = 0;
	hue = cvCreateImage(cvGetSize(originImage), 8, 1);
	IplImage *mask = getInRangeMask(maskRange, hue);

	cvCalcBackProject(&hue, backproject, hist); //使用back project方法 ，計算hue的反投影圖
	cvAnd(backproject, mask, backproject, 0);   // 將backproject 與mask 做AND 運算 再放到backproject 

	CvConnectedComp track_comp;
	CvBox2D track_box; // tracking返回的區域box，帶角度 

	CvRect zero;
	zero.x = 0; zero.y = 0; zero.width = 320; zero.height = 240;
	track_window = zero;

	for (int i = 0; i < 10; i++)
	{
		cvCamShift(
			backproject,    //色彩概率分佈圖像
			track_window,   //Search Window的初始值
			cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1),//用來判斷搜尋是否停止的一個標準
			&track_comp,    //保存運算結果,包括新的Search Window的位置和面積
			&track_box     //包含被跟蹤物體的最小矩形
			);
		track_window = track_comp.rect;
	}

	cvReleaseImage(&backproject);
	cvReleaseImage(&hue);
	cvReleaseImage(&mask);

	return track_box;
}
