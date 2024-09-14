/// \file
/// \~chinese
/// \brief 异步取帧示例
/// \example GrabAsynchronous.cpp
/// \~english
/// \brief Asynchronous frame retrieval sample
/// \example GrabAsynchronous.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>
#include <mutex>
#include "IMVFGApi.h"
#include "IMVFGDefines.h"


#define snprintf	_snprintf

#define BMP_HEADER_SIZE		(sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader))
#define Max_Count 5       //存帧队列个数，可动态调节

unsigned int g_defaultImageLen = 0;
bool g_isExitThread = false;
std::mutex g_mutex;

class frameQueue
{
public:
	frameQueue() = default;

	~frameQueue()
	{
		delete[]Qframes;
	}

	int Init(int nBufCount)
	{
		int ret = IMV_FG_OK;
		QMaxlen = nBufCount;
		Qframes = new FrameInfo[nBufCount];
		if (NULL == Qframes)
		{
			printf("Qframes new error!!\r\n");
			return IMV_FG_ERROR;
		}

		for (int i = 0; i < nBufCount; i++)
		{
			ret = Qframes[i].Init();
			if (IMV_FG_OK != ret)
			{
				return ret;
			}
		}
		return ret;
	}

	int push(uint64_t nBlockId, int nHeight, int nWidth, unsigned char *pData, int nFrameLen)
	{
		g_mutex.lock();
		if (QCurrentSize == QMaxlen)
		{
			g_mutex.unlock();
			return IMV_FG_ERROR;
		}

		QCurrentSize++;
		Qframes[end].blockId = nBlockId;
		Qframes[end].height = nHeight;
		Qframes[end].width = nWidth;
		Qframes[end].size = nFrameLen;

		//考虑变帧的情况
		if (Qframes[end].nQMemberSize < nFrameLen)
		{
			free(Qframes[end].data);
			Qframes[end].data = NULL;

			Qframes[end].data = (unsigned char*)malloc(nFrameLen);
			if (NULL == Qframes[end].data)
			{
				g_mutex.unlock();
				return IMV_FG_ERROR;
			}

			Qframes[end].nQMemberSize = nFrameLen;
		}

		if (NULL != Qframes[end].data && NULL != pData)
		{
			memcpy(Qframes[end].data, pData, nFrameLen);
		}
		else{
			printf("data ptr invalid!!\r\n");
			return IMV_FG_ERROR;
		}

		end = (end + 1) % QMaxlen;
		g_mutex.unlock();

		return IMV_FG_OK;
	}

	int pop(uint64_t *pBlockId, unsigned char *pData)
	{
		g_mutex.lock();
		if (QCurrentSize == 0)
		{
			g_mutex.unlock();
			return IMV_FG_ERROR;
		}

		*pBlockId = Qframes[start].blockId;

		if (NULL != pData && NULL != Qframes[start].data)
		{
			memcpy(pData, Qframes[start].data, Qframes[start].size);
		}
		else
		{
			printf("data ptr invalid!!\r\n");
			return IMV_FG_ERROR;
		}

		QCurrentSize--;
		start = (start + 1) % QMaxlen;
		g_mutex.unlock();

		return IMV_FG_OK;
	}

private:
	class FrameInfo {
	public:
		FrameInfo() = default;

		~FrameInfo()
		{
			if (data != NULL)
			{
				free(data);
			}
		}

		int Init()
		{
			if (NULL != data)
			{
				printf("data != NULL\n");
				free(data);
				data = NULL;
			}

			data = (unsigned char*)malloc(g_defaultImageLen);
			if (NULL == data)
			{
				printf("data malloc failed!!\r\n");
				return IMV_FG_ERROR;
			}
			nQMemberSize = g_defaultImageLen;
			return IMV_FG_OK;
		}

	public:
		uint64_t blockId = 0;
		int width = 0;
		int height = 0;
		int size = 0;
		unsigned char* data = NULL;

		int nQMemberSize = 0;
	};

	FrameInfo *Qframes = NULL;
	int QCurrentSize = 0;
	int start = 0;
	int end = 0;
	int QMaxlen = 0;
};

frameQueue *m_queue = NULL;

