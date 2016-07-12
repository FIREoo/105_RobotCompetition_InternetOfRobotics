#pragma once
#include"Cvvimage.h"
#include"..//Kinect2Capture//Kinect2Capture.h"
#include "afxwin.h"

// setROIDlg 對話方塊

class setROIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(setROIDlg)

public:
	setROIDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~setROIDlg();

// 對話方塊資料
	enum { IDD = IDD_DIALOG_setROI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic m_img_RGB;
	Kinect2Capture kinect;
	void ShowImage(IplImage *Image, CWnd *pWnd);
	void ShowImage_d(IplImage *Image, CWnd *pWnd);

	static IplImage* sImg_color__x1_ch3;
	static IplImage* sImg_depth_x2_ch3;

	int pointNum;
	cv::Point2f LTP;
	cv::Point2f LDP;
	cv::Point2f RDP;
	cv::Point2f RTP;

	static cv::Point2f pts_color[4];//set LT LD RD RT point rgb
	static cv::Point2f pts_depth[4];//set LT LD RD RT point depth
	static cv::Point2f pts_to[4];//to LT LD RD RT point
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
