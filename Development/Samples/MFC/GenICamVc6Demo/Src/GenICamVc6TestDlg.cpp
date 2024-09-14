// GenICamVc6TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GenICamVc6Test.h"
#include "GenICamVc6TestDlg.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenICamVc6TestDlg dialog

CGenICamVc6TestDlg::CGenICamVc6TestDlg(CWnd* pParent )
	: CDialog(CGenICamVc6TestDlg::IDD, pParent)
	, m_pMvCamera(NULL)
{
	//{{AFX_DATA_INIT(CGenICamVc6TestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CGenICamVc6TestDlg::~CGenICamVc6TestDlg()
{
	if (m_pMvCamera)
	{
		delete m_pMvCamera;
		m_pMvCamera = NULL;
	}
}

void CGenICamVc6TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenICamVc6TestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGenICamVc6TestDlg, CDialog)
	//{{AFX_MSG_MAP(CGenICamVc6TestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOPPLAY, OnButtonStopplay)
	ON_BN_CLICKED(IDC_BUTTON_DISCOVERY, OnButtonDiscovery)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
    ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURETIME, OnEnKillfocusEditExposuretime)
    ON_EN_KILLFOCUS(IDC_EDIT_GAINRAW, OnEnKillfocusEditGainraw)
    ON_EN_KILLFOCUS(IDC_EDIT_FRAMERATE, OnEnKillfocusEditFramerate)
    ON_CBN_KILLFOCUS(IDC_COMBO_PIXELFORMAT, OnCbnKillfocusComboPixelformat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenICamVc6TestDlg message handlers

BOOL CGenICamVc6TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGenICamVc6TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGenICamVc6TestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGenICamVc6TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGenICamVc6TestDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	if ( !m_pMvCamera )
	{
		AfxMessageBox(_T("No camera!"));
		return;
	}
	
	bool bOpenRet = m_pMvCamera->open(NULL);
	if ( !bOpenRet )
	{
		AfxMessageBox(_T("Connect failed!"));
		return;
	}
	
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_DISCOVERY)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PIXELFORMAT)->EnableWindow(TRUE);
	
	AfxMessageBox(_T("Connect success!"));

	initParamProperties();
}

void CGenICamVc6TestDlg::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	if ( m_pMvCamera )
	{
		m_pMvCamera->stopDisplay();
		m_pMvCamera->close();
	}

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_DISCOVERY)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PIXELFORMAT)->EnableWindow(TRUE);
}

void CGenICamVc6TestDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	if ( m_pMvCamera )
	{
		m_pMvCamera->startDisplay((VR_HWND)GetDlgItem(IDC_LIST_SHOWWINDOW)->GetSafeHwnd());

		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PIXELFORMAT)->EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox(_T("Start play failed!"));
		return;
	}	
}

void CGenICamVc6TestDlg::OnButtonStopplay() 
{
	// TODO: Add your control notification handler code here
	if ( m_pMvCamera )
	{
		m_pMvCamera->stopDisplay();
	}
	
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOPPLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PIXELFORMAT)->EnableWindow(TRUE);
}

void CGenICamVc6TestDlg::OnButtonDiscovery() 
{
	// 枚举设备 
	// enum camera 
	IMV_DeviceList deviceInfoList;
	if(IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return;
	}
	unsigned int iNum = deviceInfoList.nDevNum;
	if ( iNum > 0 )
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		if (m_pMvCamera)
		{
			delete m_pMvCamera;
			m_pMvCamera = NULL;
		}
		m_pMvCamera = new CMvCamera(0);
		AfxMessageBox(_T("Discovery success!"));
	}
	else
	{
		AfxMessageBox(_T("Discovery can't find cameras!"));
	}
}

