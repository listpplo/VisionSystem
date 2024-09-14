#ifndef __MV_IMAGE_CONVERT_HELPER_H__
#define __MV_IMAGE_CONVERT_HELPER_H__

#include "MvImageBuf.h"
#include "MvTool.h"

#define TIMESTAMPFREQUENCY 125000000	//大华相机的时间戳频率固定为125,000,000Hz

class CMvImageConvertHelper
{
public:
	CMvImageConvertHelper(IMV_HANDLE devHandle);
	~CMvImageConvertHelper();

//public:
//	static unsigned int __stdcall threadProc(void* pParam);

public:
	bool startConvert(int iConvertRate);

	bool stopConvert();

	CMvImageBuf* getConvertedImage();

	void setDisplayFPS(int nFPS);

	void grabbingThreadProc();

	void clearConvertedImage();

private:
	bool isTimeToDisplay();

	bool convertFrame(IMV_Frame frameBuf, CMvImageBuf* &pConvertedImage);

	int findMatchCode(int iCode);

	unsigned int calcRateCtrlNum(unsigned int iFrameRate);

	void addConvertedImage(CMvImageBuf* &pConvertedImage);

private:
	typedef std::list<CMvImageBuf*> ImageListType;

private:
	IMV_HANDLE						m_devHandle;
	ImageListType					m_listImages;
	HANDLE							m_threadHandle;
	bool							m_bRunning;
	CMvTool							m_mutexQue;

	CMvTool							m_mxTime;
	int								m_dDisplayInterval;         // 显示间隔
	uint64_t						m_nFirstFrameTime;          // 第一帧的时间戳
	uint64_t						m_nLastFrameTime;           // 上一帧的时间戳
};

#endif // __MV_IMAGE_CONVERT_HELPER_H__
