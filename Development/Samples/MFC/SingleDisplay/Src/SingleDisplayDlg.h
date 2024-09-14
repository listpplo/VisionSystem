// DemoDlg.h : 头文件
//
#ifndef __SINGDISPLAYDLG_H__
#define __SINGDISPLAYDLG_H__

#include "Render.h"
#include "Tool.h"
#include <list>

// CDemoDlg 对话框
class CSingleDisplayDlg : public CDialog
{
// 构造
public:
	CSingleDisplayDlg(CWnd* pParent = NULL);	// 标准构造函数
	void frameProc(IMV_Frame frame);
	void displayProc();
	void deviceLinkNotifyProc(IMV_SConnectArg connectArg);

// 对话框数据
	enum { IDD = IDD_DEMO_DIALOG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
private:
	void initTriggerModeParamProperty();

	void initPixelFormatParamProperty();

	void initParamProperty();

	void setParamProperty();

	// 开始拉流
	bool StartStreamGrabbing(bool bResumeConnect);

	// 停止拉流
	bool StopStreamGrabbing(bool bResumeConnect);

	BOOL OpenConsole();

	void setDisplayFPS(int nFPS);
	bool isTimeToDisplay();
	
	FrameBuffer* getConvertedImage();
	void addConvertedImage(FrameBuffer* &pConvertedImage);
	void clearConvertedImage();

private:
	IMV_HANDLE							m_devHandle;
	HANDLE								m_displayThreadHandle;
	bool                                _connected;       // 连接标识
	CRender                             _render;          // 显示对象
	bool								_bRunning;
	std::list<FrameBuffer*>				m_listImages;		// 显示队列
	CTool								m_mutexQue;
	CTool								m_mxTime;
	int								    m_dDisplayInterval;         // 显示间隔
	uint64_t							m_nFirstFrameTime;          // 第一帧的时间戳
	uint64_t							m_nLastFrameTime;           // 上一帧的时间戳
	bool								m_bResumeGrabbing;

private:
	CString m_cameraKey;
	CString m_triggerMode;
	CString m_Format;
	double m_dExposureEdit;
	double m_dFrameRateEdit;
	double m_dGainEdit;

public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboFormat();
	afx_msg void OnBnClickedBtnDiscovery();
	afx_msg void OnBnClickedBtnGetparam();
	afx_msg void OnBnClickedBtnSetparam();
	afx_msg void OnCbnSelchangeComboTriggermode();
	afx_msg void OnBnClickedBtnSofttrigger();
};

#endif// __SINGDISPLAYDLG_H__