void CGenICamVc6TestDlg::initParamProperties()
{
	// exposureTime
	CWnd* pEditExposureTime = GetDlgItem(IDC_EDIT_EXPOSURETIME);
	initFloatEditTypeProperty((const char*)"ExposureTime", pEditExposureTime);
    GetDlgItemText(IDC_EDIT_EXPOSURETIME, m_paramStruct.preExposuretimeStr);

	// gainRaw
	CWnd* pEditGainRaw = GetDlgItem(IDC_EDIT_GAINRAW);
	initFloatEditTypeProperty((const char*)"GainRaw", pEditGainRaw);
    GetDlgItemText(IDC_EDIT_GAINRAW, m_paramStruct.preGainrawStr);

	// acquistionFrameRate
	CWnd* pEditFrameRate = GetDlgItem(IDC_EDIT_FRAMERATE);
	initFloatEditTypeProperty((const char*)"AcquisitionFrameRate", pEditFrameRate);
    GetDlgItemText(IDC_EDIT_FRAMERATE, m_paramStruct.preFramerateStr);

	// pixelFormat
	CWnd* pCmbPixelFormat = GetDlgItem(IDC_COMBO_PIXELFORMAT);
	initComboBoxTypeProperty((const char*)"PixelFormat", pCmbPixelFormat);
    GetDlgItemText(IDC_COMBO_PIXELFORMAT, m_paramStruct.prePixelformatStr);
}

void CGenICamVc6TestDlg::initComboBoxTypeProperty(const char* pPropertieName, CWnd* pCmbProperty)
{
	if ( NULL == pPropertieName ||	NULL == pCmbProperty )
	{
		return;
	}

	CComboBox* pComboBox = (CComboBox*)pCmbProperty;

    pComboBox->ResetContent();

	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	unsigned int entryNum = 0;

	int ret = IMV_GetEnumFeatureEntryNum(devHandle, pPropertieName, &entryNum);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get entry num failed!"));
		return;
	}

	if(entryNum <= 0)
	{
		AfxMessageBox(_T("Get entry num failed!"));
		return;
	}

	IMV_EnumEntryList enumEntryList;
	enumEntryList.pEnumEntryInfo = new IMV_EnumEntryInfo[entryNum];
	enumEntryList.nEnumEntryBufferSize = (unsigned int)sizeof(IMV_EnumEntryInfo) * entryNum;
	if(enumEntryList.pEnumEntryInfo == NULL)
	{
		AfxMessageBox(_T("Malloc memory failed!"));
		return;
	}

	ret = IMV_GetEnumFeatureEntrys(devHandle, pPropertieName, &enumEntryList);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get entrys failed!"));
		delete[] enumEntryList.pEnumEntryInfo;
		return;
	}

	IMV_String enumSymbol;
	ret = IMV_GetEnumFeatureSymbol(devHandle, pPropertieName, &enumSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get value failed!"));
		delete[] enumEntryList.pEnumEntryInfo;
		return;
	}

	for (unsigned int iIndex = 0; iIndex < entryNum; ++iIndex)
	{
		pComboBox->InsertString(iIndex,(LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
		if ( 0 == memcmp(enumSymbol.str, enumEntryList.pEnumEntryInfo[iIndex].name, strlen(enumSymbol.str)))
		{
			pComboBox->SetCurSel(iIndex);
		}
	}

	delete[] enumEntryList.pEnumEntryInfo;

	BOOL bEnable = (BOOL)IMV_FeatureIsWriteable(devHandle, pPropertieName) ? TRUE : FALSE;
	pComboBox->EnableWindow(bEnable);
}

void CGenICamVc6TestDlg::initFloatEditTypeProperty(const char* pPropertieName, CWnd* pEditProperty)
{
	if ( NULL == pPropertieName ||	NULL == pEditProperty )
	{
		return;
	}

	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	double fltVal = 0.0;
	int ret = IMV_GetDoubleFeatureValue(devHandle, pPropertieName, &fltVal);

	char szBuf[24] = {0};
	sprintf( szBuf, "%5f", fltVal );

	pEditProperty->SetWindowText((LPCTSTR)szBuf);

	BOOL bEnable = (BOOL)IMV_FeatureIsWriteable(devHandle, pPropertieName) ? TRUE : FALSE;
	pEditProperty->EnableWindow(bEnable);
}

void CGenICamVc6TestDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnButtonStopplay();
	
	CDialog::OnClose();
}

void CGenICamVc6TestDlg::OnOK()
{
    // do nothing 防止按下回车键后， 程序退出
	// do nothing prevent the program from exiting after pressing enter
    GetDlgItem(IDC_BUTTON_DISCOVERY)->SetFocus();
}

