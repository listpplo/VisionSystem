#include "Device.h"
#include <process.h>
#include <Windows.h>

// ***********开始： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********BEGIN: These functions are not related to API call and used to display device info***********
static void displayDeviceInfo(IMV_FG_INTERFACE_INFO_LIST interfaceInfoList, IMV_FG_DEVICE_INFO_LIST deviceInfoList)
{
	IMV_FG_DEVICE_INFO*	pDevInfo = NULL;
	unsigned int cameraIndex = 0;
	char strNameCat[STR_LEN] = { 0 };

	// 打印Title行 
	// Print title line 
	printf("nIdx  Type     Vendor          Model           S/N             DeviceUserID                \n");
	printf("-------------------------------------------------------------------------------------------\n");

	for (unsigned int index = 0; index < interfaceInfoList.nInterfaceNum; ++index)
	{
		// 设备列表的相机索引  最大表示字数：3
		// Camera index in device list, display in 3 characters 
		printf("%-3d", index + 1);

		// 接口类型（GigE，U3V，CL，PCIe）
		// Interface type 
		switch (interfaceInfoList.pInterfaceInfoList[index].nInterfaceType)
		{
		case typeGigEInterface:printf(" GigE Card  "); break;
		case typeU3vInterface:printf(" U3V Card  "); break;
		case typeCLInterface:printf(" CL Card  "); break;
		case typeCXPInterface:printf(" CXP Card  "); break;
		default:printf("     "); break;
		}

		// 制造商信息  最大表示字数：15 
		// Camera vendor name, display in 15 characters
		size_t strLen = STR_LEN;
		if (strlen(interfaceInfoList.pInterfaceInfoList[index].vendorName) > strLen - 4)
		{
			memcpy(strNameCat, interfaceInfoList.pInterfaceInfoList[index].vendorName, strLen - 4);
			strNameCat[strLen - 4] = '\0';
			strcat(strNameCat, "...");
			printf(" %-15.15s", strNameCat);
		}
		else
		{
			printf(" %-15.15s", interfaceInfoList.pInterfaceInfoList[index].vendorName);
		}

		// 相机的型号信息 最大表示字数：15 
		// Camera model name, display in 15 characters 
		printf(" %-15.15s", interfaceInfoList.pInterfaceInfoList[index].modelName);

		// 相机的序列号 最大表示字数：15 
		// Camera serial number, display in 15 characters 
		printf(" %-15.15s", interfaceInfoList.pInterfaceInfoList[index].serialNumber);

		// 自定义用户ID 最大表示字数：15 
		// Camera user id, display in 15 characters 
		if (strlen(interfaceInfoList.pInterfaceInfoList[index].interfaceKey) > strLen - 1)
		{
			memcpy(strNameCat, interfaceInfoList.pInterfaceInfoList[index].interfaceKey, strLen - 4);
			strNameCat[strLen - 4] = '\0';
			strcat(strNameCat, "...");
			printf(" %-15.15s", strNameCat);
		}
		else
		{
			printf(" %-15.15s", interfaceInfoList.pInterfaceInfoList[index].interfaceKey);
		}
		printf("\n");
		printf("    |--");

		bool isFind = false;
		for (unsigned int camIndex = 0; camIndex < deviceInfoList.nDevNum; ++camIndex)
		{
			cameraIndex = camIndex;
			pDevInfo = &deviceInfoList.pDeviceInfoList[cameraIndex];

			if (strcmp(pDevInfo->FGInterfaceInfo.interfaceKey, interfaceInfoList.pInterfaceInfoList[index].interfaceKey) == 0)
			{
				printf("%-7d", camIndex + 1);

				// 制造商信息  最大表示字数：15 
				// Camera vendor name, display in 15 characters 
				if (strlen(pDevInfo->vendorName) > strLen - 4)
				{
					memcpy(strNameCat, pDevInfo->vendorName, strLen - 4);
					strNameCat[strLen - 4] = '\0';
					strcat(strNameCat, "...");
					printf(" %-15.15s", strNameCat);
				}
				else
				{
					printf(" %-15.15s", pDevInfo->vendorName);
				}

				// 相机的型号信息 最大表示字数：15 
				// Camera model name, display in 15 characters 
				printf(" %-15.15s", pDevInfo->modelName);

				// 相机的序列号 最大表示字数：15 
				// Camera serial number, display in 15 characters 
				printf(" %-15.15s", pDevInfo->serialNumber);

				// 自定义用户ID 最大表示字数：15 
				// Camera user id, display in 15 characters 
				if (strlen(pDevInfo->cameraName) > strLen - 1)
				{
					memcpy(strNameCat, pDevInfo->cameraName, strLen - 4);
					strNameCat[strLen - 4] = '\0';
					strcat(strNameCat, "...");
					printf(" %-15.15s", strNameCat);
				}
				else
				{
					printf(" %-15.15s", pDevInfo->cameraName);
				}
				isFind = true;
				printf("\n");
				break;
			}
		}
		if (!isFind)
			printf("\n");

	}

	return;
}

