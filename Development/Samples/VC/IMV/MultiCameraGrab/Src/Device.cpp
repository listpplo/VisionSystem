#include "Device.h"
#include <process.h>
#include <Windows.h>

// ***********开始： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********BEGIN: These functions are not related to API call and used to display device info***********
static void displayDeviceInfo(IMV_DeviceList deviceInfoList)
{
	IMV_DeviceInfo*	pDevInfo = NULL;
	unsigned int cameraIndex = 0;
	char vendorNameCat[11];
	char cameraNameCat[STR_LEN];

	// 打印Title行 
	// Print title line 
	printf("\nIdx Type Vendor     Model      S/N             DeviceUserID    IP Address    \n");
	printf("------------------------------------------------------------------------------\n");

	for (cameraIndex = 0; cameraIndex < deviceInfoList.nDevNum; cameraIndex++)
	{
		pDevInfo = &deviceInfoList.pDevInfo[cameraIndex];
		// 设备列表的相机索引  最大表示字数：3
		// Camera index in device list, display in 3 characters 
		printf("%-3d", cameraIndex + 1);

		// 相机的设备类型（GigE，U3V，CL，PCIe）
		// Camera type 
		switch (pDevInfo->nCameraType)
		{
		case typeGigeCamera:printf(" GigE"); break;
		case typeU3vCamera:printf(" U3V "); break;
		case typeCLCamera:printf(" CL  "); break;
		case typePCIeCamera:printf(" PCIe"); break;
		default:printf("     "); break;
		}

		// 制造商信息  最大表示字数：10 
		// Camera vendor name, display in 10 characters 
		if (strlen(pDevInfo->vendorName) > 10)
		{
			memcpy(vendorNameCat, pDevInfo->vendorName, 7);
			vendorNameCat[7] = '\0';
			strcat(vendorNameCat, "...");
			printf(" %-10.10s", vendorNameCat);
		}
		else
		{
			printf(" %-10.10s", pDevInfo->vendorName);
		}

		// 相机的型号信息 最大表示字数：10 
		// Camera model name, display in 10 characters 
		printf(" %-10.10s", pDevInfo->modelName);

		// 相机的序列号 最大表示字数：15 
		// Camera serial number, display in 15 characters 
		printf(" %-15.15s", pDevInfo->serialNumber);

		// 自定义用户ID 最大表示字数：15 
		// Camera user id, display in 15 characters 
		if (strlen(pDevInfo->cameraName) > 15)
		{
			memcpy(cameraNameCat, pDevInfo->cameraName, 12);
			cameraNameCat[12] = '\0';
			strcat(cameraNameCat, "...");
			printf(" %-15.15s", cameraNameCat);
		}
		else
		{
			printf(" %-15.15s", pDevInfo->cameraName);
		}

		// GigE相机时获取IP地址 
		// IP address of GigE camera 
		if (pDevInfo->nCameraType == typeGigeCamera)
		{
			printf(" %s", pDevInfo->DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
		}

		printf("\n");
	}

	return;
}

static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	CameraDevice* camDevice = (CameraDevice*)pUser;
	camDevice->onGetFrameCallback(pFrame);
}

CameraDevice::CameraDevice()
{
	m_index = 0xff;
	m_Key = "";
	m_userId = "";
	camHandle = NULL;
}
CameraDevice::~CameraDevice()
{

}

void DeviceSystem::initSystem()
{
	printf("find device.\r\n");
	IMV_DeviceList deviceInfoList;
	memset(&deviceInfoList, 0, sizeof(deviceInfoList));
	int ret = IMV_EnumDevices(&deviceInfoList, interfaceTypeAll);
	if (IMV_OK != ret)
	{
		printf("IMV_EnumDevices failed. ret:%d\r\n", ret);
	}

	printf("find device finished. device num:%d.\r\n", deviceInfoList.nDevNum);

	if (deviceInfoList.nDevNum < 1)
	{
		printf("no camera\n");
		getchar();
	}

	m_DeviceNum = deviceInfoList.nDevNum;

	for (unsigned int j = 0; j < deviceInfoList.nDevNum; j++)
	{
		m_Device[j].init(j, deviceInfoList.pDevInfo[j]);
	}

	displayDeviceInfo(deviceInfoList);
}

void DeviceSystem::unInitSystem()
{
	memset(&m_Device, 0, sizeof(CameraDevice) * 16);
}

int CameraDevice::init(int index, IMV_DeviceInfo camInfo)
{
	m_index = index;
	m_Key = camInfo.cameraKey;
	m_userId = camInfo.cameraName;
	return IMV_OK;
}

int CameraDevice::openDevice()
{
	unsigned int index = 0;
	int ret = IMV_OK;
	unsigned int openCameraNum = 0;

	ret = IMV_CreateHandle(&camHandle, modeByIndex, (void*)&m_index);
	if (IMV_OK != ret)
	{
		printf("Create devHandle1 failed! index[%u], ErrorCode[%d]\n", index, ret);
	}

	ret = IMV_Open(camHandle);
	if (IMV_OK != ret)
	{
		printf("Open camera1 failed! cameraKey[%s], ErrorCode[%d]\n", m_Key, ret);

		// 销毁设备句柄
		// Destroy Device Handle
		IMV_DestroyHandle(camHandle);
	}

	return ret;
}

int CameraDevice::openDevicebyKey()
{
	int ret = IMV_OK;

	ret = IMV_CreateHandle(&camHandle, modeByCameraKey, (void*)m_Key.c_str());
	if (IMV_OK != ret)
	{
		printf("Create devHandle by CameraKey failed! Key is [%s], ErrorCode[%d]\n", m_Key.c_str(), ret);
	}

	ret = IMV_Open(camHandle);
	if (IMV_OK != ret)
	{
		printf("Open camera failed! cameraKey[%s], ErrorCode[%d]\n", m_Key.c_str(), ret);

		// 销毁设备句柄
		// Destroy Device Handle
		IMV_DestroyHandle(camHandle);
	}

	return ret;
}

