// setROIDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFC_systemServer.h"
#include "setROIDlg.h"
#include "afxdialogex.h"


// setROIDlg 對話方塊
cv::Point2f setROIDlg::pts_color[] = { cv::Point2f(0, 0), cv::Point2f(0, 1080), cv::Point2f(1920, 1080), cv::Point2f(1920, 0) };
cv::Point2f setROIDlg::pts_depth[] = { cv::Point2f(0, 0), cv::Point2f(0, 424), cv::Point2f(512, 424), cv::Point2f(512, 0) };
cv::Point2f setROIDlg::pts_to[] = { cv::Point2f(0, 0), cv::Point2f(0, 240), cv::Point2f(320, 240), cv::Point2f(320, 0) };

IplImage* setROIDlg::sImg_color__x1_ch3 = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);
IplImage* setROIDlg::sImg_depth_x2_ch3 = cvCreateImage(cvSize(1024, 848), IPL_DEPTH_16U, 3);


IMPLEMENT_DYNAMIC(setROIDlg, CDialogEx)

setROIDlg::setROIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(setROIDlg::IDD, pParent)
{

}

setROIDlg::~setROIDlg()
{
}

void setROIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_RGB, m_img_RGB);
}

BEGIN_MESSAGE_MAP(setROIDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// setROIDlg 訊息處理常式
BOOL setROIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此加入額外的初始設定
	m_img_RGB.SetWindowPos(NULL, 0, 0, 1920, 1080, SWP_SHOWWINDOW);
	pointNum = 0;

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void setROIDlg::ShowImage(IplImage *Image, CWnd *pWnd)
{
	CDC	*dc = pWnd->GetWindowDC();

	IplImage* tmp = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);

	cvCvtColor(Image, tmp, CV_RGBA2RGB);

	cvCopy(tmp, sImg_color__x1_ch3);

	CvvImage Temp2;
	Temp2.CopyOf(tmp);
	Temp2.Show(*dc, 0, 0, tmp->width, tmp->height);
	cvReleaseImage(&tmp);
	ReleaseDC(dc);
}
void setROIDlg::ShowImage_d(IplImage *Image, CWnd *pWnd)
{
	CDC	*dc = pWnd->GetWindowDC();

	IplImage* tmp_ch1 = cvCreateImage(cvSize(1024, 848), IPL_DEPTH_16U, 1);
	
	cvResize(Image, tmp_ch1, CV_INTER_LINEAR);

	IplImage* tmp_ch3 = cvCreateImage(cvSize(1024, 848), IPL_DEPTH_16U, 3);
	cvCvtColor(tmp_ch1, tmp_ch3, CV_GRAY2BGR);
	
	cvCopy(tmp_ch3, sImg_depth_x2_ch3);

	CvvImage Temp2;
	Temp2.CopyOf(tmp_ch3);
	Temp2.Show(*dc, 0, 0, tmp_ch3->width, tmp_ch3->height);
	cvReleaseImage(&tmp_ch1);
	cvReleaseImage(&tmp_ch3);
	ReleaseDC(dc);
}

void setROIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	IplImage* tmp = nullptr;
	switch (pointNum)
	{
	case 0:
		ShowWindow(SW_SHOWMAXIMIZED);
		kinect.Open(1, 1, 1);
		while (1)
		{
			tmp = kinect.RGBAImage();
			if (tmp != NULL)
			{
				ShowImage(tmp, GetDlgItem(IDC_IMAGE_RGB));
				break;
			}
			cvReleaseImage(&tmp);
		}

		break;
	case 1:
		LTP.x = point.x;
		LTP.y = point.y;
		break;
	case 2:
		LDP.x = point.x;
		LDP.y = point.y;
		break;
	case 3:
		RDP.x = point.x;
		RDP.y = point.y;
		break;
	case 4:
		RTP.x = point.x;
		RTP.y = point.y;
		pts_color[0] = LTP;
		pts_color[1] = LDP;
		pts_color[2] = RDP;
		pts_color[3] = RTP;
		MessageBox(L"color done");
	case 5:
		while (1)
		{
			tmp = kinect.InfraredImage();
			if (tmp != NULL)
			{
				ShowImage_d(tmp, GetDlgItem(IDC_IMAGE_RGB));
				break;
			}
			cvReleaseImage(&tmp);
		}
		pointNum = 5;
		break;
	case 6:
		LTP.x = point.x/2;
		LTP.y = point.y/2;
		break;
	case 7:
		LDP.x = point.x/2;
		LDP.y = point.y/2;
		break;
	case 8:
		RDP.x = point.x/2;
		RDP.y = point.y/2;
		break;
	case 9:
		RTP.x = point.x/2;
		RTP.y = point.y/2;
		pts_depth[0] = LTP;
		pts_depth[1] = LDP;
		pts_depth[2] = RDP;
		pts_depth[3] = RTP;
		MessageBox(L"Done");
		OnCancel();
		break;
	}
	pointNum++;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void setROIDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	if (pointNum >= 1 && pointNum <= 5)
	{
		IplImage* img_tmp;
		img_tmp = cvCloneImage(sImg_color__x1_ch3);
		cvLine(img_tmp, cvPoint(0, point.y), cvPoint(1920, point.y), CV_RGB(255, 0, 0), 1, CV_AA, 0);
		cvLine(img_tmp, cvPoint(point.x, 0), cvPoint(point.x, 1080), CV_RGB(255, 0, 0), 1, CV_AA, 0);
		
		CDC	*dc = GetDlgItem(IDC_IMAGE_RGB)->GetWindowDC();
		CvvImage Temp2;
		Temp2.CopyOf(img_tmp);
		Temp2.Show(*dc, 0, 0, img_tmp->width, img_tmp->height);
		ReleaseDC(dc);
		cvReleaseImage(&img_tmp);
	}

	if (pointNum >= 6 && pointNum <= 9)
	{
		IplImage* img_tmp = cvCreateImage(cvSize(1024, 848), 8, 3);
		img_tmp = cvCloneImage(sImg_depth_x2_ch3);
		cvLine(img_tmp, cvPoint(0, point.y), cvPoint(1024, point.y), CV_RGB(65536, 0, 0), 2, CV_AA, 0);
		cvLine(img_tmp, cvPoint(point.x, 0), cvPoint(point.x, 848), CV_RGB(65536, 0, 0), 2, CV_AA, 0);
		

		CDC	*dc = GetDlgItem(IDC_IMAGE_RGB)->GetWindowDC();
		CvvImage Temp2;
		Temp2.CopyOf(img_tmp);
		Temp2.Show(*dc, 0, 0, img_tmp->width, img_tmp->height);
		ReleaseDC(dc);
		cvReleaseImage(&img_tmp);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
