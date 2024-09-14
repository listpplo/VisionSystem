// CameraSubDlg.cpp : 实现文件
// implementation file

#include "stdafx.h"
#include "MultiCamerasTool.h"
#include "CameraSubDlg.h"

#include <sstream>

// CCameraSubDlg 对话框
// CCameraSubDlg dialog

extern ::CString getParseLanguageString(::CString strOrl);
#define STD_WIDTH (960)
#define STD_HEIGHT (529)
#define STD_FONT_SIZE (100)

double g_widthRate = 0.0;
double g_heightRate = 0.0;

IMPLEMENT_DYNAMIC(CCameraSubDlg, CDialog)

CCameraSubDlg::CCameraSubDlg(CWnd* pParent)
	: CDialog(CCameraSubDlg::IDD, pParent)
	, m_pMvCamera(NULL)
{

}

CCameraSubDlg::~CCameraSubDlg()
{
	if (m_pMvCamera)
	{
		delete m_pMvCamera;
		m_pMvCamera = NULL;
	}
}

BOOL CCameraSubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 根据目前配置的语言初始化界面文本显示 | Initialize the text display of the interface according to the currently configured language
	{
		this->SetWindowText(getParseLanguageString(_T("_MAINFORM_TEXT")));
		GetDlgItem(IDC_STATIC_CAMERA)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CAMERA")));
		GetDlgItem(IDC_STATIC_TRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_SOFT_TRIGGER")));
		GetDlgItem(IDC_STATIC_TRIGGER_MODE)->SetWindowText(getParseLanguageString(_T("_MAINFORM_TRIGGER_MODE")));
		GetDlgItem(IDC_BUTTON_TRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_SOFTWARE_TRIGGER")));
		GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CONNECT")));
		GetDlgItem(IDC_BUTTON_DISCONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_DISCONNECT")));
		GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_PLAY")));
		GetDlgItem(IDC_BUTTON_STOPPLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_STOP_PLAY")));
	}

	MoveWindow(m_rectDlg);

	CRect rectSubDlg;
	GetClientRect(rectSubDlg);

	g_widthRate = (double)rectSubDlg.BottomRight().x / STD_WIDTH;
	g_heightRate = (double)rectSubDlg.BottomRight().y / STD_HEIGHT;

	GetDlgItem(IDC_STATIC_CAMERA)->MoveWindow( 
		(int)(10 * g_widthRate),
		(int)(10 * g_heightRate),
		(int)((STD_WIDTH - 20) * g_widthRate),
		(int)((STD_HEIGHT - 20) * g_heightRate));

	for (int iItemId = IDC_STATIC_TRIGGER; iItemId <= IDC_STATIC_TRIGGER_MODE; ++iItemId)
	{
		CWnd* pWnd = GetDlgItem(iItemId);
		if (NULL == pWnd)
		{
			continue;
		}

		// 根据屏幕大小缩放控件
		// scale controls based on screen size
		CRect rectTmp;
		pWnd->GetWindowRect(&rectTmp);
		ScreenToClient(&rectTmp);

		CRect rectAdaptor;
		rectAdaptor.TopLeft().x = (int)((rectTmp.TopLeft().x) * g_widthRate);
		rectAdaptor.TopLeft().y = (int)((rectTmp.TopLeft().y) * g_heightRate);
		rectAdaptor.BottomRight().x = (int)((rectTmp.BottomRight().x) * g_widthRate);
		rectAdaptor.BottomRight().y = (int)((rectTmp.BottomRight().y) * g_heightRate);

		pWnd->MoveWindow(rectAdaptor);

		// 根据屏幕大小缩放字体
		// scale font based on screen size
		if (IDC_STATIC_TRIGGER == iItemId)
		{
			double iFontRate = g_widthRate;
			if (g_heightRate < g_widthRate)
			{
				iFontRate = g_heightRate;
			}

			int iFontSize = (int)(STD_FONT_SIZE * iFontRate);
			m_font.CreatePointFont(iFontSize, "");
		}
	}

	// 禁用全部控件
	// disable all controls
	enableWindowAll(FALSE);

	return TRUE;
}

void CCameraSubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAMERA, m_editCameraName);
	DDX_Control(pDX, IDC_COMBO_TRIGGERMODE, m_cmbTriggerMode);
	DDX_Control(pDX, IDC_BUTTON_TRIGGER, m_btnTrigger);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_btnDisconnect);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON_STOPPLAY, m_btnStopPlay);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_lbShowWindow);
}

void CCameraSubDlg::onCameraDisconnect()
{
	PostMessage(WM_MY_DISCONNECT_MESSAGE, NULL, NULL);
}

void CCameraSubDlg::setRect(CRect rect)
{
	m_rectDlg = rect;
}

