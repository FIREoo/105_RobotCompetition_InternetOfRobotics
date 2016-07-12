#pragma once
#include "afxwin.h"
#include"Cvvimage.h"
#include "MFC_systemServer.h"
#include"..//Kinect2Capture//Kinect2Capture.h"
#include"..//CamShiftPatch//CamShiftPatch.h"
#include"..//CContour//Contour.h"

// tab1Dlg 對話方塊

class tab1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(tab1Dlg)

public:
	tab1Dlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~tab1Dlg();
// 對話方塊資料
	enum { IDD = IDD_DIALOG_tab1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	static IplImage* sImage_live;
	static IplImage* sImage_depth;
	static IplImage* sImage_depthGround;

	CvRect findCase(IplImage* image);
	void ShowImage(IplImage *Image, CWnd *pWnd);

	CamShiftPatch CSpatch_inTab1;
	CvHistogram * hisData[10];
	int m_totalObjNum;
	CStatic m_img_viewObj;
	CStatic m_img_detectObj;
	CvScalar maskRange = cvScalar(10, 256, 5);

	CComboBox m_combo_objList;
	afx_msg void OnCbnSelchangeComboobjlist();
	afx_msg void OnBnClickedButtongethisandset();
	afx_msg void OnBnClickedButtonviewobj();
	afx_msg void OnBnClickedButtondetectobj();
	float m_gripCenterX;
	float m_gripCenterY;
	float m_gripDegree;
};
