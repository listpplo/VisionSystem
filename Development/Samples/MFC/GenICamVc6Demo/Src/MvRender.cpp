#include "stdafx.h"

#include <utility>
#include <windows.h>
#include "MvRender.h"

/***********************************************************************
*
*                            常量定义区 | constant define area
*
************************************************************************/
// 默认长宽
// defult long、width
const unsigned int DEFAULT_WIDTH = 16;
const unsigned int DEFAULT_HEIGHT = 16;
const unsigned int DEFAULT_DATA_SIZE = DEFAULT_WIDTH * DEFAULT_HEIGHT * 3 / 2;


/***********************************************************************
*
*                            对象函数定义区 | object function definition area
*
************************************************************************/
/// \brief 构造函数 |constructor function

/// \brief 设置句柄 | set handle
bool CRender::display(unsigned char* pData, unsigned int iDataSize, IMV_EPixelType pixelFormat)
{
	unsigned int iWidth = m_vrParams.nWidth;
	unsigned int iHeight = m_vrParams.nHeight;

	if ( m_vrHandle == NULL ||
		pData == NULL ||
		iDataSize == 0 ||
		iDataSize < iWidth * iHeight )
	{
		return false;
	}
	
	VR_FRAME_S	renderParam = {0};

	renderParam.data[0] = pData;
	renderParam.stride[0] = iWidth;
	renderParam.nWidth = iWidth;
	renderParam.nHeight = iHeight;

	if ( pixelFormat == gvspPixelMono8)
	{
		renderParam.format = VR_PIXEL_FMT_MONO8;
	}
	else
	{
		renderParam.format = VR_PIXEL_FMT_RGB24;
	}

	if ( VR_SUCCESS == VR_RenderFrame(m_vrHandle, &renderParam, NULL) )
	{
		return true;
	}

	return false;
}

void CALLBACK CRender::drawCallBack(VR_HWND hWindow, VR_HDC hDrawHandle, void* pUserData)
{
	CRender* pRender = (CRender*)pUserData;
	if(pRender)
	{
		pRender->draw(hWindow,hDrawHandle);
	}
}

void CRender::draw(VR_HWND hWindow, VR_HDC hDrawHandle)
{
	if(hWindow == NULL || hDrawHandle == NULL)
	{
		return;
	}
	HDC hDC = (HDC)hDrawHandle;

	SetTextColor(hDC,RGB(0,255,0));

	RECT rt;
	rt.left = 30;
	rt.top = 30;
	rt.right = 200;
	rt.bottom = 60;

	SetBkMode(hDC,TRANSPARENT);
	DrawTextA(hDC,"test123",8,&rt,DT_LEFT | DT_VCENTER);
}

/// \brief 构造函数
CRender::CRender(HANDLE hWnd)
	: m_hWnd(hWnd)
	, m_vrHandle(NULL)
{
}
/// \brief 构造函数 |constructor function
CRender::CRender()
	: m_hWnd(NULL)
	, m_vrHandle(NULL)
{
}

/// \brief 设置句柄 |set handle
void CRender::setHandle(HANDLE hWnd)
{
	m_hWnd = hWnd;
}

/// \brief 析构函数 |constructor function
CRender::~CRender()
{
	close();
}

/// \brief   打开窗口 | open window
/// \param[in] width   窗口宽度 | window width
/// \param[in] height  窗口高度 | window height
bool CRender::open(unsigned int iWidth, unsigned int iHeight)
{
	if ( m_vrHandle != NULL ||
		m_hWnd == NULL ||
		0 == iWidth ||
		0 == iHeight )
	{
		return false;
	}
	
	memset(&m_vrParams, 0, sizeof(m_vrParams));
	m_vrParams.eVideoRenderMode = VR_MODE_GDI;
	m_vrParams.hWnd = (VR_HWND)m_hWnd;
	m_vrParams.nWidth = iWidth;
	m_vrParams.nHeight = iHeight;
	
	VR_ERR_E ret = VR_Open(&m_vrParams, &m_vrHandle);
	if (ret == VR_NOT_SUPPORT)
	{
		return false;
	}

	// 设置回调函数
	// set callback function
	VR_SetDrawCallback(m_vrHandle,drawCallBack,this);
	return true;
}

/// \brief  关闭窗口 |close window
bool CRender::close()
{
	if (m_vrHandle != NULL)
	{
		VR_Close(m_vrHandle);
		m_vrHandle = NULL;
	}
	return true;
}

/// \brief 是否打开 |if open
bool CRender::isOpen() const
{
	return NULL != m_vrHandle;
}

/// \brief 获取宽度 | get width
unsigned int CRender::getWidth() const
{
	return m_vrParams.nWidth;
}

/// \brief 获取高度 | get height
unsigned int CRender::getHeight() const
{
	return m_vrParams.nHeight;
}

/// \brief  显示图片 |display image
/// \param[in] image  图片
bool CRender::display(CMvImageBuf* pConvertedImage)
{
	if (pConvertedImage == NULL || 
		pConvertedImage->bufPtr() == NULL ||
		pConvertedImage->bufSize() == 0 ||
		pConvertedImage->imageWidth() == 0 ||
		pConvertedImage->imageHeight() == 0 )
	{
		//printf("%s image is invalid.", __FUNCTION__);
		return false;
	}
	
	unsigned int iWidth = pConvertedImage->imageWidth();
	unsigned int iHeight = pConvertedImage->imageHeight();
	
	if ( isOpen() &&
		(getWidth() != iWidth || getHeight() != iHeight) )
	{
		close();
	}
	
	if (!isOpen())
	{
		open(iWidth, iHeight);
	}
	
	bool bDisplayRet = false;
	if (isOpen())
	{
		bDisplayRet = display(pConvertedImage->bufPtr(), pConvertedImage->bufSize(), pConvertedImage->imagePixelFormat());
	}

	return bDisplayRet;
}

