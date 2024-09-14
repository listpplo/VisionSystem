/// \file
/// \~chinese
/// \brief 保存BMP图像示例
/// \example SaveImageToBmp.cpp
/// \~english
/// \brief Save bmp image sample
/// \example SaveImageToBmp.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVFGApi.h"
#include "IMVFGDefines.h"


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
static bool saveImageToBmp(IMV_FG_IF_HANDLE devHandle, IMV_FG_Frame *pFrame)
{
	int						ret = IMV_FG_OK;
	IMV_FG_PixelConvertParam stPixelConvertParam;
	unsigned char*			pImageData = NULL;
	IMV_FG_EPixelType		pixelFormat = IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8;
	unsigned int			imageSize = 0;
	unsigned int			uRgbQuadLen = 0;
	char					fileName[256] = { 0 };
	BitmapFileHeader		bmpFileHeader = { 0 };
	BitmapInfoHeader		bmpInfoHeader = { 0 };

	// mono8和BGR8裸数据不需要转码
	// mono8 and BGR8 raw data is not need to convert
	if ((pFrame->frameInfo.pixelFormat != IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8)
		&& (pFrame->frameInfo.pixelFormat != IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8))
	{
		if (g_pConvertBuf == NULL)
		{
			printf("g_pConvertBuf is NULL\n");
			return false;
		}

		if (g_pConvertBufSize < (pFrame->frameInfo.width * pFrame->frameInfo.height * 3))
		{
			free(g_pConvertBuf);
			g_pConvertBuf = NULL;

			g_pConvertBufSize = pFrame->frameInfo.width * pFrame->frameInfo.height * 3;
			g_pConvertBuf = (unsigned char*)malloc(g_pConvertBufSize);
			if (g_pConvertBuf == NULL)
			{
				printf("Malloc g_pConvertBuf failed!\n");
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
		stPixelConvertParam.eBayerDemosaic = IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR;
		stPixelConvertParam.eDstPixelFormat = IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8;
		stPixelConvertParam.pDstBuf = g_pConvertBuf;
		stPixelConvertParam.nDstBufSize = g_pConvertBufSize;

		ret = IMV_FG_PixelConvert(devHandle, &stPixelConvertParam);
		if (IMV_FG_OK != ret)
		{
			printf("image convert to BGR failed! ErrorCode[%d]\n", ret);
			return false;
		}

		pImageData = g_pConvertBuf;
		pixelFormat = IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8;
	}
	else
	{
		pImageData = pFrame->pData;
		pixelFormat = pFrame->frameInfo.pixelFormat;
	}

	snprintf(fileName, sizeof(fileName), "%llu.bmp", pFrame->frameInfo.blockId);

	if (pixelFormat == IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8)
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
	bmpInfoHeader.biBitCount = (pixelFormat == IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8) ? 8 : 24;

	// 图像大小
	// the size of image
	bmpInfoHeader.biSizeImage = imageSize;

	// 写入BMP文件头
	// write BMP file header
	fwrite((void*)&bmpFileHeader, 1, sizeof(bmpFileHeader), hFile);

	// 写入BMP信息头
	// write BMP info header
	fwrite((void*)&bmpInfoHeader, 1, sizeof(bmpInfoHeader), hFile);

	if (pixelFormat == IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8)
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
static void onGetFrame(IMV_FG_Frame* pFrame, void* pUser)
{
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
		if (saveImageToBmp((IMV_FG_IF_HANDLE)pUser, pFrame))
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

// 检查是否需要申请存放转码数据的内存 
// Check whether malloc buffer for saving the convert data
static int mallocConvertBuffer(IMV_FG_IF_HANDLE ifHandle)
{
	int ret = IMV_FG_OK;
	uint64_t pixelFormatVal = 0;
	int64_t widthVal = 0;
	int64_t heightVal = 0;

	ret = IMV_FG_GetEnumFeatureValue(ifHandle, "PixelFormat", &pixelFormatVal);
	if (IMV_FG_OK != ret)
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

	if ((pixelFormatVal == (uint64_t)IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_Mono8)
		|| (pixelFormatVal == (uint64_t)IMV_FG_EPixelType::IMV_FG_PIXEL_TYPE_BGR8))
	{
		// mono8和BGR8裸数据不需要转码
		// mono8 and BGR8 raw data is not need to convert
		return IMV_FG_OK;
	}

	ret = IMV_FG_GetIntFeatureValue(ifHandle, "Width", &widthVal);
	if (IMV_FG_OK != ret)
	{
		printf("Get Width feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	ret = IMV_FG_GetIntFeatureValue(ifHandle, "Height", &heightVal);
	if (IMV_FG_OK != ret)
	{
		printf("Get Height feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	g_pConvertBufSize = sizeof(unsigned char) * (int)widthVal * (int)heightVal * 3;
	g_pConvertBuf = (unsigned char*)malloc(g_pConvertBufSize);
	if (g_pConvertBuf == NULL)
	{
		printf("Malloc g_pConvertBuf failed!\n");
		return IMV_FG_NO_MEMORY;
	}

	return IMV_FG_OK;
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
		nRet = IMV_FG_OpenDevice(IMV_FG_MODE_BY_INDEX, (void*)&cameraIndex, &devHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Open camera failed! ErrorCode[%d]\n", nRet);
			break;
		}

		// 检查是否需要申请存放转码数据的内存 
		// Check whether malloc buffer for saving the convert data
		nRet = mallocConvertBuffer(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("mallocConvertBuffer falied.\r\n");
			break;
		}

		printf("attach Grabbing.\r\n");
		// 注册数据帧回调函数
		// Register data frame callback function
		nRet = IMV_FG_AttachGrabbing(ifHandle, onGetFrame, ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Attach grabbing failed! errorCode:[%d]\r\n", nRet);
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
		printf("Grabbing... get frame 5 seconds.");
		// 取图2秒
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

	printf("Press enter key to exit...\n");
	getchar();
	return 0;
}
