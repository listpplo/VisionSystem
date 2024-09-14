/// \file
/// \~chinese
/// \brief 多相机拉流处理示例
/// \example MultiCameraGrab.cpp
/// \~english
/// \brief stream grabbing sample
/// \example MultiCameraGrab.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>
#include <vector>
#include "Device.h"
#include "iostream"

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

int main()
{
	printf("SDK Version:%s.\r\n", IMV_GetVersion());

	int nRet = IMV_OK;
	DeviceSystem SysDevice;
	SysDevice.initSystem();

	unsigned int cameraIndex = selectDevice(SysDevice.m_DeviceNum);
	do 
	{
		nRet = SysDevice.m_Device[cameraIndex].openDevicebyKey();
		if (IMV_OK != nRet)
		{
			printf("open camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}

		// 相机属性测试(以int为例)
		int64_t width = 0;
		nRet = SysDevice.m_Device[cameraIndex].getIntValue("Width", &width);
		if (IMV_OK != nRet)
		{
			printf("getIntValue camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}

		nRet = SysDevice.m_Device[cameraIndex].setIntValue("Width", width);
		if (IMV_OK != nRet)
		{
			printf("getIntValue camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}

		// 主动取图
		nRet = SysDevice.m_Device[cameraIndex].startGrabbing();
		if (IMV_OK != nRet)
		{
			printf("start grabing camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}
		Sleep(3000);
		nRet = SysDevice.m_Device[cameraIndex].stopGrabbing();
		if (IMV_OK != nRet)
		{
			printf("stop grabing camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}

		printf("start callback grab.\r\n");
		// 被动取图
		nRet = SysDevice.m_Device[cameraIndex].startGrabbingCallback();
		if (IMV_OK != nRet)
		{
			printf("start callback grabing camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}
		Sleep(3000);
		nRet = SysDevice.m_Device[cameraIndex].stopGrabbingCallback();
		if (IMV_OK != nRet)
		{
			printf("stop callback grabing camera[%d] failed[%d].\r\n", cameraIndex, nRet);
			break;
		}

	} while (false);

	SysDevice.m_Device[cameraIndex].closeDevice();
	SysDevice.unInitSystem();

	printf("Press enter key to exit...\r\n");
	getchar();
	return 0;
}
