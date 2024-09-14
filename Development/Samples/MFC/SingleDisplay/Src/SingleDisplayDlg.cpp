// DemoDlg.cpp : 实现文件
//Implementation file

#include "stdafx.h"
#include "SingleDisplay.h"
#include "SingleDisplayDlg.h"
#include "RGBConvert.h"
#include "SingleDisplayDlg.h"
#include "ConfigOperate.h"
#include <sstream>
#include <io.h>
#include <process.h>
#include <fcntl.h>
#include <afx.h>
extern ::CString getParseLanguageString(::CString strOrl);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_SHOW_RATE (30)

// 数据帧回调函数
// Data frame callback function
static void OnConnect(const IMV_SConnectArg* pConnectArg, void* pUser)
{
	CSingleDisplayDlg* pSingleDisplayDlg = (CSingleDisplayDlg*)pUser;
	if (!pSingleDisplayDlg)
	{
		TRACE("pSingleDisplayDlg is NULL!\n");
		return;
	}

	if (pConnectArg == NULL)
	{
		TRACE("pConnectArg is NULL!\n");
		return;
	}

	pSingleDisplayDlg->deviceLinkNotifyProc(*pConnectArg);
}

// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	CSingleDisplayDlg* pSingleDisplayDlg = (CSingleDisplayDlg*)pUser;
	if (!pSingleDisplayDlg)
	{
		TRACE("pSingleDisplayDlg is NULL!\n");
		return;
	}

	if (pFrame == NULL)
	{
		TRACE("pFrame is NULL\n");
		return;
	}

	pSingleDisplayDlg->frameProc(*pFrame);
}


// 显示线程
// display thread
static unsigned int __stdcall displayThread(void* pUser)
{
	CSingleDisplayDlg* pSingleDisplayDlg = (CSingleDisplayDlg*)pUser;
	if (!pSingleDisplayDlg)
	{
		TRACE("pSingleDisplayDlg is NULL!\n");
		return -1;
	}

	pSingleDisplayDlg->displayProc();

	return 0;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
// dialog data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSingleDisplayDlg 对话框
// CSingleDisplayDlg dialog
CSingleDisplayDlg::CSingleDisplayDlg(CWnd* pParent)
	: CDialog(CSingleDisplayDlg::IDD, pParent)
	, m_devHandle(NULL)
	, m_displayThreadHandle(NULL)
	, _connected(false)
	, _bRunning(false)
	, m_dDisplayInterval(0)
	, m_nFirstFrameTime(0)
	, m_nLastFrameTime(0)
	, m_cameraKey(_T(""))
	, m_triggerMode(_T(""))
	, m_Format(_T(""))
	, m_dExposureEdit(0)
	, m_dFrameRateEdit(0)
	, m_dGainEdit(0)
	, m_bResumeGrabbing(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	setDisplayFPS(30);   // 默认显示30帧 | Default display 30 frames
}

void CSingleDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	_render.setHandle(GetDlgItem(IDC_DISPLAY)->GetSafeHwnd());
	DDX_Text(pDX, IDC_EDIT_EXPOSURETIME, m_dExposureEdit);
	DDX_Text(pDX, IDC_EDIT_FRAMERATE, m_dFrameRateEdit);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainEdit);
	DDX_CBString(pDX, IDC_COMBO_DEVICE, m_cameraKey);
	DDX_CBString(pDX, IDC_COMBO_FORMAT, m_Format);
	DDX_CBString(pDX, IDC_COMBO_TRIGGERMODE, m_triggerMode);
}

BOOL CSingleDisplayDlg::OpenConsole()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle(static_cast<long>(reinterpret_cast<uintptr_t>(handle)), _O_TEXT);
	FILE* hf = _fdopen(hCrt, "w");
	*stdout = *hf;
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSingleDisplayDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CSingleDisplayDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CSingleDisplayDlg::OnBnClickedBtnPlay)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_DISCOVERY, &CSingleDisplayDlg::OnBnClickedBtnDiscovery)
	ON_BN_CLICKED(IDC_BTN_GETPARAM, &CSingleDisplayDlg::OnBnClickedBtnGetparam)
	ON_BN_CLICKED(IDC_BTN_SETPARAM, &CSingleDisplayDlg::OnBnClickedBtnSetparam)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CSingleDisplayDlg::OnCbnSelchangeComboTriggermode)
	ON_BN_CLICKED(IDC_BTN_SOFTTRIGGER, &CSingleDisplayDlg::OnBnClickedBtnSofttrigger)
