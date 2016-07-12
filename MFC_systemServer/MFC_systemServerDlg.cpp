
// MFC_systemServerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFC_systemServer.h"
#include "MFC_systemServerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMG_WIDTH 320
#define IMG_HIGHT 240

// CMFC_systemServerDlg 對話方塊

CMFC_systemServerDlg::CMFC_systemServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_systemServerDlg::IDD, pParent)
	, m_testvalue(0)
	, m_detectObjNum(_T(""))
	, m_camera2global_x(0)
	, m_camera2global_y(0)
	, m_camera2global_x_zero(0)
	, m_camera2global_y_zero(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_systemServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_binPickLive, m_img_binPickLive);
	DDX_Text(pDX, IDC_EDIT2, m_testvalue);
	DDX_Control(pDX, IDC_TAB2, m_Tab);
	DDX_Control(pDX, IDC_IMAGE_binPickLiveDepth, m_img_binPickLiveDepth);
	DDX_Text(pDX, IDC_EDIT_detectObjNum, m_detectObjNum);
	DDX_Control(pDX, IDC_LIST_server, m_list_server);
	DDX_Text(pDX, IDC_EDIT_camera2global_x, m_camera2global_x);
	DDX_Text(pDX, IDC_EDIT_camera2global_y, m_camera2global_y);
	DDX_Text(pDX, IDC_EDIT_camera2global_x_zero, m_camera2global_x_zero);
	DDX_Text(pDX, IDC_EDIT_camera2global_y_zero, m_camera2global_y_zero);
}

BEGIN_MESSAGE_MAP(CMFC_systemServerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_test1, &CMFC_systemServerDlg::OnBnClickedButtontest1)
	ON_BN_CLICKED(IDC_BUTTON_test4, &CMFC_systemServerDlg::OnBnClickedButtontest4)
	ON_BN_CLICKED(IDC_BUTTON_test5, &CMFC_systemServerDlg::OnBnClickedButtontest5)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CMFC_systemServerDlg::OnTcnSelchangeTab2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC_systemServerDlg::OnBnClickedButton1)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_serverOn, &CMFC_systemServerDlg::OnBnClickedButtonserveron)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFC_systemServerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()




// CMFC_systemServerDlg 訊息處理常式

BOOL CMFC_systemServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO:  在此加入額外的初始設定
	GetDlgItem(IDC_EDIT_scara_X)->SetWindowText(L"550");
	GetDlgItem(IDC_EDIT_scara_Y)->SetWindowText(L"0");
	GetDlgItem(IDC_EDIT_scara_Z)->SetWindowText(L"150");
	GetDlgItem(IDC_EDIT_scara_T)->SetWindowText(L"0");

	m_Tab.InsertItem(0, _T("貨品選擇"));
	m_Tab.InsertItem(1, _T("運輸"));
	m_Tab.InsertItem(2, _T("設定"));

	m_TabPage1.Create(IDD_DIALOG_tab1, &m_Tab);
	m_TabPage2.Create(IDD_DIALOG_tab2, &m_Tab);
	m_TabPage3.Create(IDD_DIALOG_tab3, &m_Tab);

	//設定tab control 的第一頁在開始時能正確顯示
	CRect rTab, rItem;
	m_Tab.GetItemRect(0, &rItem);
	m_Tab.GetClientRect(&rTab);
	int x = rItem.left;
	int y = rItem.bottom + 1;
	int cx = rTab.right - rItem.left - 3;
	int cy = rTab.bottom - y - 2;
	m_TabPage3.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage2.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage1.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
	//**************************************************************************//
	CRect winRect;
	m_img_binPickLive.GetWindowRect(&winRect);
	m_img_binPickLive.SetWindowPos(NULL, 30, 30, 320, 240, SWP_SHOWWINDOW);
	//m_img_binPickLiveDepth.GetWindowRect(&winRect);
	m_img_binPickLiveDepth.SetWindowPos(NULL, 30 + 320, 30, 320, 240, SWP_SHOWWINDOW);


	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMFC_systemServerDlg::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRect rTab, rItem;
	m_Tab.GetItemRect(0, &rItem);
	m_Tab.GetClientRect(&rTab);
	int x = rItem.left;
	int y = rItem.bottom + 1;
	int cx = rTab.right - rItem.left - 3;
	int cy = rTab.bottom - y - 2;
	int tab = m_Tab.GetCurSel();

	m_TabPage1.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage2.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage3.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);

	switch (tab)
	{
	case 0:
		m_TabPage1.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
		break;
	case 1:
		m_TabPage2.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
		break;
	case 2:
		m_TabPage3.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
		break;
	}

	*pResult = 0;
}

