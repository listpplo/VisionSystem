/// \file
/// \~chinese
/// \brief 动态加载库示例
/// \example DynamicallyLoadDLL.cpp
/// \~english
/// \brief Dynamically load DLL sample
/// \example DynamicallyLoadDLL.cpp

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

typedef const char*	(IMV_CALL * DLL_GetVersion)							();
typedef int			(IMV_CALL * DLL_EnumDevices)						(OUT IMV_DeviceList *pDeviceList, IN unsigned int interfaceType);
typedef int			(IMV_CALL * DLL_EnumDevicesByUnicast)				(OUT IMV_DeviceList *pDeviceList, IN const char* pIpAddress);
typedef int			(IMV_CALL * DLL_CreateHandle)						(OUT IMV_HANDLE* handle, IN IMV_ECreateHandleMode mode, IN void* pIdentifier);
typedef int			(IMV_CALL * DLL_DestroyHandle)						(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_GetDeviceInfo)						(IN IMV_HANDLE handle, OUT IMV_DeviceInfo *pDevInfo);
typedef int			(IMV_CALL * DLL_Open)								(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_OpenEx)								(IN IMV_HANDLE handle, IN IMV_ECameraAccessPermission accessPermission);
typedef bool		(IMV_CALL * DLL_IsOpen)								(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_Close)								(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_GIGE_ForceIpAddress)				(IN IMV_HANDLE handle, IN const char* pIpAddress, IN const char* pSubnetMask, IN const char* pGateway);
typedef int			(IMV_CALL * DLL_GIGE_GetAccessPermission)			(IN IMV_HANDLE handle, IMV_ECameraAccessPermission* pAccessPermission);
typedef int			(IMV_CALL * DLL_GIGE_SetAnswerTimeout)				(IN IMV_HANDLE handle, IN unsigned int timeout);
typedef int			(IMV_CALL * DLL_DownLoadGenICamXML)					(IN IMV_HANDLE handle, IN const char* pFullFileName);
typedef int			(IMV_CALL * DLL_SaveDeviceCfg)						(IN IMV_HANDLE handle, IN const char* pFullFileName);
typedef int			(IMV_CALL * DLL_LoadDeviceCfg)						(IN IMV_HANDLE handle, IN const char* pFullFileName, OUT IMV_ErrorList* pErrorList);
typedef int			(IMV_CALL * DLL_WriteUserPrivateData)				(IN IMV_HANDLE handle, IN void* pBuffer, IN_OUT unsigned int* pLength);
typedef int			(IMV_CALL * DLL_ReadUserPrivateData)				(IN IMV_HANDLE handle, OUT void* pBuffer, IN_OUT unsigned int* pLength);
typedef int			(IMV_CALL * DLL_WriteUARTData)						(IN IMV_HANDLE handle, IN void* pBuffer, IN_OUT unsigned int* pLength);
typedef int			(IMV_CALL * DLL_ReadUARTData)						(IN IMV_HANDLE handle, OUT void* pBuffer, IN_OUT unsigned int* pLength);
typedef int			(IMV_CALL * DLL_SubscribeConnectArg)				(IN IMV_HANDLE handle, IN IMV_ConnectCallBack proc, IN void* pUser);
typedef int			(IMV_CALL * DLL_SubscribeParamUpdateArg)			(IN IMV_HANDLE handle, IN IMV_ParamUpdateCallBack proc, IN void* pUser);
typedef int			(IMV_CALL * DLL_SubscribeStreamArg)					(IN IMV_HANDLE handle, IN IMV_StreamCallBack proc, IN void* pUser);
typedef int			(IMV_CALL * DLL_SubscribeMsgChannelArg)				(IN IMV_HANDLE handle, IN IMV_MsgChannelCallBack proc, IN void* pUser);
typedef int			(IMV_CALL * DLL_SetBufferCount)						(IN IMV_HANDLE handle, IN unsigned int nSize);
typedef int			(IMV_CALL * DLL_ClearFrameBuffer)					(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_GIGE_SetInterPacketTimeout)			(IN IMV_HANDLE handle, IN unsigned int nTimeout);
typedef int			(IMV_CALL * DLL_GIGE_SetSingleResendMaxPacketNum)	(IN IMV_HANDLE handle, IN unsigned int maxPacketNum);
typedef int			(IMV_CALL * DLL_GIGE_SetMaxLostPacketNum)			(IN IMV_HANDLE handle, IN unsigned int maxLostPacketNum);
typedef int			(IMV_CALL * DLL_StartGrabbing)						(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_StartGrabbingEx)					(IN IMV_HANDLE handle, IN uint64_t maxImagesGrabbed, IN IMV_EGrabStrategy strategy);
typedef bool		(IMV_CALL * DLL_IsGrabbing)							(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_StopGrabbing)						(IN IMV_HANDLE handle);
typedef int			(IMV_CALL * DLL_AttachGrabbing)						(IN IMV_HANDLE handle, IN IMV_FrameCallBack proc, IN void* pUser);
typedef int			(IMV_CALL * DLL_GetFrame)							(IN IMV_HANDLE handle, OUT IMV_Frame* pFrame, IN unsigned int timeoutMS);
typedef int			(IMV_CALL * DLL_ReleaseFrame)						(IN IMV_HANDLE handle, IN IMV_Frame* pFrame);
typedef int			(IMV_CALL * DLL_CloneFrame)							(IN IMV_HANDLE handle, IN IMV_Frame* pFrame, OUT IMV_Frame* pCloneFrame);
typedef int			(IMV_CALL * DLL_GetChunkDataByIndex)				(IN IMV_HANDLE handle, IN IMV_Frame* pFrame, IN unsigned int index, OUT IMV_ChunkDataInfo *pChunkDataInfo);
typedef int			(IMV_CALL * DLL_GetStatisticsInfo)					(IN IMV_HANDLE handle, OUT IMV_StreamStatisticsInfo* pStreamStatsInfo);
typedef int			(IMV_CALL * DLL_ResetStatisticsInfo)				(IN IMV_HANDLE handle);
typedef bool		(IMV_CALL * DLL_FeatureIsAvailable)					(IN IMV_HANDLE handle, IN const char* pFeatureName);
typedef bool		(IMV_CALL * DLL_FeatureIsReadable)					(IN IMV_HANDLE handle, IN const char* pFeatureName);
typedef bool		(IMV_CALL * DLL_FeatureIsWriteable)					(IN IMV_HANDLE handle, IN const char* pFeatureName);
typedef bool		(IMV_CALL * DLL_FeatureIsStreamable)				(IN IMV_HANDLE handle, IN const char* pFeatureName);
typedef bool		(IMV_CALL * DLL_FeatureIsValid)						(IN IMV_HANDLE handle, IN const char* pFeatureName);
typedef int			(IMV_CALL * DLL_GetIntFeatureValue)					(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
typedef int			(IMV_CALL * DLL_GetIntFeatureMin)					(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
typedef int			(IMV_CALL * DLL_GetIntFeatureMax)					(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
typedef int			(IMV_CALL * DLL_GetIntFeatureInc)					(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);
typedef int			(IMV_CALL * DLL_SetIntFeatureValue)					(IN IMV_HANDLE handle, IN const char* pFeatureName, IN int64_t intValue);
typedef int			(IMV_CALL * DLL_GetDoubleFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);
typedef int			(IMV_CALL * DLL_GetDoubleFeatureMin)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);
typedef int			(IMV_CALL * DLL_GetDoubleFeatureMax)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);
typedef int			(IMV_CALL * DLL_SetDoubleFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, IN double doubleValue);
typedef int			(IMV_CALL * DLL_GetBoolFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT bool* pBoolValue);
typedef int			(IMV_CALL * DLL_SetBoolFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, IN bool boolValue);
typedef int			(IMV_CALL * DLL_GetEnumFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT uint64_t* pEnumValue);
typedef int			(IMV_CALL * DLL_SetEnumFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, IN uint64_t enumValue);
typedef int			(IMV_CALL * DLL_GetEnumFeatureSymbol)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT IMV_String* pEnumSymbol);
typedef int			(IMV_CALL * DLL_SetEnumFeatureSymbol)				(IN IMV_HANDLE handle, IN const char* pFeatureName, IN const char* pEnumSymbol);
typedef int			(IMV_CALL * DLL_GetEnumFeatureEntryNum)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT unsigned int* pEntryNum);
typedef int			(IMV_CALL * DLL_GetEnumFeatureEntrys)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT IMV_EnumEntryList* pEnumEntryList);
typedef int			(IMV_CALL * DLL_GetStringFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, OUT IMV_String* pStringValue);
typedef int			(IMV_CALL * DLL_SetStringFeatureValue)				(IN IMV_HANDLE handle, IN const char* pFeatureName, IN const char* pStringValue);
typedef int			(IMV_CALL * DLL_ExecuteCommandFeature)				(IN IMV_HANDLE handle, IN const char* pFeatureName);
typedef int			(IMV_CALL * DLL_PixelConvert)						(IN IMV_HANDLE handle, IN_OUT IMV_PixelConvertParam* pstPixelConvertParam);
typedef int			(IMV_CALL * DLL_OpenRecord)							(IN IMV_HANDLE handle, IN IMV_RecordParam *pstRecordParam);
typedef int			(IMV_CALL * DLL_InputOneFrame)						(IN IMV_HANDLE handle, IN IMV_RecordFrameInfoParam *pstRecordFrameInfoParam);
typedef int			(IMV_CALL * DLL_CloseRecord)						(IN IMV_HANDLE handle);