int CameraDevice::openDevicebyUserId()
{
	int ret = IMV_OK;

	ret = IMV_CreateHandle(&camHandle, modeByDeviceUserID, (void*)m_userId.c_str());
	if (IMV_OK != ret)
	{
		printf("Create devHandle by device user id failed! User id is [%s], ErrorCode[%d]\n", m_userId.c_str(), ret);
	}

	ret = IMV_Open(camHandle);
	if (IMV_OK != ret)
	{
		printf("Open camera failed! User id is [%s] ErrorCode[%d]\n", m_userId.c_str(), ret);

		// 销毁设备句柄
		// Destroy Device Handle
		IMV_DestroyHandle(camHandle);
	}

	return ret;
}

int CameraDevice::closeDevice()
{
	int ret = IMV_OK;
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}

	ret = IMV_Close(camHandle);
	if (IMV_OK != ret)
	{
		printf("Close grabbing failed! EcameraKey[%s], ErrorCode[%d]\n", m_Key.c_str(), ret);
	}

	ret = IMV_DestroyHandle(camHandle);
	if (IMV_OK != ret)
	{
		printf("Destroy device Handle failed! EcameraKey[%s], ErrorCode[%d]\n", m_Key.c_str(), ret);
	}

	return ret;
}

int CameraDevice::setIntValue(const char* pFeatureName, int64_t intValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_SetIntFeatureValue(camHandle, pFeatureName, intValue);
}

int CameraDevice::getIntValue(const char* pFeatureName, int64_t* pIntValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_GetIntFeatureValue(camHandle, pFeatureName, pIntValue);
}

int CameraDevice::setBoolValue(const char* pFeatureName, bool boolValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_SetBoolFeatureValue(camHandle, pFeatureName, boolValue);
}

int CameraDevice::getBoolValue(const char* pFeatureName, bool* boolValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_GetBoolFeatureValue(camHandle, pFeatureName, boolValue);
}

int CameraDevice::setDoubleValue(const char* pFeatureName, double doubleValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_SetDoubleFeatureValue(camHandle, pFeatureName, doubleValue);
}

int CameraDevice::getDoubleValue(const char* pFeatureName, double* pDoubleValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_GetDoubleFeatureValue(camHandle, pFeatureName, pDoubleValue);
}

int CameraDevice::setStringValue(const char* pFeatureName, const char* pStringValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_SetStringFeatureValue(camHandle, pFeatureName, pStringValue);
}

int CameraDevice::getStringValue(const char* pFeatureName, IMV_String* pStringValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_GetStringFeatureValue(camHandle, pFeatureName, pStringValue);
}

int CameraDevice::setEnumSymbol(const char* pFeatureName, const char* pStringValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_SetEnumFeatureSymbol(camHandle, pFeatureName, pStringValue);
}

int CameraDevice::getEnumSymbol(const char* pFeatureName, IMV_String* pStringValue)
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	return IMV_GetEnumFeatureSymbol(camHandle, pFeatureName, pStringValue);
}

void CameraDevice::onGetFrameCallback(IMV_Frame* pFrame)
{
	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("[%p] CB Get frame blockId = %llu\n", camHandle, pFrame->frameInfo.blockId);
	return;
}

int CameraDevice::getFrameThreadProc()
{
	int ret = IMV_OK;
	IMV_Frame frame;

	if (NULL == camHandle)
	{
		return IMV_INVALID_HANDLE;
	}

	while (!m_isExitThread)
	{
		// 获取一帧图像
		// Get a frame image
		ret = IMV_GetFrame(camHandle, &frame, 500);
		if (IMV_OK != ret)
		{
			continue;
		}

		printf("[%p] Get frame blockId = %llu\n", camHandle, frame.frameInfo.blockId);

		// 释放图像缓存
		// Free image buffer
		ret = IMV_ReleaseFrame(camHandle, &frame);
		if (IMV_OK != ret)
		{
			printf("Release frame failed! ErrorCode[%d]\n", ret);
		}
	}
	return IMV_OK;
}

int CameraDevice::startGrabbing()
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	// 启动拉流线程 
	// Start grabbing thread 
	m_isExitThread = false;
	m_thread = new std::thread(&CameraDevice::getFrameThreadProc, this);
	printf("Start [%p] grabbing.\r\n", camHandle);
	return IMV_StartGrabbing(camHandle);
}

int CameraDevice::stopGrabbing()
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	m_isExitThread = true;
	m_thread->join();
	if (m_thread)
	{
		delete m_thread;
		m_thread = NULL;
	}

	printf("Stop [%p] grabbing.\r\n", camHandle);
	return IMV_StopGrabbing(camHandle);
}

int CameraDevice::stopGrabbingCallback()
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	printf("Stop [%p] CB grabbing.\r\n", camHandle);
	return IMV_StopGrabbing(camHandle);
}

int CameraDevice::startGrabbingCallback()
{
	if (!camHandle)
	{
		return IMV_INVALID_HANDLE;
	}
	int ret = IMV_AttachGrabbing(camHandle, onGetFrame, this);
	if (IMV_OK != ret)
	{
		printf("IMV_AttachGrabbing failed. ret:%d\r\n", ret);
	}

	printf("Start [%p] CB grabbing.\r\n", camHandle);
	return IMV_StartGrabbing(camHandle);
}