// 数据帧回调函数
// Data frame callback function
static void onGetFrame(IMV_FG_Frame* pFrame, void* pUser)
{
	int nRet = IMV_FG_OK;

	if (pFrame == NULL)
	{
		printf("pFrame is NULL\n");
		return;
	}

	//将帧存入队列
	nRet = m_queue->push(pFrame->frameInfo.blockId, pFrame->frameInfo.width, pFrame->frameInfo.height, pFrame->pData, pFrame->frameInfo.size);
	if (IMV_FG_OK != nRet)
	{
		printf("Add frame to list failed!! blockId is [%llu].\r\n", pFrame->frameInfo.blockId);
	}
	else
	{
		printf("Add frame to list success!! blockId is [%llu].\r\n", pFrame->frameInfo.blockId);
	}

	return;
}

unsigned __stdcall getFrameProc(void* pUserData)
{
	int ret = IMV_FG_OK;
	IMV_FG_IF_HANDLE ifHandle = (IMV_FG_IF_HANDLE)pUserData;
	unsigned int ndefaultImageLen = 0;

	if (NULL == ifHandle)
	{
		printf("ifHandle is NULL\r\n");
		return IMV_FG_ERROR;
	}

	ndefaultImageLen = g_defaultImageLen;
	unsigned char *pOutData = (unsigned char *)malloc(g_defaultImageLen);
	if (NULL == pOutData)
	{
		printf("saveImageProc malloc size [%d] failed. \r\n", g_defaultImageLen);
		return  IMV_FG_ERROR;
	}

	while (!g_isExitThread)
	{
		//考虑变帧的情况
		if (ndefaultImageLen < g_defaultImageLen)
		{
			free(pOutData);
			pOutData = NULL;

			pOutData = (unsigned char*)malloc(g_defaultImageLen);
			if (NULL == pOutData)
			{
				printf("ndefaultImageLen < g_defaultImageLen! malloc fail\r\n");
				return IMV_FG_ERROR;
			}

			ndefaultImageLen = g_defaultImageLen;
		}

		uint64_t blockId = 0;
		ret = m_queue->pop(&blockId, pOutData);
		if (IMV_FG_OK != ret)
		{
			printf("Pop failed, maybe no data. \r\n");
			Sleep(2);
			continue;
		}
		else
		{
			printf("Pop success BlockId is [%d].\r\n", blockId);
		}
	}

	if (pOutData)
	{
		free(pOutData);
		pOutData = NULL;
	}

	return 0;
}

// 计算帧缓存大小
// Calculate frame cache size
static int getFrameSize(IMV_FG_IF_HANDLE ifHandle)
{
	int ret = IMV_FG_OK;
	int64_t widthVal = 0;
	int64_t heightVal = 0;

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

	g_defaultImageLen = sizeof(unsigned char) * (int)widthVal * (int)heightVal * 3;

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
	HANDLE threadHandle = NULL;

	do
	{
		//创建队列
		m_queue = new frameQueue();
		if (NULL == m_queue)
		{
			printf("m_queue new error!!\r\n");
			break;
		}

		printf("Enum capture board interface info.\r\n");
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

		// 计算帧缓存大小
		// Calculate frame cache size
		nRet = getFrameSize(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Get frame size falied.\r\n");
			break;
		}

		//初始化队列
		//Initiation queue
		nRet = m_queue->Init(Max_Count);
		if (IMV_FG_OK != nRet)
		{
			printf("Init frameQueue failed! ErrorCode[%d]\n", nRet);
			break;
		}

		printf("attach Grabbing.\r\n");
		// 注册数据帧回调函数
		// Register data frame callback function
		nRet = IMV_FG_AttachGrabbing(ifHandle, onGetFrame, NULL);
		if (IMV_FG_OK != nRet)
		{
			printf("Attach grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}

		// 创建从队列取帧线程 
		// Create frame grabbing thread 
		threadHandle = (HANDLE)_beginthreadex(NULL,
			0,
			getFrameProc,
			(void*)ifHandle,
			CREATE_SUSPENDED,
			NULL);

		if (!threadHandle)
		{
			printf("Failed to create getFrame thread!\n");
			break;
		}

		ResumeThread(threadHandle);

		// 开始拉流 
		// Start grabbing 
		nRet = IMV_FG_StartGrabbing(ifHandle);
		if (IMV_FG_OK != nRet)
		{
			printf("Start grabbing failed! errorCode:[%d]\r\n", nRet);
			break;
		}
		printf("Grabbing... get frame 5 seconds.");
		// 取图5秒
		// get frame 5 seconds
		Sleep(5000);

		// 退出取帧线程 
		// Stop Framing thread 
		g_isExitThread = true;

		WaitForSingleObject(threadHandle, INFINITE);
		CloseHandle(threadHandle);

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

	if (NULL != m_queue)
	{
		delete m_queue;
	}

	printf("Press enter key to exit...\n");
	getchar();
	return 0;
}
