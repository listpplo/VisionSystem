#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "MvCamera.h"

// 自定义Windows消息ID，预留200个给控件使用
// Custom windows message ID, 200 reserved for control
#define WM_MY_MESSAGE_BASE (200)
#define	WM_MY_DISCOVERY_MESSAGE (WM_USER + WM_MY_MESSAGE_BASE + 1)
#define	WM_MY_DISCONNECT_MESSAGE (WM_USER + WM_MY_MESSAGE_BASE + 2)

// CCameraSubDlg 对话框 | dialog

class CCameraSubDlg 
	: public CDialog
	, public IMvCameraSink
{
	DECLARE_DYNAMIC(CCameraSubDlg)

public:
	// 标准构造函数
	// construct function
	CCameraSubDlg(CWnd* pParent = NULL); 
	virtual ~CCameraSubDlg();

public:
	// MvCamera状态回调函数
	// MvCamera status callback function
	virtual void onCameraDisconnect();

	// 自定义函数
	// define function
	void setRect(CRect rect);
	void setCamera(unsigned int index);

public:
	// MFC事件回调处理函数
	// MFC event callback processing function
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStopplay();
	afx_msg void OnCbnSelchangeComboTriggermode();
	afx_msg void OnBnClickedButtonTrigger();
	afx_msg LRESULT OnMyDisconnectMsg(WPARAM wParam, LPARAM lParam);

	// 对话框数据
	// dialog data
	enum { IDD = IDD_CAMERA_DIALOG };

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持 | DDX/DDV support

	virtual void OnOK();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	void enableProperties(BOOL bEnable);
	void enableWindowAll(BOOL bEnbale);
	void initParamProperty();
	bool procTriggerMode( const char* pMode );

private:
	// 各类MFC控件变量
	// Various MFC control variables
	CEdit							m_editCameraName;
	CComboBox						m_cmbTriggerMode;
	CButton							m_btnTrigger;
	CListBox						m_lbShowWindow;
	CButton							m_btnConnect;
	CButton							m_btnDisconnect;
	CButton							m_btnPlay;
	CButton							m_btnStopPlay;

	// 自定义变量
	// define variables
	CRect							m_rectDlg;
	CMvCamera*						m_pMvCamera;
	CFont							m_font;	
};