// ***********开始： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********BEGIN: These functions are not related to API call and used to display device info***********
// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("Get frame blockId = %llu.\n", pFrame->frameInfo.blockId);

	return;
}

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

// ***********结束： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********END: These functions are not related to API call and used to display device info***********

int main()
{
	int ret = IMV_OK;
	unsigned int cameraIndex = 0;
	IMV_HANDLE devHandle = NULL;
	HINSTANCE libHandle = NULL;
	DLL_DestroyHandle DLLDestroyHandle = NULL;

	do
	{
		// 加载SDK库  
		// Load SDK library
		libHandle = LoadLibraryA("MVSDKmd.dll");
		if (NULL == libHandle)
		{
			printf("Load MVSDKmd.dll library failed!\n");
			break;
		}
		 
		// 获取发现设备接口函数地址 
		// Get discover camera interface address
		DLL_EnumDevices DLLEnumDevices = (DLL_EnumDevices)GetProcAddress(libHandle, "IMV_EnumDevices");
		if (NULL == DLLEnumDevices)
		{
			printf("Get IMV_EnumDevices address failed!\n");
			break;
		}

		// 获取创建设备句柄接口函数地址 
		// Get create Device Handle interface address
		DLL_CreateHandle DLLCreateHandle = (DLL_CreateHandle)GetProcAddress(libHandle, "IMV_CreateHandle");
		if (NULL == DLLCreateHandle)
		{
			printf("Get IMV_CreateHandle address failed!\n");
			break;
		}

		// 获取销毁设备句柄接口函数地址 
		// Get destroy Device Handle interface address
		DLLDestroyHandle = (DLL_DestroyHandle)GetProcAddress(libHandle, "IMV_DestroyHandle");
		if (NULL == DLLDestroyHandle)
		{
			printf("Get IMV_DestroyHandle address failed!\n");
			break;
		}

		// 获取打开相机接口函数地址 
		// Get open camera interface address
		DLL_Open DLLOpen = (DLL_Open)GetProcAddress(libHandle, "IMV_Open");
		if (NULL == DLLOpen)
		{
			printf("Get IMV_Open address failed!\n");
			break;
		}

		// 获取注册数据帧回调接口函数地址 
		// Get register data frame callback interface address
		DLL_AttachGrabbing DLLAttachGrabbing = (DLL_AttachGrabbing)GetProcAddress(libHandle, "IMV_AttachGrabbing");
		if (NULL == DLLAttachGrabbing)
		{
			printf("Get IMV_AttachGrabbing address failed!\n");
			break;
		}

		// 获取开始拉流接口函数地址 
		// Get start grabbing interface address
		DLL_StartGrabbing DLLStartGrabbing = (DLL_StartGrabbing)GetProcAddress(libHandle, "IMV_StartGrabbing");
		if (NULL == DLLStartGrabbing)
		{
			printf("Get IMV_StartGrabbing address failed!\n");
			break;
		}

		// 获取停止拉流接口函数地址 
		// Get stop grabbing interface address
		DLL_StopGrabbing DLLStopGrabbing = (DLL_StopGrabbing)GetProcAddress(libHandle, "IMV_StopGrabbing");
		if (NULL == DLLStopGrabbing)
		{
			printf("Get IMV_StopGrabbing address failed!\n");
			break;
		}

		// 获取关闭相机接口函数地址 
		// Get close camera interface address
		DLL_Close DLLClose = (DLL_Close)GetProcAddress(libHandle, "IMV_Close");
		if (NULL == DLLClose)
		{
			printf("Get IMV_Close address failed!\n");
			break;
		}

		// 发现设备 
		// discover camera 
		IMV_DeviceList deviceInfoList;
		ret = DLLEnumDevices(&deviceInfoList, interfaceTypeAll);
		if (IMV_OK != ret)
		{
			printf("Enumeration devices failed! ErrorCode[%d]\n", ret);
			break;
		}

		if (deviceInfoList.nDevNum < 1)
		{
			printf("no camera\n");
			break;
		}

		// 打印相机基本信息（序号,类型,制造商信息,型号,序列号,用户自定义ID,IP地址） 
		// Print camera info (Index, Type, Vendor, Model, Serial number, DeviceUserID, IP Address) 
		displayDeviceInfo(deviceInfoList);

		// 选择需要连接的相机 
		// Select one camera to connect to  
		cameraIndex = selectDevice(deviceInfoList.nDevNum);

		// 创建设备句柄
		// Create Device Handle
		ret = DLLCreateHandle(&devHandle, modeByIndex, (void*)&cameraIndex);
		if (IMV_OK != ret)
		{
			printf("Create devHandle failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 打开相机 
		// Open camera 
		ret = DLLOpen(devHandle);
		if (IMV_OK != ret)
		{
			printf("Open camera failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 注册数据帧回调函数
		// Register data frame callback function
		ret = DLLAttachGrabbing(devHandle, onGetFrame, NULL);
		if (IMV_OK != ret)
		{
			printf("Attach grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 开始拉流 
		// Start grabbing 
		ret = DLLStartGrabbing(devHandle);
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
		ret = DLLStopGrabbing(devHandle);
		if (IMV_OK != ret)
		{
			printf("Stop grabbing failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 关闭相机
		// Close camera 
		ret = DLLClose(devHandle);
		if (IMV_OK != ret)
		{
			printf("Close camera failed! ErrorCode[%d]\n", ret);
			break;
		}
	}while (false);

	// 销毁设备句柄
	// Destroy Device Handle
	if (NULL != devHandle)
	{
		// 销毁设备句柄
		// Destroy Device Handle
		DLLDestroyHandle(devHandle);
	}

	if (NULL != libHandle)
	{
		FreeLibrary(libHandle);
	}

	printf("Press enter key to exit...\n");
	getchar();

	return 0;
}
