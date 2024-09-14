#ifndef __IMV_FG_API_H__
#define __IMV_FG_API_H__

#include "IMVFGDefines.h"
/// \~chinese
/// \brief 动态库导入导出定义
/// \~english
/// \brief Dynamic library import and export definition
#if (defined (_WIN32) || defined(WIN64))
#ifdef SUPPORT_CAPTURE_BOARD
		#define IMV_FG_API _declspec(dllexport)
	#else
		#define IMV_FG_API _declspec(dllimport)
	#endif

	#define IMV_FG_CALL __stdcall
#else
	#define IMV_FG_API
	#define IMV_FG_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif 

/// \~chinese
/// \brief 获取版本信息
/// \return 成功时返回版本信息，失败时返回NULL
/// \~english
/// \brief get version information
/// \return Success, return version info. Failure, return NULL 
IMV_FG_API const char* IMV_FG_CALL IMV_FG_GetVersion(void);

/// \~chinese
/// \brief 枚举采集卡设备
/// \param nInterfaceType	[IN] 枚举设备接口类型
/// \param pInterfaceList	[OUT] 采集卡设备列表
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
///	/// \~english
/// \brief Enumerate capture card devices
///	\param nInterfaceType	[IN] interface Type
/// \param pInterfaceList	[OUT] capture card Info List
/// \return Success, return version info. Failure, return NULL 
IMV_FG_API int IMV_FG_CALL IMV_FG_EnumInterface(IN unsigned int nInterfaceType, OUT IMV_FG_INTERFACE_INFO_LIST* pInterfaceList);

/// \~chinese
/// \brief  打开采集卡设备
/// \param nIndex [IN] 采集卡序号
/// \param hIFDev [OUT] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief  Open IMV_FGDevice
/// \param nIndex [IN] capture card device Index
/// \param hIFDev [OUT] capture card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_OpenInterface(IN unsigned int nIndex, OUT IMV_FG_IF_HANDLE* hIFDev);

/// \~chinese
/// \brief 打开采集卡设备
/// \param mode			[IN] 创建句柄模式
///	\param pIdentifier	[IN] 打开采集卡设备标识
/// \param hIFDev		[OUT] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief  Open camera device
/// \param mode			[IN] create handle mode
///	\param pIdentifier	[IN] open card device identifier
/// \param hDev			[OUT] card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_OpenInterfaceEx(IN IMV_FG_ECreateHandleMode mode, IN void* pIdentifier, OUT IMV_FG_IF_HANDLE* hIFDev);

/// \~chinese
/// \brief 判断采集卡设备是否已打开
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 打开状态，返回true；关闭状态或者掉线状态，返回false
/// \~english
/// \brief Check capture card is opened or not
/// \param hIFDev [IN] capture card device handle
/// \return Opened, return true. Closed or Offline, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_IsOpenInterface(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief  关闭采集卡设备
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Close Device
/// \param hIFDev [IN] capture card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_CloseInterface(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief 枚举相机设备
/// \param nInterfaceType	[IN] 枚举设备接口类型
/// \param pDeviceList		[OUT] 相机列表
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Enumerate camera devices
///	\param nInterfaceType	[IN] interface Type
/// \param pDeviceList		[OUT] Camera Info List
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_EnumDevices(IN unsigned int nInterfaceType, OUT IMV_FG_DEVICE_INFO_LIST* pDeviceList);

/// \~chinese
/// \brief 打开相机设备
/// \param mode			[IN] 创建句柄模式
///	\param pIdentifier	[IN] 打开相机标识
/// \param hDev			[OUT] 相机句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief  Open camera device
/// \param mode			[IN] create handle mode
///	\param pIdentifier	[IN] open identifier
/// \param hDev			[OUT] camera device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_OpenDevice(IN IMV_FG_ECreateHandleMode mode, IN void* pIdentifier, OUT IMV_FG_DEV_HANDLE* hDev);

