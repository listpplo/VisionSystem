// MultiCamerasToolDlg.h : 头文件
// header.h

#pragma once

class CCameraSubDlg;

// CMultiCamerasToolDlg 对话框 | dialog
class CMultiCamerasToolDlg : public CDialog
{
// 构造 | construct
public:
	// 标准构造函数 | construct function
	CMultiCamerasToolDlg(CWnd* pParent = NULL);	

	// 对话框数据 | dialog data
	enum { IDD = IDD_MULTICAMERASTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持 | DDX/DDV support


public:
	// MFC消息回调处理函数 | message callback function
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnMyDiscoveryMsg(WPARAM wParam, LPARAM lParam);

// 实现 | implementation
protected:
	HICON m_hIcon;

	// 生成的消息映射函数 | message reflect function
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// 自定义函数 | difine function
	void initCameraSubDlg();

private:
	// 自定义变量 | define various
	CCameraSubDlg*					m_pCameraSubDlgs;	
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
