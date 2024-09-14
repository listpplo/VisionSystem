// MultiCamerasTool.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号 | main symbol


// CMultiCamerasToolApp:
// 有关此类的实现，请参阅 MultiCamerasTool.cpp
//

class CMultiCamerasToolApp : public CWinApp
{
public:
	CMultiCamerasToolApp();

// 重写 | rewrite
	public:
	virtual BOOL InitInstance();

// 实现 |implementation

	DECLARE_MESSAGE_MAP()
};

extern CMultiCamerasToolApp theApp;