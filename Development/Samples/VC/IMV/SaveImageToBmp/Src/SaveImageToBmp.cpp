/// \file
/// \~chinese
/// \brief 保存BMP图像示例
/// \example SaveImageToBmp.cpp
/// \~english
/// \brief Save bmp image sample
/// \example SaveImageToBmp.cpp

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

#define snprintf	_snprintf

#pragma pack(1)
typedef struct _BitmapFileHeader {
	unsigned short	bfType;
	unsigned int	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned int	bfOffBits;
} BitmapFileHeader;

typedef struct _BitmapInfoHeader{
	unsigned int	biSize;
	int				biWidth;
	int				biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned int	biCompression;
	unsigned int	biSizeImage;
	int				biXPelsPerMeter;
	int				biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
} BitmapInfoHeader;

typedef struct _BitmapRGBQuad{
	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;
} BitmapRGBQuad;
#pragma pack()

#define BMP_HEADER_SIZE		(sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader))

unsigned char* g_pConvertBuf = NULL;
unsigned int g_pConvertBufSize = 0;
BitmapRGBQuad g_colorTable[256];

// 保存bmp图像
// save image to bmp
static bool saveImageToBmp(IMV_HANDLE devHandle, IMV_Frame *pFrame)
{
	int						ret = IMV_OK;
	IMV_PixelConvertParam stPixelConvertParam;
	unsigned char*			pImageData = NULL;
	IMV_EPixelType		pixelFormat = gvspPixelMono8;
	unsigned int			imageSize = 0;
	unsigned int			uRgbQuadLen = 0;
	char					fileName[256] = {0};
	BitmapFileHeader		bmpFileHeader = { 0 };
	BitmapInfoHeader		bmpInfoHeader = { 0 };

	// mono8和BGR8裸数据不需要转码
	// mono8 and BGR8 raw data is not need to convert
	if ((pFrame->frameInfo.pixelFormat != gvspPixelMono8)
		&& (pFrame->frameInfo.pixelFormat != gvspPixelBGR8))
	{
		if (g_pConvertBuf == NULL)
		{
			printf("g_pConvertBuf is NULL\n");
			return false;
		}

		if (g_pConvertBufSize < (pFrame->frameInfo.width * pFrame->frameInfo.height * 3))
		{
			g_pConvertBufSize = pFrame->frameInfo.width * pFrame->frameInfo.height * 3;

			free(g_pConvertBuf);
			g_pConvertBuf = NULL;

			g_pConvertBuf = (unsigned char*)malloc(g_pConvertBufSize);
			if (g_pConvertBuf == NULL)
			{
				printf("g_pConvertBuf re-malloc failed!!\n");
				return false;
			}
		}

		// 图像转换成BGR8
		// convert image to BGR8
		memset(&stPixelConvertParam, 0, sizeof(stPixelConvertParam));
		stPixelConvertParam.nWidth = pFrame->frameInfo.width;
		stPixelConvertParam.nHeight = pFrame->frameInfo.height;
		stPixelConvertParam.ePixelFormat = pFrame->frameInfo.pixelFormat;
		stPixelConvertParam.pSrcData = pFrame->pData;
		stPixelConvertParam.nSrcDataLen = pFrame->frameInfo.size;
		stPixelConvertParam.nPaddingX = pFrame->frameInfo.paddingX;
		stPixelConvertParam.nPaddingY = pFrame->frameInfo.paddingY;
		stPixelConvertParam.eBayerDemosaic = demosaicNearestNeighbor;
		stPixelConvertParam.eDstPixelFormat = gvspPixelBGR8;
		stPixelConvertParam.pDstBuf = g_pConvertBuf;
		stPixelConvertParam.nDstBufSize = g_pConvertBufSize;

		ret = IMV_PixelConvert(devHandle, &stPixelConvertParam);
		if (IMV_OK != ret)
		{
			printf("image convert to BGR failed! ErrorCode[%d]\n", ret);
			return false;
		}

		pImageData = g_pConvertBuf;
		pixelFormat = gvspPixelBGR8;
	}
	else
	{
		pImageData = pFrame->pData;
		pixelFormat = pFrame->frameInfo.pixelFormat;
	}

	snprintf(fileName, sizeof(fileName), "%llu.bmp", pFrame->frameInfo.blockId);

	if (pixelFormat == gvspPixelMono8)
	{
		uRgbQuadLen = (unsigned int)sizeof(g_colorTable);
		imageSize = pFrame->frameInfo.width * pFrame->frameInfo.height;
	}
	else
	{
		uRgbQuadLen = 0;
		imageSize = pFrame->frameInfo.width * pFrame->frameInfo.height * 3;
	}

	// 打开BMP文件
	// open BMP file
	FILE* hFile = fopen(fileName, "wb");
	if (hFile == NULL)
	{
		// 如果打开失败，请用管理权限执行
		// If opefailed, Run as Administrator
		printf("Open file (%s) failed!\n", fileName);
		return false;
	}

	// 设置BMP文件头
	// set BMP file header

	// 文件头类型 'BM'(42 4D)
	// file header type 'BM'(42 4D)
	bmpFileHeader.bfType = 0x4D42;

	// 文件大小
	// file size
	bmpFileHeader.bfSize = BMP_HEADER_SIZE + uRgbQuadLen + imageSize;

	// 位图像素数据的起始位置
	// start position of bitmap pixel data
	bmpFileHeader.bfOffBits = BMP_HEADER_SIZE + uRgbQuadLen;

	// 设置BMP信息头
	// set BMP info header

	// 信息头所占字节数
	// the number of header bytes
	bmpInfoHeader.biSize = (unsigned int)sizeof(bmpInfoHeader);

	// 位图宽度
	// bmp width
	bmpInfoHeader.biWidth = (int)pFrame->frameInfo.width;

	// RGB数据保存为bmp，上下会颠倒，需要设置height为负值
	// bmp height
	bmpInfoHeader.biHeight = -(int)pFrame->frameInfo.height;

	// 位图平面数
	// the number of bitmap planes
	bmpInfoHeader.biPlanes = 1;

	// 像素位数
	// the number of pixels
	bmpInfoHeader.biBitCount = (pixelFormat == gvspPixelMono8) ? 8 : 24;

	// 图像大小
	// the size of image
	bmpInfoHeader.biSizeImage = imageSize;

	// 写入BMP文件头
	// write BMP file header
	fwrite((void*)&bmpFileHeader, 1, sizeof(bmpFileHeader), hFile);

	// 写入BMP信息头
	// write BMP info header
	fwrite((void*)&bmpInfoHeader, 1, sizeof(bmpInfoHeader), hFile);

	if (pixelFormat == gvspPixelMono8)
	{
		// 黑白图像写入颜色表
		// write color table of mono8 image
		fwrite((void*)&g_colorTable, 1, uRgbQuadLen, hFile);
	}

	// 写入图像数据
	// write image data
	fwrite((void*)pImageData, 1, imageSize, hFile);

	// 关闭BMP文件
	// close BMP file
	fclose(hFile);

	return true;
}

// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	if (pUser == NULL)
	{
		printf("pUser is NULL\n");
		return;
	}

	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("Get frame blockId = %llu\n", pFrame->frameInfo.blockId);

	// 保存一张BMP图像
	// save a BMP image
	if (pFrame->frameInfo.blockId == 1)
	{
		printf("Save image to bmp start...\n");
		if (saveImageToBmp((IMV_HANDLE)pUser, pFrame))
		{
			printf("Save image to bmp successfully!\n");
		}
		else
		{
			printf("Save image to bmp failed!\n");
		}
	}

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

// 检查是否需要申请存放转码数据的内存 
// Check whether malloc buffer for saving the convert data
static int mallocConvertBuffer(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	uint64_t pixelFormatVal = 0;
	int64_t widthVal = 0;
	int64_t heightVal = 0;

	ret = IMV_GetEnumFeatureValue(devHandle, "PixelFormat", &pixelFormatVal);
	if (IMV_OK != ret)
	{
		printf("Get PixelFormat feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	for (int i = 0; i < 256; i++)
	{
		g_colorTable[i].rgbRed = (unsigned char)i;
		g_colorTable[i].rgbBlue = (unsigned char)i;
		g_colorTable[i].rgbGreen = (unsigned char)i;
		g_colorTable[i].rgbReserved = (unsigned char)0;
	}

	if ((pixelFormatVal == (uint64_t)gvspPixelMono8)
		|| (pixelFormatVal == (uint64_t)gvspPixelBGR8))
	{
		// mono8和BGR8裸数据不需要转码
		// mono8 and BGR8 raw data is not need to convert
		return IMV_OK;
	}

	ret = IMV_GetIntFeatureValue(devHandle, "Width", &widthVal);
	if (IMV_OK != ret)
	{
		printf("Get Width feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_GetIntFeatureValue(devHandle, "Height", &heightVal);
	if (IMV_OK != ret)
	{
		printf("Get Height feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	g_pConvertBufSize = sizeof(unsigned char) * (int)widthVal * (int)heightVal * 3;
	g_pConvertBuf = (unsigned char*)malloc(g_pConvertBufSize);
	if (g_pConvertBuf == NULL)
	{
		printf("Malloc g_pConvertBuf failed!\n");
		return IMV_NO_MEMORY;
	}

	return IMV_OK;
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

		// 检查是否需要申请存放转码数据的内存 
		// Check whether malloc buffer for saving the convert data
		ret = mallocConvertBuffer(devHandle);
		if (IMV_OK != ret)
		{
			break;
		}

		// 注册数据帧回调函数
		// Register data frame callback function
		ret = IMV_AttachGrabbing(devHandle, onGetFrame, (void*)devHandle);
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

	if (g_pConvertBuf)
	{
		free(g_pConvertBuf);
		g_pConvertBuf = NULL;
	}

	printf("Press enter key to exit...\n");
	getchar();

	return 0;
}
