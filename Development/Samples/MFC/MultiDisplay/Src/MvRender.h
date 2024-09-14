#ifndef __MVRENDER_H__
#define __MVRENDER_H__

#include "MvImageBuf.h"
#include "Media/VideoRender.h"

class CRender
{
public:
	/// \brief 构造函数 | construc function
	CRender(HANDLE hWnd);

	/// \brief 构造函数 | construc function
	CRender();

	/// \brief 析构函数
	~CRender();

	/// \brief 设置句柄 | set handle
	void setHandle(HANDLE hWnd);

	/// \brief 打开窗口 | open window
	bool open();

	/// \brief  关闭窗口 | close window
	bool close();

	/// \brief 是否打开 | whether open
	bool isOpen() const;

	/// \brief 设置宽度 | set width
	void setWidth(unsigned int iWidth){	m_width = iWidth; }

	/// \brief 设置高度 | set height
	void setHeight(unsigned int iHeight){ m_height = iHeight; }

	/// \brief 获取宽度 | get width 
	unsigned int getWidth()  { return m_width; }

	/// \brief 获取高度 | get height
	unsigned int getHeight() { return m_height; }

	/// \brief  显示图片 | display image
	/// \param[in] image  图片 | image
	bool display(unsigned char* pData, int iWidth, int iHeight, IMV_EPixelType iPixelFormat);
	
private:
	int					m_width;			///< 图像宽		| image width
	int					m_height;			///< 图像高		| image height		
	HANDLE				m_hWnd;				///< 窗口句柄	| window handle
	VR_HANDLE			m_vrHandle;			///< 绘图句柄	| draw inco handle
	VR_OPEN_PARAM_S		m_vrParams;			///< 显示参数   | display parameters
};

#endif // __MVRENDER_H__