void CCameraSubDlg::setCamera(unsigned int index)
{
	// 激活子界面
	// activate sub interface
	enableWindowAll(TRUE);

	// 控制属性控件状态
	// control property control state
	enableProperties(FALSE);	

	// 控制相关按钮状态
	// control related button status
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(FALSE);
	m_btnTrigger.EnableWindow(FALSE);

	if (m_pMvCamera)
	{
		delete m_pMvCamera;
		m_pMvCamera = NULL;
	}
	m_pMvCamera = new CMvCamera(index);

	if (!m_pMvCamera)
	{
		AfxMessageBox(_T("m_pMvCamera is NULL!"));
	}
}

void CCameraSubDlg::enableProperties(BOOL bEnable)
{
	if ( FALSE == bEnable )
	{
		m_cmbTriggerMode.ResetContent();
	}
	m_cmbTriggerMode.EnableWindow(bEnable);
}

void CCameraSubDlg::enableWindowAll(BOOL bEnbale)
{
	for (int iItemId = IDC_STATIC_CAMERA; iItemId <= IDC_BUTTON_STOPPLAY; ++iItemId)
	{
		CWnd* pWnd = GetDlgItem(iItemId);
		if ( NULL == pWnd )
		{
			continue;
		}

		pWnd->EnableWindow(bEnbale);
	}
}

void CCameraSubDlg::initParamProperty()
{
	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	// TriggerMode
	m_cmbTriggerMode.Clear();

	IMV_String enumSymbol;
	int ret = IMV_GetEnumFeatureSymbol(devHandle, "TriggerMode", &enumSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode symbol value failed!"));
		return;
	}

	std::string triggerModeSymbol = std::string(enumSymbol.str);

	unsigned int nEntryNum = 0;
	ret = IMV_GetEnumFeatureEntryNum(devHandle, "TriggerMode", &nEntryNum);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode settable enumeration number failed!"));
		return;
	}

	IMV_EnumEntryList enumEntryList;
	enumEntryList.nEnumEntryBufferSize = sizeof(IMV_EnumEntryInfo) * nEntryNum;
	enumEntryList.pEnumEntryInfo = (IMV_EnumEntryInfo*)malloc(sizeof(IMV_EnumEntryInfo) * nEntryNum);
	if (NULL == enumEntryList.pEnumEntryInfo)
	{
		AfxMessageBox(_T("Malloc pEnumEntryInfo failed!"));
		return;
	}

	if (IMV_OK != IMV_GetEnumFeatureEntrys(devHandle, "TriggerMode", &enumEntryList))
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
		AfxMessageBox(_T("Get TriggerMode settable enumeration value list failed!"));
		return;
	}

	for (unsigned int iIndex = 0; iIndex < nEntryNum; ++iIndex)
	{
		m_cmbTriggerMode.InsertString(iIndex, (LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
		if (triggerModeSymbol == std::string(enumEntryList.pEnumEntryInfo[iIndex].name))
		{
			m_cmbTriggerMode.SetCurSel(iIndex);
		}
	}
	bool bEnable = IMV_FeatureIsWriteable(devHandle, "TriggerMode");
	m_cmbTriggerMode.EnableWindow(bEnable);

	free(enumEntryList.pEnumEntryInfo);
	enumEntryList.pEnumEntryInfo = NULL;
}

bool CCameraSubDlg::procTriggerMode(const char* pMode)
{
	if (NULL == m_pMvCamera)
	{
		AfxMessageBox(_T("Set triggerMode failed, camera is null!"));
		return false;
	}

	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	if (IMV_OK != IMV_SetEnumFeatureSymbol(devHandle, "TriggerSelector", "FrameStart"))
	{
		return false;
	}

	if (IMV_OK != IMV_SetEnumFeatureSymbol(devHandle, "TriggerMode", pMode))
	{
		return false;
	}

	return true;
}

BEGIN_MESSAGE_MAP(CCameraSubDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CCameraSubDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CCameraSubDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CCameraSubDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOPPLAY, &CCameraSubDlg::OnBnClickedButtonStopplay)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CCameraSubDlg::OnCbnSelchangeComboTriggermode)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGER, &CCameraSubDlg::OnBnClickedButtonTrigger)
	ON_MESSAGE(WM_MY_DISCONNECT_MESSAGE, &CCameraSubDlg::OnMyDisconnectMsg)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCameraSubDlg 消息处理程序
