#ifndef __MULTICAMERAS_MVIMAGEBUF_H__
#define __MULTICAMERAS_MVIMAGEBUF_H__

#include "IMVAPI/IMVApi.h"
#include <process.h>
#include <list>

class CMvImageBuf
{
public:
	CMvImageBuf(IMV_Frame frame)
		: m_pDataBuf(NULL), m_iBufSize(0)
	{
		if ( frame.frameInfo.size > 0 )
		{
			if(frame.frameInfo.pixelFormat == gvspPixelMono8)
			{
				m_iBufSize = frame.frameInfo.size;
			}
			else
			{
				m_iBufSize = frame.frameInfo.width * frame.frameInfo.height * 3;
			}		

			m_pDataBuf = new(std::nothrow) unsigned char[m_iBufSize];

			if(m_pDataBuf)
			{
				m_iWidth = frame.frameInfo.width;
				m_iHeight = frame.frameInfo.height;
				m_paddingX = frame.frameInfo.paddingX;
				m_paddingY = frame.frameInfo.paddingY;
				m_iDataSize = frame.frameInfo.size;
				m_pixelFormat = frame.frameInfo.pixelFormat;
			}
		}
	}

	~CMvImageBuf()
	{
		if ( NULL != m_pDataBuf )
		{
			delete[] m_pDataBuf;
			m_pDataBuf = NULL;
		}
	}

public:
	unsigned char* bufPtr()
	{
		return m_pDataBuf;
	}

	unsigned int bufSize()
	{
		return m_iBufSize;
	}

	unsigned int dataSize()
	{
		return m_iDataSize;
	}

	unsigned int imageWidth()
	{
		return m_iWidth;
	}

	unsigned int imageHeight()
	{
		return m_iHeight;
	}

	unsigned int imagePaddingX()
	{
		return m_paddingX;
	}

	unsigned int imagePaddingY()
	{
		return m_paddingY;
	}

	IMV_EPixelType imagePixelFormat()
	{
		return m_pixelFormat;
	}

	void setImageWidth(unsigned int iWidth)
	{
		m_iWidth = iWidth;
	}

	void setImageHeight(unsigned int iHeight)
	{
		m_iHeight = iHeight;
	}

	void setImagePixelFormat(IMV_EPixelType iPixelFormat)
	{
		m_pixelFormat = iPixelFormat;
	}

private:
	unsigned char*					m_pDataBuf;
	unsigned int					m_iBufSize;
	unsigned int					m_iDataSize;
	unsigned int					m_iWidth;
	unsigned int					m_iHeight;
	unsigned int					m_paddingX;
	unsigned int					m_paddingY;
	IMV_EPixelType					m_pixelFormat;
};

#endif // __MULTICAMERAS_MVIMAGEBUF_H__
