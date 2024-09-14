#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <thread>
#include "IMVApi.h"
#include "IMVDefines.h"

#define STR_LEN 16
class CameraDevice
{
public:
	CameraDevice();
	~CameraDevice();
public:
	int openDevice();
	int openDevicebyKey();
	int openDevicebyUserId();
	int closeDevice();
	int setIntValue(const char* pFeatureName, int64_t intValue);
	int getIntValue(const char* pFeatureName, int64_t* pIntValue);
	int setBoolValue(const char* pFeatureName, bool boolValue);
	int getBoolValue(const char* pFeatureName, bool* boolValue);
	int setDoubleValue(const char* pFeatureName, double doubleValue);
	int getDoubleValue(const char* pFeatureName, double* pDoubleValue);
	int setStringValue(const char* pFeatureName, const char* pStringValue);
	int getStringValue(const char* pFeatureName, IMV_String* pStringValue);
	int setEnumSymbol(const char* pFeatureName, const char* pStringValue);
	int getEnumSymbol(const char* pFeatureName, IMV_String* pStringValue);

public:
	int init(int index, IMV_DeviceInfo camInfo);
	int startGrabbing();
	int startGrabbingCallback();
	int stopGrabbing();
	int stopGrabbingCallback();

public:
	int getFrameThreadProc();
	void onGetFrameCallback(IMV_Frame* pFrame);

public:
	IMV_HANDLE	        camHandle;
	int					m_index;
	std::string			m_Key;
	std::string			m_userId;
	bool				m_isExitThread;
	std::thread*		m_thread;
};

class DeviceSystem
{
public:
	void initSystem();
	void unInitSystem();

public:
	CameraDevice	    m_Device[16];
	int					m_DeviceNum;
};

#endif
