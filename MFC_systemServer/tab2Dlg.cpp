// tab2Dlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFC_systemServer.h"
#include "tab2Dlg.h"
#include "afxdialogex.h"
#include "Aria.h"
#include "fstream"
#include "iostream"

//P3DX
ArTcpConnection con;
ArRobot robot;
ArTime start;

// tab2Dlg 對話方塊

IMPLEMENT_DYNAMIC(tab2Dlg, CDialogEx)

tab2Dlg::tab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(tab2Dlg::IDD, pParent)
	, m_ipText(_T(""))
	, m_map_Xpos(0)
	, m_map_Ypos(0)
{
	m_ipText = _T("192.168.1.213:88");

}

// 設定變換[之前]與[之後]的坐標 (左上,左下,右下,右上)
cv::Point2f gPts1[4] = { cv::Point2f(29, 0), cv::Point2f(0, 230), cv::Point2f(320, 240), cv::Point2f(305, 5) };
cv::Point2f gPts2[4] = { cv::Point2f(0, 0), cv::Point2f(0, 240), cv::Point2f(320, 240), cv::Point2f(320, 0) };

tab2Dlg::~tab2Dlg()
{
}

void tab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AXISMEDIACONTROL1, m_AMC);
	DDX_Text(pDX, IDC_EDIT_IPcamIP, m_ipText);
	DDX_Control(pDX, IDC_IMAGE_IPcam, m_img_IPcam);
	DDX_Control(pDX, IDC_IMAGE_IPcam_map, m_img_IPcam_map);
	DDX_Text(pDX, IDC_EDIT_map_Xpos, m_map_Xpos);
	DDX_Text(pDX, IDC_EDIT_map_Ypos, m_map_Ypos);
	DDX_Control(pDX, IDC_CHECK_IPcam_keystone, m_check_IPcam_showKeystone);
	DDX_Control(pDX, IDC_IMAGE_map, m_map);
}


BEGIN_MESSAGE_MAP(tab2Dlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_connectIPcam, &tab2Dlg::OnBnClickedButtonconnectipcam)
	ON_BN_CLICKED(IDC_BUTTON_setGround, &tab2Dlg::OnBnClickedButtonsetground)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_IPcam_keystone, &tab2Dlg::OnBnClickedCheckIpcamkeystone)
	ON_BN_CLICKED(IDC_BUTTON2, &tab2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_connect_P3DX, &tab2Dlg::OnBnClickedButtonconnectP3dx)
	ON_BN_CLICKED(IDC_BUTTON_P3DX_back, &tab2Dlg::OnBnClickedButtonP3dxback)
	ON_BN_CLICKED(IDC_BUTTON_P3DX_stop, &tab2Dlg::OnBnClickedButtonP3dxstop)
	ON_BN_CLICKED(IDC_BUTTON_P3DX_front, &tab2Dlg::OnBnClickedButtonP3dxfront)
END_MESSAGE_MAP()

IplImage* tab2Dlg::sImage_IPcam = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
IplImage* tab2Dlg::sImage_IPcam_ground = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
// tab2Dlg 訊息處理常式
BOOL tab2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_AMC.SetWindowPos(NULL, 10, 10, 320, 240, SWP_SHOWWINDOW);
	m_img_IPcam.SetWindowPos(NULL, 10, 10, 320, 240, SWP_HIDEWINDOW);
	m_img_IPcam_map.SetWindowPos(NULL, 10, 250, 320, 240, SWP_SHOWWINDOW);
	m_map.SetWindowPos(NULL, 10, 490, 400, 400, SWP_SHOWWINDOW);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void tab2Dlg::ShowImage(IplImage *Image, CWnd *pWnd, int channels)
{
	CDC	*dc = pWnd->GetWindowDC();
	IplImage *Temp = NULL;

	Temp = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, channels);
	cvResize(Image, Temp, CV_INTER_LINEAR);

	CvvImage Temp2;
	Temp2.CopyOf(Temp);
	Temp2.Show(*dc, 0, 0, Temp->width, Temp->height);
	cvReleaseImage(&Temp);
	ReleaseDC(dc);
}
void tab2Dlg::ShowMap(IplImage *Image, CWnd *pWnd)
{
	CDC	*dc = pWnd->GetWindowDC();

	CvvImage Temp2;
	Temp2.CopyOf(Image);
	Temp2.Show(*dc, 0, 0, Image->width, Image->height);
	ReleaseDC(dc);
}