void CMFC_systemServerDlg::ShowImage(IplImage *Image, CWnd *pWnd)
{
	CDC	*dc = pWnd->GetWindowDC();
	IplImage *Temp = NULL;

	Temp = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
	cvResize(Image, Temp, CV_INTER_LINEAR);

	CvvImage Temp2;
	Temp2.CopyOf(Temp);
	Temp2.Show(*dc, 0, 0, Temp->width, Temp->height);
	cvReleaseImage(&Temp);
	ReleaseDC(dc);
}
void CMFC_systemServerDlg::ShowImage(IplImage *Image, CWnd *pWnd, int channels)
{
	CDC	*dc = pWnd->GetWindowDC();
	IplImage *Temp = NULL;

	Temp = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, channels);
	cvResize(Image, Temp, CV_INTER_LINEAR);
	if (channels == 4)
	cvCvtColor(Temp, Temp, CV_BGRA2BGR);

	CvvImage Temp2;
	Temp2.CopyOf(Temp);
	Temp2.Show(*dc, 0, 0, Temp->width, Temp->height);
	cvReleaseImage(&Temp);
	ReleaseDC(dc);
}

CvRect findCase(IplImage* image)
{
	CvScalar pixel;
	CvPoint MaxP = cvPoint(0, 0);
	CvPoint MinP = cvPoint(320, 240);

	for (int i = 0; i < image->width; i++)
	{
		for (int j = 0; j < image->height; j++)
		{
			pixel = cvGet2D(image, j, i);
			if (pixel.val[0] > 1)
			{
				if (j < MinP.y)
					MinP.y = j;
				if (j > MaxP.y)
					MaxP.y = j;
				if (i < MinP.x)
					MinP.x = i;
				if (i > MaxP.x)
					MaxP.x = i;
			}
		}
	}
	CvRect theCase;
	theCase.x = MinP.x;
	theCase.y = MinP.y;
	theCase.width = MaxP.x - MinP.x;
	theCase.height = MaxP.y - MinP.y;
	return theCase;
}

UINT CMFC_systemServerDlg::MythreadFun(LPVOID LParam)
{
	CMythreadParam* para = (CMythreadParam*)LParam;
	CMFC_systemServerDlg* lpview = (CMFC_systemServerDlg*)(para->m_lpPara);
	para->m_blthreading = TRUE;

	switch (para->m_case)
	{
	case 0:
		lpview->Thread_getImage(LParam);
		break;
	case 1:
		lpview->Thread_mode(LParam);
		break;
	case 2:
		lpview->Thread_OpenSetROI();
		break;
	case 3:
		lpview->Thread_serverOn(LParam);
		break;
	case 4:
		lpview->Thread_sendPacket(LParam);
	default:
		break;
	}

	para->m_blthreading = FALSE;
	para->m_case = 0xFF;
	return 0;

}