static void onGetFrame(IMV_FG_Frame* pFrame, void* pUser)
{
	CaptureCardDevice* cardDevice = (CaptureCardDevice*)pUser;
	cardDevice->onGetFrameCallback(pFrame);
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
	printf("find interface.\r\n");
	IMV_FG_INTERFACE_INFO_LIST interfaceList;
	memset(&interfaceList, 0, sizeof(interfaceList));
	int ret = IMV_FG_EnumInterface(typeCLInterface, &interfaceList);
	if (IMV_FG_OK != ret)
	{
		printf("IMV_FG_EnumInterface failed. ret:%d\r\n", ret);
	}

	printf("find interface finished. interface num:%d.\r\n", interfaceList.nInterfaceNum);

	printf("find camera.\r\n");
	IMV_FG_DEVICE_INFO_LIST camList;
	memset(&camList, 0, sizeof(camList));
	ret = IMV_FG_EnumDevices(typeCLInterface, &camList);
	if (IMV_FG_OK != ret)
	{
		printf("IMV_FG_EnumDevices failed. ret:%d\r\n", ret);
	}
	printf("find camera finished. camera num:%d.\r\n", camList.nDevNum);

	m_cardDeviceNum = interfaceList.nInterfaceNum;

	for (unsigned int i = 0; i < interfaceList.nInterfaceNum; i++)
	{
		m_cardDevice[i].init(i, interfaceList.pInterfaceInfoList[i]);

		for (unsigned int j = 0; j < camList.nDevNum; j++)
		{
			if (strcmp(interfaceList.pInterfaceInfoList[i].interfaceKey, camList.pDeviceInfoList[j].FGInterfaceInfo.interfaceKey) == 0)
			{
				m_cardDevice[i].clCamera.init(j, camList.pDeviceInfoList[j]);
			}
		}
	}

	displayDeviceInfo(interfaceList, camList);
}

void DeviceSystem::unInitSystem()
{
	memset(&m_cardDevice, 0, sizeof(CaptureCardDevice) * 16);
}

int CaptureCardDevice::init(int index, IMV_FG_INTERFACE_INFO interfaceInfo)
{
	m_index = index;
	m_Key = interfaceInfo.interfaceKey;
	m_userId = interfaceInfo.interfaceName;
	return IMV_FG_OK;
}

CaptureCardDevice::CaptureCardDevice()
{
	m_index = 0xff;
	m_Key = "";
	m_userId = "";
	ifHandle = NULL;

}
CaptureCardDevice::~CaptureCardDevice()
{

}

int CaptureCardDevice::openDevice()
{
	return IMV_FG_OpenInterface(m_index, &ifHandle);
}

int CaptureCardDevice::openDevicebyKey()
{
	return IMV_FG_OpenInterfaceEx(IMV_FG_MODE_BY_CAMERAKEY, (void*)m_Key.c_str(), &ifHandle);
}

int CaptureCardDevice::openDevicebyUserId()
{
	return IMV_FG_OpenInterfaceEx(IMV_FG_MODE_BY_DEVICE_USERID, (void*)m_userId.c_str(), &ifHandle);
}

int CaptureCardDevice::closeDevice()
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_CloseInterface(ifHandle);
}

int CaptureCardDevice::setIntValue(const char* pFeatureName, int64_t intValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetIntFeatureValue(ifHandle, pFeatureName, intValue);
}

int CaptureCardDevice::getIntValue(const char* pFeatureName, int64_t* pIntValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetIntFeatureValue(ifHandle, pFeatureName, pIntValue);
}

int CaptureCardDevice::setBoolValue(const char* pFeatureName, bool boolValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetIntFeatureValue(ifHandle, pFeatureName, boolValue);
}

int CaptureCardDevice::getBoolValue(const char* pFeatureName, bool* boolValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetBoolFeatureValue(ifHandle, pFeatureName, boolValue);
}

int CaptureCardDevice::setDoubleValue(const char* pFeatureName, double doubleValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetDoubleFeatureValue(ifHandle, pFeatureName, doubleValue);
}

int CaptureCardDevice::getDoubleValue(const char* pFeatureName, double* pDoubleValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetDoubleFeatureValue(ifHandle, pFeatureName, pDoubleValue);
}

int CaptureCardDevice::setStringValue(const char* pFeatureName, const char* pStringValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetStringFeatureValue(ifHandle, pFeatureName, pStringValue);
}

int CaptureCardDevice::getStringValue(const char* pFeatureName, IMV_FG_String* pStringValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetStringFeatureValue(ifHandle, pFeatureName, pStringValue);
}

int CaptureCardDevice::setEnumSymbol(const char* pFeatureName, const char* pStringValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetEnumFeatureSymbol(ifHandle, pFeatureName, pStringValue);
}