void tab2Dlg::GetImage()
{
	/*	-----------------------------------------------------------------
	*	Get Image from PTZ
	*	-----------------------------------------------------------------*/
	// Initial Variable
	int pictureShow_width = 320;
	int pictureShow_height = 240;

	PTZImage = new unsigned char[pictureShow_width*pictureShow_height * 3];
	VARIANT var;
	long bufferSize;
	VariantInit(&var);
	m_AMC.GetCurrentImage(1, &var, &bufferSize);
	BYTE* buf = var.pbVal;			// get the buffer as a byte array
	buf += sizeof(BITMAPINFOHEADER);// seek the beginnig of the   

	for (int i = 0; i < pictureShow_height; i++)
	{
		for (int j = 0; j < pictureShow_width; j++)
		{
			if (j > pictureShow_width - 8)
			{
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 0) = *(buf + 3 * (j - (pictureShow_width - 8)) + 3 * (i + 1)*pictureShow_width + 0);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 1) = *(buf + 3 * (j - (pictureShow_width - 8)) + 3 * (i + 1)*pictureShow_width + 1);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 2) = *(buf + 3 * (j - (pictureShow_width - 8)) + 3 * (i + 1)*pictureShow_width + 2);
			}
			else
			{
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 0) = *(buf + 3 * (j + 8) + 3 * i*pictureShow_width + 0);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 1) = *(buf + 3 * (j + 8) + 3 * i*pictureShow_width + 1);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 2) = *(buf + 3 * (j + 8) + 3 * i*pictureShow_width + 2);
			}
		}
	}
	VariantClear(&var);

	cvSetData(sImage_IPcam, PTZImage, 320 * 3);
	/****************************/
	cvFlip(sImage_IPcam);
}
void tab2Dlg::GetImageAndColorDetect()
{
	/*	-----------------------------------------------------------------
	*	Get Image from PTZ
	*	-----------------------------------------------------------------*/
	// Initial Variable
	int pictureShow_width = 320;
	int pictureShow_height = 240;

	PTZImage = new unsigned char[pictureShow_width*pictureShow_height * 3];
	VARIANT var;
	long bufferSize;
	VariantInit(&var);
	m_AMC.GetCurrentImage(1, &var, &bufferSize);
	BYTE* buf = var.pbVal;			// get the buffer as a byte array
	buf += sizeof(BITMAPINFOHEADER);// seek the beginnig of the   

	for (int i = 0; i < pictureShow_height; i++)
	{
		for (int j = 0; j < pictureShow_width; j++)
		{
			if (j > pictureShow_width - 8)
			{
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 0) = *(buf + 3 * (j - (pictureShow_width - 8)) + 3 * (i + 1)*pictureShow_width + 0);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 1) = *(buf + 3 * (j - (pictureShow_width - 8)) + 3 * (i + 1)*pictureShow_width + 1);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 2) = *(buf + 3 * (j - (pictureShow_width - 8)) + 3 * (i + 1)*pictureShow_width + 2);
			}
			else
			{
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 0) = *(buf + 3 * (j + 8) + 3 * i*pictureShow_width + 0);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 1) = *(buf + 3 * (j + 8) + 3 * i*pictureShow_width + 1);
				*(PTZImage + 3 * j + 3 * i*pictureShow_width + 2) = *(buf + 3 * (j + 8) + 3 * i*pictureShow_width + 2);
			}
		}
	}
	VariantClear(&var);

	cvSetData(sImage_IPcam, PTZImage, 320 * 3);
	/****************************/
	cvFlip(sImage_IPcam);

	//*****keystone*****
	//cv::Mat src_img = sImage_IPcam;

	//// 透視變換行列計算
	//cv::Mat perspective_matrix = cv::getPerspectiveTransform(gPts1, gPts2);
	//cv::Mat dst_img;
	//// 變換
	//cv::warpPerspective(src_img, dst_img, perspective_matrix, cvSize(320, 240), cv::INTER_LINEAR);

	//cvCopy(&(IplImage)dst_img, sImage_IPcam);

	IplImage* nLive_hsv = cvCreateImage(cvGetSize(sImage_IPcam), 8, 3);
	IplImage* nLive_mask = cvCreateImage(cvGetSize(sImage_IPcam), 8, 1);

	cvCopy(sImage_IPcam, nLive_hsv);
	cvCvtColor(nLive_hsv, nLive_hsv, CV_BGR2HSV);
	cvInRangeS(//cvInRangeS 功能為 => 檢查數組元素是否在兩個數量之間
		nLive_hsv, //第一個原數組
		cvScalar(45, 50, 85),  //180,255,255
		cvScalar(80, 160, 256), //不包括進的上邊界
		nLive_mask);

	//*****erode & dilate*****
	IplConvKernel *pKernel = NULL;
	pKernel = cvCreateStructuringElementEx(4, 4, 1, 1, CV_SHAPE_ELLIPSE, NULL);
	cvDilate(nLive_mask, nLive_mask, pKernel, 1);
	cvErode(nLive_mask, nLive_mask, pKernel, 2);
	//********************
	CvPoint centerPoint;
	double m00, m10, m01;
	CvMoments moment;
	cvMoments(nLive_mask, &moment, 1);
	m00 = cvGetSpatialMoment(&moment, 0, 0);
	m10 = cvGetSpatialMoment(&moment, 1, 0);
	m01 = cvGetSpatialMoment(&moment, 0, 1);
	centerPoint.x = (int)(m10 / m00);
	centerPoint.y = (int)(m01 / m00);
	
	UpdateData(1);
	m_map_Xpos = centerPoint.x;
	m_map_Ypos = centerPoint.y;
	UpdateData(0);

	cvSaveImage("ipcam_ks.png", sImage_IPcam);
	ShowImage(sImage_IPcam, GetDlgItem(IDC_IMAGE_IPcam), 3);
	ShowImage(nLive_mask, GetDlgItem(IDC_IMAGE_IPcam_map), 1);

	cvReleaseImage(&nLive_hsv);
	cvReleaseImage(&nLive_mask);
}
void tab2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0) //timer 0
	{
		GetImage();
		//*****keystone*****
		cv::Mat src_img = sImage_IPcam;
		// 設定變換[之前]與[之後]的坐標 (左上,左下,右下,右上)
		cv::Point2f pts1[4] = { cv::Point2f(32, 0), cv::Point2f(0, 235), cv::Point2f(320, 240), cv::Point2f(300, 5) };
		cv::Point2f pts2[4] = { cv::Point2f(0, 0), cv::Point2f(0, 240), cv::Point2f(320, 240), cv::Point2f(320, 0) };
		// 透視變換行列計算
		cv::Mat perspective_matrix = cv::getPerspectiveTransform(gPts1, gPts2);
		cv::Mat dst_img;
		// 變換
		cv::warpPerspective(src_img, dst_img, perspective_matrix, cvSize(320, 240), cv::INTER_LINEAR);

		cvCopy(&(IplImage)dst_img, sImage_IPcam);
		//********************
		ShowImage(sImage_IPcam, GetDlgItem(IDC_IMAGE_IPcam), 3);

		//*****影像相減*****
		IplImage* imgDiff = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 1);
		IplImage* img_gray_IPcam = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 1);
		IplImage* img_gray_IPcam_ground = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 1);
		cvCvtColor(sImage_IPcam, img_gray_IPcam, CV_BGR2GRAY);
		cvCvtColor(sImage_IPcam_ground, img_gray_IPcam_ground, CV_BGR2GRAY);
		cvAbsDiff(img_gray_IPcam, img_gray_IPcam_ground, imgDiff);
		cvThreshold(imgDiff, imgDiff, 50, 255, CV_THRESH_BINARY);
		//*******************
		//*****erode & dilate*****
		IplConvKernel *pKernel = NULL;
		pKernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
		cvErode(imgDiff, imgDiff, pKernel, 2);
		cvDilate(imgDiff, imgDiff, pKernel, 1);
		//********************

		IplImage* imgDiff_ch3 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		cvCvtColor(imgDiff, imgDiff_ch3, CV_GRAY2BGR);

		IplImage* pImg_colorMask = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		cvAnd(sImage_IPcam, imgDiff_ch3, pImg_colorMask);//(ch3,ch3,ch3)

		ShowImage(pImg_colorMask, GetDlgItem(IDC_IMAGE_IPcam_map), 3);
		delete[] PTZImage;
		cvReleaseImage(&imgDiff);
		cvReleaseImage(&img_gray_IPcam);
		cvReleaseImage(&img_gray_IPcam_ground);
		cvReleaseImage(&imgDiff_ch3);
		cvReleaseImage(&pImg_colorMask);
	}
	if (nIDEvent == 1) //timer 0
	{
		GetImageAndColorDetect();
		delete[] PTZImage;
	}

	CDialogEx::OnTimer(nIDEvent);
}