// CCameraSubDlg message processing program
void CCameraSubDlg::OnBnClickedButtonConnect()
{
	if (NULL == m_pMvCamera)
	{
		AfxMessageBox(_T("Connect failed, camera is null!"));
		return;
	}

	if (!m_pMvCamera->open(this))
	{
		// 处理连接失败
		// processing connect failure
		AfxMessageBox(_T("Connect failed!"));
		return;
	}

	IMV_DeviceInfo devInfo;
	if (!m_pMvCamera->getDeviceInfo(devInfo))
	{
		// 处理连接失败
		// processing connect failure
		AfxMessageBox(_T("getDeviceInfo failed!"));
		return;
	}

	std::string showCamName = "";

	showCamName.append(devInfo.cameraName);
	showCamName.append(" [");
	if (devInfo.nCameraType == typeGigeCamera)
	{
		showCamName.append(devInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
	}
	else
	{
		showCamName.append(devInfo.serialNumber);
	}
	showCamName.append("]");

	m_editCameraName.SetWindowText((LPCSTR)showCamName.c_str());

	// 初始化相机属性
	// initialize camera properties
	initParamProperty();

	// 控制属性控件状态
	// control property control state
	enableProperties(TRUE);

	// 控制相关按钮状态
	// Control related button status
	m_btnConnect.EnableWindow(FALSE);
	m_btnDisconnect.EnableWindow(TRUE);
	m_btnPlay.EnableWindow(TRUE);
}

void CCameraSubDlg::OnBnClickedButtonDisconnect()
{
	if (NULL != m_pMvCamera)
	{
		OnBnClickedButtonStopplay();
		m_pMvCamera->close();
	}

	GetDlgItem(IDC_STATIC_CAMERA)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CAMERA")));

	// 控制属性控件状态
	// Control property control edit status
	enableProperties(FALSE);

	// 控制相关按钮状态
	// Control related button status
	m_btnConnect.EnableWindow(TRUE);
	m_btnDisconnect.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(FALSE);
}

void CCameraSubDlg::OnBnClickedButtonPlay()
{
	if (NULL != m_pMvCamera)
	{
		m_pMvCamera->startDisplay((VR_HWND)m_lbShowWindow.GetSafeHwnd());
	}
	else
	{
		AfxMessageBox(_T("Start play failed!, camera is null!"));
		return;
	}

	// 控制相关按钮状态
	// Control related button status
	m_btnPlay.EnableWindow(FALSE);
	m_btnStopPlay.EnableWindow(TRUE);
	OnCbnSelchangeComboTriggermode();
}

void CCameraSubDlg::OnBnClickedButtonStopplay()
{
	if (NULL != m_pMvCamera)
	{
		// TODO: 在TriggerMode开启情况下关闭SDK中似乎有死锁
		// 暂时先这么处理，后面再来跟这个问题
		// When triggermode is turned on, it seems that there is a deadlock in the SDK. For now, do this first, and then follow this problem
		// 		procTriggerMode("Off");
		// 		int iIndex = m_cmbTriggerMode.FindStringExact(0, _T("Off"));
		// 		m_cmbTriggerMode.SetCurSel(iIndex);

		m_pMvCamera->stopDisplay();
	}

	// 控制相关按钮状态
	// Control related button status
	m_btnPlay.EnableWindow(TRUE);
	m_btnStopPlay.EnableWindow(FALSE);
	m_btnTrigger.EnableWindow(FALSE);
}

void CCameraSubDlg::OnCbnSelchangeComboTriggermode()
{
	TCHAR szBuf[24] = { 0 };
	int iIndex = m_cmbTriggerMode.GetCurSel();
	m_cmbTriggerMode.GetLBText(iIndex, szBuf);

#ifdef UNICODE
	std::string strBuf = WStringToString(szBuf);
#else
	std::string strBuf = szBuf;
#endif // UNICODE

	bool bRet = procTriggerMode(strBuf.c_str());
	if (!bRet)
	{
		AfxMessageBox(_T("Set trigger mode failed!"));
	}

	if (strBuf == "On")
	{
		if ((FALSE == m_btnPlay.IsWindowEnabled()) 
		 && (TRUE == m_btnStopPlay.IsWindowEnabled()))
		{
			m_btnTrigger.EnableWindow(TRUE);
		}
		else
		{
			m_btnTrigger.EnableWindow(FALSE);
		}
	}
	else
	{
		m_btnTrigger.EnableWindow(FALSE);
	}
}

void CCameraSubDlg::OnBnClickedButtonTrigger()
{
	if (!m_pMvCamera)
	{
		AfxMessageBox(_T("Trigger failed!, camera is null!"));
		return;
	}

	IMV_HANDLE devHandle = m_pMvCamera->getCameraHandle();

	if (IMV_OK != IMV_ExecuteCommandFeature(devHandle, "TriggerSoftware"))
	{
		AfxMessageBox(_T("TriggerSoftware failed!"));
	}
}

LRESULT CCameraSubDlg::OnMyDisconnectMsg(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedButtonDisconnect();
	AfxMessageBox(_T("Camera is disconnected!!!"));
	return 0;
}

void CCameraSubDlg::OnOK()
{
	// 屏蔽关闭对话框操作
	// Shield close dialog action
}

BOOL CCameraSubDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		// 屏蔽ESC键关闭对话框
		// Shield ESCbutton close dialog action
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
