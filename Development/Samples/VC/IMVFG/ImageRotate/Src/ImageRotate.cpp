﻿/// \file
/// \~chinese
/// \brief 图像旋转示例
/// \example ImageRotate.cpp
/// \~english
/// \brief Image Rotate sample
/// \example ImageRotate.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVFGApi.h"
#include "IMVFGDefines.h"

#define	MONO_CHANNEL_NUM	1
#define	RGB_CHANNEL_NUM		3
#define	BGR_CHANNEL_NUM		3

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

#define STR_LEN 16
#define CAPTURECARD 0
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

// 选择图像旋转角度
// Select image rotate angle
static IMV_FG_ERotationAngle selectRotationAngle(void)
{
	char inputStr[256];
	char* pTrimStr;
	int inputIndex = -1;
	int ret = -1;
	int rotationAngleCnt = 3;
	char* find = NULL;

	printf("\n--------------------------------------------\n");
	printf("\t0.Image rotation 90 degree angle\n");
	printf("\t1.Image rotation 180 degree angle\n");
	printf("\t2.Image rotation 270 degree angle\n");
	printf("--------------------------------------------\n");

	printf("\nPlease select the rotation angle index: ");
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
			if ((inputIndex >= 0) && (inputIndex < (int)rotationAngleCnt))
			{
				break;
			}
		}

		printf("Input invalid! Please select the rotation angle index: ");
	}

	return (IMV_FG_ERotationAngle)inputIndex;
}

