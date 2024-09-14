/// \file
/// \~chinese
/// \brief 消息事件示例
/// \example MsgEvents.cpp
/// \~english
/// \brief message events sample
/// \example MsgEvents.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVFGApi.h"
#include "IMVFGDefines.h"
#include <iostream>
#include <windows.h>
#include <process.h>

#define STR_LEN 16
#define CAPTURECARD 0
#define CAM 1
static SYSTEMTIME sys;
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
		if (strlen(interfaceInfoList.pInterfaceInfoList[index].interfaceName) > strLen - 1)
		{
			memcpy(strNameCat, interfaceInfoList.pInterfaceInfoList[index].interfaceName, strLen - 4);
			strNameCat[strLen - 4] = '\0';
			strcat(strNameCat, "...");
			printf(" %-15.15s", strNameCat);
		}
		else
		{
			printf(" %-15.15s", interfaceInfoList.pInterfaceInfoList[index].interfaceName);
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

static int setMessageEventConf(IMV_FG_IF_HANDLE ifHandle)
{
	int ret = IMV_FG_OK;
	ret = IMV_FG_SetBoolFeatureValue(ifHandle, "EventActive", true);
	if (IMV_FG_OK != ret)
	{
		printf("Set EventActive value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventSelector", "FrameStart");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventNotification", "On");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventNotification value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventSelector", "FrameEnd");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventNotification", "On");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventNotification value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventSelector", "FirstLine");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventNotification", "On");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventNotification value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventSelector", "LastLine");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_SetEnumFeatureSymbol(ifHandle, "EventNotification", "On");
	if (IMV_FG_OK != ret)
	{
		printf("Set EventNotification value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	return ret;
}

static unsigned int selectDevice(unsigned int interfaceCnt, unsigned int cameraCnt, int devType)
{
	char inputStr[256];
	char* pTrimStr;
	int inputIndex = -1;
	int ret = -1;
	char* find = NULL;

	if (CAPTURECARD == devType)
		printf("\nPlease input the interface index: ");
	else
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
			if (CAPTURECARD == devType)
			{
				if ((inputIndex >= 0) && (inputIndex < (int)interfaceCnt))
				{
					break;
				}
			}
			else
			{
				if ((inputIndex >= 0) && (inputIndex < (int)cameraCnt))
				{
					break;
				}
			}

		}

		if (CAPTURECARD == devType)
			printf("Input invalid! Please input the interface index: ");
		else
			printf("Input invalid! Please input the camera index: ");
	}

	return (unsigned int)inputIndex;
}

// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_FG_Frame* pFrame, void* pUser)
{
	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("Get frame blockId = %llu\n", pFrame->frameInfo.blockId);

	return;
}

// 消息通道事件回调函数
// Message channel event callback function
static void onMessageEventCallback(const IMV_FG_SMsgEventArg* pMsgChannelArg, void* pUser)
{
	GetLocalTime(&sys);
	//按照年/月/日 时/分/秒.毫秒 星期几格式打印 
	printf("onMessageChannelEvent %4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute,
		sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);

	unsigned int index = 0;
	const char* pEventIdStr = NULL;

	if (pMsgChannelArg == NULL)
	{
		printf("pMsgChannelArg is NULL\n");
		return;
	}

	switch (pMsgChannelArg->eventId)
	{
	case IMV_FG_MSG_EVENT_ID_EXPOSURE_END:pEventIdStr = "ExposureEnd"; break;
	case IMV_FG_MSG_EVENT_ID_FRAME_TRIGGER:pEventIdStr = "FrameTrigger"; break;
	case IMV_FG_MSG_EVENT_ID_FRAME_START:pEventIdStr = "FrameStart"; break;
	case IMV_FG_MSG_EVENT_ID_ACQ_START:pEventIdStr = "AcquisitionStart"; break;
	case IMV_FG_MSG_EVENT_ID_ACQ_TRIGGER:pEventIdStr = "AcquisitionTrigger"; break;
	case IMV_FG_MSG_EVENT_ID_DATA_READ_OUT:pEventIdStr = "ReadOut"; break;
	case IMV_FG_MSG_EVENT_ID_FRAME_END:pEventIdStr = "FRAME_END"; break;
	case IMV_FG_MSG_EVENT_ID_FRAMEACTIVE_START:pEventIdStr = "FRAMEACTIVE_START"; break;
	case IMV_FG_MSG_EVENT_ID_FRAMEACTIVE_END:pEventIdStr = "FRAMEACTIVE_END"; break;
	case IMV_FG_MSG_EVENT_ID_FIRST_LINE:pEventIdStr = "FIRST_LINE"; break;
	case IMV_FG_MSG_EVENT_ID_LAST_LINE:pEventIdStr = "LAST_LINE"; break;
	default:pEventIdStr = "Unknow"; break;
	}
	printf("eventId = [0x%x : %s]\n", pMsgChannelArg->eventId, pEventIdStr);
	printf("channelId = %u\n", pMsgChannelArg->channelId);
	printf("blockId = %llu\n", pMsgChannelArg->blockId);
	printf("timestamp = %llu\n", pMsgChannelArg->timeStamp);
	printf("pEventData = [%u]\n", *(ULONG*)pMsgChannelArg->pEventData);

	printf("\n");
	return;
}