void CMFC_systemServerDlg::Thread_getImage(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_systemServerDlg * hWnd = (CMFC_systemServerDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	
	Kinect2Capture kinect;
	kinect.Open(1, 1, 0);

	kinect.uDepthMax = 2000;
	IplImage* img_get = nullptr;
	while (1)
	{
		img_get = kinect.DepthImage();
		if (img_get != NULL)
		{
			cv::Mat src_img = img_get;
			// 設定變換[之前]與[之後]的坐標 (左上,左下,右下,右上)
			cv::Point2f pts1[4] = { roi.pts_depth[0], roi.pts_depth[1], roi.pts_depth[2], roi.pts_depth[3] };
			cv::Point2f pts2[4] = { roi.pts_to[0], roi.pts_to[1], roi.pts_to[2], roi.pts_to[3] };
			// 透視變換行列計算
			cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
			cv::Mat dst_img;
			// 變換
			cv::warpPerspective(src_img, dst_img, perspective_matrix, cvSize(320, 240), cv::INTER_LINEAR);
			
			cvCopy(&(IplImage)dst_img, m_TabPage1.sImage_depth);
			//*****影像相減*****
			cvAbsDiff(m_TabPage1.sImage_depth, m_TabPage1.sImage_depthGround, m_TabPage1.sImage_depth);
			cvThreshold(m_TabPage1.sImage_depth, m_TabPage1.sImage_depth, 1, 255, CV_THRESH_BINARY);
			//*******************
			//*****erode & dilate*****
			IplConvKernel *pKernel = NULL;
			pKernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
			cvErode(m_TabPage1.sImage_depth, m_TabPage1.sImage_depth, pKernel, 1);
			cvDilate(m_TabPage1.sImage_depth, m_TabPage1.sImage_depth, pKernel, 1);
			//*************************
			hWnd->ShowImage(m_TabPage1.sImage_depth, hWnd->GetDlgItem(IDC_IMAGE_binPickLiveDepth),1);
		}
		cvReleaseImage(&img_get);

		img_get = kinect.RGBAImage();
		if (img_get != NULL)
		{
			cv::Mat src_img = img_get;
			// 設定變換[之前]與[之後]的坐標 (左上,左下,右下,右上)
			cv::Point2f pts1[4] = { roi.pts_color[0], roi.pts_color[1], roi.pts_color[2], roi.pts_color[3] };
			cv::Point2f pts2[4] = { roi.pts_to[0], roi.pts_to[1], roi.pts_to[2], roi.pts_to[3] };
			// 透視變換行列計算
			cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
			cv::Mat dst_img;
			// 變換
			cv::warpPerspective(src_img, dst_img, perspective_matrix, cvSize(320, 240), cv::INTER_LINEAR);
			
			cvCopy(&(IplImage)dst_img, m_TabPage1.sImage_live);
			 
			IplImage* image_show = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
			cvCvtColor(m_TabPage1.sImage_live, image_show, CV_BGRA2BGR);
			hWnd->ShowImage(image_show, hWnd->GetDlgItem(IDC_IMAGE_binPickLive));
			cvReleaseImage(&image_show);
		}
		cvReleaseImage(&img_get);
	}
}

void CMFC_systemServerDlg::Thread_mode(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_systemServerDlg * hWnd = (CMFC_systemServerDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);

}

void CMFC_systemServerDlg::Thread_OpenSetROI()
{
	setROIDlg  pSetROIDlg(this);
	pSetROIDlg.DoModal();
}

void CMFC_systemServerDlg::Thread_serverOn(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_systemServerDlg * hWnd = (CMFC_systemServerDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);

	CListBox* listBox = (CListBox*)hWnd->GetDlgItem(IDC_LIST_server);

		if (!AfxSocketInit())
			listBox->AddString(L"Failed to Initialize Sockets");

		//creat two socket
		CSocket server_socket, receiveSocket;
		if (!server_socket.Socket())
			listBox->AddString(L"Create Faild");

		BOOL bOptVal = TRUE;
		int bOptLen = sizeof(BOOL);

		server_socket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);

		//CString strServerPort;
		//hWnd->GetDlgItemText(IDC_EDIT_serverPort, strServerPort);
		int nServerPort = 8266;//wifi IOT
		if (!server_socket.Bind(nServerPort))
			listBox->AddString(L"Bind Faild");
		if (!server_socket.Listen(10))
			listBox->AddString(L"Listen Faild");

		listBox->AddString(L"server on.");

		while (1)
		{
			if (!server_socket.Accept(receiveSocket))
				continue;
			else
			{
				CString ClientIP;
				UINT ClientPort;
				receiveSocket.m_nTimeOut = 1000;
				receiveSocket.GetPeerName(ClientIP, ClientPort);
				listBox->AddString(ClientIP + L" connected");

				char szRecvMsg[256] = { 0 };
				receiveSocket.Receive(szRecvMsg, 256);
				
				CString report;
				packetMeasure(szRecvMsg,report);

				//if (report == "1")
				//	listBox->AddString(L"yep");

				
				receiveSocket.GetPeerName(ClientIP, ClientPort);
				report =report + L"   from:" + ClientIP;
				listBox->AddString(ClientIP + L" : " + report);


				char c_report[100];
				USES_CONVERSION;
				strcpy_s(c_report, T2A(report));
				receiveSocket.Send(c_report, sizeof(c_report));

				receiveSocket.Close();
			}

		}

		server_socket.Close();
		receiveSocket.Close();

		listBox->AddString(L"sever close");
}

