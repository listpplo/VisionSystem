#ifndef __MV_RENDER_H__
#define __MV_RENDER_H__

#include "MvImageBuf.h"
#include "Media/VideoRender.h"

class CRender
{
public:
	/// \brief 构造函数
	CRender(HANDLE hWnd);

	/// \brief 构造函数
	CRender();

	/// \brief 析构函数
	~CRender();

	/// \brief 设置句柄
	void setHandle(HANDLE hWnd);

	/// \brief   打开窗口
	/// \param[in] width   窗口宽度
	/// \param[in] height  窗口高度
	bool open(unsigned int iWidth, unsigned int iHeight);

	/// \brief  关闭窗口
	bool close();

	/// \brief 是否打开
	bool isOpen() const;

	/// \brief 获取宽度
	unsigned int getWidth() const;

	/// \brief 获取高度
	unsigned int getHeight() const;

	/// \brief  显示图片
	/// \param[in] image  图片
	bool display(CMvImageBuf* pConvertedImage);

	bool display(unsigned char* pData, unsigned int iDataSize, IMV_EPixelType pixelFormat);
	
private:
	//回调函数
	static void CALLBACK drawCallBack(VR_HWND hWindow, VR_HDC hDrawHandle, void* pUserData);

	//实际的描绘函数
	void draw(VR_HWND hWindow, VR_HDC hDrawHandle);

	HANDLE             m_hWnd;            ///< 窗口句柄
	VR_HANDLE          m_vrHandle;        ///< 绘图句柄
	VR_OPEN_PARAM_S    m_vrParams;        ///< 显示参数
};

#endif // __MV_RENDER_H__
