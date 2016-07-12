#pragma once
#include "axismediacontrol1.h"
#include"Cvvimage.h"
#include "afxwin.h"

struct CTab2threadParam
{
public:
	HWND hWnd;
	LPVOID m_lpPara;
	UINT   m_case;
	BOOL m_blthreading;
};


// tab2Dlg 對話方塊

class tab2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(tab2Dlg)

public:
	tab2Dlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~tab2Dlg();

// 對話方塊資料
	enum { IDD = IDD_DIALOG_tab2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	static IplImage* sImage_IPcam;
	static IplImage* sImage_IPcam_ground;
	//afx
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonconnectipcam();

	CAxismediacontrol1 m_AMC;
	// IPcam ip
	CString m_ipText;
	CString	m_user;
	CString	m_pass;
	CString ctrlURL;
	CString mediaURL;
	unsigned char *PTZImage;

	//thread
	CTab2threadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT MythreadFun(LPVOID LParam);
	void Thread_localization(LPVOID lParam);
	void Thread_map(LPVOID lParam);
	void Thread_mapBace(LPVOID lParam);

	void GetImage();
	void GetImageAndColorDetect();
	void ShowImage(IplImage *Image, CWnd *pWnd, int channels);
	void ShowMap(IplImage *Image, CWnd *pWnd);
	CStatic m_img_IPcam;
	afx_msg void OnBnClickedButtonsetground();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CStatic m_img_IPcam_map;
	int m_map_Xpos;
	int m_map_Ypos;
	afx_msg void OnBnClickedCheckIpcamkeystone();
	CButton m_check_IPcam_showKeystone;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonconnectP3dx();
	afx_msg void OnBnClickedButtonP3dxback();
	afx_msg void OnBnClickedButtonP3dxstop();
	afx_msg void OnBnClickedButtonP3dxfront();
	CStatic m_map;
};
