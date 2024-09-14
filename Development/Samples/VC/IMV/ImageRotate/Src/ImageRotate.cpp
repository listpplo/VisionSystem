/// \file
/// \~chinese
/// \brief 图像顺时针旋转示例
/// \example ImageRotate.cpp
/// \~english
/// \brief Image rotate sample
/// \example ImageRotate.cpp

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
#include <time.h>
#include <Windows.h>
#include "IMVApi.h"

#define	MONO_CHANNEL_NUM	1
#define	RGB_CHANNEL_NUM		3
#define	BGR_CHANNEL_NUM		3

// 图片旋转
// Image rotate
void imageRotate(IMV_HANDLE devHandle, IMV_Frame frame, IMV_ERotationAngle rotationAngle)
{
	IMV_PixelConvertParam	stPixelConvertParam;
	IMV_RotateImageParam	stRotateImageParam;
	unsigned char*			pConvertBuf = NULL;
	unsigned int			nConvertBufSize = 0;
	unsigned char*			pRotateBuf = NULL;
	unsigned int			nRotateBufSize = 0;
	unsigned int			nChannelNum = 0;
	int						ret = IMV_OK;
	FILE*					hFile = NULL;

	memset(&stRotateImageParam, 0, sizeof(stRotateImageParam));

	if (gvspPixelMono8 == frame.frameInfo.pixelFormat)
	{
		stRotateImageParam.pSrcData = frame.pData;
		stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * MONO_CHANNEL_NUM;
		stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
		nChannelNum = MONO_CHANNEL_NUM;
	}
	else if (gvspPixelBGR8 == frame.frameInfo.pixelFormat)
	{
		stRotateImageParam.pSrcData = frame.pData;
		stRotateImageParam.nSrcDataLen = frame.frameInfo.width * frame.frameInfo.height * BGR_CHANNEL_NUM;
		stRotateImageParam.ePixelFormat = frame.frameInfo.pixelFormat;
		nChannelNum = BGR_CHANNEL_NUM;
	}
	else if (gvspPixelRGB8 == frame.frameInfo.pixelFormat)
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
		stPixelConvertParam.eBayerDemosaic = demosaicBilinear;
		stPixelConvertParam.eDstPixelFormat = gvspPixelBGR8;
		stPixelConvertParam.pDstBuf = pConvertBuf;
		stPixelConvertParam.nDstBufSize = nConvertBufSize;

		ret = IMV_PixelConvert(devHandle, &stPixelConvertParam);
		if (IMV_OK == ret)
		{
			stRotateImageParam.pSrcData = pConvertBuf;
			stRotateImageParam.nSrcDataLen = stPixelConvertParam.nDstDataLen;
			stRotateImageParam.ePixelFormat = gvspPixelBGR8;
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

		ret = IMV_RotateImage(devHandle, &stRotateImageParam);
		if (IMV_OK == ret)
		{
			if (rotationAngle90 == rotationAngle)
			{
				printf("Image rotation angle 90 degree successfully!\n");
				hFile = fopen((const char*)"rotationAngle90.bin", "wb");
			}
			else if (rotationAngle180 == rotationAngle)
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
			if (rotationAngle90 == rotationAngle)
			{
				printf("Image rotation angle 90 degree failed! ErrorCode[%d]\n", ret);
			}
			else if (rotationAngle180 == rotationAngle)
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

// 选择图像旋转角度
// Select image rotate angle
static IMV_ERotationAngle selectRotationAngle(void)
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

	return (IMV_ERotationAngle)inputIndex;
}

// ***********结束： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********END: These functions are not related to API call and used to display device info***********

int main()
{
	int ret = IMV_OK;
	unsigned int cameraIndex = 0;
	IMV_HANDLE devHandle = NULL;
	IMV_Frame frame;
	IMV_ERotationAngle imageRotationAngle = rotationAngle90;

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

		// 开始拉流 
		// Start grabbing 
		ret = IMV_StartGrabbing(devHandle);
		if (IMV_OK != ret)
		{
			printf("Start grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 获取一帧图像
		// Get a frame image
		ret = IMV_GetFrame(devHandle, &frame, 500);
		if (IMV_OK != ret)
		{
			printf("Get frame failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 选择图像旋转角度
		// Select image rotate angle
		imageRotationAngle = selectRotationAngle();

		printf("BlockId (%llu) pixelFormat (%d) imageRotateAngle(%d), Start image rotate...\n",
			frame.frameInfo.blockId, frame.frameInfo.pixelFormat, (int)imageRotationAngle);

		// 图片旋转
		// Image rotate
		imageRotate(devHandle, frame, imageRotationAngle);

		// 释放图像缓存
		// Free image buffer
		ret = IMV_ReleaseFrame(devHandle, &frame);
		if (IMV_OK != ret)
		{
			printf("Release frame failed! ErrorCode[%d]\n", ret);
			break;
		}

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