int main()
{
	printf("SDK Version:%s.\r\n", IMV_FG_GetVersion());
	int nRet = IMV_FG_OK;
	IMV_FG_IF_HANDLE ifHandle = NULL;
	IMV_FG_DEV_HANDLE devHandle = NULL;
	IMV_FG_INTERFACE_INFO_LIST interfaceList;
	IMV_FG_DEVICE_INFO_LIST camListPtr;

	printf("Enum capture board interface info.\r\n");
	do
	{
		//枚举采集卡设备
		// Discover capture board device
		nRet = IMV_FG_EnumInterface(typeCLInterface, &interfaceList);
		if (IMV_FG_OK != nRet)
		{
			printf("Enumeration devices failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		if (interfaceList.nInterfaceNum == 0)
		{
			printf("No board device find. board list size:%d.\r\n", interfaceList.nInterfaceNum);
			break;
		}

		printf("Enum camera device.\r\n");
		//枚举相机设备
		// discover camera 
		nRet = IMV_FG_EnumDevices(typeCLInterface, &camListPtr);
		if (IMV_FG_OK != nRet)
		{
			printf("Enumeration camera devices failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		// 打印相机基本信息（序号,类型,制造商信息,型号,序列号,用户自定义ID)
		// Print camera info (Index, Type, Vendor, Model, Serial number, DeviceUserID) 
		displayDeviceInfo(interfaceList, camListPtr);

		// 选择需要连接的采集卡设备开流
		// Select one camera to connect to  
		unsigned int boardIndex = selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, CAPTURECARD);
		int cameraIndex = selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, CAM);

		// 打开采集卡
		// Open capture card
		nRet = IMV_FG_OpenInterface(boardIndex, &ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Open cameralink capture board device failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		// 设置消息事件配置
		// Set msg event config 
		nRet = setMessageEventConf(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("set MessageEvent Conf failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		printf("Subscribe msg event.\r\n");
		// 注册消息事件通道 
		// Subscribe msg channel 
		nRet = IMV_FG_SubscribeMsgChannelArg(ifHandle, onMessageEventCallback, NULL);
		if (IMV_FG_OK != nRet)
		{
			printf("Subscribe msg channel failed! ErrorCode[%d]\n", nRet);
			break;
		}

		// 注册数据帧回调函数
		// Register data frame callback function
		nRet = IMV_FG_AttachGrabbing(ifHandle, onGetFrame, NULL);
		if (IMV_FG_OK != nRet)
		{
			printf("Attach grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		printf("Start grabbing.\r\n");
		// 开始拉流 
		// Start grabbing 
		nRet = IMV_FG_StartGrabbing(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Start grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		printf("Grabbing... get frame 5 seconds.\r\n");
		// 取图5秒
		// get frame 5 seconds
		Sleep(5000);

		printf("Stop grabbing.\r\n");
		// 停止拉流 
		// Stop grabbing 
		nRet = IMV_FG_StopGrabbing(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Stop grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		printf("Close capture device.\r\n");
		if (NULL != ifHandle)
		{
			// 关闭采集卡
			// Close capture device 
			nRet = IMV_FG_CloseInterface(ifHandle);
			if (IMV_FG_OK != nRet)
			{
				printf("Close cameralink board failed! errorCode:[%d]\r\n", nRet);
				break;
			}
		}
	} while (false);

	printf("Press enter key to exit...\n");
	getchar();
	return 0;
}