void CMFC_systemServerDlg::Thread_sendPacket(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_systemServerDlg * hWnd = (CMFC_systemServerDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	CListBox* listBox = (CListBox*)hWnd->GetDlgItem(IDC_LIST_server);

	AfxSocketInit();
	CSocket client_socket;

	char resp[256];

	if (!client_socket.Create())
	{
		listBox->AddString(L"Create Faild");
		return;
	}

	if (client_socket.Connect(L"192.168.1.3", 8888))
	{
		CString str;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_X, str);
		//float tarX = _wtof(str.GetBuffer(0));
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Y, str);
		//float tarY = _wtof(str.GetBuffer(0));
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Z, str);
		//float tarZ = _wtof(str.GetBuffer(0));
		//hWnd->GetDlgItemText(IDC_EDIT_scara_T, str);
		//float tarTheta = _wtof(str.GetBuffer(0));


		float tarX = 170;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Y, str);
		float tarY = 493;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Z, str);
		float tarZ = 100;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_T, str);
		float tarTheta = 0;

	
		packetCreat_toPoint(tarX, tarY, tarZ, tarTheta, &resp[0]);

		client_socket.Send(resp, sizeof(resp));
		listBox->AddString(L"go to point");
		


		//char szRecValue[1024] = { 0 };
		//aSocket.Receive((void *)szRecValue, 1024);
		//CString temp;
		//temp = (szRecValue);
		//GetDlgItem(IDC_EDIT_clientBuf)->SetWindowText(temp);
	}
	else
	{
		listBox->AddString(L"Creat Faild(Connect)");
	}
	client_socket.Close();
	//////////////////////////////////////////////////////////
	Sleep(5000);
	if (!client_socket.Create())
	{
		listBox->AddString(L"Create Faild");
		return;
	}

	if (client_socket.Connect(L"192.168.1.3", 8888))
	{
		CString str;
		float tarX = 170;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Y, str);
		float tarY = 493;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Z, str);

		packetCreat_toPoint(tarX, tarY, 200, -35, &resp[0]);

		client_socket.Send(resp, sizeof(resp));
		listBox->AddString(L"down");
	}
		
	else
	{
		listBox->AddString(L"Creat Faild(Connect)");
	}
	client_socket.Close();
	Sleep(3000);
	/////////////////////////////////////////////
	if (!client_socket.Create())
	{
		listBox->AddString(L"Create Faild");
		return;
	}

	if (client_socket.Connect(L"192.168.1.3", 8888))
	{

		packetCreat(L"N1r3", resp);
		client_socket.Send(resp, sizeof(resp));
		listBox->AddString(L"grap");
	}
	else
	{
		listBox->AddString(L"Creat Faild(Connect)");
	}
	client_socket.Close();
	Sleep(1000);
	/////////////////////////////////////////////
	if (!client_socket.Create())
	{
		listBox->AddString(L"Create Faild");
		return;
	}

	if (client_socket.Connect(L"192.168.1.3", 8888))
	{

		float tarX = 170;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Y, str);
		float tarY = 493;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_Z, str);
		float tarZ = 100;
		//hWnd->GetDlgItemText(IDC_EDIT_scara_T, str);
		float tarTheta = 0;


		packetCreat_toPoint(tarX, tarY, tarZ, tarTheta, &resp[0]);

		client_socket.Send(resp, sizeof(resp));
		listBox->AddString(L"UP");
	}
	else
	{
		listBox->AddString(L"Creat Faild(Connect)");
	}
	client_socket.Close();

}


void CMFC_systemServerDlg::saveImage(IplImage* live)
{
	pLive = cvCloneImage(live);
}

