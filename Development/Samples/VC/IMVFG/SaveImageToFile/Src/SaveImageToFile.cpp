/// \file
/// \~chinese
/// \brief 保存图像示例
/// \example SaveImageToFile.cpp
/// \~english
/// \brief Save image sample
/// \example SaveImageToFile.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVFGApi.h"
#include "IMVFGDefines.h"


#define snprintf	_snprintf

#define STR_LEN 16
#define INTERFACE 0
#define CAM 1
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
		int strLen = STR_LEN;
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

static unsigned int selectDevice(unsigned int interfaceCnt, unsigned int cameraCnt, int devType)
{
	char inputStr[256];
	char* pTrimStr;
	int inputIndex = -1;
	int nRet = -1;
	char* find = NULL;

	if (INTERFACE == devType)
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
		nRet = isInputValid(pTrimStr);
		if (nRet == 0)
		{
			inputIndex = atoi(pTrimStr);
			// 输入的序号从1开始 
			// Input index starts from 1 
			inputIndex -= 1;
			if (INTERFACE == devType)
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

		if (INTERFACE == devType)
			printf("Input invalid! Please input the interface index: ");
		else
			printf("Input invalid! Please input the camera index: ");
	}

	return (unsigned int)inputIndex;
}

// 选择图像保存格式
// Select target format for image save 
static IMV_FG_ESaveFileType selectSaveFormat(void)
{
	char inputStr[256];
	char* pTrimStr;
	int inputIndex = -1;
	int nRet = -1;
	int convertFormatyCnt = 4;
	IMV_FG_ESaveFileType saveFormat = IMV_FG_TYPE_SAVE_BMP;
	char* find = NULL;

	printf("\n--------------------------------------------\n");
	printf("\t0.Save to BMP\n");
	printf("\t1.Save to Jpeg\n");
	printf("\t2.Save to Png\n");
	printf("\t3.Save to Tif\n");
	printf("--------------------------------------------\n");

	printf("\nPlease select the save format index: ");
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
		nRet = isInputValid(pTrimStr);
		if (nRet == 0)
		{
			inputIndex = atoi(pTrimStr);
			if ((inputIndex >= 0) && (inputIndex < (int)convertFormatyCnt))
			{
				break;
			}
		}

		printf("Input invalid! Please select the save format index: ");
	}

	switch (inputIndex)
	{
	case 0: saveFormat = IMV_FG_TYPE_SAVE_BMP; break;
	case 1: saveFormat = IMV_FG_TYPE_SAVE_JPEG; break;
	case 2: saveFormat = IMV_FG_TYPE_SAVE_PNG; break;
	case 3: saveFormat = IMV_FG_TYPE_SAVE_TIFF; break;
	default: saveFormat = IMV_FG_TYPE_SAVE_BMP; break;
	}

	return saveFormat;
}