UINT tab2Dlg::MythreadFun(LPVOID LParam)
{
	CTab2threadParam* para = (CTab2threadParam*)LParam;
	tab2Dlg* lpview = (tab2Dlg*)(para->m_lpPara);
	para->m_blthreading = TRUE;

	switch (para->m_case)
	{
	case 0:
		lpview->Thread_localization(LParam);
		break;
	case 1:
		lpview->Thread_mapBace(LParam);
		break;
	case 2:
		lpview->Thread_map(LParam);
		break;
	default:
		break;
	}

	para->m_blthreading = FALSE;
	para->m_case = 0xFF;
	return 0;

}

void tab2Dlg::Thread_localization(LPVOID lParam)
{
	CTab2threadParam * Thread_Info = (CTab2threadParam *)lParam;
	tab2Dlg * hWnd = (tab2Dlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);

	while (1)
	{
		CString str;
		hWnd->GetDlgItemText(IDC_EDIT_map_Ypos, str);
		float tarY = _wtof(str.GetBuffer(0));
		int int_Ypos = tarY;
		if (int_Ypos<=100)
		{
			robot.setVel(-30);
		}
		if (int_Ypos<137 && int_Ypos>100)
		{
			robot.setVel(-10);
		}

		if (int_Ypos>=137)
		{
			robot.setVel(0);
			break;
		}
	}
	


}