void CMFC_systemServerDlg::OnBnClickedButtontest1()
{
	//save

	Kinect2Capture kinect;
	kinect.Open(1, 1, 1);

	IplImage* tmp = nullptr;
	while (1)
	{
		kinect.uDepthMax = 1000;
		tmp = kinect.DepthImage();
		if (tmp != NULL)
		{
			//cvSetImageROI(tmp, depthROI);
			cvShowImage("depth", tmp);
			cvSaveImage("depth.png", tmp);
			if (cv::waitKey(30) == VK_ESCAPE){
				break;
			}
		}
		cvReleaseImage(&tmp);

		tmp = kinect.InfraredImage();
		if (tmp != NULL)
		{
			//cvSetImageROI(tmp, depthROI);
			cvShowImage("IR", tmp);
			cvSaveImage("IR.png", tmp);
			if (cv::waitKey(30) == VK_ESCAPE){
				break;
			}
		}
		cvReleaseImage(&tmp);

		tmp = kinect.RGBAImage();
		if (tmp != NULL)
		{
			//cvSetImageROI(tmp, colorROI);
			cvShowImage("color", tmp);
			cvSaveImage("color.png", tmp);
			if (cv::waitKey(30) == VK_ESCAPE){
				break;
			}
		}
		cvReleaseImage(&tmp);
	}
}

void CMFC_systemServerDlg::OnBnClickedButtontest4()
{
	IplImage* pImg_live = cvLoadImage("objectHis/liveTest.bmp", 1);
	IplImage* pImg_depth = cvLoadImage("objectHis/depthTest.bmp", 1);

	m_TabPage1.sImage_live = cvCloneImage(pImg_live);
	m_TabPage1.sImage_depth = cvCloneImage(pImg_depth);

	ShowImage(m_TabPage1.sImage_live, GetDlgItem(IDC_IMAGE_binPickLive));
	ShowImage(m_TabPage1.sImage_depth, GetDlgItem(IDC_IMAGE_binPickLiveDepth));

	cvReleaseImage(&pImg_live);
	cvReleaseImage(&pImg_depth);
}

void CMFC_systemServerDlg::OnBnClickedButtontest5()
{
	IplImage* pImg_live = cvLoadImage("objectHis/liveTest2.bmp", 1);
	IplImage* pImg_depth = cvLoadImage("objectHis/depthTest2.bmp", 1);

	m_TabPage1.sImage_live = cvCloneImage(pImg_live);
	m_TabPage1.sImage_depth = cvCloneImage(pImg_depth);

	ShowImage(m_TabPage1.sImage_live, GetDlgItem(IDC_IMAGE_binPickLive));
	ShowImage(m_TabPage1.sImage_depth, GetDlgItem(IDC_IMAGE_binPickLiveDepth));

	cvReleaseImage(&pImg_live);
	cvReleaseImage(&pImg_depth);
}


void CMFC_systemServerDlg::OnBnClickedButton1()
{
	IplImage* pImg_live = cvLoadImage("objectHis/liveTest0.bmp", 1);
	IplImage* pImg_depth = cvLoadImage("objectHis/depthTest0.bmp", 1);

	m_TabPage1.sImage_live = cvCloneImage(pImg_live);
	m_TabPage1.sImage_depth = cvCloneImage(pImg_depth);

	ShowImage(m_TabPage1.sImage_live, GetDlgItem(IDC_IMAGE_binPickLive));
	ShowImage(m_TabPage1.sImage_depth, GetDlgItem(IDC_IMAGE_binPickLiveDepth));

	cvReleaseImage(&pImg_live);
	cvReleaseImage(&pImg_depth);
}
int tt = 0;
void CMFC_systemServerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	//左上1
	if (point.x > 30 && point.x < (30 + 320) && point.y > 30 && point.y < (30 + 240) && tt ==0)
	{
		//對準世界座標的(0,0)
		m_camera2global_x_zero = point.x - 30;
		m_camera2global_y_zero = point.y - 30;
		UpdateData(false);
		tt++;
	}
	else if (point.x > 30 && point.x < (30 + 320) && point.y > 30 && point.y < (30 + 240) && tt == 1)
	{
		//對準世界座標的(0,0)
		m_camera2global_x = point.x - 30;
		m_camera2global_y = point.y - 30;
		UpdateData(false);
		tt = 0;
	}
	CDialogEx::OnRButtonDown(nFlags, point);
}
void CMFC_systemServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect winRect;
	if (point.x > 30 && point.x < (30 + 320) && point.y > 30 && point.y < (30 + 240))
	{
		//open live
		UpdateData(TRUE);
		m_threadPara.m_case = 0;
		m_threadPara.hWnd = m_hWnd;
		m_lpThread = AfxBeginThread(&CMFC_systemServerDlg::MythreadFun, (LPVOID)&m_threadPara);
	}

	if (point.x > (30 + 320) && point.x < (30 + 320 + 320) && point.y > 30 && point.y < (30 + 240))
	{
		//get depth ground 
		Kinect2Capture kinect;
		kinect.Open(1, 1, 0);
		kinect.uDepthMax = 2000;
		IplImage* img_get = nullptr;
		while (1)
		{
			img_get = kinect.DepthImage();
			if (img_get != NULL)
			{
				cv::Mat src_img = img_get;
				// 設定變換[之前]與[之後]的坐標 (左上,左下,右下,右上)
				cv::Point2f pts1[4] = { roi.pts_depth[0], roi.pts_depth[1], roi.pts_depth[2], roi.pts_depth[3] };
				cv::Point2f pts2[4] = { roi.pts_to[0], roi.pts_to[1], roi.pts_to[2], roi.pts_to[3] };
				// 透視變換行列計算
				cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
				cv::Mat dst_img;
				// 變換
				cv::warpPerspective(src_img, dst_img, perspective_matrix, cvSize(320, 240), cv::INTER_LINEAR);

				m_TabPage1.sImage_depthGround = cvCloneImage(&(IplImage)dst_img);

				ShowImage(m_TabPage1.sImage_depthGround, GetDlgItem(IDC_IMAGE_binPickLiveDepth), 1);
				break;
			}
			cvReleaseImage(&img_get);
		}
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}
void CMFC_systemServerDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (point.x > 30 && point.x < (30 + 320) && point.y > 30 && point.y < (30 + 240))
	{
		//open live
		UpdateData(TRUE);
		m_threadPara.m_case = 2;
		m_threadPara.hWnd = m_hWnd;
		m_lpThread = AfxBeginThread(&CMFC_systemServerDlg::MythreadFun, (LPVOID)&m_threadPara);
	}

	CDialogEx::OnRButtonDblClk(nFlags, point);
}

