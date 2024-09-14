#ifndef __MV_DISPLAY_HELPER_H__
#define __MV_DISPLAY_HELPER_H__

#include "MvImageConvertHelper.h"
#include "MvRender.h"

class CMvDisplayHelper
{
public:
	CMvDisplayHelper(IMV_HANDLE devHandle);
	~CMvDisplayHelper();

public:
	bool isValid();

	bool startDisplay(void* pHandle, int iDispalyRate);

	bool stopDisplay();

	void displayThreadProc();


private:
	CMvImageConvertHelper*			m_pImageConvertHelper;
	HANDLE							m_displayThreadHandle;
	bool							m_bRunning;
	CRender							m_render;
	unsigned int					m_iWidth;
	unsigned int					m_iHeight;
};

#endif // __MV_DISPLAY_HELPER_H__
