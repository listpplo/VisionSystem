/// \file
/// \~chinese
/// \brief 通用属性访问接口使用示例
/// \example CommPropAccess.cpp
/// \~english
/// \brief common property access interface sample
/// \example CommPropAccess.cpp

//**********************************************************************
// 本Demo为简单演示SDK的使用，没有附加修改相机IP的代码，在运行之前，请使
// 用相机客户端修改相机IP地址的网段与主机的网段一致。                 
// This Demo shows how to use GenICam API(C) to write a simple program.
// Please make sure that the camera and PC are in the same subnet before running the demo.
// If not, you can use camera client software to modify the IP address of camera to the same subnet with PC. 
// 本程序演示了发现设备，连接设备，修改相机曝光时间、图像宽度、ReverseX、DeviceUserID、TriggerSource等属性，断开连接操作 
// This program shows how to discover and connect device, set camera params such as ExposureTime, Image Width, ReverseX, DeviceUserID,
// TriggerSource etc. and disconnect device 
//**********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IMVApi.h"

static int modifyCameraExposureTime(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	double exposureTimeValue = 0.0;
	double exposureMinValue = 0;
	double exposureMaxValue = 0;

	// 获取属性值
	// Get feature value 
	ret = IMV_GetDoubleFeatureValue(devHandle, "ExposureTime", &exposureTimeValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,exposureTime is %0.2f\n", exposureTimeValue);

	// 获取属性可设的最小值
	// Get property's settable minimum value
	ret = IMV_GetDoubleFeatureMin(devHandle, "ExposureTime", &exposureMinValue);
	if (IMV_OK != ret)
	{
		printf("Get feature minimum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("exposureTime settable minimum value is %0.2f\n", exposureMinValue);

	// 获取属性可设的最大值
	// Get property's settable maximum value
	ret = IMV_GetDoubleFeatureMax(devHandle, "ExposureTime", &exposureMaxValue);
	if (IMV_OK != ret)
	{
		printf("Get feature maximum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("exposureTime settable maximum value is %0.2f\n", exposureMaxValue);

	if (exposureTimeValue < (exposureMinValue + 2.0))
	{
		exposureTimeValue += 2.0;
	}
	else
	{
		exposureTimeValue -= 2.0;
	}

	// 设置属性值
	// Set feature value 
	ret = IMV_SetDoubleFeatureValue(devHandle, "ExposureTime", exposureTimeValue);
	if (IMV_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_GetDoubleFeatureValue(devHandle, "ExposureTime", &exposureTimeValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,exposureTime is %0.2f\n", exposureTimeValue);

	return ret;
}

static int modifyCameraWidth(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	int64_t widthValue = 0;
	int64_t widthMinValue = 0;
	int64_t widthMaxValue = 0;
	int64_t incrementValue = 0;

	// 获取属性值
	// Get feature value 
	ret = IMV_GetIntFeatureValue(devHandle, "Width", &widthValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,width is %lld\n", widthValue);

	// 获取属性可设的最小值
	// Get property's settable minimum value
	ret = IMV_GetIntFeatureMin(devHandle, "Width", &widthMinValue);
	if (IMV_OK != ret)
	{
		printf("Get feature minimum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("width settable minimum value is %lld\n", widthMinValue);

	// 获取属性可设的最大值
	// Get property's settable maximum value
	ret = IMV_GetIntFeatureMax(devHandle, "Width", &widthMaxValue);
	if (IMV_OK != ret)
	{
		printf("Get feature maximum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("width settable maximum value is %lld\n", widthMaxValue);

	// 获取属性步长
	// Get feature increment
	ret = IMV_GetIntFeatureInc(devHandle, "Width", &incrementValue);
	if (IMV_OK != ret)
	{
		printf("Get feature increment value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("width increment value is %lld\n", incrementValue);

	if (widthValue < (widthMinValue + incrementValue))
	{
		widthValue += incrementValue;
	}
	else
	{
		widthValue -= incrementValue;
	}

	// 设置属性值
	// Set feature value 
	ret = IMV_SetIntFeatureValue(devHandle, "Width", widthValue);
	if (IMV_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_GetIntFeatureValue(devHandle, "Width", &widthValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,width is %lld\n", widthValue);

	return ret;
}

static int modifyCameraReverseX(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	bool reverseXValue = 0;

	// 获取属性值
	// Get feature value 
	ret = IMV_GetBoolFeatureValue(devHandle, "ReverseX", &reverseXValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,reverseX is %d\n", reverseXValue);

	// 设置属性值
	// Set feature value 
	ret = IMV_SetBoolFeatureValue(devHandle, "ReverseX", !reverseXValue);
	if (IMV_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_GetBoolFeatureValue(devHandle, "ReverseX", &reverseXValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,reverseX is %d\n", reverseXValue);

	return ret;
}

static int modifyCameraDeviceUserID(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	IMV_String stringValue;

	// 获取属性值
	// Get feature value 
	ret = IMV_GetStringFeatureValue(devHandle, "DeviceUserID", &stringValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,deviceUserID is %s\n", stringValue.str);

	// 设置属性值
	// Set feature value 
	ret = IMV_SetStringFeatureValue(devHandle, "DeviceUserID", "Camera");
	if (IMV_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_GetStringFeatureValue(devHandle, "DeviceUserID", &stringValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,deviceUserID is %s\n", stringValue.str);

	return ret;
}

static int modifyCameraTriggerSelector(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	IMV_String enumSymbolValue;

	// 获取属性值
	// Get feature value 
	ret = IMV_GetEnumFeatureSymbol(devHandle, "TriggerSelector", &enumSymbolValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,triggerSelector is %s\n", enumSymbolValue.str);

	// 设置属性值
	// Set feature value 
	ret = IMV_SetEnumFeatureSymbol(devHandle, "TriggerSelector", "FrameStart");
	if (IMV_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_GetEnumFeatureSymbol(devHandle, "TriggerSelector", &enumSymbolValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,triggerSelector is %s\n", enumSymbolValue.str);

	return ret;
}

static int modifyCameraTriggerMode(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	IMV_String enumSymbolValue;

	// 获取属性值
	// Get feature value 
	ret = IMV_GetEnumFeatureSymbol(devHandle, "TriggerMode", &enumSymbolValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,triggerMode is %s\n", enumSymbolValue.str);

	// 设置属性值
	// Set feature value 
	ret = IMV_SetEnumFeatureSymbol(devHandle, "TriggerMode", "On");
	if (IMV_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_GetEnumFeatureSymbol(devHandle, "TriggerMode", &enumSymbolValue);
	if (IMV_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,triggerMode is %s\n", enumSymbolValue.str);

	return ret;
}

static int modifyCameraTriggerSource(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;
	IMV_EnumEntryList enumEntryList;
	unsigned int index = 0;
	uint64_t enumValue = 0;
	unsigned int nEntryNum = 0;

	memset(&enumEntryList, 0, sizeof(enumEntryList));

	do
	{
		// 获取枚举属性的可设枚举值的个数
		//  Get the number of enumeration property settable enumeration
		ret = IMV_GetEnumFeatureEntryNum(devHandle, "TriggerSource", &nEntryNum);
		if (IMV_OK != ret)
		{
			printf("Get settable enumeration number failed! ErrorCode[%d]\n", ret);
			break;
		}

		enumEntryList.nEnumEntryBufferSize = sizeof(IMV_EnumEntryInfo) * nEntryNum;
		enumEntryList.pEnumEntryInfo = (IMV_EnumEntryInfo*)malloc(sizeof(IMV_EnumEntryInfo) * nEntryNum);
		if (NULL == enumEntryList.pEnumEntryInfo)
		{
			printf("Malloc pEnumEntryInfo failed!\n");
			break;
		}

		// 获取枚举属性的可设枚举值列表
		// Get enumeration property's settable enumeration value list
		ret = IMV_GetEnumFeatureEntrys(devHandle, "TriggerSource", &enumEntryList);
		if (IMV_OK != ret)
		{
			printf("Get settable enumeration value list failed! ErrorCode[%d]\n", ret);
			break;
		}

		for (index = 0; index < nEntryNum; index++)
		{
			printf("Enum Entry Name[%s] <-> Enum Entry Value[%llu]\n",
				enumEntryList.pEnumEntryInfo[index].name, enumEntryList.pEnumEntryInfo[index].value);
		}

		// 获取属性值
		// Get feature value 
		ret = IMV_GetEnumFeatureValue(devHandle, "TriggerSource", &enumValue);
		if (IMV_OK != ret)
		{
			printf("Get feature value failed! ErrorCode[%d]\n", ret);
			break;
		}

		for (index = 0; index < nEntryNum; index++)
		{
			if (enumEntryList.pEnumEntryInfo[index].value == enumValue)
			{
				printf("Before change ,triggerSource is %s\n", enumEntryList.pEnumEntryInfo[index].name);
				break;
			}
		}

		for (index = 0; index < nEntryNum; index++)
		{
			if (0 == (memcmp(enumEntryList.pEnumEntryInfo[index].name, "Software", sizeof("Software"))))
			{
				enumValue = enumEntryList.pEnumEntryInfo[index].value;
				break;
			}
		}

		// 设置属性值
		// Set feature value 
		ret = IMV_SetEnumFeatureValue(devHandle, "TriggerSource", enumValue);
		if (IMV_OK != ret)
		{
			printf("Set feature value failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 获取属性值
		// Get feature value 
		ret = IMV_GetEnumFeatureValue(devHandle, "TriggerSource", &enumValue);
		if (IMV_OK != ret)
		{
			printf("Get feature value failed! ErrorCode[%d]\n", ret);
			break;
		}

		for (index = 0; index < nEntryNum; index++)
		{
			if (enumEntryList.pEnumEntryInfo[index].value == enumValue)
			{
				printf("After change ,triggerSource is %s\n", enumEntryList.pEnumEntryInfo[index].name);
				break;
			}
		}
	} while (false);

	if (enumEntryList.pEnumEntryInfo)
	{
		free(enumEntryList.pEnumEntryInfo);
		enumEntryList.pEnumEntryInfo = NULL;
	}

	return ret;

}

static int executeTriggerSoftware(IMV_HANDLE devHandle)
{
	int ret = IMV_OK;

	// 执行命令属性
	// Execute command property
	ret = IMV_ExecuteCommandFeature(devHandle, "TriggerSoftware");
	if (IMV_OK != ret)
	{
		printf("Execute TriggerSoftware failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Execute TriggerSoftware success.\n");

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

		// 修改相机曝光时间，通用double型属性访问实例 
		// set camera's ExposureTime, an example of double value property access 
		if (IMV_OK != modifyCameraExposureTime(devHandle))
		{
			break;
		}

		// 修改相机像素宽度，通用int型属性访问实例 
		// set camera's Width, an example of integer value property access 
		if (IMV_OK != modifyCameraWidth(devHandle))
		{
			break;
		}

		// 修改相机ReverseX，通用bool型属性访问实例 
		// set camera's ReverseX, an example of boolean value property access 
		if (IMV_OK != modifyCameraReverseX(devHandle))
		{
			break;
		}

		// 修改相机DeviceUserID，通用string型属性访问实例 
		// set camera's DeviceUserID, an example of string value property access 
		if (IMV_OK != modifyCameraDeviceUserID(devHandle))
		{
			break;
		}

		// 修改相机TriggerSelector，通用enum型属性访问实例 
		// set camera's TriggerSelector, an example of enumeration value property access 
		if (IMV_OK != modifyCameraTriggerSelector(devHandle))
		{
			break;
		}

		// 修改相机TriggerMode，通用enum型属性访问实例 
		// set camera's TriggerMode, an example of enumeration value property access 
		if (IMV_OK != modifyCameraTriggerMode(devHandle))
		{
			break;
		}

		// 修改相机TriggerSource，通用enum型属性访问实例 
		// set camera's TriggerSource, an example of enumeration value property access 
		if (IMV_OK != modifyCameraTriggerSource(devHandle))
		{
			break;
		}

		// 执行相机TriggerSoftware，通用command型属性访问实例 
		// execute camera's TriggerSoftware, an example of command type property access 
		if (IMV_OK != executeTriggerSoftware(devHandle))
		{
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
