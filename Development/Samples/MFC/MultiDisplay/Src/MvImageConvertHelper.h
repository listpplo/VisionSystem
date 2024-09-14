#ifndef __MVIMAGE_CONVERT_H__
#define __MVIMAGE_CONVERT_H__

#include "MvImageBuf.h"
#include "MvTool.h"

#define TIMESTAMPFREQUENCY 125000000

class CMvImageConvertHelper
{
public:
	CMvImageConvertHelper(IMV_HANDLE devHandle);
	~CMvImageConvertHelper();

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

	int findMatchCode(IMV_EPixelType iCode);

	unsigned int calcRateCtrlNum();

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
	int							    m_dDisplayInterval;         // 显示间隔			| diaplay interval
	uint64_t						m_nFirstFrameTime;          // 第一帧的时间戳	| frist frame time stamp 
	uint64_t						m_nLastFrameTime;           // 上一帧的时间戳	| previous frame time stamp
};

#endif // __MVIMAGE_CONVERT_H__
