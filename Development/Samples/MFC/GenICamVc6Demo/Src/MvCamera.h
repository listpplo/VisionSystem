#ifndef __MV_CAMERA_H__
#define __MV_CAMERA_H__

#include "MvDisplayHelper.h"

#include <string>
#include <vector>

class IMvCameraSink
{
public:
	virtual ~IMvCameraSink() {}

public:
	virtual void onCameraDisconnect() = 0;
};

class CMvCamera
{
public:
	typedef std::vector<std::string> ParamVectorType;

public:
	CMvCamera(unsigned int index);
	~CMvCamera();

public:
	bool open(IMvCameraSink* pSink = NULL);

	bool isOpen();

	void close();

	bool getDeviceInfo(IMV_DeviceInfo& devInfo);

	bool startDisplay(void* pShowHandle, int iShowRate = 30);

	bool stopDisplay();

	IMV_HANDLE getCameraHandle();

	void procConnectArg(const IMV_SConnectArg* pConnectArg);

private:
	unsigned int					m_index;
	IMV_HANDLE						m_devHandle;
	CMvDisplayHelper*				m_pDisplayHelper;
	IMvCameraSink*					m_pMvCameraSink;
	int								m_nInterfaceType;
};

#endif // __MV_CAMERA_H__
