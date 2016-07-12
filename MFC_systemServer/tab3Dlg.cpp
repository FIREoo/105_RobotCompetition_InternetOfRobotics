// tab3Dlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MFC_systemServer.h"
#include "tab3Dlg.h"
#include "afxdialogex.h"



// tab3Dlg 對話方塊

IMPLEMENT_DYNAMIC(tab3Dlg, CDialogEx)

tab3Dlg::tab3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(tab3Dlg::IDD, pParent)
	, m_globleZeroOnImg_x(0)
	, m_globleZeroOnImg_y(0)
	, m_cam2globle_x(0)
	, m_cam2globle_y(0)
	, m_G_Xpos(0)
	, m_G_Ypos(0)
	, m_A_Xpos(0)
	, m_A_Ypos(0)
	, m_globleZeroOnArm_x(0)
	, m_globleZeroOnArm_y(0)
	, m_Ac_Xpos(0)
	, m_Ac_Ypos(0)
{
	
}

tab3Dlg::~tab3Dlg()
{
}

void tab3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_keystone, m_img_keystone);
	DDX_Text(pDX, IDC_EDIT_globleZeroOnImg_x, m_globleZeroOnImg_x);
	DDX_Text(pDX, IDC_EDIT_globleZeroOnImg_y, m_globleZeroOnImg_y);
	DDX_Text(pDX, IDC_EDIT_cam2globle_x, m_cam2globle_x);
	DDX_Text(pDX, IDC_EDIT_cam2globle_y, m_cam2globle_y);
	DDX_Text(pDX, IDC_STATIC_xpos, m_Xpos);
	DDX_Text(pDX, IDC_STATIC_Ypos, m_Ypos);
	DDX_Text(pDX, IDC_STATIC_G_Xpos, m_G_Xpos);
	DDX_Text(pDX, IDC_STATIC_G_Ypos, m_G_Ypos);
	DDX_Text(pDX, IDC_STATIC_A_Xpos, m_A_Xpos);
	DDX_Text(pDX, IDC_STATIC_A_Ypos, m_A_Ypos);
	DDX_Text(pDX, IDC_EDIT_globleZeroOnArm_x, m_globleZeroOnArm_x);
	DDX_Text(pDX, IDC_EDIT_globleZeroOnArm_y, m_globleZeroOnArm_y);
	DDX_Text(pDX, IDC_STATIC_Ac_Xpos, m_Ac_Xpos);
	DDX_Text(pDX, IDC_STATIC_Ac_Ypos, m_Ac_Ypos);
}

BEGIN_MESSAGE_MAP(tab3Dlg, CDialogEx)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// tab3Dlg 訊息處理常式
BOOL tab3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_img_keystone.SetWindowPos(NULL, 10, 10, 320, 240, SWP_SHOWWINDOW);
	FLAG_liveStart = false;

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}
void tab3Dlg::ShowImage(IplImage *Image, CWnd *pWnd, int channels)
{
	CDC	*dc = pWnd->GetWindowDC();
	IplImage *Temp = NULL;

	if (channels == 4)
	{
		Temp = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
		cvCvtColor(Image, Temp, CV_BGRA2BGR);
	}
	else
	{
		Temp = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, channels);
	}
		

	CvvImage Temp2;
	Temp2.CopyOf(Temp);
	Temp2.Show(*dc, 0, 0, Temp->width, Temp->height);
	cvReleaseImage(&Temp);
	ReleaseDC(dc);
}

void tab3Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > (10) && point.x < (10 + 320) && point.y > 10 && point.y < (10 + 240))
	{
		m_globleZeroOnImg_x = point.x - 10;
		m_globleZeroOnImg_y = point.y - 10;
		UpdateData(false);
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void tab3Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	if (point.x > (10) && point.x < (10 + 320) && point.y > 10 && point.y < (10 + 240))
	{
		m_cam2globle_x = 200.000 / abs((point.x - 10) - m_globleZeroOnImg_x);
		m_cam2globle_y = 200.000 / abs((point.y - 10) - m_globleZeroOnImg_y);
		UpdateData(false);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void tab3Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (point.x > (10) && point.x < (10 + 320) && point.y > 10 && point.y < (10 + 240))
	{
		UpdateData(true);
		m_Xpos = point.x-10;
		m_Ypos = point.y-10;
		m_G_Xpos = -((point.y - 10) - m_globleZeroOnImg_y)*m_cam2globle_y;
		m_G_Ypos = -((point.x - 10) - m_globleZeroOnImg_x)*m_cam2globle_x;
		m_A_Xpos = -m_G_Xpos;
		m_A_Ypos = m_G_Ypos;
		UpdateData(false);
		UpdateData(true);
		m_Ac_Xpos = m_A_Xpos + m_globleZeroOnArm_x;
		m_Ac_Ypos = m_A_Ypos + m_globleZeroOnArm_y;
		UpdateData(false);
	}


	if (point.x > (10) && point.x < (10 + 320) && point.y > 10 && point.y < (10 + 240) && nFlags == MK_RBUTTON)
	{
		m_globleZeroOnImg_x = point.x - 10;
		m_globleZeroOnImg_y = point.y - 10;
		UpdateData(false);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void tab3Dlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > (10) && point.x < (10 + 320) && point.y > 10 && point.y < (10 + 240))
	{
		ShowImage(m_TabPage1.sImage_live, GetDlgItem(IDC_IMAGE_keystone), 4);
	}

	CDialogEx::OnMButtonDown(nFlags, point);
}
