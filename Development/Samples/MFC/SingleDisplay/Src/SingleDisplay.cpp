// Demo.cpp : 定义应用程序的类行为。
// Demo.cpp : defining the class behavior of an application

#include "stdafx.h"
#include "SingleDisplay.h"
#include "SingleDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSingleDisplayApp

BEGIN_MESSAGE_MAP(CSingleDisplayApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSingleDisplayApp 构造
// CSingleDisplayApp constructor
CSingleDisplayApp::CSingleDisplayApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中 |place all important initializations in InitInstance
}


// 唯一的一个 CSingleDisplayApp 对象
// the only csingledisplayapp object
CSingleDisplayApp theApp;


// CSingleDisplayApp 初始化
// CSingleDisplayApp initialization
BOOL CSingleDisplayApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	// 则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	// If an application manifest running on Windows XP specifies to use comctl32.dll version 6 or later to enable visualization,
	// initcommoncontrolsex function is required. Otherwise, the window cannot be created.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	// set it to include all the public control classes in this application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// Standard initialization if you do not use these features and want to reduce the size of the final executable,
	 //remove the following specific initialization routines that are not required to change the registry key used to store the settings

	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	// the string should be modified as appropriate, 
	// for example to a company or organization name
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CSingleDisplayDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		// 对话框的代码
		// place code here to process when to close the dialog box with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		// 对话框的代码
		// place code here to process when to close the dialog box with cancel
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	// Since the dialog box is closed, false is returned to exit the application instead of starting the application's message pump.
	return FALSE;
}
