/// \file
/// \~chinese
/// \brief 修改相机IP示例
/// \example ForceIp.cpp
/// \~english
/// \brief modify device IP sample
/// \example ForceIp.cpp

//**********************************************************************
// 本Demo为简单演示SDK的使用。                
// This Demo shows how to use GenICam API(C) to write a simple program.
//**********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVApi.h"

#define snprintf	_snprintf

// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("Get frame blockId = %llu\n", pFrame->frameInfo.blockId);

	return;
}

// ***********开始： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********BEGIN: These functions are not related to API call and used to display device info***********
static void displayDeviceInfo(IMV_DeviceList deviceInfoList)
{
	IMV_DeviceInfo*	pDevInfo = NULL;
	unsigned int cameraIndex = 0;
	char vendorNameCat[11];
	char cameraNameCat[16];

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
			case typeGigeCamera:printf(" GigE");break;
			case typeU3vCamera:printf(" U3V ");break;
			case typeCLCamera:printf(" CL  ");break;
			case typePCIeCamera:printf(" PCIe");break;
			default:printf("     ");break;
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

static char* trim(char* pStr)
{
	char* pDst = pStr;
	char* pTemStr = NULL;

	if (pDst != NULL)
	{
		pTemStr = pDst + strlen(pStr) - 1;
		while (*pDst == ' ')
		{
			pDst++;
		}
		while ((pTemStr > pDst) && (*pTemStr == ' '))
		{
			*pTemStr-- = '\0';
		}
	}

	return pDst;
}

static int isInputValid(char* pInpuStr)
{
	char numChar;
	char* pStr = pInpuStr;
	while (*pStr != '\0')
	{
		numChar = *pStr;
		if ((numChar > '9') || (numChar < '0'))
		{
			return -1;
		}
		pStr++;
	}

	return 0;
}

static unsigned int selectDevice(unsigned int cameraCnt)
{
	char inputStr[256];
	char* pTrimStr;
	int inputIndex = -1;
	int ret = -1;
	char* find = NULL;

	printf("\nPlease input the camera index: ");
	while (1)
	{
		memset(inputStr, 0, sizeof(inputStr));
		fgets(inputStr, sizeof(inputStr), stdin);
		// 清空输入缓存
		// clear flush
		fflush(stdin);

		// fgets比gets多吃一个换行符号，取出换行符号 
		// fgets eats one more line feed symbol than gets, and takes out the line feed symbol
		find = strchr(inputStr, '\n');
		if (find) { *find = '\0'; }

		pTrimStr = trim(inputStr);
		ret = isInputValid(pTrimStr);
		if (ret == 0)
		{
			inputIndex = atoi(pTrimStr);
			// 输入的序号从1开始 
			// Input index starts from 1 
			inputIndex -= 1;
			if ((inputIndex >= 0) && (inputIndex < (int)cameraCnt))
			{
				break;
			}
		}

		printf("Input invalid! Please input the camera index: ");
	}

	return (unsigned int)inputIndex;
}

static int autoSetCameraIP(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	IMV_DeviceInfo deviceInfo;
	char newIPStr[IMV_MAX_STRING_LENTH];
	char* subnetStr[5];
	int subnetStrIndex = 0;
	int ipValue = 253;

	// 获取设备信息
	// Get device information
	ret = IMV_GetDeviceInfo(devHandle, &deviceInfo);
	if (IMV_OK != ret)
	{
		printf("Get device info failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 判断设备和主机IP的网段是否匹配。匹配(Valid) 不匹配(Invalid On This Interface)
	// Check whether match of device and PC subnet.match(Valid) mismatch(Invalid On This Interface)
	if (0 == memcmp(deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipConfiguration, "Valid",
		strlen(deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipConfiguration)))
	{
		return IMV_OK;
	}

	printf("Device ip address (before):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
	printf("Device subnetMask (before):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.subnetMask);
	printf("Device gateway (before):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.defaultGateWay);
	printf("Device macAddress (before):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.macAddress);
	printf("\n");

	printf("Interface ip address:[%s]\n", deviceInfo.InterfaceInfo.gigeInterfaceInfo.ipAddress);
	printf("Interface subnetMask:[%s]\n", deviceInfo.InterfaceInfo.gigeInterfaceInfo.subnetMask);
	printf("Interface gateway:[%s]\n", deviceInfo.InterfaceInfo.gigeInterfaceInfo.defaultGateWay);
	printf("Interface macAddress:[%s]\n", deviceInfo.InterfaceInfo.gigeInterfaceInfo.macAddress);
	printf("\n");
	
	memset(newIPStr, 0, IMV_MAX_STRING_LENTH);
	subnetStr[subnetStrIndex] = strtok(deviceInfo.InterfaceInfo.gigeInterfaceInfo.ipAddress, ".");
	while (subnetStr[subnetStrIndex] != NULL)
	{
		subnetStrIndex++;
		subnetStr[subnetStrIndex] = strtok(NULL, ".");
	}

	while (ipValue)
	{
		if (ipValue != atoi(subnetStr[3]))
		{
			break;
		}
		ipValue--;
	}

	snprintf(newIPStr, sizeof(newIPStr), "%s.%s.%s.%d", subnetStr[0], subnetStr[1], subnetStr[2], ipValue);

	printf("New device ip address:[%s]\n", newIPStr);
	printf("\n");

	// 修改设备临时IP
	// Modify device Temporary IP
	ret = IMV_GIGE_ForceIpAddress(devHandle, (const char*)newIPStr,
		(const char*)deviceInfo.InterfaceInfo.gigeInterfaceInfo.subnetMask,
		(const char*)deviceInfo.InterfaceInfo.gigeInterfaceInfo.defaultGateWay);
	if (IMV_OK != ret)
	{
		printf("Set device ip failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 重新获取设备信息
	// Get device information again
	ret = IMV_GetDeviceInfo(devHandle, &deviceInfo);
	if (IMV_OK != ret)
	{
		printf("Get device info failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Device ip address (after):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
	printf("Device subnetMask (after):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.subnetMask);
	printf("Device gateway (after):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.defaultGateWay);
	printf("Device macAddress (after):[%s]\n", deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.macAddress);
	printf("\n");

	// 打开相机(返回错误码为IMV_ERROR_INVALID_IP，表示设备与主机网段不匹配)
	// Open camera( Return error code:IMV_ERROR_INVALID_IP,indicating that the device and PC subnet is mismatch)
	ret = IMV_Open(devHandle);
	if (IMV_OK != ret)
	{
		printf("Open camera failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置静态IP
	// Set persistent IP
	ret = IMV_SetBoolFeatureValue(devHandle, "GevCurrentIPConfigurationPersistentIP", true);
	if (IMV_OK != ret)
	{
		printf("Set GevCurrentIPConfigurationPersistentIP failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_SetStringFeatureValue(devHandle, "GevPersistentIPAddress", (const char*)deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.ipAddress);
	if (IMV_OK != ret)
	{
		printf("Set GevPersistentIPAddress failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_SetStringFeatureValue(devHandle, "GevPersistentSubnetMask", (const char*)deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.subnetMask);
	if (IMV_OK != ret)
	{
		printf("Set GevPersistentSubnetMask failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_SetStringFeatureValue(devHandle, "GevPersistentDefaultGateway", (const char*)deviceInfo.DeviceSpecificInfo.gigeDeviceInfo.defaultGateWay);
	if (IMV_OK != ret)
	{
		printf("Set GevPersistentDefaultGateway failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	return ret;
}

// ***********结束： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********END: These functions are not related to API call and used to display device info***********

int main()
{
	int ret = IMV_OK;
	unsigned int cameraIndex = 0;
	IMV_HANDLE devHandle = NULL;

	// 发现GigE相机
	// discover GigE camera 
	IMV_DeviceList deviceInfoList;
	ret = IMV_EnumDevices(&deviceInfoList, interfaceTypeGige);
	if (IMV_OK != ret)
	{
		printf("Enumeration GigE devices failed! ErrorCode[%d]\n", ret);
		getchar();
		return -1;
	}

	if (deviceInfoList.nDevNum < 1)
	{
		printf("no camera\n");
		getchar();
		return -1;
	}

	// 打印相机基本信息（序号,类型,制造商信息,型号,序列号,用户自定义ID,IP地址） 
	// Print camera info (Index, Type, Vendor, Model, Serial number, DeviceUserID, IP Address) 
	displayDeviceInfo(deviceInfoList);

	// 选择需要连接的相机 
	// Select one camera to connect to  
	cameraIndex = selectDevice(deviceInfoList.nDevNum);

	do
	{
		// 创建设备句柄
		// Create Device Handle
		ret = IMV_CreateHandle(&devHandle, modeByIndex, (void*)&cameraIndex);
		if (IMV_OK != ret)
		{
			printf("Create devHandle failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 检查和设置设备IP
		// Check and set the device IP
		ret = autoSetCameraIP(devHandle);
		if (IMV_OK != ret)
		{
			printf("Set camera IP failed!\n");
			break;
		}

		// 判断相机是否打开状态
		// Check whether device is open
		if (!IMV_IsOpen(devHandle))
		{
			// 打开相机(返回错误码为IMV_ERROR_INVALID_IP，表示设备与主机网段不匹配)
			// Open camera( Return error code:IMV_ERROR_INVALID_IP,indicating that the device and PC subnet is mismatch)
			ret = IMV_Open(devHandle);
			if (IMV_OK != ret)
			{
				printf("Open camera failed! ErrorCode[%d]\n", ret);
				break;
			}
		}

		// 注册数据帧回调函数
		// Register data frame callback function
		ret = IMV_AttachGrabbing(devHandle, onGetFrame, NULL);
		if (IMV_OK != ret)
		{
			printf("Attach grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 开始拉流 
		// Start grabbing 
		ret = IMV_StartGrabbing(devHandle);
		if (IMV_OK != ret)
		{
			printf("Start grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 取图2秒
		// get frame 2 seconds
		Sleep(2000);

		// 停止拉流 
		// Stop grabbing 
		ret = IMV_StopGrabbing(devHandle);
		if (IMV_OK != ret)
		{
			printf("Stop grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 关闭相机
		// Close camera 
		ret = IMV_Close(devHandle);
		if (IMV_OK != ret)
		{
			printf("Close camera failed! ErrorCode[%d]\n", ret);
			break;
		}
	} while (false);

	if (devHandle != NULL)
	{
		// 销毁设备句柄
		// Destroy Device Handle
		IMV_DestroyHandle(devHandle);
	}

	printf("Press enter key to exit...\n");
	getchar();

	return 0;
}