void CMFC_systemServerDlg::OnBnClickedButtonserveron()
{
	UpdateData(TRUE);
	m_threadPara.m_case = 3;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&CMFC_systemServerDlg::MythreadFun, (LPVOID)&m_threadPara);
}

void CMFC_systemServerDlg::packetMeasure(char* msg, CString &report)
{
	report = msg;
	//int data[256];
	//data[0] = msg[0];
	if (msg[0] == 'N' && msg[1] == '2')
	{
		//switch (msg[2])
		//{
		//case 'r':
		//	break;
		//case 'a':
		//	break;
		//case 'p':
		//	break;
		//case 't':
		//	break;

		//default:
		//	break;
		//}
	}

}
void CMFC_systemServerDlg::packetCreat(CString msg, char* report)
{
	char cMsg[256];
	USES_CONVERSION;
	strcpy_s(cMsg, T2A(msg));
	
	for (int i = 0; i < sizeof(cMsg); i++)
	{
		report[i] = cMsg[i];
		if (cMsg[i] == '\0')
			break;
	}
}
void CMFC_systemServerDlg::packetCreat_toPoint(float x, float y, float z, float t, char* report)
{
	int temp;

	int X_1 = x*0.01;
	int X_2 = x - X_1 * 100;
	temp = x * 100;
	int X_3 = temp % 100;

	int Y_1 = y*0.01;
	int Y_2 = y - Y_1 * 100;
	temp = y * 100;
	int Y_3 = temp % 100;

	int Z_1 = z*0.01;
	int Z_2 = z - Z_1 * 100;
	temp = z * 100;
	int Z_3 = temp % 100;

	int theta_1 = t*0.01;
	int theta_2 = t - theta_1 * 100;
	temp = t * 100;
	int theta_3 = temp % 100;

	report[0] = 'N';
	report[1] = '1';
	report[2] = 'r';
	report[3] = '2';
	report[4] = X_1;
	report[5] = X_2;
	report[6] = X_3;
	report[7] = Y_1;
	report[8] = Y_2;
	report[9] = Y_3;
	report[10] = Z_1;
	report[11] = Z_2;
	report[12] = Z_3;
	report[13] = theta_1;
	report[14] = theta_2;
	report[15] = theta_3;
}

void CMFC_systemServerDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	m_threadPara.m_case = 4;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&CMFC_systemServerDlg::MythreadFun, (LPVOID)&m_threadPara);
}
