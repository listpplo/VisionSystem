#include "stdafx.h"

#include "MvDisplayHelper.h"

// ÏÔÊ¾Ïß³Ì
// display thread
static unsigned int __stdcall displayThread(void* pUser)
{
	CMvDisplayHelper* pCMvDisplayHelper = (CMvDisplayHelper*)pUser;
	if (!pCMvDisplayHelper)
	{
		TRACE("pCMvDisplayHelper is NULL!\n");
		return -1;
	}

	pCMvDisplayHelper->displayThreadProc();

	return 0;
}

CMvDisplayHelper::CMvDisplayHelper(IMV_HANDLE devHandle)
	: m_pImageConvertHelper(NULL)
	, m_displayThreadHandle(NULL)
	, m_bRunning(false)
	, m_iWidth(0)
	, m_iHeight(0)
{
	if (!m_pImageConvertHelper)
	{
		m_pImageConvertHelper = new CMvImageConvertHelper(devHandle);
	}

	if (!m_pImageConvertHelper)
	{
		TRACE("m_pImageConvertHelper is NULL!\n");
	}

	int64_t iWidthVal = 0;
	IMV_GetIntFeatureValue(devHandle, "Width", &iWidthVal);
	m_iWidth = (unsigned int)iWidthVal;

	int64_t iHeightVal = 0;
	IMV_GetIntFeatureValue(devHandle, "Height", &iHeightVal);
	m_iWidth = (unsigned int)iWidthVal;
	m_iHeight = (unsigned int)iHeightVal;
}

CMvDisplayHelper::~CMvDisplayHelper()
{
	stopDisplay();

	if (m_pImageConvertHelper)
	{
		delete m_pImageConvertHelper;
		m_pImageConvertHelper = NULL;
	}
}

bool CMvDisplayHelper::isValid()
{
	return (NULL == m_pImageConvertHelper) ? false : true;
}

bool CMvDisplayHelper::startDisplay(void* pHandle, int iDispalyRate)
{
	bool bOpenRet = m_pImageConvertHelper->startConvert(iDispalyRate);
	if ( !bOpenRet )
	{
		return false;
	}

	m_render.setHandle(pHandle);

	bOpenRet = m_render.open();
	if ( !bOpenRet )
	{
		m_pImageConvertHelper->stopConvert();

		return false;
	}

	m_bRunning = true;

	m_displayThreadHandle = (HANDLE)_beginthreadex(NULL,
		0,
		displayThread,
		this,
		CREATE_SUSPENDED,
		NULL);

	if (!m_displayThreadHandle)
	{
		TRACE("Failed to create display thread!\n");
		return false;
	}
	else
	{
		ResumeThread(m_displayThreadHandle);
	}

	return true;
}

bool CMvDisplayHelper::stopDisplay()
{
	m_bRunning = false;

	if (NULL != m_pImageConvertHelper)
	{
		m_pImageConvertHelper->stopConvert();
	}

	if (m_displayThreadHandle)
	{
		WaitForSingleObject(m_displayThreadHandle, INFINITE);
		CloseHandle(m_displayThreadHandle);
		m_displayThreadHandle = NULL;
	}

	m_render.close();

	return true;
}

void CMvDisplayHelper::displayThreadProc()
{
	while ( m_bRunning )
	{
		if (NULL == m_pImageConvertHelper)
		{
			Sleep(1);
			continue;
		}

		CMvImageBuf* pConvertedImage = m_pImageConvertHelper->getConvertedImage();
		if (NULL != pConvertedImage)
		{
			m_render.display(pConvertedImage->bufPtr(), (int)pConvertedImage->imageWidth(), (int)pConvertedImage->imageHeight(), pConvertedImage->imagePixelFormat());
			delete pConvertedImage;
			pConvertedImage = NULL;
		}		
	}

	m_pImageConvertHelper->clearConvertedImage();
}
