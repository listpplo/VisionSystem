/// \file
/// \~chinese
/// \brief 事件处理示例
/// \example Events.cpp
/// \~english
/// \brief Events sample
/// \example Events.cpp

//**********************************************************************
// 本Demo为简单演示SDK的使用，没有附加修改相机IP的代码，在运行之前，请使
// 用相机客户端修改相机IP地址的网段与主机的网段一致。                 
// This Demo shows how to use GenICam API(C) to write a simple program.
// Please make sure that the camera and PC are in the same subnet before running the demo.
// If not, you can use camera client software to modify the IP address of camera to the same subnet with PC. 
//**********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVApi.h"

// 参数更新事件回调函数
// Updating parameters event callback function
static void onParameterUpdateEvent(const IMV_SParamUpdateArg* pParamUpdateArg, void* pUser)
{
	unsigned int index = 0;

	if (pParamUpdateArg == NULL)
	{
		printf("pParamUpdateArg is NULL\n");
		return;
	}

	printf("isPoll = %d\n", pParamUpdateArg->isPoll);

	for (index = 0; index < pParamUpdateArg->nParamCnt; index++)
	{
		printf("paramName = %s\n", pParamUpdateArg->pParamNameList[index].str);
	}

	printf("\n");
	return;
}

// 消息通道事件回调函数
// Message channel event callback function
static void onMessageChannelEvent(const IMV_SMsgChannelArg* pMsgChannelArg, void* pUser)
{
	unsigned int index = 0;
	const char* pEventIdStr = NULL;

	if (pMsgChannelArg == NULL)
	{
		printf("pMsgChannelArg is NULL\n");
		return;
	}

	switch (pMsgChannelArg->eventId)
	{
		case IMV_MSG_EVENT_ID_EXPOSURE_END:pEventIdStr = "ExposureEnd"; break;
		case IMV_MSG_EVENT_ID_FRAME_TRIGGER:pEventIdStr = "FrameTrigger"; break;
		case IMV_MSG_EVENT_ID_FRAME_START:pEventIdStr = "FrameStart"; break;
		case IMV_MSG_EVENT_ID_ACQ_START:pEventIdStr = "AcquisitionStart"; break;
		case IMV_MSG_EVENT_ID_ACQ_TRIGGER:pEventIdStr = "AcquisitionTrigger"; break;
		case IMV_MSG_EVENT_ID_DATA_READ_OUT:pEventIdStr = "ReadOut"; break;
		default:pEventIdStr = "Unknow"; break;
	}
	printf("eventId = [0x%x : %s]\n", pMsgChannelArg->eventId, pEventIdStr);
	printf("channelId = %u\n", pMsgChannelArg->channelId);
	printf("blockId = %llu\n", pMsgChannelArg->blockId);
	printf("timestamp = %llu\n", pMsgChannelArg->timeStamp);

	for (index = 0; index < pMsgChannelArg->nParamCnt; index++)
	{
		printf("paramName = %s\n", pMsgChannelArg->pParamNameList[index].str);
	}

	printf("\n");
	return;
}

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

static int setMessageEventConf(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	IMV_DeviceInfo deviceInfo;

	// 获取设备信息
	// Get device information
	ret = IMV_GetDeviceInfo(devHandle, &deviceInfo);
	if (IMV_OK != ret)
	{
		printf("Get device info failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// USB设备没有消息事件
	// USB device has nomessage event
	if (typeGigeCamera == deviceInfo.nCameraType)
	{
		// 设置EventSelector
		// Set EventSelector
		ret = IMV_SetEnumFeatureSymbol(devHandle, "EventSelector", "FrameStart");
		if (IMV_OK != ret)
		{
			printf("Set EventSelector value failed! ErrorCode[%d]\n", ret);
			return ret;
		}

		// 设置EventNotification
		// Set EventNotification
		ret = IMV_SetEnumFeatureSymbol(devHandle, "EventNotification", "On");
		if (IMV_OK != ret)
		{
			printf("Set EventNotification value failed! ErrorCode[%d]\n", ret);
			return ret;
		}

		// 设置EventSelector
		// Set EventSelector
		ret = IMV_SetEnumFeatureSymbol(devHandle, "EventSelector", "ReadOut");
		if (IMV_OK != ret)
		{
			printf("Set EventSelector value failed! ErrorCode[%d]\n", ret);
			return ret;
		}

		// 设置EventNotification
		// Set EventNotification
		ret = IMV_SetEnumFeatureSymbol(devHandle, "EventNotification", "On");
		if (IMV_OK != ret)
		{
			printf("Set EventNotification value failed! ErrorCode[%d]\n", ret);
			return ret;
		}
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

	// 发现设备 
	// discover camera 
	IMV_DeviceList deviceInfoList;
	ret = IMV_EnumDevices(&deviceInfoList, interfaceTypeAll);
	if (IMV_OK != ret)
	{
		printf("Enumeration devices failed! ErrorCode[%d]\n", ret);
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

		// 打开相机 
		// Open camera 
		ret = IMV_Open(devHandle);
		if (IMV_OK != ret)
		{
			printf("Open camera failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 设置消息事件
		// Set message event
		ret = setMessageEventConf(devHandle);
		if (IMV_OK != ret)
		{
			break;
		}

		// 消息事件回调注册
		// Register message channel event callback function
		ret = IMV_SubscribeMsgChannelArg(devHandle, onMessageChannelEvent, NULL);
		if (IMV_OK != ret)
		{
			printf("Register message channel event failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 参数更新回调注册
		// Register parameters update event callback function
		ret = IMV_SubscribeParamUpdateArg(devHandle, onParameterUpdateEvent, NULL);
		if (IMV_OK != ret)
		{
			printf("Register parameters update event failed! ErrorCode[%d]\n", ret);
			break;
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
