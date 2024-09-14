// MultiCamerasToolDlg.cpp : 实现文件
// Implementation file

#include "stdafx.h"
#include "MultiCamerasTool.h"
#include "MultiCamerasToolDlg.h"
#include "CameraSubDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 定义子窗口格局
// define child window pattern
#define SUB_DLG_NUM (4)
#define ROW_NUM (2)
#define COLUM_NUM (2)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
// Caboutdlg dialog box for application about menu item
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


// CMultiCamerasToolDlg 对话框
// CMultiCamerasToolDlg dialog



CMultiCamerasToolDlg::CMultiCamerasToolDlg(CWnd* pParent)
	: CDialog(CMultiCamerasToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiCamerasToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiCamerasToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_MESSAGE(WM_MY_DISCOVERY_MESSAGE, &CMultiCamerasToolDlg::OnMyDiscoveryMsg)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CMultiCamerasToolDlg 消息处理程序
// CMultiCamerasToolDlg message processing program

BOOL CMultiCamerasToolDlg::OnInitDialog()
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
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	// 执行此操作
	// set the icon for this dialog box. When the application's main window is not a dialog box, the framework does this automatically
	SetIcon(m_hIcon, TRUE);			// 设置大图标 | set big icon 
	SetIcon(m_hIcon, FALSE);		// 设置小图标 | set small icon 
	

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	// add additional initialization code here
	initCameraSubDlg();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE | return true unless focus is set to control
}

void CMultiCamerasToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (61458 == nID)
	{
		return;
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
// 来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
// 这将由框架自动完成。
// If you add a minimize button to the dialog box, you need the following code to draw the icon. 
// For MFC applications that use the document / view model, this is done automatically by the framework.
void CMultiCamerasToolDlg::OnPaint()
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

		// 绘制图标 | drawing icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
// When the user drags the minimized window, the system calls this function to get the cursor display.
HCURSOR CMultiCamerasToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultiCamerasToolDlg::initCameraSubDlg()
{
	int iColum = COLUM_NUM;
	int iRow = ROW_NUM;
	m_pCameraSubDlgs = new CCameraSubDlg[SUB_DLG_NUM];

	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(0, 0 , iScreenWidth, iScreenHeight);

	CRect rectMainDlg;
	GetClientRect(&rectMainDlg);

	for( int i = 0; i < SUB_DLG_NUM; i ++ )
	{
		CRect rect;
		int nWidth = rectMainDlg.Width()/iColum;
		int nHeight = rectMainDlg.Height()/iRow;

		rect.SetRect(
			rectMainDlg.TopLeft().x + (i % iColum ) * nWidth,
			rectMainDlg.TopLeft().y + (i / iColum) * nHeight,
			rectMainDlg.TopLeft().x + (i % iColum +1) * nWidth,
			rectMainDlg.TopLeft().y + ((i / iColum)+1) * nHeight);

		m_pCameraSubDlgs[i].setRect(&rect);
		m_pCameraSubDlgs[i].Create(IDD_CAMERA_DIALOG, this);
		m_pCameraSubDlgs[i].ShowWindow(SW_SHOW);
	}

	// 枚举设备 
	// enum camera 
	IMV_DeviceList deviceInfoList;
	if(IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return;
	}

	unsigned int iShowCameraNum = deviceInfoList.nDevNum < SUB_DLG_NUM ? deviceInfoList.nDevNum : SUB_DLG_NUM;
	for (unsigned int iIndex = 0; iIndex < iShowCameraNum; ++iIndex)
	{
		m_pCameraSubDlgs[iIndex].setCamera(iIndex);
	}
}


void CMultiCamerasToolDlg::OnClose()
{
	for( int i = 0; i < SUB_DLG_NUM; i ++ )
	{
		m_pCameraSubDlgs[i].OnBnClickedButtonDisconnect();
	}

    if(m_pCameraSubDlgs != NULL)
    {
        delete[] m_pCameraSubDlgs;
        m_pCameraSubDlgs = NULL;
    }
    
	CDialog::OnClose();
}

void CMultiCamerasToolDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加控件通知处理程序代码 | add control notification processing code here

	//CDialog::OnNcLButtonDblClk(nHitTest, point);
	ShowWindow(SW_MINIMIZE);
}

LRESULT CMultiCamerasToolDlg::OnMyDiscoveryMsg(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(_T("Tips: it'll discovery camera again!"));

	IMV_DeviceList deviceInfoList;
	if (IMV_OK != IMV_EnumDevices(&deviceInfoList, interfaceTypeAll))
	{
		AfxMessageBox(_T("Enum devices failed!"));
		return 0;
	}

	unsigned int iShowCameraNum = deviceInfoList.nDevNum < SUB_DLG_NUM ? deviceInfoList.nDevNum : SUB_DLG_NUM;
	for (unsigned int iIndex = 0; iIndex < iShowCameraNum; ++iIndex)
	{
		m_pCameraSubDlgs[iIndex].setCamera(iIndex);
	}

	return 0;
}

void CMultiCamerasToolDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// add control notification processing code here
	OnMyDiscoveryMsg(NULL, NULL);

	CDialog::OnRButtonUp(nFlags, point);
}
