#include "stdafx.h"
#include "MvCamera.h"

static void onDeviceLinkNotify(const IMV_SConnectArg* pConnectArg, void* pUser)
{
	CMvCamera* pCMvCamera = (CMvCamera*)pUser;
	if (!pCMvCamera)
	{
		TRACE("pCMvCamera is NULL!\n");
		return;
	}

	if (!pConnectArg)
	{
		TRACE("pConnectArg is NULL!\n");
		return;
	}

	pCMvCamera->procConnectArg(pConnectArg);
}


CMvCamera::CMvCamera(unsigned int index)
	: m_index(index)
	, m_devHandle(NULL)
	, m_pDisplayHelper(NULL)
	, m_pMvCameraSink(NULL)
{

}

CMvCamera::~CMvCamera()
{
	if (m_pDisplayHelper)
	{
		delete m_pDisplayHelper;
		m_pDisplayHelper = NULL;
	}
}

bool CMvCamera::open(IMvCameraSink* pSink)
{
	if (IMV_OK != IMV_CreateHandle(&m_devHandle, modeByIndex, (void*)&m_index))
	{
		return false;
	}

	if (IMV_OK != IMV_Open(m_devHandle))
	{
		return false;
	}

	if (NULL != pSink)
	{
		if (IMV_OK != IMV_SubscribeConnectArg(m_devHandle, onDeviceLinkNotify, this))
		{
			return false;
		}
		m_pMvCameraSink = pSink;
	}

	return true;
}

bool CMvCamera::isOpen()
{
	return IMV_IsOpen(m_devHandle);
}

void CMvCamera::close()
{
	if (NULL != m_devHandle)
	{
		if (m_pDisplayHelper)
		{
			delete m_pDisplayHelper;
			m_pDisplayHelper = NULL;
		}
		m_pMvCameraSink = NULL;

		IMV_Close(m_devHandle);

		IMV_DestroyHandle(m_devHandle);
	}
}

bool CMvCamera::startDisplay(void* pShowHandle, int iShowRate)
{
	if (!IMV_IsOpen(m_devHandle))
	{
		return false;
	}

	if (m_pDisplayHelper)
	{
		delete m_pDisplayHelper;
		m_pDisplayHelper = NULL;
	}

	m_pDisplayHelper = new CMvDisplayHelper(m_devHandle);

	if (!m_pDisplayHelper)
	{
		return false;
	}

	if (!m_pDisplayHelper->isValid())
	{
		return false;
	}

	return m_pDisplayHelper->startDisplay(pShowHandle, iShowRate);
}

bool CMvCamera::stopDisplay()
{
	if (NULL == m_pDisplayHelper)
	{
		return false;
	}

	return m_pDisplayHelper->stopDisplay();
}

IMV_HANDLE CMvCamera::getCameraHandle()
{
	return m_devHandle;
}

void CMvCamera::procConnectArg(const IMV_SConnectArg* pConnectArg)
{
	if (pConnectArg->event == offLine && NULL != m_pMvCameraSink)
	{
		m_pMvCameraSink->onCameraDisconnect();
	}
}