// 图片旋转
// Image rotate
void imageRotate(IMV_FG_IF_HANDLE devHandle, IMV_FG_Frame frame, IMV_FG_ERotationAngle rotationAngle)
{
	IMV_FG_PixelConvertParam	stPixelConvertParam;
	IMV_FG_RotateImageParam	stRotateImageParam;
	unsigned char*			pConvertBuf = NULL;
	unsigned int			nConvertBufSize = 0;
	unsigned char*			pRotateBuf = NULL;
	unsigned int			nRotateBufSize = 0;
	unsigned int			nChannelNum = 0;
	int						ret = IMV_FG_OK;
	FILE*					hFile = NULL;

	memset(&stRotateImageParam, 0, sizeof(stRotateImageParam));

	if (IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8 == frame.frameInfo.pixelFormat)
	{
		stRotateImageParam.pSrcData = frame.pData;
		stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * MONO_CHANNEL_NUM;
		stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
		nChannelNum = MONO_CHANNEL_NUM;
	}
	else if (IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8 == frame.frameInfo.pixelFormat)
	{
		stRotateImageParam.pSrcData = frame.pData;
		stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM;
		stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
		nChannelNum = BGR_CHANNEL_NUM;
	}
	else if (IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_RGB8 == frame.frameInfo.pixelFormat)
	{
		stRotateImageParam.pSrcData = frame.pData;
		stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * RGB_CHANNEL_NUM;
		stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
		nChannelNum = RGB_CHANNEL_NUM;
	}
	// MONO8/RGB24/BGR24以外的格式都转化成BGR24
	// All formats except MONO8/RGB24/BGR24 are converted to BGR24
	else
	{
		nConvertBufSize = sizeof(unsigned char) * frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM;
		pConvertBuf = (unsigned char*)malloc(nConvertBufSize);
		if (NULL == pConvertBuf)
		{
			printf("malloc pConvertBuf failed!\n");
			return;
		}

		// 图像转换成BGR8
		// convert image to BGR8
		memset(&stPixelConvertParam, 0, sizeof(stPixelConvertParam));
		stPixelConvertParam.nWidth = frame.frameInfo.width;
		stPixelConvertParam.nHeight = frame.frameInfo.height;
		stPixelConvertParam.ePixelFormat = frame.frameInfo.pixelFormat;
		stPixelConvertParam.pSrcData = frame.pData;
		stPixelConvertParam.nSrcDataLen = frame.frameInfo.size;
		stPixelConvertParam.nPaddingX = frame.frameInfo.paddingX;
		stPixelConvertParam.nPaddingY = frame.frameInfo.paddingY;
		stPixelConvertParam.eBayerDemosaic = IMV_FG_DEMOSAIC_BILINEAR;
		stPixelConvertParam.eDstPixelFormat = IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8;
		stPixelConvertParam.pDstBuf = pConvertBuf;
		stPixelConvertParam.nDstBufSize = nConvertBufSize;

		ret = IMV_FG_PixelConvert(devHandle, &stPixelConvertParam);
		if (IMV_FG_OK == ret)
		{
			stRotateImageParam.pSrcData = pConvertBuf;
			stRotateImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen;
			stRotateImageParam.ePixelFormat = IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8;
			nChannelNum = BGR_CHANNEL_NUM;
		}
		else
		{
			stRotateImageParam.pSrcData = NULL;
			printf("image convert to BGR8 failed! ErrorCode[%d]\n", ret);
		}
	}

	do
	{
		if (NULL == stRotateImageParam.pSrcData)
		{
			printf("stRotateImageParam pSrcData is NULL!\n");
			break;
		}

		nRotateBufSize = sizeof(unsigned char) * frame.frameInfo.width * frame.frameInfo.height * nChannelNum;
		pRotateBuf = (unsigned char*)malloc(nRotateBufSize);
		if (NULL == pRotateBuf)
		{
			printf("malloc pRotateBuf failed!\n");
			break;
		}

		stRotateImageParam.nWidth = frame.frameInfo.width;
		stRotateImageParam.nHeight = frame.frameInfo.height;
		stRotateImageParam.eRotationAngle = rotationAngle;
		stRotateImageParam.pDstBuf = pRotateBuf;
		stRotateImageParam.nDstBufSize = nRotateBufSize;

		ret = IMV_FG_RotateImage(devHandle, &stRotateImageParam);
		if (IMV_FG_OK == ret)
		{
			if (IMV_FG_ROTATION_ANGLE90 == rotationAngle)
			{
				printf("Image rotation angle 90 degree successfully!\n");
				hFile = fopen((const char*)"rotationAngle90.bin", "wb");
			}
			else if (IMV_FG_ROTATION_ANGLE180 == rotationAngle)
			{
				printf("Image rotation angle 180 degree successfully!\n");
				hFile = fopen((const char*)"rotationAngle180.bin", "wb");
			}
			else
			{
				printf("Image rotation angle 270 degree successfully!\n");
				hFile = fopen((const char*)"rotationAngle270.bin", "wb");
			}
			fwrite((void*)pRotateBuf, 1, stRotateImageParam.nDstBufSize, hFile);
			fclose(hFile);
		}
		else
		{
			if (IMV_FG_ROTATION_ANGLE90 == rotationAngle)
			{
				printf("Image rotation angle 90 degree failed! ErrorCode[%d]\n", ret);
			}
			else if (IMV_FG_ROTATION_ANGLE180 == rotationAngle)
			{
				printf("Image rotation angle 180 degree failed! ErrorCode[%d]\n", ret);
			}
			else
			{
				printf("Image rotation angle 270 degree failed! ErrorCode[%d]\n", ret);
			}
		}
	} while (false);

	if (pConvertBuf)
	{
		free(pConvertBuf);
		pConvertBuf = NULL;
	}

	if (pRotateBuf)
	{
		free(pRotateBuf);
		pRotateBuf = NULL;
	}
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
		IMV_FG_DEV_HANDLE devHandle = NULL;
		nRet = IMV_FG_OpenDevice(IMV_FG_MODE_BY_INDEX, (void*)&cameraIndex, &devHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Open camera failed! ErrorCode[%d]\n", nRet);
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

		IMV_FG_Frame frame;
		IMV_FG_EPixelType convertFormat = IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8;

		// 获取一帧图像
		// Get a frame image
		nRet = IMV_FG_GetFrame(ifHandle, &frame, 500);
		if (IMV_FG_OK != nRet)
		{
			printf("Get frame failed! ErrorCode[%d]\n", nRet);
			break;
		}

		// 选择图像旋转角度
		// Select image rotate angle
		IMV_FG_ERotationAngle imageRotationAngle = IMV_FG_ROTATION_ANGLE90;
		imageRotationAngle = selectRotationAngle();

		printf("BlockId (%llu) pixelFormat (%d) imageRotateAngle(%d), Start image rotate...\n",
			frame.frameInfo.blockId, frame.frameInfo.pixelFormat, (int)imageRotationAngle);

		// 图片旋转
		// Image rotate
		imageRotate(ifHandle, frame, imageRotationAngle);

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

	if (NULL != devHandle)
	{
		printf("Close camera device.\r\n");
		//关闭相机
		//Close camera 
		nRet = IMV_FG_CloseDevice(devHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Close camera failed! errorCode:[%d]\r\n", nRet);
		}
	}

	if (NULL != ifHandle)
	{
		printf("Close capture device.\r\n");
		// 关闭采集卡
		// Close capture device 
		nRet = IMV_FG_CloseInterface(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Close cameralink board failed! errorCode:[%d]\r\n", nRet);
		}
	}

	printf("Press enter key to exit...\r\n");
	getchar();
	return 0;
}