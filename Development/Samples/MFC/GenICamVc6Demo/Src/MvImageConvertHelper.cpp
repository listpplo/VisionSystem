#include "stdafx.h"

#include "MvImageConvertHelper.h"

#define DEFAULT_SHOW_RATE (30)

/**
* @enum gFormatTransferTbl
* @brief 图像像素格式 | Image pixel format
* @attention 无
*/
static IMV_EPixelType gFormatTransferTbl[] =
{	
	// Mono Format
	gvspPixelMono1p,
	gvspPixelMono8,
	gvspPixelMono10,                
	gvspPixelMono10Packed,			
	gvspPixelMono12,			
	gvspPixelMono12Packed,

	// Bayer Format			
	gvspPixelBayRG8,                
	gvspPixelBayGB8,                
	gvspPixelBayBG8,                              
	gvspPixelBayRG10,               
	gvspPixelBayGB10,               
	gvspPixelBayBG10,                            
	gvspPixelBayRG12,               
	gvspPixelBayGB12,          
	gvspPixelBayBG12,                      
	gvspPixelBayRG10Packed,			
	gvspPixelBayGB10Packed,			
	gvspPixelBayBG10Packed,	        
	gvspPixelBayRG12Packed,			
	gvspPixelBayGB12Packed,			
	gvspPixelBayBG12Packed,			
	gvspPixelBayRG16,    
	gvspPixelBayGB16, 
	gvspPixelBayBG16,
	gvspPixelBayRG10p,              
	gvspPixelBayRG12p,             

	gvspPixelMono1c,

	// RGB Format
	gvspPixelRGB8,				
	gvspPixelBGR8,            

	// YVR Format
	gvspPixelYUV411_8_UYYVYY,     
	gvspPixelYUV422_8_UYVY,       
	gvspPixelYUV422_8,         
	gvspPixelYUV8_UYV,                    
};
#define gFormatTransferTblLen	sizeof(gFormatTransferTbl)/sizeof(gFormatTransferTbl[0])

static unsigned int __stdcall getFrameThread(void* pUser)
{
	CMvImageConvertHelper* pCMvImageConvertHelper = (CMvImageConvertHelper*)pUser;
	if (!pCMvImageConvertHelper)
	{
		TRACE("pCMvImageConvertHelper is NULL!\n");
		return -1;
	}

	pCMvImageConvertHelper->grabbingThreadProc();

	return 0;
}

CMvImageConvertHelper::CMvImageConvertHelper(IMV_HANDLE devHandle)
	: m_devHandle(devHandle)
	, m_threadHandle(NULL)
	, m_bRunning(false)
	, m_dDisplayInterval(0)
	, m_nFirstFrameTime(0)
	, m_nLastFrameTime(0)
{
	setDisplayFPS(30);
}

CMvImageConvertHelper::~CMvImageConvertHelper()
{
	stopConvert();
}

bool CMvImageConvertHelper::startConvert(int iConvertRate)
{
	if (IMV_IsGrabbing(m_devHandle))
	{
		return false;
	}

	if (IMV_OK != IMV_StartGrabbing(m_devHandle))
	{
		return false;
	}

	m_bRunning = true;

	m_threadHandle = (HANDLE)_beginthreadex(NULL,
		0,
		getFrameThread,
		this,
		CREATE_SUSPENDED,
		NULL);

	if (!m_threadHandle)
	{
		TRACE("Failed to create get frame thread!\n");
		return false;
	}
	else
	{
		ResumeThread(m_threadHandle);
	}

	return true;;
}

bool CMvImageConvertHelper::stopConvert()
{
	m_bRunning = false;

	if (m_threadHandle)
	{
		WaitForSingleObject(m_threadHandle, INFINITE);
		CloseHandle(m_threadHandle);
		m_threadHandle = NULL;
	}

	if (!IMV_IsGrabbing(m_devHandle))
	{
		return true;
	}

	IMV_StopGrabbing(m_devHandle);

	return true;
}

CMvImageBuf* CMvImageConvertHelper::getConvertedImage()
{
	CMvImageBuf* pConvertedImage = NULL;

	while ( m_bRunning )
	{
		m_mutexQue.enter();

		if ( m_listImages.empty() )
		{
			m_mutexQue.leave();
			Sleep(1);

			continue;
		}
		else
		{
			pConvertedImage = m_listImages.front();
			m_listImages.pop_front();

			m_mutexQue.leave();

			break;
		}		
	}
	
	return pConvertedImage;
}


void CMvImageConvertHelper::grabbingThreadProc()
{
	int ret = IMV_OK;

	while ( m_bRunning )
	{
		IMV_Frame frameBuf;
		if (IMV_OK != IMV_GetFrame(m_devHandle, &frameBuf, 500))
		{
			Sleep(1);
			continue;
		}

		if (isTimeToDisplay())
		{
			CMvImageBuf* pConvertedImage = NULL;
			bool bRet = convertFrame(frameBuf, pConvertedImage);
			if (bRet)
			{
				addConvertedImage(pConvertedImage);
			}

		}

		IMV_ReleaseFrame(m_devHandle, &frameBuf);
	}
}

