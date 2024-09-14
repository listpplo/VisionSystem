// GenICamVc6TestDlg.h : header file
//

#include "../Src/MvCamera.h"

#if !defined(AFX_GENICAMVC6TESTDLG_H__7C31DE94_D14C_4B6C_B02A_1A695931D4EE__INCLUDED_)
#define AFX_GENICAMVC6TESTDLG_H__7C31DE94_D14C_4B6C_B02A_1A695931D4EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGenICamVc6TestDlg dialog

class CGenICamVc6TestDlg : public CDialog
{
// Construction
public:
	CGenICamVc6TestDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CGenICamVc6TestDlg();

// Dialog Data
	//{{AFX_DATA(CGenICamVc6TestDlg)
	enum { IDD = IDD_GENICAMVC6TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenICamVc6TestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGenICamVc6TestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonStopplay();
	afx_msg void OnButtonDiscovery();
	afx_msg void OnClose();
    afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void initParamProperties();
	void initComboBoxTypeProperty(const char* propertieName, CWnd* pCmbProperty);
	void initFloatEditTypeProperty(const char* propertieName, CWnd* pEditProperty);

private:
	CMvCamera*		m_pMvCamera;

public:
    afx_msg void OnEnKillfocusEditExposuretime();
    afx_msg void OnEnKillfocusEditGainraw();
    afx_msg void OnEnKillfocusEditFramerate();
    afx_msg void OnCbnKillfocusComboPixelformat();

private:
    struct paramStruct
    {
        CString     preExposureautoStr;
        CString     preExposuremodeStr;
        CString     preExposuretimeStr;
        CString     preGainrawStr;
        CString     prePixelformatStr;
        CString     preFramerateStr;
        CString     preGainautoStr;

        paramStruct()
        {
            preExposureautoStr  = "";
            preExposuremodeStr  = "";
            preExposuretimeStr  = "";
            preGainrawStr       = "";
            prePixelformatStr   = "";
            preFramerateStr     = "";
            preGainautoStr      = "";
        }
    };

    paramStruct m_paramStruct;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENICAMVC6TESTDLG_H__7C31DE94_D14C_4B6C_B02A_1A695931D4EE__INCLUDED_)
