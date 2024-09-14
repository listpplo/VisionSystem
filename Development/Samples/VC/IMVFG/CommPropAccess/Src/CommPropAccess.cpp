/// \file
/// \~chinese
/// \brief 通用属性访问接口使用示例
/// \example CommPropAccess.cpp
/// \~english
/// \brief common property access interface sample
/// \example CommPropAccess.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "IMVFGApi.h"
#include "IMVFGDefines.h"


static int modifyCameraExposureTime(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	double exposureTimeValue = 0.0;
	double exposureMinValue = 0;
	double exposureMaxValue = 0;

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetDoubleFeatureValue(camHandle, "ExposureTime", &exposureTimeValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,exposureTime is %0.2f\n", exposureTimeValue);

	// 获取属性可设的最小值
	// Get property's settable minimum value
	ret = IMV_FG_GetDoubleFeatureMin(camHandle, "ExposureTime", &exposureMinValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature minimum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("exposureTime settable minimum value is %0.2f\n", exposureMinValue);

	// 获取属性可设的最大值
	// Get property's settable maximum value
	ret = IMV_FG_GetDoubleFeatureMax(camHandle, "ExposureTime", &exposureMaxValue);
	if (IMV_FG_OK != ret)
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
	ret = IMV_FG_SetDoubleFeatureValue(camHandle, "ExposureTime", exposureTimeValue);
	if (IMV_FG_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetDoubleFeatureValue(camHandle, "ExposureTime", &exposureTimeValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,exposureTime is %0.2f\n", exposureTimeValue);

	return ret;
}

static int modifyWidth(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	int64_t widthValue = 0;
	int64_t widthMinValue = 0;
	int64_t widthMaxValue = 0;
	int64_t incrementValue = 0;

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetIntFeatureValue(camHandle, "Width", &widthValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get Width value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,width is %lld\n", widthValue);

	// 获取属性可设的最小值
	// Get property's settable minimum value
	ret = IMV_FG_GetIntFeatureMin(camHandle, "Width", &widthMinValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get Width minimum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("width settable minimum value is %lld\n", widthMinValue);

	// 获取属性可设的最大值
	// Get property's settable maximum value
	ret = IMV_FG_GetIntFeatureMax(camHandle, "Width", &widthMaxValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get Width maximum value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("width settable maximum value is %lld\n", widthMaxValue);

	// 获取属性步长
	// Get feature increment
	ret = IMV_FG_GetIntFeatureInc(camHandle, "Width", &incrementValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get Width increment value failed! ErrorCode[%d]\n", ret);
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
	ret = IMV_FG_SetIntFeatureValue(camHandle, "Width", widthValue);
	if (IMV_FG_OK != ret)
	{
		printf("Set Width value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetIntFeatureValue(camHandle, "Width", &widthValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get Width value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,width is %lld\n", widthValue);

	return ret;
}

static int modifyCameraReverseX(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	bool reverseXValue = 0;

	if (!IMV_FG_FeatureIsValid(camHandle, "ReverseX") || !IMV_FG_FeatureIsAvailable(camHandle, "ReverseX") ||
		!IMV_FG_FeatureIsReadable(camHandle, "ReverseX") || !IMV_FG_FeatureIsWriteable(camHandle, "ReverseX"))
	{
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetBoolFeatureValue(camHandle, "ReverseX", &reverseXValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,reverseX is %d\n", reverseXValue);

	// 设置属性值
	// Set feature value 
	ret = IMV_FG_SetBoolFeatureValue(camHandle, "ReverseX", !reverseXValue);
	if (IMV_FG_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetBoolFeatureValue(camHandle, "ReverseX", &reverseXValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,reverseX is %d\n", reverseXValue);

	return ret;
}

static int modifyDeviceUserID(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	IMV_FG_String stringValue;

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetStringFeatureValue(camHandle, "DeviceUserID", &stringValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,deviceUserID is %s\n", stringValue.str);

	// 设置属性值
	// Set feature value 
	ret = IMV_FG_SetStringFeatureValue(camHandle, "DeviceUserID", "Camera");
	if (IMV_FG_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetStringFeatureValue(camHandle, "DeviceUserID", &stringValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,deviceUserID is %s\n", stringValue.str);

	return ret;
}

static int modifyCameraTriggerSelector(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	IMV_FG_String enumSymbolValue;

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetEnumFeatureSymbol(camHandle, "TriggerSelector", &enumSymbolValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,triggerSelector is %s\n", enumSymbolValue.str);

	// 设置属性值
	// Set feature value 
	ret = IMV_FG_SetEnumFeatureSymbol(camHandle, "TriggerSelector", "FrameStart");
	if (IMV_FG_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetEnumFeatureSymbol(camHandle, "TriggerSelector", &enumSymbolValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,triggerSelector is %s\n", enumSymbolValue.str);

	return ret;
}

static int modifyCameraTriggerMode(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	IMV_FG_String enumSymbolValue;

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetEnumFeatureSymbol(camHandle, "TriggerMode", &enumSymbolValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Before change ,triggerMode is %s\n", enumSymbolValue.str);

	// 设置属性值
	// Set feature value 
	ret = IMV_FG_SetEnumFeatureSymbol(camHandle, "TriggerMode", "On");
	if (IMV_FG_OK != ret)
	{
		printf("Set feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	// 获取属性值
	// Get feature value 
	ret = IMV_FG_GetEnumFeatureSymbol(camHandle, "TriggerMode", &enumSymbolValue);
	if (IMV_FG_OK != ret)
	{
		printf("Get feature value failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("After change ,triggerMode is %s\n", enumSymbolValue.str);

	return ret;
}

static int modifyCameraTriggerSource(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;
	IMV_FG_EnumEntryList enumEntryList;
	unsigned int index = 0;
	uint64_t enumValue = 0;
	unsigned int nEntryNum = 0;

	memset(&enumEntryList, 0, sizeof(enumEntryList));

	do
	{
		// 获取枚举属性的可设枚举值的个数
		//  Get the number of enumeration property settable enumeration
		ret = IMV_FG_GetEnumFeatureEntryNum(camHandle, "TriggerSource", &nEntryNum);
		if (IMV_FG_OK != ret)
		{
			printf("Get settable enumeration number failed! ErrorCode[%d]\n", ret);
			break;
		}

		enumEntryList.nEnumEntryBufferSize = sizeof(IMV_FG_EnumEntryInfo) * nEntryNum;
		enumEntryList.pEnumEntryInfo = (IMV_FG_EnumEntryInfo*)malloc(sizeof(IMV_FG_EnumEntryInfo) * nEntryNum);
		if (NULL == enumEntryList.pEnumEntryInfo)
		{
			printf("Malloc pEnumEntryInfo failed!\n");
			break;
		}

		// 获取枚举属性的可设枚举值列表
		// Get enumeration property's settable enumeration value list
		ret = IMV_FG_GetEnumFeatureEntrys(camHandle, "TriggerSource", &enumEntryList);
		if (IMV_FG_OK != ret)
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
		ret = IMV_FG_GetEnumFeatureValue(camHandle, "TriggerSource", &enumValue);
		if (IMV_FG_OK != ret)
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
		ret = IMV_FG_SetEnumFeatureValue(camHandle, "TriggerSource", enumValue);
		if (IMV_FG_OK != ret)
		{
			printf("Set feature value failed! ErrorCode[%d]\n", ret);
			break;
		}

		// 获取属性值
		// Get feature value 
		ret = IMV_FG_GetEnumFeatureValue(camHandle, "TriggerSource", &enumValue);
		if (IMV_FG_OK != ret)
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

static int executeTriggerSoftware(IMV_FG_DEV_HANDLE camHandle)
{
	int ret = IMV_FG_OK;

	// 执行命令属性
	// Execute command property
	ret = IMV_FG_ExecuteCommandFeature(camHandle, "TriggerSoftware");
	if (IMV_FG_OK != ret)
	{
		printf("Execute TriggerSoftware failed! ErrorCode[%d]\n", ret);
		return ret;
	}

	printf("Execute TriggerSoftware success.\n");

	return ret;
}

static int modifyCxpInterfaceProperty(IMV_FG_IF_HANDLE handle)
{
	int ret = IMV_FG_OK;
	printf("begin modifyCxpInterfaceProperty. \r\n");
	do
	{
		//判断属性是否有效可流可读可写
		//Check feature IsAvailable、IsReadable、IsWriteable、IsStreamable、IsValid
		if (IMV_FG_FeatureIsValid(handle, "InterfaceID"))
			printf("Feature InterfaceID Is Valid.\n");
		else
			printf("Feature InterfaceID Not Valid.\n");

		if (IMV_FG_FeatureIsAvailable(handle, "InterfaceID"))
			printf("Feature InterfaceID Is Available.\n");
		else
			printf("Feature InterfaceID Not Available.\n");

		if (IMV_FG_FeatureIsReadable(handle, "InterfaceID"))
			printf("Feature InterfaceID Is Readable.\n");
		else
			printf("Feature InterfaceID Not Readable.\n");

		if (IMV_FG_FeatureIsWriteable(handle, "InterfaceID"))
			printf("Feature InterfaceID Is Writeable.\n");
		else
			printf("Feature InterfaceID Not Writeable.\n");

		if (IMV_FG_FeatureIsStreamable(handle, "InterfaceID"))
			printf("Feature InterfaceID Is Streamable.\n");
		else
			printf("Feature InterfaceID Not Streamable.\n");

		//获取属性类型
		//Get feature type
		IMV_FG_EFeatureType valueType = IMV_FG_EFeatureType::IMV_FG_FEATURE_UNDEFINED;
		IMV_FG_GetFeatureType(handle, "InterfaceID", &valueType);
		switch (valueType)
		{
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_BOOL:printf("Feature Type: featureBool.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_COMMAND:printf("Feature Type: featureCommand.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_ENUM:printf("Feature Type: featureEnum.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_FLOAT:printf("Feature Type: featureFloat.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_GROUP:printf("Feature Type: featureGroup.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_INT:printf("Feature Type: featureInt.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_REG:printf("Feature Type: featureReg.\n"); break;
		case IMV_FG_EFeatureType::IMV_FG_FEATURE_STRING:printf("Feature Type: featureString.\n"); break;
		default:
			printf("Feature Type: featureUndefined.\n");
			break;
		}

		IMV_FG_String stringValue;
		// 获取属性值
		// Get feature value 
		ret = IMV_FG_GetStringFeatureValue(handle, "InterfaceID", &stringValue);
		if (IMV_FG_OK != ret)
		{
			printf("Get InterfaceID value failed! ErrorCode[%d]\n", ret);
			break;
		}
		printf("interface id is %s.\r\n", stringValue);
	} while (false);

	printf("end modifyCxpInterfaceProperty.\r\n");
	return ret;
}

static int modifyCxpDeviceProperty(IMV_FG_IF_HANDLE handle)
{
	int ret = IMV_FG_OK;
	printf("begin modifyCxpDeviceProperty.\r\n");
	do
	{
		IMV_FG_String stringValue;
		// 获取属性值
		// Get feature value 
		ret = IMV_FG_GetStringFeatureValue(handle, "DeviceID", &stringValue);
		if (IMV_FG_OK != ret)
		{
			printf("Get DeviceID value failed! ErrorCode[%d]\n", ret);
			break;
		}
		printf("DeviceID is %s.\r\n", stringValue);

		double doubleValue = 0.0;
		// 获取属性值
		// Get feature value 
		ret = IMV_FG_GetDoubleFeatureValue(handle, "DeviceExposureTime", &doubleValue);
		if (IMV_FG_OK != ret)
		{
			printf("Get DeviceExposureTime value failed! ErrorCode[%d]\n", ret);
			break;
		}

		printf("Before change ,DeviceExposureTime is %0.2f\n", doubleValue);

		int64_t intValue = 0;
		// 获取属性值
		// Get feature value 
		ret = IMV_FG_GetIntFeatureValue(handle, "DevicePulseNumber", &intValue);
		if (IMV_FG_OK != ret)
		{
			printf("Get DevicePulseNumber value failed! ErrorCode[%d]\n", ret);
			break;
		}

		printf("Before change ,DevicePulseNumber is %lld\n", intValue);

		// 设置属性值
		// Set feature value 
		uint64_t enumValue = 0;
		ret = IMV_FG_GetEnumFeatureValue(handle, "CardioInMask", &enumValue);
		if (IMV_FG_OK != ret)
		{
			printf("Set CardioInMask value failed! ErrorCode[%d]\n", ret);
			break;
		}
		printf("Before change ,CardioInMask is %d\n", enumValue);
	} while (false);
	printf("end modifyCxpDeviceProperty. \r\n");
	return ret;
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

		// 修改相机曝光时间，通用double型属性访问实例 
		// set camera's ExposureTime, an example of double value property access 
		if (IMV_FG_OK != modifyCameraExposureTime(devHandle))
		{
			break;
		}

		// 修改采集卡像素宽度，通用int型属性访问实例 
		// set capture baord's Width, an example of integer value property access 
		if (IMV_FG_OK != modifyWidth(ifHandle))
		{
			break;
		}

		// 修改相机像素宽度，通用int型属性访问实例 
		// set camera's Width, an example of integer value property access 
		if (IMV_FG_OK != modifyWidth(devHandle))
		{
			break;
		}

		// 修改相机ReverseX，通用bool型属性访问实例 
		// set camera's ReverseX, an example of boolean value property access 
		if (IMV_FG_OK != modifyCameraReverseX(devHandle))
		{
			break;
		}

		// 修改采集卡DeviceUserID，通用string型属性访问实例 
		// set capture baord's DeviceUserID, an example of string value property access 
		if (IMV_FG_OK != modifyDeviceUserID(ifHandle))
		{
			break;
		}

		// 修改相机DeviceUserID，通用string型属性访问实例 
		// set camera's DeviceUserID, an example of string value property access 
		if (IMV_FG_OK != modifyDeviceUserID(devHandle))
		{
			break;
		}

		// 修改相机TriggerSelector，通用enum型属性访问实例 
		// set camera's TriggerSelector, an example of enumeration value property access 
		if (IMV_FG_OK != modifyCameraTriggerSelector(devHandle))
		{
			break;
		}

		// 修改相机TriggerMode，通用enum型属性访问实例 
		// set camera's TriggerMode, an example of enumeration value property access 
		if (IMV_FG_OK != modifyCameraTriggerMode(devHandle))
		{
			break;
		}

		// 修改采集卡TriggerSource，通用enum型属性访问实例 
		// set camera's TriggerSource, an example of enumeration value property access 
		if (IMV_FG_OK != modifyCameraTriggerSource(devHandle))
		{
			break;
		}

		// 执行相机TriggerSoftware，通用command型属性访问实例 
		// execute capture baord's TriggerSoftware, an example of command type property access 
		if (IMV_FG_OK != executeTriggerSoftware(ifHandle))
		{
			break;
		}

		// 执行相机TriggerSoftware，通用command型属性访问实例 
		// execute camera's TriggerSoftware, an example of command type property access 
		if (IMV_FG_OK != executeTriggerSoftware(devHandle))
		{
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