int CaptureCardDevice::getEnumSymbol(const char* pFeatureName, IMV_FG_String* pStringValue)
{
	if (!ifHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetEnumFeatureSymbol(ifHandle, pFeatureName, pStringValue);
}

void CaptureCardDevice::onGetFrameCallback(IMV_FG_Frame* pFrame)
{
	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("[%s] CB Get frame blockId = %llu\n", m_Key.c_str(), pFrame->frameInfo.blockId);
	return;
}

int CaptureCardDevice::getFrameThreadProc()
{
	int ret = IMV_FG_OK;
	IMV_FG_Frame frame;

	if (NULL == ifHandle)
	{
		return IMV_FG_NO_DATA;
	}

	while (!m_isExitThread)
	{
		// 获取一帧图像
		// Get a frame image
		ret = IMV_FG_GetFrame(ifHandle, &frame, 500);
		if (IMV_FG_OK != ret)
		{
			//printf("[0x%x] Get frame failed! ErrorCode[%d]\n", this, ret);
			continue;
		}

		printf("[%s] Get frame blockId = %llu\n", m_Key.c_str(), frame.frameInfo.blockId);

		// 释放图像缓存
		// Free image buffer
		ret = IMV_FG_ReleaseFrame(ifHandle, &frame);
		if (IMV_FG_OK != ret)
		{
			printf("Release frame failed! ErrorCode[%d]\n", ret);
		}
	}
	return IMV_FG_OK;
}

int CaptureCardDevice::startGrabbing()
{
	// 启动拉流线程 
	// Start grabbing thread 
	m_isExitThread = false;
	m_thread = new std::thread(&CaptureCardDevice::getFrameThreadProc, this);
	printf("Start [%p] grabbing.\r\n", ifHandle);
	return IMV_FG_StartGrabbing(ifHandle);
}

int CaptureCardDevice::stopGrabbing()
{
	m_isExitThread = true;
	m_thread->join();
	if (m_thread)
	{
		delete m_thread;
		m_thread = NULL;
	}

	printf("Stop [%p] grabbing.\r\n", ifHandle);
	return IMV_FG_StopGrabbing(ifHandle);
}

int CaptureCardDevice::stopGrabbingCallback()
{
	printf("Stop [%p] CB grabbing.\r\n", ifHandle);
	return IMV_FG_StopGrabbing(ifHandle);
}

int CaptureCardDevice::startGrabbingCallback()
{
	int ret = IMV_FG_AttachGrabbing(ifHandle, onGetFrame, this);
	if (IMV_FG_OK != ret)
	{
		printf("IMV_FG_AttachGrabbing failed. ret:%d\r\n", ret);
	}

	printf("Start [%p] CB grabbing.\r\n", ifHandle);
	return IMV_FG_StartGrabbing(ifHandle);
}

int CameraDevice::init(int index, IMV_FG_DEVICE_INFO camInfo)
{
	m_index = index;
	m_Key = camInfo.cameraKey;
	m_userId = camInfo.cameraName;
	return IMV_FG_OK;
}

int CameraDevice::openDevice()
{
	return IMV_FG_OpenDevice(IMV_FG_MODE_BY_INDEX, &m_index, &camHandle);
}

int CameraDevice::openDevicebyKey()
{
	return IMV_FG_OpenDevice(IMV_FG_MODE_BY_CAMERAKEY, (void*)m_Key.c_str(), &camHandle);
}

int CameraDevice::openDevicebyUserId()
{
	return IMV_FG_OpenDevice(IMV_FG_MODE_BY_DEVICE_USERID, (void*)m_userId.c_str(), &camHandle);
}

int CameraDevice::closeDevice()
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_CloseDevice(camHandle);
}

int CameraDevice::setIntValue(const char* pFeatureName, int64_t intValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetIntFeatureValue(camHandle, pFeatureName, intValue);
}

int CameraDevice::getIntValue(const char* pFeatureName, int64_t* pIntValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetIntFeatureValue(camHandle, pFeatureName, pIntValue);
}

int CameraDevice::setBoolValue(const char* pFeatureName, bool boolValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetIntFeatureValue(camHandle, pFeatureName, boolValue);
}

int CameraDevice::getBoolValue(const char* pFeatureName, bool* boolValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetBoolFeatureValue(camHandle, pFeatureName, boolValue);
}

int CameraDevice::setDoubleValue(const char* pFeatureName, double doubleValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetDoubleFeatureValue(camHandle, pFeatureName, doubleValue);
}

int CameraDevice::getDoubleValue(const char* pFeatureName, double* pDoubleValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetDoubleFeatureValue(camHandle, pFeatureName, pDoubleValue);
}

int CameraDevice::setStringValue(const char* pFeatureName, const char* pStringValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetStringFeatureValue(camHandle, pFeatureName, pStringValue);
}

int CameraDevice::getStringValue(const char* pFeatureName, IMV_FG_String* pStringValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetStringFeatureValue(camHandle, pFeatureName, pStringValue);
}

int CameraDevice::setEnumSymbol(const char* pFeatureName, const char* pStringValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_SetEnumFeatureSymbol(camHandle, pFeatureName, pStringValue);
}

int CameraDevice::getEnumSymbol(const char* pFeatureName, IMV_FG_String* pStringValue)
{
	if (!camHandle)
	{
		return IMV_FG_INVALID_HANDLE;
	}

	return IMV_FG_GetEnumFeatureSymbol(camHandle, pFeatureName, pStringValue);
}