int main()
{
	printf("SDK Version:%s.\r\n", IMV_FG_GetVersion());

	int nRet = IMV_FG_OK;
	IMV_FG_IF_HANDLE ifHandle = NULL;
	IMV_FG_DEV_HANDLE devHandle = NULL;
	IMV_FG_INTERFACE_INFO_LIST interfaceList;
	IMV_FG_DEVICE_INFO_LIST camListPtr;
	IMV_FG_Frame frame;
	IMV_FG_ESaveFileType saveFormat = IMV_FG_TYPE_SAVE_BMP;
	IMV_FG_SaveImageToFileParam saveImageToFileParam;
	memset(&saveImageToFileParam, 0, sizeof(saveImageToFileParam));
	CRITICAL_SECTION saveImagelock;
	InitializeCriticalSection(&saveImagelock);
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
		unsigned int boardIndex = selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, INTERFACE);
		int cameraIndex = selectDevice(interfaceList.nInterfaceNum, camListPtr.nDevNum, CAM);

		printf("Open capture device.\r\n");
		// 打开采集卡设备
		// Open capture device 
		nRet = IMV_FG_OpenInterface(boardIndex, &ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Open cameralink capture board device failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		printf("Open camera device.\r\n");
		// 打开采集卡相机设备 
		// Connect to camera 
		nRet = IMV_FG_OpenDevice(IMV_FG_MODE_BY_INDEX, (void*)&cameraIndex, &devHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Open camera failed! ErrorCode[%d]\n", nRet);
			break;
		}

		// 选择图像保存目标格式
		// Select target format for image save 
		saveFormat = selectSaveFormat();

		// 关闭触发模式
		// Set TriggerMode off
		nRet = IMV_FG_SetEnumFeatureSymbol(devHandle, "TriggerMode", "Off");
		if (IMV_FG_OK != nRet)
		{
			printf("Set TriggerMode failed! ErrorCode[%d]\n", nRet);
			break;
		}

		// 开始拉流 
		// Start grabbing 
		nRet = IMV_FG_StartGrabbing(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Start grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		// 获取一帧图像
		// Get a frame image
		nRet = IMV_FG_GetFrame(ifHandle, &frame, 1000);
		if (IMV_FG_OK != nRet)
		{
			printf("Get frame failed! ErrorCode[%d]\n", nRet);
			break;
		}

		EnterCriticalSection(&saveImagelock);
		saveImageToFileParam.eImageType = saveFormat;
		saveImageToFileParam.nWidth = frame.frameInfo.width;
		saveImageToFileParam.nHeight = frame.frameInfo.height;
		saveImageToFileParam.ePixelFormat = frame.frameInfo.pixelFormat;
		saveImageToFileParam.pSrcData = frame.pData;
		saveImageToFileParam.nSrcDataLen = frame.frameInfo.size;
		saveImageToFileParam.eBayerDemosaic = IMV_FG_EBayerDemosaic::IMV_FG_DEMOSAIC_EDGE_SENSING;
		saveImageToFileParam.pImagePath = (char*)malloc(256);

		memset(saveImageToFileParam.pImagePath, 0, 256);

		if (IMV_FG_TYPE_SAVE_BMP == saveImageToFileParam.eImageType)
		{
			sprintf_s(saveImageToFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03lu.bmp", saveImageToFileParam.nWidth, saveImageToFileParam.nHeight, frame.frameInfo.blockId);
		}
		else if (IMV_FG_TYPE_SAVE_JPEG == saveImageToFileParam.eImageType)
		{
			saveImageToFileParam.nQuality = 90;
			sprintf_s(saveImageToFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03lu.jpg", saveImageToFileParam.nWidth, saveImageToFileParam.nHeight, frame.frameInfo.blockId);
		}
		else if (IMV_FG_TYPE_SAVE_PNG == saveImageToFileParam.eImageType)
		{
			saveImageToFileParam.nQuality = 8;
			sprintf_s(saveImageToFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03lu.png", saveImageToFileParam.nWidth, saveImageToFileParam.nHeight, frame.frameInfo.blockId);
		}
		else if (IMV_FG_TYPE_SAVE_TIFF == saveImageToFileParam.eImageType)
		{
			sprintf_s(saveImageToFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03lu.tif", saveImageToFileParam.nWidth, saveImageToFileParam.nHeight, frame.frameInfo.blockId);
		}

		// 保存图片
		// Save image
		nRet = IMV_FG_SaveImageToFile(ifHandle, &saveImageToFileParam);
		if (IMV_FG_OK != nRet)
		{
			printf("Save image failed! ErrorCode[%d]\n", nRet);
			LeaveCriticalSection(&saveImagelock);
			break;
		}

		LeaveCriticalSection(&saveImagelock);

		// 释放图像缓存
		// Free image buffer
		nRet = IMV_FG_ReleaseFrame(ifHandle, &frame);
		if (IMV_FG_OK != nRet)
		{
			printf("Release frame failed! ErrorCode[%d]\n", nRet);
			break;
		}

		printf("Stop grabbing.\r\n");
		// 停止拉流 
		// Stop grabbing 
		nRet = IMV_FG_StopGrabbing(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Stop grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}
	} while (false);

	printf("Close camera device.\r\n");
	if (NULL != devHandle)
	{
		//关闭相机
		//Close camera 
		nRet = IMV_FG_CloseDevice(devHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Close camera failed! errorCode:[%d]\r\n", nRet);
		}
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
		}
	}

	DeleteCriticalSection(&saveImagelock);
	printf("Press enter key to exit...\n");
	getchar();
	return 0;
}