END_MESSAGE_MAP()

// CSingleDisplayDlg 消息处理程序
// CSingleDisplayDlg message processing program

BOOL CSingleDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	// put about ..add menu item to system menu
	// IDM_ABOUTBOX 必须在系统命令范围内。 | IDM_ABOUTBOX must be within system command.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		::CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 根据目前配置的语言初始化界面文本显示 | Initialize the text display of the interface according to the currently configured language
	{
		this->SetWindowText(getParseLanguageString(_T("_MAINFORM_TEXT")));
		GetDlgItem(IDC_BTN_DISCOVERY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_DISCOVERY")));
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CONNECT")));
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_PLAY")));

		GetDlgItem(IDC_GROUP_SOFTTRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_SOFTTRIGGER")));
		GetDlgItem(IDC_STATIC_SOFTTRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_MODE")));
		GetDlgItem(IDC_BTN_SOFTTRIGGER)->SetWindowText(getParseLanguageString(_T("_MAINFORM_SOFTWARE_TRIGGER")));

		GetDlgItem(IDC_GROUP_SETPARAMCONFIG)->SetWindowText(getParseLanguageString(_T("_CFGFORM_PRAMASET")));
		GetDlgItem(IDC_STATIC_FRAME_RATE)->SetWindowText(getParseLanguageString(_T("_CFGFORM_FRAMERATE")));
		GetDlgItem(IDC_STATIC_IMAGE_FORMAT)->SetWindowText(getParseLanguageString(_T("_CFGFORM_IMAGEFORMAT")));
		GetDlgItem(IDC_STATIC_EXPOSE_VALUE)->SetWindowText(getParseLanguageString(_T("_CFGFORM_EXPOSETIMEVALUE")));
		GetDlgItem(IDC_STATIC_GAIN_VALUE)->SetWindowText(getParseLanguageString(_T("_CFGFORM_GAINVALUE")));
		GetDlgItem(IDC_BTN_GETPARAM)->SetWindowText(getParseLanguageString(_T("_CFGFORM_GETPRAMA")));
		GetDlgItem(IDC_BTN_SETPARAM)->SetWindowText(getParseLanguageString(_T("_CFGFORM_SETPRAMA")));
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	// 执行此操作
	// set the icon for this dialog box. When the application's main window is not a dialog box, the framework does this automatically
	SetIcon(m_hIcon, TRUE);			// 设置大图标 |set big icon 
	SetIcon(m_hIcon, FALSE);		// 设置小图标 |set small icon

	OnBnClickedBtnDiscovery();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE | return true unless focus is set to control
}

void CSingleDisplayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 显示线程
// display thread
void CSingleDisplayDlg::displayProc()
{
	while (_bRunning)
	{
		FrameBuffer* pConvertedImage = getConvertedImage();

		if (NULL != pConvertedImage)
		{
			_render.display(pConvertedImage->bufPtr(), (int)pConvertedImage->Width(), (int)pConvertedImage->Height(), pConvertedImage->PixelFormat());
			delete pConvertedImage;
			pConvertedImage = NULL;
		}
	}

	clearConvertedImage();
}

bool CSingleDisplayDlg::isTimeToDisplay()
{
	m_mxTime.enter();

	// 不显示
	// not display
	if (m_dDisplayInterval <= 0)
	{
		m_mxTime.leave();
		return false;
	}

	// 第一帧必须显示
	// the first frame must be displayed
	if (m_nFirstFrameTime == 0 || m_nLastFrameTime == 0)
	{
		m_nFirstFrameTime = CTool::getCurTimeMicroSecond() * 1000;;
		m_nLastFrameTime = m_nFirstFrameTime;
		m_mxTime.leave();
		TRACE("set m_nFirstFrameTime: %I64d\n", m_nFirstFrameTime);
		return true;
	}

	uint64_t nCurTimeTmp = CTool::getCurTimeMicroSecond() * 1000;
	uint64_t nAcquisitionInterval = nCurTimeTmp - m_nLastFrameTime;
	if (nAcquisitionInterval > m_dDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		m_mxTime.leave();
		return true;
	}

	// 当前帧相对于第一帧的时间间隔
	// time interval between the current frame and the first frame
	uint64_t nPre = (m_nLastFrameTime - m_nFirstFrameTime) % m_dDisplayInterval;
	if (nPre + nAcquisitionInterval > m_dDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		m_mxTime.leave();
		return true;
	}

	m_mxTime.leave();
	return false;
}

void CSingleDisplayDlg::setDisplayFPS(int nFPS)
{
	m_mxTime.enter();
	if (nFPS > 0)
	{
		m_dDisplayInterval = 1000 * 1000 * 1000 / nFPS;
	}
	else
	{
		m_dDisplayInterval = 0;
	}
	m_mxTime.leave();
}

// 如果向对话框添加最小化按钮，则需要下面的代码
// 来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
// 这将由框架自动完成。
// If you add a minimize button to the dialog box, you need the following code to draw the icon. 
// For MFC applications that use the document / view model, this is done automatically by the framework.
void CSingleDisplayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文 | device context for drawing
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
		// 使图标在工作矩形中居中
		// center icon in working rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// 绘制图标
		// drawing icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
// When the user drags the minimized window, the system calls this function to get the cursor display.
HCURSOR CSingleDisplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSingleDisplayDlg::deviceLinkNotifyProc(IMV_SConnectArg connectArg)
{
	if (!m_devHandle)
	{
		TRACE("m_devHandle is NULL\n");
		return;
	}

	if (connectArg.event == offLine)
	{
		GetDlgItem(IDC_BTN_DISCOVERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GETPARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SETPARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FRAMERATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EXPOSURETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(FALSE);

		m_bResumeGrabbing = IMV_IsGrabbing(m_devHandle);
		// 断线前不在拉流状态 
		// Not grabbing status before disConnect
		if (!m_bResumeGrabbing)
		{
			return;
		}

		StopStreamGrabbing(true);
	}
	else if (onLine == connectArg.event)
	{
		// 关闭相机
		// Close camera 
		if (IMV_OK != IMV_Close(m_devHandle))
		{
			AfxMessageBox(_T("Close camera failed!"));
			return;
		}

		while (IMV_OK != IMV_Open(m_devHandle))
		{
			Sleep(500);
		}

		// 重新设备连接状态事件回调函数
		// Device connection status event callback function again
		if (IMV_OK != IMV_SubscribeConnectArg(m_devHandle, OnConnect, this))
		{
			AfxMessageBox(_T("Subscribe connect Failed!"));
			return;
		}

		GetDlgItem(IDC_BTN_DISCOVERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GETPARAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SETPARAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FRAMERATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EXPOSURETIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(TRUE);

		// 断线前不在拉流状态 
		// Not grabbing status before disConnect
		if (m_bResumeGrabbing)
		{
			// 开始拉流 
			// Start grabbing 
			StartStreamGrabbing(true);
		}
	}
}

void CSingleDisplayDlg::frameProc(IMV_Frame frame)
{
	// 判断是否要显示。超过显示上限（30帧），就不做转码、显示处理
	// determine whether to display. If the upper display limit (30 frames) is exceeded, transcoding and display processing will not be performed
	if (!this->isTimeToDisplay())
	{
		return;
	}

	// 转码
	// transcoding
	FrameBuffer* pConvertFrameBuffer = new FrameBuffer(m_devHandle, frame);
	addConvertedImage(pConvertFrameBuffer);
}

// 连接按钮被按下
// connection button pressed
void CSingleDisplayDlg::OnBnClickedConnect()
{
	CButton* pConnectBtn = (CButton*)GetDlgItem(IDC_BTN_CONNECT);
	ASSERT(pConnectBtn != NULL);

	if (!_connected)
	{
		if (m_devHandle)
		{
			IMV_DestroyHandle(m_devHandle);
			m_devHandle = NULL;
		}

		if (!UpdateData(TRUE))
		{
			return;
		}

		std::string cameraKey = m_cameraKey.GetBuffer(m_cameraKey.GetLength());

		if (IMV_OK != IMV_CreateHandle(&m_devHandle, modeByCameraKey, (void*)cameraKey.c_str()))
		{
			AfxMessageBox(_T("Create handle Failed!"));
			return;
		}

		if (IMV_OK != IMV_Open(m_devHandle))
		{
			AfxMessageBox(_T("Open camera Failed!"));
			return;
		}

		_connected = true;

		// 设备连接状态事件回调函数
		// Device connection status event callback function
		if (IMV_OK != IMV_SubscribeConnectArg(m_devHandle, OnConnect, this))
		{
			AfxMessageBox(_T("Subscribe connect Failed!"));
			return;
		}

		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_DISCONNECT")));

		GetDlgItem(IDC_BTN_DISCOVERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GETPARAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SETPARAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FRAMERATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EXPOSURETIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(TRUE);

		// 初始化相机属性
		// initialize camera properties
		initTriggerModeParamProperty();
		OnBnClickedBtnGetparam();
	}
	else
	{
		StopStreamGrabbing(false);
		
		if ((NULL != m_devHandle) && (IMV_OK != IMV_Close(m_devHandle)))
		{
			AfxMessageBox(_T("Close camera Failed!"));
		}

		IMV_DestroyHandle(m_devHandle);
		m_devHandle = NULL;

		//EnableBtns(false);
		_connected = false;
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_CONNECT")));
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(getParseLanguageString(_T("_MAINFORM_PLAY")));

		GetDlgItem(IDC_BTN_DISCOVERY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DEVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GETPARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SETPARAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FRAMERATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EXPOSURETIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(FALSE);
	}
}

// 播放按钮响应函数
// play button response function
void CSingleDisplayDlg::OnBnClickedBtnPlay()
{
	if (NULL == m_devHandle)
	{
		return;
	}

	if (IMV_IsGrabbing(m_devHandle))
	{
		StopStreamGrabbing(false);
	}
	else
	{
		StartStreamGrabbing(false);
	}
}

// 窗口关闭响应函数
// window close response function
void CSingleDisplayDlg::OnClose()
{
	OnBnClickedConnect();
	
	CDialog::OnClose();
}

void CSingleDisplayDlg::initTriggerModeParamProperty()
{
	CComboBox* pCmbTriggerMode = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);
	ASSERT(pCmbTriggerMode != NULL);
	pCmbTriggerMode->ResetContent();
	pCmbTriggerMode->Clear();

	IMV_String triggerModeSymbol;
	int ret = IMV_GetEnumFeatureSymbol(m_devHandle, "TriggerMode", &triggerModeSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get TriggerMode symbol value failed!"));
		return;
	}

	m_triggerMode = CString(_T(triggerModeSymbol.str));

	unsigned int nEntryNum = 0;
	ret = IMV_GetEnumFeatureEntryNum(m_devHandle, "TriggerMode", &nEntryNum);
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

	if (IMV_OK != IMV_GetEnumFeatureEntrys(m_devHandle, "TriggerMode", &enumEntryList))
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
		AfxMessageBox(_T("Get TriggerMode settable enumeration value list failed!"));
		return;
	}

	for (unsigned int iIndex = 0; iIndex < nEntryNum; ++iIndex)
	{
		pCmbTriggerMode->InsertString(iIndex, (LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
	}
	bool bEnable = IMV_FeatureIsWriteable(m_devHandle, "TriggerMode");
	pCmbTriggerMode->EnableWindow(bEnable);

	free(enumEntryList.pEnumEntryInfo);
	enumEntryList.pEnumEntryInfo = NULL;
}

void CSingleDisplayDlg::initPixelFormatParamProperty()
{
	CComboBox* pCmbPixelFormat = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	ASSERT(pCmbPixelFormat != NULL);
	pCmbPixelFormat->ResetContent();
	pCmbPixelFormat->Clear();

	IMV_String pixelFormatSymbol;
	int ret = IMV_GetEnumFeatureSymbol(m_devHandle, "PixelFormat", &pixelFormatSymbol);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get PixelFormat symbol value failed!"));
		return;
	}

	m_Format = CString(_T(pixelFormatSymbol.str));

	unsigned int nEntryNum = 0;
	ret = IMV_GetEnumFeatureEntryNum(m_devHandle, "PixelFormat", &nEntryNum);
	if (IMV_OK != ret)
	{
		AfxMessageBox(_T("Get PixelFormat settable enumeration number failed!"));
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

	if (IMV_OK != IMV_GetEnumFeatureEntrys(m_devHandle, "PixelFormat", &enumEntryList))
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
		AfxMessageBox(_T("Get PixelFormat settable enumeration value list failed!"));
		return;
	}

	for (unsigned int iIndex = 0; iIndex < nEntryNum; ++iIndex)
	{
		pCmbPixelFormat->InsertString(iIndex, (LPCTSTR)enumEntryList.pEnumEntryInfo[iIndex].name);
	}

	bool bEnable = IMV_FeatureIsWriteable(m_devHandle, "PixelFormat");
	pCmbPixelFormat->EnableWindow(bEnable);

	free(enumEntryList.pEnumEntryInfo);
	enumEntryList.pEnumEntryInfo = NULL;
}

void CSingleDisplayDlg::initParamProperty()
{
	if (IMV_OK != IMV_GetDoubleFeatureValue(m_devHandle, "AcquisitionFrameRate", &m_dFrameRateEdit))
	{
		AfxMessageBox(_T("Get AcquisitionFrameRate value failed!"));
		return;
	}

	if (IMV_OK != IMV_GetDoubleFeatureValue(m_devHandle, "ExposureTime", &m_dExposureEdit))
	{
		AfxMessageBox(_T("Get ExposureTime value failed!"));
		return;
	}

	if (IMV_OK != IMV_GetDoubleFeatureValue(m_devHandle, "GainRaw", &m_dGainEdit))
	{
		AfxMessageBox(_T("Get GainRaw value failed!"));
		return;
	}
}

void CSingleDisplayDlg::setParamProperty()
{
	if (!IMV_IsGrabbing(m_devHandle))
	{
		std::string pixelFormat = m_Format.GetBuffer(m_Format.GetLength());

		if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "PixelFormat", pixelFormat.c_str()))
		{
			AfxMessageBox(_T("Set PixelFormat symbol value failed!"));
		}
	}

	if (IMV_OK != IMV_SetBoolFeatureValue(m_devHandle, "AcquisitionFrameRateEnable", true))
	{
		AfxMessageBox(_T("Set AcquisitionFrameRateEnable value failed!"));
	}

	if (IMV_OK != IMV_SetDoubleFeatureValue(m_devHandle, "AcquisitionFrameRate", m_dFrameRateEdit))
	{
		AfxMessageBox(_T("Set AcquisitionFrameRate value failed!"));
	}

	if (IMV_OK != IMV_SetDoubleFeatureValue(m_devHandle, "ExposureTime", m_dExposureEdit))
	{
		AfxMessageBox(_T("Set PixelFormat value failed!"));
	}

	if (IMV_OK != IMV_SetDoubleFeatureValue(m_devHandle, "GainRaw", m_dGainEdit))
	{
		AfxMessageBox(_T("Set PixelFormat value failed!"));
	}
}

// 开始拉流
// start grabbing
bool CSingleDisplayDlg::StartStreamGrabbing(bool bResumeConnect)
{
	if (NULL == m_devHandle)
	{
		return false;
	}

	if (!bResumeConnect)
	{
		OnBnClickedBtnGetparam();
	}

	// 显示打开
	// open display
	if (!_render.isOpen())
	{
		_render.open();
		if (!_render.isOpen())
		{
			return false;
		}
	}

	if (IMV_OK != IMV_AttachGrabbing(m_devHandle, onGetFrame, this))
	{
		AfxMessageBox(_T("Attach grabbing Failed!"));
		return false;
	}

	if (IMV_OK != IMV_StartGrabbing(m_devHandle))
	{
		AfxMessageBox(_T("Start Grabbing Failed!"));
		return false;
	}

	_bRunning = true;

	if (!bResumeConnect)
	{
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_STOP_PLAY")));
	}

	// 启动显示线程
	// start display thread
	m_displayThreadHandle = (HANDLE)_beginthreadex(NULL,
		0,
		displayThread,
		this,
		CREATE_SUSPENDED,
		NULL);

	if (!m_displayThreadHandle)
	{
		TRACE("Failed to create display thread!\n");
		return false;
	}
	else
	{
		ResumeThread(m_displayThreadHandle);
	}


	GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(FALSE);
	OnCbnSelchangeComboTriggermode();

	return true;
}

// 停止拉流
// stop grabbing
bool CSingleDisplayDlg::StopStreamGrabbing(bool bResumeConnect)
{
	_bRunning = false;

	if (m_displayThreadHandle)
	{
		WaitForSingleObject(m_displayThreadHandle, INFINITE);
		CloseHandle(m_displayThreadHandle);
		m_displayThreadHandle = NULL;
	}

	_render.close();

	if (!IMV_IsGrabbing(m_devHandle))
	{
		return true;
	}

	if (IMV_OK != IMV_StopGrabbing(m_devHandle))
	{
		AfxMessageBox(_T("Start Grabbing Failed!"));
		return false;
	}

	if (!bResumeConnect)
	{
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(getParseLanguageString(_T("_MAINFORM_PLAY")));
		GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(TRUE);
		OnCbnSelchangeComboTriggermode();
	}
	
	return true;
}

FrameBuffer* CSingleDisplayDlg::getConvertedImage()
{
	FrameBuffer* pConvertedImage = NULL;

	while ( _bRunning )
	{
		m_mutexQue.enter();

		if ( m_listImages.empty() )
		{
			m_mutexQue.leave();
			// Sleep(1)是为了防止拉流回调往m_listImages投递速度比显示慢，导致CPU使用率很高 
			//Sleep(1) is to prevent the get frame callback from delivering to the displayque slower than the display, resulting in high CPU utilization
			Sleep(1);

			continue;
		}
		else
		{
			pConvertedImage = m_listImages.front();
			m_listImages.pop_front();

			m_mutexQue.leave();

			break;
		}		
	}

	return pConvertedImage;
}

void CSingleDisplayDlg::clearConvertedImage()
{
	m_mutexQue.enter();
	while (!m_listImages.empty())
	{
		FrameBuffer* pConvertedImageDrop = m_listImages.front();
		m_listImages.pop_front();
		if (pConvertedImageDrop)
		{
			delete pConvertedImageDrop;
			pConvertedImageDrop = NULL;
		}
	}
	m_mutexQue.leave();
}

void CSingleDisplayDlg::addConvertedImage(FrameBuffer* &pConvertedImage)
{
	static bool bThrowCtrl = true;

	m_mutexQue.enter();

	if ( m_listImages.size() > DEFAULT_SHOW_RATE )
	{
		if ( bThrowCtrl )
		{
			FrameBuffer* pConvertedImageDrop = m_listImages.front();
			m_listImages.pop_front();
			if (pConvertedImageDrop)
			{
				delete pConvertedImageDrop;
				pConvertedImageDrop = NULL;
			}
			bThrowCtrl = false;
		}
		else
		{
			bThrowCtrl = true;
			m_mutexQue.leave();
			return;
		}
	}

	m_listImages.push_back(pConvertedImage);
	m_mutexQue.leave();
}

void CSingleDisplayDlg::OnBnClickedBtnDiscovery()
{
	IMV_DeviceList deviceInfoList;
	if (IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return;
	}
	CComboBox* pDeviceList = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE);
	ASSERT(pDeviceList != NULL);

	pDeviceList->ResetContent();
	pDeviceList->Clear();

	if (deviceInfoList.nDevNum > 0)
	{
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_GETPARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SETPARAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FRAMERATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EXPOSURETIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_FORMAT)->EnableWindow(FALSE);

	for (unsigned int index = 0; index < deviceInfoList.nDevNum; index++)
	{
		pDeviceList->AddString(_T(deviceInfoList.pDevInfo[index].cameraKey));
	}

	pDeviceList->EnableWindow(TRUE);

	pDeviceList->SetCurSel(0);
}


void CSingleDisplayDlg::OnBnClickedBtnGetparam()
{
	initPixelFormatParamProperty();
	initParamProperty();

	UpdateData(FALSE);
}


void CSingleDisplayDlg::OnBnClickedBtnSetparam()
{
	if (UpdateData(TRUE))
	{
		setParamProperty();
	}
}

void CSingleDisplayDlg::OnCbnSelchangeComboTriggermode()
{
	int iIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE))->GetCurSel();
	char szBuf[24] = { 0 };
	((CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE))->GetLBText(iIndex, szBuf);

	std::string triggermode = szBuf;

	if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "TriggerSelector", "FrameStart"))
	{
		AfxMessageBox(_T("Set TriggerSelector Failed!"));
		return;
	}

	if (IMV_OK != IMV_SetEnumFeatureSymbol(m_devHandle, "TriggerMode", triggermode.c_str()))
	{
		AfxMessageBox(_T("Set TriggerMode Failed!"));
	}

	m_triggerMode = _T(szBuf);

	BOOL isEnable = FALSE;

	if ((triggermode == "On") && IMV_IsGrabbing(m_devHandle))
	{
		isEnable = TRUE;
	}

	GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(isEnable);
}


void CSingleDisplayDlg::OnBnClickedBtnSofttrigger()
{
	if (IMV_OK != IMV_ExecuteCommandFeature(m_devHandle, "TriggerSoftware"))
	{
		AfxMessageBox(_T("TriggerSoftware failed!"));
	}
}