void tab2Dlg::Thread_mapBace(LPVOID lParam)
{
	CTab2threadParam * Thread_Info = (CTab2threadParam *)lParam;
	tab2Dlg * hWnd = (tab2Dlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);

	IplImage* imgMap = cvCreateImage(cvSize(400, 400), IPL_DEPTH_8U, 3);
	cvZero(imgMap);
	hWnd->ShowMap(imgMap, hWnd->GetDlgItem(IDC_IMAGE_map));
	for (int x = 0; x <= 400 ; x=x+25)
		for (int y = 0; y <= 400 ; y = y+25)
			cvCircle(imgMap, cvPoint(x, y),0,CV_RGB(255,255,255));
	hWnd->ShowMap(imgMap, hWnd->GetDlgItem(IDC_IMAGE_map));
	


	cvCircle(imgMap, cvPoint(200, 350), 3, CV_RGB(255, 255, 0),5);
	cvLine(imgMap, cvPoint(200, 350), cvPoint(200, 340), CV_RGB(255, 0, 0),2 );

	hWnd->ShowMap(imgMap, hWnd->GetDlgItem(IDC_IMAGE_map));
}

void tab2Dlg::Thread_map(LPVOID lParam)
{
	CTab2threadParam * Thread_Info = (CTab2threadParam *)lParam;
	tab2Dlg * hWnd = (tab2Dlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	//讀值時間
	
	int matrix[3227][4];

	std::fstream input("Pos01.txt", std::ios::in);
	for (int i = 0; i < 3227; i++)
		for (int j = 0; j<4; j++)
			input >> matrix[i][j];

	input.close();

}

void tab2Dlg::OnBnClickedButtonconnectipcam()
{
	try
	{
		this->UpdateData(true);
		//m_ipText = _T("169.254.179.228");//ip
		//m_ipText = _T("192.168.1.213:88");//ip
		mediaURL = (CString)"http://" + m_ipText + (CString)"/axis-cgi/mjpg/video.cgi";
		ctrlURL = (CString)"http://" + m_ipText + (CString)"/axis-cgi/com/ptz.cgi";
		//ctrlURL.Format(_T("http://%s/axis-cgi/com/ptz.cgi"), m_ipText);
		//mediaURL.Format(_T("http://%s/axis-cgi/mjpg/video.cgi"), m_ipText);
		m_user = "root";
		m_pass = "pass";
		m_AMC.Stop();
		m_AMC.put_MediaUsername(m_user);
		m_AMC.put_MediaPassword(m_pass);
		m_AMC.put_PTZControlURL(ctrlURL);
		m_AMC.put_MediaURL(mediaURL);
		m_AMC.put_StretchToFit(true);
		m_AMC.Play();
	}
	catch (COleDispatchException *e)
	{
		MessageBox(e->m_strDescription);
	}
	Sleep(2000);
}

void tab2Dlg::OnBnClickedButtonsetground()
{
	robot.disconnect();
}

void tab2Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	//第二張圖
	if (point.x > 10 && point.x < (10 + 320) && point.y >(10 + 240) && point.y < (10 + 240 + 240))
	{
		SetTimer(1, 10, NULL);//set timer 1
	}
	//第三張圖
	if (point.x > 10 && point.x < (10 + 320) && point.y >(10 + 240 + 240) && point.y < (10 + 240 + 240 + 240))
	{
		m_threadPara.m_case = 1;
		m_threadPara.hWnd = m_hWnd;
		m_lpThread = AfxBeginThread(&tab2Dlg::MythreadFun, (LPVOID)&m_threadPara);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void tab2Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	//第二張圖
	if (point.x > 10 && point.x < (10 + 320) && point.y >(10 + 240) && point.y < (10 + 240 + 240))
	{
		KillTimer(1);//kill timer 0
	}
	//第三張圖
	if (point.x > 10 && point.x < (10 + 320) && point.y >(10 + 240 + 240) && point.y < (10 + 240 + 240 + 240))
	{
		m_threadPara.m_case = 2;
		m_threadPara.hWnd = m_hWnd;
		m_lpThread = AfxBeginThread(&tab2Dlg::MythreadFun, (LPVOID)&m_threadPara);
	}
	CDialogEx::OnRButtonDown(nFlags, point);
}

void tab2Dlg::OnBnClickedCheckIpcamkeystone()
{
	if (m_check_IPcam_showKeystone.GetCheck() == 0)
	{
		m_AMC.SetWindowPos(NULL, 10, 10, 320, 240, SWP_SHOWWINDOW);
		m_img_IPcam.SetWindowPos(NULL, 10, 10, 320, 240, SWP_HIDEWINDOW);
	}
	else if(m_check_IPcam_showKeystone.GetCheck() == 1)
	{
		m_AMC.SetWindowPos(NULL, 10, 10, 320, 240, SWP_HIDEWINDOW);
		m_img_IPcam.SetWindowPos(NULL, 10, 10, 320, 240, SWP_SHOWWINDOW);
	}
}


void tab2Dlg::OnBnClickedButtonconnectP3dx()
{
	Aria::init();
	robot.lock();
	con.setPort("10.0.126.12", 8101);

	if (!con.openSimple())
	{
		("Open failed.");
		Aria::shutdown();
	}
	robot.setDeviceConnection(&con);
	if (!robot.blockingConnect())
	{
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
	}
	robot.enableMotors();
	robot.disableSonar();		                 // Disables the sonar.
	robot.requestEncoderPackets();// Starts a continuous stream of encoder packets.

	robot.runAsync(true);
	robot.unlock();
}


void tab2Dlg::OnBnClickedButtonP3dxback()
{
	robot.setVel(-10);
}


void tab2Dlg::OnBnClickedButtonP3dxstop()
{
	robot.setVel(0);
}


void tab2Dlg::OnBnClickedButtonP3dxfront()
{
	robot.setVel(10);
}

void tab2Dlg::OnBnClickedButton2()
{ 
	m_threadPara.m_case = 0;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&tab2Dlg::MythreadFun, (LPVOID)&m_threadPara);

}