void CGenICamVc6TestDlg::OnEnKillfocusEditExposuretime()
{
    // TODO:  在此添加控件通知处理程序代码
	// TODO:  Add control notification handler code here
    if (m_pMvCamera && m_pMvCamera->isOpen())
    {

        CString curStr;
        GetDlgItemText(IDC_EDIT_EXPOSURETIME, curStr);
        if (m_paramStruct.preExposuretimeStr == curStr)
        {
            return;
        }

		IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

        double dVal = atof(curStr);

		int ret = IMV_SetDoubleFeatureValue(devHandle, (const char*)"ExposureTime", dVal);
		if (IMV_OK != ret)
        {
            AfxMessageBox(_T("set exposureTime failed!"));
            if (m_paramStruct.preExposuretimeStr != "")
            {
                SetDlgItemText(IDC_EDIT_EXPOSURETIME, m_paramStruct.preExposuretimeStr);
            }
            else
            {
                m_paramStruct.preExposuretimeStr = curStr;
            }
        }
        else
        {
            m_paramStruct.preExposuretimeStr = curStr;
        }
    }
}

void CGenICamVc6TestDlg::OnEnKillfocusEditGainraw()
{
    // TODO:  在此添加控件通知处理程序代码
	// TODO:  Add control notification handler code here
    if (m_pMvCamera && m_pMvCamera->isOpen())
    {
        CString curStr;
        GetDlgItemText(IDC_EDIT_GAINRAW, curStr);
        if (m_paramStruct.preGainrawStr == curStr)
        {
            return;
        }

		IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

        double dVal = atof(curStr);

		int ret = IMV_SetDoubleFeatureValue(devHandle, (const char*)"GainRaw", dVal);
		if (IMV_OK != ret)
        {
            AfxMessageBox(_T("set gainRaw failed!"));
            if (m_paramStruct.preGainrawStr != "")
            {
                SetDlgItemText(IDC_EDIT_GAINRAW, m_paramStruct.preGainrawStr);
            }
            else
            {
                m_paramStruct.preGainrawStr = curStr;
            }
        }
        else
        {
            m_paramStruct.preGainrawStr = curStr;
        }
    }
}

void CGenICamVc6TestDlg::OnEnKillfocusEditFramerate()
{
    // TODO: 在此添加控件通知处理程序代码
	// TODO: Add control notification handler code here
    if (m_pMvCamera && m_pMvCamera->isOpen())
    {
        CString curStr;
        GetDlgItemText(IDC_EDIT_FRAMERATE, curStr);
        if (m_paramStruct.preFramerateStr == curStr)
        {
            return;
        }

		IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

        double dVal = atof(curStr);

		int ret = IMV_SetDoubleFeatureValue(devHandle, (const char*)"AcquisitionFrameRate", dVal);
		if (IMV_OK != ret)
        {
            AfxMessageBox(_T("set acquisitionFrameRate failed!"));
            if (m_paramStruct.preFramerateStr != "")
            {
                SetDlgItemText(IDC_EDIT_FRAMERATE, m_paramStruct.preFramerateStr);
            }
            else
            {
                m_paramStruct.preFramerateStr = curStr;
            }
        }
        else
        {
            m_paramStruct.preFramerateStr = curStr;
			int ret = IMV_SetBoolFeatureValue(devHandle, (const char*)"AcquisitionFrameRateEnable", true);
			if (IMV_OK != ret)
			{
				AfxMessageBox(_T("set acquisitionFrameRateEnable failed!"));
			}
        }
    }
}

void CGenICamVc6TestDlg::OnCbnKillfocusComboPixelformat()
{
    // TODO:  在此添加控件通知处理程序代码
	// TODO:  Add control notification handler code here
    if (m_pMvCamera && m_pMvCamera->isOpen())
    {
        CString curStr;
        GetDlgItemText(IDC_COMBO_PIXELFORMAT, curStr);
        if (m_paramStruct.prePixelformatStr == curStr)
        {
            return;
        }

		IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

		std::string infraStr(curStr);
		int ret = IMV_SetEnumFeatureSymbol(devHandle, (const char*)"PixelFormat", infraStr.c_str());

		if (IMV_OK != ret)
        {
            AfxMessageBox(_T("set pixelFormat failed!"));
            if (m_paramStruct.prePixelformatStr != "")
            {
                SetDlgItemText(IDC_COMBO_PIXELFORMAT, m_paramStruct.prePixelformatStr);
            }
            else
            {
                m_paramStruct.prePixelformatStr = curStr;
            }
        }
        else
        {
            m_paramStruct.prePixelformatStr = curStr;
        }
    }
}