/// \~chinese
/// \brief 打开相机设备（支持相机属性自动同步到采集卡功能，如宽，高，tap，像素格式）
/// \param mode			[IN] 创建句柄模式
///	\param pIdentifier	[IN] 打开相机标识
/// \param hDev			[OUT] 相机句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Open camera device（Support automatic synchronization of camera feature to acquisition card function. eg:width, height, tap, pixelForamt）
/// \param mode			[IN] create handle mode
///	\param pIdentifier	[IN] open identifier
/// \param hDev			[OUT] camera device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_OpenDeviceEx(IN IMV_FG_ECreateHandleMode mode, IN void* pIdentifier, OUT IMV_FG_DEV_HANDLE* hDev);

/// \~chinese
/// \brief 判断相机设备是否已打开
/// \param hDev [IN] 相机设备句柄
/// \return 打开状态，返回true；关闭状态或者掉线状态，返回false
/// \~english
/// \brief Check camera device is opened or not
/// \param hDev [IN] camera device handle
/// \return Opened, return true. Closed or Offline, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_IsDeviceOpen(IN IMV_FG_DEV_HANDLE hDev);

/// \~chinese
/// \brief 关闭相机设备
/// \param hDev [IN] 相机设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Close camera device
/// \param hDev [IN] camera device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_CloseDevice(IN IMV_FG_DEV_HANDLE hDev);

/// \~chinese
/// \brief 设置帧数据缓存个数(不能小于2)
/// \param hIFDev	[IN] 采集卡设备句柄
/// \param nSize	[IN] 缓存数量
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 不能在拉流过程中设置
/// \~english
/// \brief Set frame buffer count(Cannot be less than 2)
/// \param hIFDev	[IN] capture card device handle
/// \param nSize	[IN] The buffer count
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// It can not be set during frame grabbing
IMV_FG_API int IMV_FG_CALL IMV_FG_SetBufferCount(IN IMV_FG_IF_HANDLE hIFDev, IN unsigned int nSize);

/// \~chinese
/// \brief 开始取流
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Start grabbing
/// \param hIFDev [IN] capture card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_StartGrabbing(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief 开始取流(CL专用)
/// \param hIFDev [IN] 采集卡设备句柄
/// \param maxImagesGrabbed [IN] 允许最多的取帧数，达到指定取帧数后停止取流，如果为0，表示忽略此参数连续取流(IMV_FG_StartGrabbing默认0)
/// \param strategy [IN] 取流策略,(IMV_FG_StartGrabbing默认使用grabStrartegySequential策略取流)
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Start grabbing(CL Only)
/// \param hIFDev [IN] capture card device handle
/// \param maxImagesGrabbed [IN] Maximum images allowed to grab, once it reaches the limit then stop grabbing; 
/// If it is 0, then ignore this parameter and start grabbing continuously(default 0 in IMV_FG_StartGrabbing)
/// \param strategy [IN] Image grabbing strategy; (Default grabStrartegySequential in IMV_FG_StartGrabbing)
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_StartGrabbingEx(IN IMV_FG_IF_HANDLE hIFDev, IN uint64_t maxImagesGrabbed, IN IMV_FG_EGrabStrategy strategy);

