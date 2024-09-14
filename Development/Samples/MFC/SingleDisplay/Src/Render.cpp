#include "stdafx.h"
#include <utility>
#include <windows.h>
#include "Render.h"


/***********************************************************************
*
*                            常量定义区 |constant define area
*
************************************************************************/
// 默认长宽
// defult long、width
const  unsigned int DEFAULT_WIDTH = 16;
const  unsigned int DEFAULT_HEIGHT = 16;
const  unsigned int DEFAULT_DATA_SIZE = DEFAULT_WIDTH * DEFAULT_HEIGHT * 3 / 2;


/***********************************************************************
*
*                            对象函数定义区 |object function definition area
*
************************************************************************/
/// \brief 构造函数 |constructor function
CRender::CRender(HANDLE handler)
	: m_wind(handler)
	, m_handler(NULL)
{
}
/// \brief 构造函数 |constructor function
CRender::CRender()
: m_wind(NULL)
, m_handler(NULL)
{
}

/// \brief 设置句柄 |set handle
void CRender::setHandle(HANDLE handler)
{
	m_wind = handler;
}

/// \brief 析构函数 |constructor function
CRender::~CRender()
{
	close();
}

/// \brief   打开窗口 | open window
bool CRender::open()
{
	if(m_wind == NULL)
	{
		return false;
	}

	memset(&m_params, 0, sizeof(m_params));
	m_params.eVideoRenderMode = VR_MODE_GDI;
	m_params.hWnd = (VR_HWND)m_wind;
	m_params.nWidth = DEFAULT_WIDTH;	//GDI显示的时候，值可以为0以外的任意 | When GDI is displayed, the value can be any value other than 0
	m_params.nHeight = DEFAULT_HEIGHT;	//GDI显示的时候，值可以为0以外的任意 | When GDI is displayed, the value can be any value other than 0
	
	if(VR_SUCCESS != VR_Open(&m_params, &m_handler)) 
	{
		TRACE("%s cant't display BGR on this computer", __FUNCTION__);
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
	if (m_handler != NULL)
	{
		VR_Close(m_handler);
		m_handler = NULL;
	}
	return true;
}

/// \brief 是否打开 |if open
bool CRender::isOpen() const
{
	return NULL != m_handler;
}

/// \brief  显示图片 |display image
/// \param[in] image  图片
bool CRender::display(unsigned char* displayBuffer, int iWidth, int iHeight, IMV_EPixelType iPixelFormat)
{
	if (displayBuffer == NULL || iWidth == 0 || iHeight == 0)
	{
		return false;
	}
	
	 unsigned int width = iWidth;
	 unsigned int height = iHeight;
	
	if (isOpen() && (getWidth() != width || getHeight() != height) && (getWidth() != 0 || getHeight() != 0))
	{
		close();
	}
	
	if (!isOpen())
	{
		open();
	}

	if (isOpen())
	{
		setHeight(iHeight);
		setWidth(iWidth);

		VR_FRAME_S	renderParam = {0};
		renderParam.data[0] = displayBuffer;
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

		if (VR_SUCCESS != VR_RenderFrame(m_handler, &renderParam, NULL))
		{
			TRACE("%s render failed.", __FUNCTION__);
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}	
}

