/// \file
/// \~chinese
/// \brief 获取ChunkData示例
/// \example ChunkData.cpp
/// \~english
/// \brief get ChunkData sample
/// \example ChunkData.cpp

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
#include <process.h>
#include "IMVApi.h"

bool g_isExitThread = false;

unsigned __stdcall executeSoftTriggerProc(void* pUserData)
{
	int ret = IMV_OK;
	IMV_HANDLE devHandle = (IMV_HANDLE)pUserData;
	if (NULL == devHandle)
	{
		return 0;
	}

	while (!g_isExitThread)
	{
		ret = IMV_ExecuteCommandFeature(devHandle, "TriggerSoftware");
		if (IMV_OK != ret)
		{
			printf("Execute TriggerSoftware failed! ErrorCode[%d]\n", ret);
			continue;
		}

		// 通过睡眠时间来调节帧率
		// Adjust the frame rate by sleep time
		Sleep(50);
	}

	return 0;
}

// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_Frame* pFrame, void* pUser)
{
	int ret = IMV_OK;
	unsigned int chunkDataIndex = 0;
	unsigned int paramIndex = 0;
	IMV_ChunkDataInfo chunkDataInfo;
	IMV_HANDLE devHandle = (IMV_HANDLE)pUser;

	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	printf("Get frame blockId = %llu\n", pFrame->frameInfo.blockId);

	if (!devHandle)
	{
		printf("devHandle is NULL!\n");
		return;
	}

	// 获取ChunkData数据
	// Get ChunkData
	for (chunkDataIndex = 0; chunkDataIndex < pFrame->frameInfo.chunkCount; chunkDataIndex++)
	{
		ret = IMV_GetChunkDataByIndex(devHandle, pFrame, chunkDataIndex, &chunkDataInfo);
		if (IMV_OK != ret)
		{
			printf("Get ChunkData failed! ErrorCode[%d]\n", ret);
			continue;
		}

		printf("chunkID = %u\n", chunkDataInfo.chunkID);
		for (paramIndex = 0; paramIndex < chunkDataInfo.nParamCnt; paramIndex++)
		{
			printf("paramName = %s\n", chunkDataInfo.pParamNameList[paramIndex].str);
		}
		printf("\n");
	}

	return;
}

static int setSoftTriggerConf(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;

	// 设置触发源为软触发 
	// Set trigger source to Software 
	ret = IMV_SetEnumFeatureSymbol(devHandle, "TriggerSource", "Software");
	if (IMV_OK != ret)
	{
		printf("Set triggerSource value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置触发器 
	// Set trigger selector to FrameStart 
	ret = IMV_SetEnumFeatureSymbol(devHandle, "TriggerSelector", "FrameStart");
	if (IMV_OK != ret)
	{
		printf("Set triggerSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置触发模式 
	// Set trigger mode to On 
	ret = IMV_SetEnumFeatureSymbol(devHandle, "TriggerMode", "On");
	if (IMV_OK != ret)
	{
		printf("Set triggerMode value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	return ret;
}

static int setChunkDataConf(IMV_HANDLE devHandle)
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

	if ((typeGigeCamera != deviceInfo.nCameraType) &&
		(typeU3vCamera != deviceInfo.nCameraType))
	{
		printf("CameraType [%d]\n", (int)deviceInfo.nCameraType);
		return IMV_NOT_SUPPORT;
	}

	// 设置CounterSelector
	// Set CounterSelector
	ret = IMV_SetEnumFeatureSymbol(devHandle, "CounterSelector", "Counter0");
	if (IMV_OK != ret)
	{
		printf("Set CounterSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置CounterResetSource
	// Set CounterResetSource
	ret = IMV_SetEnumFeatureSymbol(devHandle, "CounterResetSource", "SoftwareSignal0");
	if (IMV_OK != ret)
	{
		printf("Set CounterResetSource value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 执行CounterReset
	// Execute CounterReset
	ret = IMV_ExecuteCommandFeature(devHandle, "CounterReset");
	if (IMV_OK != ret)
	{
		printf("Execute CounterReset failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置CounterResetSource
	// Set CounterResetSource
	ret = IMV_SetEnumFeatureSymbol(devHandle, "CounterResetSource", "SoftwareSignal0");
	if (IMV_OK != ret)
	{
		printf("Set CounterResetSource value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置CounterResetSource
	// Set CounterResetSource
	ret = IMV_SetEnumFeatureSymbol(devHandle, "CounterResetSource", "Off");
	if (IMV_OK != ret)
	{
		printf("Set CounterResetSource value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	if (typeGigeCamera == deviceInfo.nCameraType)
	{
		// 设置GevGVSPExtendedIDMode
		// Set GevGVSPExtendedIDMode
		ret = IMV_SetEnumFeatureSymbol(devHandle, "GevGVSPExtendedIDMode", "On");
		if (IMV_OK != ret)
		{
			printf("Set GevGVSPExtendedIDMode value failed! ErrorCode[%d]\n", ret);
			return ret;
		}
	}

	// 设置ChunkModeActive
	// Set ChunkModeActive
	ret = IMV_SetBoolFeatureValue(devHandle, "ChunkModeActive", true);
	if (IMV_OK != ret)
	{
		printf("Set ChunkModeActive value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置ChunkSelector
	// Set ChunkSelector
	ret = IMV_SetEnumFeatureSymbol(devHandle, "ChunkSelector", "Counter0Value");
	if (IMV_OK != ret)
	{
		printf("Set ChunkSelector value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 设置ChunkEnable
	// Set ChunkEnable
	ret = IMV_SetBoolFeatureValue(devHandle, "ChunkEnable", true);
	if (IMV_OK != ret)
	{
		printf("Set ChunkEnable value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	return ret;
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
// ***********结束： 这部分处理与SDK操作相机无关，用于显示设备列表 ***********
// ***********END: These functions are not related to API call and used to display device info***********

int main()
{
	int ret = IMV_OK;
	unsigned int cameraIndex = 0;
	IMV_HANDLE devHandle = NULL;
	HANDLE threadHandle = NULL;

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

		// 设置软触发配置 
		// Set software trigger config 
		ret = setSoftTriggerConf(devHandle);
		if (IMV_OK != ret)
		{
			break;
		}

		// 设置ChunkData配置
		// Set ChunkData config 
		ret = setChunkDataConf(devHandle);
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

		// 创建软触发线程 
		// Create soft trigger thread 
		threadHandle = (HANDLE)_beginthreadex(NULL,
			0,
			executeSoftTriggerProc,
			(void*)devHandle,
			CREATE_SUSPENDED,
			NULL);

		if (!threadHandle)
		{
			printf("Failed to create soft trigger thread!\n");
			break;
		}

		ResumeThread(threadHandle);

		// 启动拉流线程 
		// Start grabbing thread 
		g_isExitThread = false;

		// 取图2秒
		// get frame 2 seconds
		Sleep(2000);

		// 退出拉流线程 
		// Stop grabbing thread 
		g_isExitThread = true;

		WaitForSingleObject(threadHandle, INFINITE);
		CloseHandle(threadHandle);

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