/// \~chinese
/// \brief 判断设备是否正在取流
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 正在取流，返回true；不在取流，返回false
/// \~english
/// \brief Check whether device is grabbing or not
/// \param hIFDev [IN] capture card device handle
/// \return Grabbing, return true. Not grabbing, return false 
IMV_FG_API bool IMV_FG_CALL  IMV_FG_IsGrabbing(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief 停止取流
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Stop grabbing
/// \param hIFDev [IN] capture card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_StopGrabbing(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief 注册帧数据回调函数(异步获取帧数据机制)
/// \param hIFDev	[IN] 采集卡设备句柄
/// \param proc		[IN] 帧数据信息回调函数，建议不要在该函数中处理耗时的操作，否则会阻塞后续帧数据的实时性
/// \param pUser	[IN] 用户自定义数据, 可设为NULL
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 该异步获取帧数据机制和同步获取帧数据机制(IMV_FG_GetFrame)互斥，对于同一设备，系统中两者只能选其一\n
/// 只支持一个回调函数, 且设备关闭后，注册会失效，打开设备后需重新注册
/// \~english
/// \brief Register frame data callback function( asynchronous getting frame data mechanism);
/// \param hIFDev	[IN] capture card device handle or camera device handle
/// \param proc		[IN] Frame data information callback function; It is advised to not put time-cosuming operation in this function, 
/// otherwise it will block follow-up data frames and affect real time performance
/// \param pUser	[IN] User defined data，It can be set to NULL
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// This asynchronous getting frame data mechanism and synchronous getting frame data mechanism(IMV_FG_GetFrame) are mutually exclusive,\n
/// only one method can be choosed between these two in system for the same device.\n
/// Only one call back function is supported.\n
/// Registration becomes invalid after the device is closed, , and need to re-register after the device is opened
IMV_FG_API int IMV_FG_CALL IMV_FG_AttachGrabbing(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_FrameCallBack proc, IN void* pUser);

/// \~chinese
/// \brief 获取一帧图像(同步获取帧数据机制)
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pFrame [OUT] 帧数据信息
/// \param timeoutMS [IN] 获取一帧图像的超时时间,INFINITE时表示无限等待,直到收到一帧数据或者停止取流。单位是毫秒
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 该接口不支持多线程调用。\n
/// 该同步获取帧机制和异步获取帧机制(IMV_FG_AttachGrabbing)互斥,对于同一设备，系统中两者只能选其一。\n
/// 使用内部缓存获取图像，需要IMV_FG_ReleaseFrame进行释放图像缓存。
/// \~english
/// \brief Get a frame image(synchronous getting frame data mechanism)
/// \param hIFDev [IN] capture card device handle
/// \param pFrame [OUT] Frame data information
/// \param timeoutMS [IN] The time out of getting one image, INFINITE means infinite wait until the one frame data is returned or stop grabbing.unit is MS
/// \return Success, return IMV_FG_OK. Failure, return error code 
/// \remarks
/// This interface does not support multi-threading.\n
/// This synchronous getting frame data mechanism and asynchronous getting frame data mechanism(IMV_FG_AttachGrabbing) are mutually exclusive,\n
/// only one method can be chose between these two in system for the same device.\n
/// Use internal cache to get image, need to release image buffer by IMV_FG_ReleaseFrame
IMV_FG_API int IMV_FG_CALL IMV_FG_GetFrame(IN IMV_FG_IF_HANDLE hIFDev, OUT IMV_FG_Frame* pFrame, IN unsigned int timeoutMS);

/// \~chinese
/// \brief 释放图像缓存
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pFrame [IN] 帧数据信息
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Free image buffer
/// \param hIFDev [IN] capture card device handle
/// \param pFrame [IN] Frame image data information
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_ReleaseFrame(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_Frame* pFrame);

/// \~chinese
/// \brief 帧数据深拷贝克隆
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pFrame [IN] 克隆源帧数据信息
/// \param pCloneFrame [OUT] 新的帧数据信息
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 使用IMV_FG_ReleaseFrame进行释放图像缓存。
/// \~english
/// \brief Frame data deep clone
/// \param hIFDev [IN] capture card device handle
/// \param pFrame [IN] Frame data information of clone source
/// \param pCloneFrame [OUT] New frame data information
/// \return Success, return IMV_FG_OK. Failure, return error code 
/// \remarks
/// Use IMV_FG_ReleaseFrame to free image buffer
IMV_FG_API int IMV_FG_CALL IMV_FG_CloneFrame(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_Frame* pFrame, OUT IMV_FG_Frame* pCloneFrame);

/// \~chinese
/// \brief 获取流统计信息(IMV_StartGrabbing / IMV_StartGrabbing执行后调用)
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pStreamStatsInfo [OUT] 流统计信息数据
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get stream statistics infomation(Used after excuting IMV_StartGrabbing / IMV_StartGrabbing)
/// \param hIFDev [IN] capture card device handle
/// \param pStreamStatsInfo [OUT] Stream statistics infomation
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetStatisticsInfo(IN IMV_FG_IF_HANDLE hIFDev, OUT IMV_FG_StreamStatisticsInfo* pStreamStatsInfo);

/// \~chinese
/// \brief 重置流统计信息(IMV_StartGrabbing / IMV_StartGrabbing执行后调用)
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Reset stream statistics infomation(Used after excuting IMV_StartGrabbing / IMV_StartGrabbing)
/// \param hIFDev [IN] capture card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_ResetStatisticsInfo(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief 下载设备描述XML文件，并保存到指定路径，如：D:\\xml.zip(CXP采集卡当前只支持下载相机配置)
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFullFileName [IN] 文件要保存的路径
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Download device description XML file, and save the files to specified path.  e.g. D:\\xml.zip(cxp capture card only support download the configuration of the camera device)
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFullFileName [IN] The full paths where the downloaded XMl files would be saved to
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_DownLoadGenICamXML(IN HANDLE handle, IN const char* pFullFileName);

/// \~chinese
/// \brief 保存设备配置到指定的位置。同名文件已存在时，覆盖。(CXP采集卡当前只支持保存相机配置)
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFullFileName [IN] 导出的设备配置文件全名(含路径)，如：D:\\config.xml 或 D:\\config.mvcfg
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english 
/// \brief Save the configuration of the device. Overwrite the file if exists.(cxp capture card only support save the configuration of the camera device)
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFullFileName [IN] The full path name of the property file(xml).  e.g. D:\\config.xml or D:\\config.mvcfg
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SaveDeviceCfg(IN HANDLE handle, IN const char* pFullFileName);

/// \~chinese
/// \brief 从文件加载设备xml配置(CXP采集卡当前只支持加载相机配置)
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFullFileName [IN] 设备配置(xml)文件全名(含路径)，如：D:\\config.xml 或 D:\\config.mvcfg
/// \param pErrorList [OUT] 加载失败的属性名列表。存放加载失败的属性上限为IMV_MAX_ERROR_LIST_NUM。
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english 
/// \brief load the configuration of the device(cxp capture card only support load the configuration of the camera device)
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFullFileName [IN] The full path name of the property file(xml). e.g. D:\\config.xml or D:\\config.mvcfg
/// \param pErrorList [OUT] The list of load failed properties. The failed to load properties list up to IMV_MAX_ERROR_LIST_NUM.
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_LoadDeviceCfg(IN HANDLE handle, IN const char* pFullFileName, OUT IMV_FG_ErrorList* pErrorList);

/// \~chinese
/// \brief 判断属性是否可用
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \return 可用，返回true；不可用，返回false
/// \~english
/// \brief Check the property is available or not
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return Available, return true. Not available, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_FeatureIsAvailable(IN HANDLE handle, IN const char* pFeatureName);

/// \~chinese
/// \brief 判断属性是否可读
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \return 可读，返回true；不可读，返回false
/// \~english
/// \brief Check the property is readable or not
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return Readable, return true. Not readable, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_FeatureIsReadable(IN HANDLE handle, IN const char* pFeatureName);

/// \~chinese
/// \brief 判断属性是否可写
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \return 可写，返回true；不可写，返回false
/// \~english
/// \brief Check the property is writeable or not
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return Writeable, return true. Not writeable, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_FeatureIsWriteable(IN HANDLE handle, IN const char* pFeatureName);

/// \~chinese
/// \brief 判断属性是否可流
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \return 可流，返回true；不可流，返回false
/// \~english
/// \brief Check the property is streamable or not
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return Streamable, return true. Not streamable, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_FeatureIsStreamable(IN HANDLE handle, IN const char* pFeatureName);

/// \~chinese
/// \brief 判断属性是否有效
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \return 有效，返回true；无效，返回false
/// \~english
/// \brief Check the property is valid or not
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return Valid, return true. Invalid, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_FeatureIsValid(IN HANDLE handle, IN const char* pFeatureName);

/// \~chinese
/// \brief 获取属性类型
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pPropertyType [OUT] 属性类型
/// \return 获取成功，返回true；获取失败，返回false
/// \~english
/// \brief get property type
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return get success, return true. get failed, return false 
IMV_FG_API bool IMV_FG_CALL IMV_FG_GetFeatureType(IN HANDLE handle, IN const char* pFeatureName, OUT IMV_FG_EFeatureType* pPropertyType);

/// \~chinese
/// \brief 获取整型属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pIntValue [OUT] 整型属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get integer property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pIntValue [OUT] Integer property value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetIntFeatureValue(IN HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);

/// \~chinese
/// \brief 获取整型属性可设的最小值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pIntValue [OUT] 整型属性可设的最小值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get the integer property settable minimum value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pIntValue [OUT] Integer property settable minimum value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetIntFeatureMin(IN HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);

/// \~chinese
/// \brief 获取整型属性可设的最大值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pIntValue [OUT] 整型属性可设的最大值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get the integer property settable maximum value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pIntValue [OUT] Integer property settable maximum value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetIntFeatureMax(IN HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);

/// \~chinese
/// \brief 获取整型属性步长
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pIntValue [OUT] 整型属性步长
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get integer property increment
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pIntValue [OUT] Integer property increment
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetIntFeatureInc(IN HANDLE handle, IN const char* pFeatureName, OUT int64_t* pIntValue);

/// \~chinese
/// \brief 设置整型属性值（如用OpenEx打开相机，配置相机宽，高时会自动同步到采集卡）
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param intValue [IN] 待设置的整型属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Set integer property value（If using OpenEx to open the camera, configure the width and height of the camera to automatically synchronize to the acquisition card）
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param intValue [IN] Integer property value to be set 
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SetIntFeatureValue(IN HANDLE handle, IN const char* pFeatureName, IN int64_t intValue);

/// \~chinese
/// \brief 获取浮点属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pDoubleValue [OUT] 浮点属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get double property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pDoubleValue [OUT] Double property value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetDoubleFeatureValue(IN HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);

/// \~chinese
/// \brief 获取浮点属性可设的最小值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pDoubleValue [OUT] 浮点属性可设的最小值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get the double property settable minimum value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pDoubleValue [OUT] Double property settable minimum value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetDoubleFeatureMin(IN HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);

/// \~chinese
/// \brief 获取浮点属性可设的最大值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pDoubleValue [OUT] 浮点属性可设的最大值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get the double property settable maximum value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pDoubleValue [OUT] Double property settable maximum value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetDoubleFeatureMax(IN HANDLE handle, IN const char* pFeatureName, OUT double* pDoubleValue);

/// \~chinese
/// \brief 设置浮点属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param doubleValue [IN] 待设置的浮点属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Set double property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param doubleValue [IN] Double property value to be set 
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SetDoubleFeatureValue(IN HANDLE handle, IN const char* pFeatureName, IN double doubleValue);

/// \~chinese
/// \brief 获取布尔属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pBoolValue [OUT] 布尔属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get boolean property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pBoolValue [OUT] Boolean property value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetBoolFeatureValue(IN HANDLE handle, IN const char* pFeatureName, OUT bool* pBoolValue);

/// \~chinese
/// \brief 设置布尔属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param boolValue [IN] 待设置的布尔属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Set boolean property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param boolValue [IN] Boolean property value to be set 
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SetBoolFeatureValue(IN HANDLE handle, IN const char* pFeatureName, IN bool boolValue);

/// \~chinese
/// \brief 获取枚举属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pEnumValue [OUT] 枚举属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get enumeration property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pEnumValue [OUT] Enumeration property value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetEnumFeatureValue(IN HANDLE handle, IN const char* pFeatureName, OUT uint64_t* pEnumValue);

/// \~chinese
/// \brief 设置枚举属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param enumValue [IN] 待设置的枚举属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Set enumeration property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param enumValue [IN] Enumeration property value to be set 
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SetEnumFeatureValue(IN HANDLE handle, IN const char* pFeatureName, IN uint64_t enumValue);

/// \~chinese
/// \brief 获取枚举属性symbol值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pEnumSymbol [OUT] 枚举属性symbol值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get enumeration property symbol value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pEnumSymbol [OUT] Enumeration property symbol value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetEnumFeatureSymbol(IN HANDLE handle, IN const char* pFeatureName, OUT IMV_FG_String* pEnumSymbol);

/// \~chinese
/// \brief 设置枚举属性symbol值（如用OpenEx打开相机，配置相机像素格式，Tap时会自动同步到采集卡）
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pEnumSymbol [IN] 待设置的枚举属性symbol值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Set enumeration property symbol value（If using OpenEx to open the camera, configure the pixelFormat and tap of the camera to automatically synchronize to the acquisition card）
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pEnumSymbol [IN] Enumeration property symbol value to be set 
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SetEnumFeatureSymbol(IN HANDLE handle, IN const char* pFeatureName, IN const char* pEnumSymbol);

/// \~chinese
/// \brief 获取枚举属性的可设枚举值的个数
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pEntryNum [OUT] 枚举属性的可设枚举值的个数
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get the number of enumeration property settable enumeration
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pEntryNum [OUT] The number of enumeration property settable enumeration value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetEnumFeatureEntryNum(IN HANDLE handle, IN const char* pFeatureName, OUT unsigned int* pEntryNum);

/// \~chinese
/// \brief 获取枚举属性的可设枚举值列表
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pEnumEntryList [OUT] 枚举属性的可设枚举值列表
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get settable enumeration value list of enumeration property
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pEnumEntryList [OUT] Settable enumeration value list of enumeration property 
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetEnumFeatureEntrys(IN HANDLE handle, IN const char* pFeatureName, IN_OUT IMV_FG_EnumEntryList* pEnumEntryList);

/// \~chinese
/// \brief 获取字符串属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pStringValue [OUT] 字符串属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Get string property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pStringValue [OUT] String property value
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_GetStringFeatureValue(IN HANDLE handle, IN const char* pFeatureName, OUT IMV_FG_String* pStringValue);

/// \~chinese
/// \brief 设置字符串属性值
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \param pStringValue [IN] 待设置的字符串属性值
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Set string property value
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \param pStringValue [IN] String property value to be set
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_SetStringFeatureValue(IN HANDLE handle, IN const char* pFeatureName, IN const char* pStringValue);

/// \~chinese
/// \brief 执行命令属性
/// \param handle [IN] 采集卡或相机设备句柄
/// \param pFeatureName [IN] 属性名
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Execute command property
/// \param handle [IN] capture card device handle or camera device handle
/// \param pFeatureName [IN] Feature name
/// \return Success, return IMV_FG_OK. Failure, return error code 
IMV_FG_API int IMV_FG_CALL IMV_FG_ExecuteCommandFeature(IN HANDLE handle, IN const char* pFeatureName);

/// \~chinese
/// \brief 像素格式转换
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pstPixelConvertParam [IN][OUT] 像素格式转换参数结构体
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 只支持转化成目标像素格式gvspPixelRGB8 / gvspPixelBGR8 / gvspPixelMono8 / gvspPixelBGRA8\n
/// 通过该接口将原始图像数据转换成用户所需的像素格式并存放在调用者指定内存中。\n
/// 像素格式为YUV411Packed的时，图像宽须能被4整除\n
/// 像素格式为YUV422Packed的时，图像宽须能被2整除\n
/// 像素格式为YUYVPacked的时，图像宽须能被2整除\n
/// 转换后的图像:数据存储是从最上面第一行开始的，这个是相机数据的默认存储方向
/// \~english
/// \brief Pixel format conversion
/// \param hIFDev [IN] capture card device handle
/// \param pstPixelConvertParam [IN][OUT] Convert Pixel Type parameter structure
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// Only support converting to destination pixel format of gvspPixelRGB8 / gvspPixelBGR8 / gvspPixelMono8 / gvspPixelBGRA8\n
/// This API is used to transform the collected original data to pixel format and save to specified memory by caller.\n
/// pixelFormat:YUV411Packed, the image width is divisible by 4\n
/// pixelFormat : YUV422Packed, the image width is divisible by 2\n
/// pixelFormat : YUYVPacked，the image width is divisible by 2\n
/// converted image：The first row of the image is located at the start of the image buffer.This is the default for image taken by a camera.
IMV_FG_API int IMV_FG_CALL IMV_FG_PixelConvert(IN IMV_FG_IF_HANDLE hIFDev, IN_OUT IMV_FG_PixelConvertParam* pstPixelConvertParam);

/// \~chinese
/// \brief 打开录像
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pstRecordParam [IN] 录像参数结构体
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Open record
/// \param hIFDev [IN] capture card device handle
/// \param pstRecordParam [IN] Record param structure
/// \return Success, return IMV_FG_OK. Failure, return error code
IMV_FG_API int IMV_FG_CALL IMV_FG_OpenRecord(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_RecordParam *pstRecordParam);

/// \~chinese
/// \brief 录制一帧图像
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pstRecordFrameInfoParam [IN] 录像用帧信息结构体
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Record one frame
/// \param hIFDev [IN] capture card device handle
/// \param pstRecordFrameInfoParam [IN] Frame information for recording structure
/// \return Success, return IMV_FG_OK. Failure, return error code
IMV_FG_API int IMV_FG_CALL IMV_FG_InputOneFrame(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_RecordFrameInfoParam *pstRecordFrameInfoParam);

/// \~chinese
/// \brief 关闭录像
/// \param hIFDev [IN] 采集卡设备句柄
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \~english
/// \brief Close record
/// \param hIFDev [IN] capture card device handle
/// \return Success, return IMV_FG_OK. Failure, return error code
IMV_FG_API int IMV_FG_CALL IMV_FG_CloseRecord(IN IMV_FG_IF_HANDLE hIFDev);

/// \~chinese
/// \brief 图像翻转
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pstFlipImageParam [IN][OUT] 图像翻转参数结构体
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 只支持像素格式gvspPixelRGB8 / gvspPixelBGR8 / gvspPixelMono8的图像的垂直和水平翻转。\n
/// 通过该接口将原始图像数据翻转后并存放在调用者指定内存中。
/// \~english
/// \brief Flip image
/// \param hIFDev [IN] capture card device handle
/// \param pstFlipImageParam [IN][OUT] Flip image parameter structure
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// Only support vertical and horizontal flip of image data with gvspPixelRGB8 / gvspPixelBGR8 / gvspPixelMono8 pixel format.\n
/// This API is used to flip original data and save to specified memory by caller.
IMV_FG_API int IMV_FG_CALL IMV_FG_FlipImage(IN IMV_FG_IF_HANDLE hIFDev, IN_OUT IMV_FG_FlipImageParam* pstFlipImageParam);

/// \~chinese
/// \brief 图像顺时针旋转
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pstRotateImageParam [IN][OUT] 图像旋转参数结构体
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 只支持gvspPixelRGB8 / gvspPixelBGR8 / gvspPixelMono8格式数据的90/180/270度顺时针旋转。\n
/// 通过该接口将原始图像数据旋转后并存放在调用者指定内存中。
/// \~english
/// \brief Rotate image clockwise
/// \param hIFDev [IN] capture card device handle
/// \param pstRotateImageParam [IN][OUT] Rotate image parameter structure
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// Only support 90/180/270 clockwise rotation of data in the gvspPixelRGB8 / gvspPixelBGR8 / gvspPixelMono8 format.\n
/// This API is used to rotation original data and save to specified memory by caller.
IMV_FG_API int IMV_FG_CALL IMV_FG_RotateImage(IN IMV_FG_IF_HANDLE hIFDev, IN_OUT IMV_FG_RotateImageParam* pstRotateImageParam);

/// \~chinese
/// \brief 设备连接状态事件回调注册(CL专用)
/// \param hDev [IN] 相机设备句柄
/// \param proc [IN] 设备连接状态事件回调函数
/// \param pUser [IN] 用户自定义数据, 可设为NULL
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 只支持一个回调函数,且设备关闭后，注册会失效，打开设备后需重新注册
/// \~english
/// \brief Register call back function of device connection status event.(CL Only)
/// \param hDev [IN] camera device handle
/// \param proc [IN] Call back function of device connection status event
/// \param pUser [IN] User defined data，It can be set to NULL
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// Only one call back function is supported.\n
/// Registration becomes invalid after the device is closed, , and need to re-register after the device is opened
IMV_FG_API int IMV_FG_CALL IMV_FG_SubscribeConnectArg(IN IMV_FG_DEV_HANDLE hDev, IN IMV_FG_ConnectCallBack proc, IN void* pUser);

/// \~chinese
/// \brief 消息通道事件回调注册
/// \param handle [IN] 设备句柄
/// \param proc [IN] 消息通道事件回调注册函数(需要及时处理回调函数中的数据,否则数据会失效)
/// \param pUser [IN] 用户自定义数据, 可设为NULL
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 只支持一个回调函数,且设备关闭后，注册会失效，打开设备后需重新注册
/// \~english
/// \brief Register call back function of message channel event.(Device universal interface. For the same device, the system can only choose between the two)
/// \param handle [IN] Device handle
/// \param proc [IN] Call back function of message channel event(The data in the callback function needs to be processed in a timely manner, otherwise the data will become invalid)
/// \param pUser [IN] User defined data，It can be set to NULL
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// Only one call back function is supported.\n
/// Registration becomes invalid after the device is closed, , and need to re-register after the device is opened
IMV_FG_API int IMV_FG_CALL IMV_FG_SubscribeMsgChannelArg(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_MsgChannelCallBack proc, IN void* pUser);

/// \~chinese
/// \brief 保存图像到文件
/// \param hIFDev [IN] 采集卡设备句柄
/// \param pstSaveFileParam [IN] 保存图片到文件参数结构体
/// \return 成功，返回IMV_FG_OK；错误，返回错误码
/// \remarks
/// 该接口支持保存BMP/JPEG/PNG/TIFF
/// JPEG格式最大支持宽高为65500
/// \~english
/// \brief Save the image to file.
/// \param hIFDev [IN] capture card device handle
/// \param pstSaveFileParam [IN] Save the image file parameter structure
/// \return Success, return IMV_FG_OK. Failure, return error code
/// \remarks
/// This API support save BMP/JPEG/PNG/TIFF.
/// JPEG format supports a maximum width and height of 65500
IMV_FG_API int IMV_FG_CALL IMV_FG_SaveImageToFile(IN IMV_FG_IF_HANDLE hIFDev, IN IMV_FG_SaveImageToFileParam* pstSaveFileParam);

#ifdef __cplusplus
}
#endif 

#endif