bool CMvImageConvertHelper::isTimeToDisplay()
{
	m_mxTime.enter();

	// 不显示
	// don't display
	if (m_dDisplayInterval <= 0)
	{
		m_mxTime.leave();
		return false;
	}

	// 第一帧必须显示
	// The first frame must be displayed
	if (m_nFirstFrameTime == 0 || m_nLastFrameTime == 0)
	{
		m_nFirstFrameTime = CMvTool::getCurTimeMicroSecond() * 1000;
		m_nLastFrameTime = m_nFirstFrameTime;
		m_mxTime.leave();
		return true;
	}
	uint64_t nCurTimeTmp = CMvTool::getCurTimeMicroSecond() * 1000;
	uint64_t nAcquisitionInterval = nCurTimeTmp - m_nLastFrameTime;

	if (nAcquisitionInterval > m_dDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;

		m_mxTime.leave();
		return true;
	}

	// 当前帧与第一帧的时间间隔
	// Time interval between the current frame and the first frame
	uint64_t nPre = (m_nLastFrameTime - m_nFirstFrameTime) % m_dDisplayInterval;
	if (nPre + nAcquisitionInterval > m_dDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;

		m_mxTime.leave();
		return true;
	}
	m_mxTime.leave();
	return false;
}

void CMvImageConvertHelper::setDisplayFPS(int nFPS)
{
	m_mxTime.enter();
	if (nFPS > 0)
	{
		m_dDisplayInterval = 1000 * 1000 * 1000 / nFPS;
	}
	else
	{
		m_dDisplayInterval = 0;
	}
	m_mxTime.leave();
}

bool CMvImageConvertHelper::convertFrame(IMV_Frame frameBuf, CMvImageBuf* &pConvertedImage)
{
	int iFormat = findMatchCode(frameBuf.frameInfo.pixelFormat);
	if (iFormat < 0)
	{
		return false;
	}

	CMvImageBuf *pImageBuf = new CMvImageBuf(frameBuf);
	if (!pImageBuf)
	{
		TRACE("sptrImageBuf is null.\n");
		return false;
	}

	// Mono8不需要转码直接显示
	// Mono8 does not need to convert direct source data display
	if (frameBuf.frameInfo.pixelFormat == gvspPixelMono8)
	{
		memcpy(pImageBuf->bufPtr(), frameBuf.pData, frameBuf.frameInfo.size);
	}
	else
	{
		IMV_PixelConvertParam stPixelConvertParam;
		stPixelConvertParam.nWidth = pImageBuf->imageWidth();
		stPixelConvertParam.nHeight = pImageBuf->imageHeight();
		stPixelConvertParam.ePixelFormat = pImageBuf->imagePixelFormat();
		stPixelConvertParam.pSrcData = frameBuf.pData;
		stPixelConvertParam.nSrcDataLen = pImageBuf->dataSize();
		stPixelConvertParam.nPaddingX = pImageBuf->imagePaddingX();
		stPixelConvertParam.nPaddingY = pImageBuf->imagePaddingY();
		stPixelConvertParam.eBayerDemosaic = demosaicNearestNeighbor;
		stPixelConvertParam.eDstPixelFormat = gvspPixelBGR8;
		stPixelConvertParam.pDstBuf = pImageBuf->bufPtr();
		stPixelConvertParam.nDstBufSize = pImageBuf->bufSize();

		int ret = IMV_PixelConvert(m_devHandle, &stPixelConvertParam);
		if (IMV_OK != ret)
		{
			TRACE("IMV_PixelConvert is failed.\n");
			return false;
		}
	}

	pConvertedImage = pImageBuf;
	return true;
}

int CMvImageConvertHelper::findMatchCode(int iCode)
{
	for ( int i = 0; i < gFormatTransferTblLen; ++i )
	{
		if ( gFormatTransferTbl[i] == iCode )
		{
			return i;
		}
	}

	return -1;
}

unsigned int CMvImageConvertHelper::calcRateCtrlNum(unsigned int iFrameRate)
{
	unsigned int iControlNum = 1;

	if ( iFrameRate > DEFAULT_SHOW_RATE )
	{
		if ( iFrameRate < 40 )
		{
			iControlNum = 6;
		}
		else if ( iFrameRate < 50 )
		{
			iControlNum = 4;
		}
		else if ( iFrameRate < 60 )
		{
			iControlNum = 3;
		}
		else
		{
			iControlNum = 2;
		}
	}
	else
	{
		iControlNum = 1;
	}

	return iControlNum;
}

void CMvImageConvertHelper::addConvertedImage(CMvImageBuf* &pConvertedImage)
{
	static bool bThrowCtrl = true;

	m_mutexQue.enter();
	
	pConvertedImage->setImagePixelFormat(pConvertedImage->imagePixelFormat());
	pConvertedImage->setImageWidth(pConvertedImage->imageWidth());
	pConvertedImage->setImageHeight(pConvertedImage->imageHeight());

	if ( m_listImages.size() > DEFAULT_SHOW_RATE )
	{
		if ( bThrowCtrl )
		{
			CMvImageBuf* pConvertedImageDrop = m_listImages.front();
			m_listImages.pop_front();
			if (pConvertedImageDrop)
			{
				delete pConvertedImageDrop;
				pConvertedImageDrop = NULL;
			}
			bThrowCtrl = false;
		}
		else
		{
			bThrowCtrl = true;
			m_mutexQue.leave();
			return;
		}
	}

	m_listImages.push_back(pConvertedImage);
	m_mutexQue.leave();
}

void CMvImageConvertHelper::clearConvertedImage()
{
	m_mutexQue.enter();
	while (!m_listImages.empty())
	{
		CMvImageBuf* pConvertedImageDrop = m_listImages.front();
		m_listImages.pop_front();
		if (pConvertedImageDrop)
		{
			delete pConvertedImageDrop;
			pConvertedImageDrop = NULL;
		}
	}
	m_mutexQue.leave();
}


