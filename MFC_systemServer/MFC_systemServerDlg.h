
// MFC_systemServerDlg.h : 標頭檔
//

#pragma once
#include"Cvvimage.h"
#include"..//Kinect2Capture//Kinect2Capture.h"
#include"..//CamShiftPatch//CamShiftPatch.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "tab1Dlg.h"
#include "tab2Dlg.h"
#include "tab3Dlg.h"
#include "setROIDlg.h"

struct CMythreadParam
{
public:
	HWND hWnd;
	LPVOID m_lpPara;
	UINT   m_case;
	BOOL m_blthreading;
};


// CMFC_systemServerDlg 對話方塊
class CMFC_systemServerDlg : public CDialogEx
{
// 建構
public:
	CMFC_systemServerDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_MFC_SYSTEMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
public:
	// tab control
	CTabCtrl m_Tab;
	tab1Dlg m_TabPage1;
	tab2Dlg m_TabPage2;
	tab3Dlg m_TabPage3;
	afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);

	//Thread
	CMythreadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT MythreadFun(LPVOID LParam);
	void Thread_getImage(LPVOID lParam);
	void Thread_mode(LPVOID lParam);//目前形成狀況
	void Thread_OpenSetROI();//open dlg
	void Thread_serverOn(LPVOID lParam);
	void Thread_sendPacket(LPVOID lParam);

	void ShowImage(IplImage *Image, CWnd *pWnd);
	void ShowImage(IplImage *Image, CWnd *pWnd, int channels);
	
	//kinect
	CvRect colorROI = cvRect(853, 536, 370, 237);//1920*1080
	CvRect depthROI = cvRect(197, 204, 129, 83);//512*424

	//afx
	afx_msg void OnBnClickedButtontest1();
	afx_msg void OnBnClickedButtontest4();
	afx_msg void OnBnClickedButtontest5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonserveron();

	setROIDlg roi;

	void saveImage(IplImage* live);
	IplImage* pLive;
	IplImage* pDepth;

	Kinect2Capture kinect;
	CamShiftPatch CSpatch;

	CStatic m_img_binPickLive;
	
	int m_testvalue;
	void add(CString str);

	IplImage* depthMaskImg = nullptr;

	CStatic m_img_binPickLiveDepth;
	CString m_detectObjNum;

	CListBox m_list_server;

	void packetMeasure(char* msg, CString &report);
	void packetCreat(CString msg, char* report);
	void packetCreat_toPoint(float x, float y, float z, float t, char* report);
	afx_msg void OnBnClickedButton3();
	int m_camera2global_x;
	int m_camera2global_y;
	int m_camera2global_x_zero;
	int m_camera2global_y_zero;
};
