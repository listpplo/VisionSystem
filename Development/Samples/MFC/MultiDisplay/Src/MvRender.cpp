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
CRender::CRender(HANDLE hWnd)
	: m_hWnd(hWnd)
	, m_vrHandle(NULL)
{
}
/// \brief 构造函数 | constructor function
CRender::CRender()
	: m_hWnd(NULL)
	, m_vrHandle(NULL)
{
}

/// \brief 设置句柄 | set handle
void CRender::setHandle(HANDLE hWnd)
{
	m_hWnd = hWnd;
}

/// \brief 析构函数 | constructor function
CRender::~CRender()
{
	close();
}

/// \brief   打开窗口 | open window
bool CRender::open()
{
	if (m_vrHandle != NULL || m_hWnd == NULL)
	{
		return false;
	}

	memset(&m_vrParams, 0, sizeof(m_vrParams));
	m_vrParams.eVideoRenderMode = VR_MODE_GDI;
	m_vrParams.hWnd = (VR_HWND)m_hWnd;
	m_vrParams.nWidth = DEFAULT_WIDTH;   // GDI显示的时候，值可以为0以外的任意   | When GDI is displayed, the value can be any value other than 0
	m_vrParams.nHeight = DEFAULT_HEIGHT; // GDI显示的时候，值可以为0以外的任意 | When GDI is displayed, the value can be any value other than 0

	VR_ERR_E ret = VR_Open(&m_vrParams, &m_vrHandle);
	if (ret == VR_NOT_SUPPORT)
	{
		TRACE("%s cant't display RGB on this computer", __FUNCTION__);
		return false;
	}

	m_width = 0;
	m_height = 0;
	TRACE("%s open success.\n", __FUNCTION__);
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

/// \brief  显示图片 |display image
/// \param[in] image  图片
bool CRender::display(unsigned char* pData, int iWidth, int iHeight, IMV_EPixelType iPixelFormat)
{
	if (pData == NULL || iWidth == 0 || iHeight == 0)
	{
		TRACE("%s image is invalid.", __FUNCTION__);
		return false;
	}
	
	if ( isOpen() &&
		(getWidth() != iWidth || getHeight() != iHeight) &&
		(getWidth() != 0 || getHeight() != 0) )
	{
		close();
	}
	
	if (!isOpen())
	{
		open();
	}

	if (isOpen())
	{
		setWidth(iWidth);
		setHeight(iHeight);

		VR_FRAME_S	renderParam = {0};
		renderParam.data[0] = pData;
		renderParam.stride[0] = iWidth;
		renderParam.nWidth = iWidth;
		renderParam.nHeight = iHeight;

		if (iPixelFormat == gvspPixelMono8)
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

		TRACE("%s render failed.", __FUNCTION__);
		return false;

	}
	else
	{
		return false;
	}

	return true;
}
