#ifndef __RENDER_H__
#define __RENDER_H__

#include "RGBConvert.h"
#include "Media/VideoRender.h"

class CRender
{
public:

	/// \brief 构造函数 | construc function
	CRender(HANDLE handler);

	/// \brief 构造函数 | construc function
	CRender();

	/// \brief 设置句柄 | set handle
	void setHandle(HANDLE handler);

	/// \brief 析构函数 | destruct function 
	~CRender();

	/// \brief 打开窗口 | open window
	bool open();

	/// \brief  关闭窗口 | close window
	bool close();

	/// \brief 是否打开 | whether open
	bool isOpen() const;

	/// \brief 设置宽度 | set width
	void setWidth(int iWidth){	m_width = iWidth; }

	/// \brief 设置高度 | set height
	void setHeight(int iHeight){ m_height = iHeight; }

	/// \brief 获取宽度 | get width 
	 int getWidth()  { return m_width; }

	/// \brief 获取高度 | get height
	 int getHeight() { return m_height; }

	/// \brief  显示图片 | display image
	/// \param[in] displayBuffer	数据缓冲  | data buffer
	/// \param[in] iWidth			图片宽	  | image width
	/// \param[in] iHeight			图片高	  | image height
	/// \param[in] iPixelFormat		图片像素格式 | image pixel format
	 bool display(unsigned char* displayBuffer, int iWidth, int iHeight, IMV_EPixelType iPixelFormat);
	
private:
	int					m_width;			///< 图像宽		| image width
	int					m_height;			///< 图像高		| image height		
	HANDLE				m_wind;				///< 窗口句柄	| window handle
	VR_HANDLE			m_handler;			///< 绘图句柄	| draw inco handle
	VR_OPEN_PARAM_S		m_params;			///< 显示参数	| display parameter
};

#endif //__RENDER_H__

