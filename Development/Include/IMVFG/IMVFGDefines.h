#ifndef __IMV_FG_DEFINES_H__
#define __IMV_FG_DEFINES_H__

#ifdef WIN32
typedef __int64 int64_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#include <string>

#ifndef IN
#define IN		///< \~chinese 输入型参数		\~english Input param 
#endif

#ifndef OUT
#define OUT		///< \~chinese 输出型参数		\~english Output param 
#endif

#ifndef IN_OUT
#define IN_OUT	///< \~chinese 输入/输出型参数  \~english Input/Output param 
#endif

#ifndef __cplusplus
typedef char    bool;
#define true    1
#define false   0
#endif

/// \~chinese
/// \brief 字符串信息
/// \~english
/// \brief String information
typedef struct _IMV_FG_String
{
	char str[256];						///< \~chinese	字符串.长度不超过256  \~english Strings and the maximum length of strings is 255.
}IMV_FG_String;


/// \~chinese
/// \brief 错误码 
/// \~english
/// \brief Error code
#define IMV_FG_OK						0			///< \~chinese 成功，无错误							\~english Successed, no error
#define IMV_FG_ERROR					-101		///< \~chinese 通用的错误							\~english Generic error
#define IMV_FG_INVALID_HANDLE			-102		///< \~chinese 错误或无效的句柄						\~english Error or invalid handle
#define IMV_FG_INVALID_PARAM			-103		///< \~chinese 错误的参数							\~english Incorrect parameter
#define IMV_FG_INVALID_FRAME_HANDLE		-104		///< \~chinese 错误或无效的帧句柄					\~english Error or invalid frame handle
#define IMV_FG_INVALID_FRAME			-105		///< \~chinese 无效的帧								\~english Invalid frame
#define IMV_FG_INVALID_RESOURCE			-106		///< \~chinese 相机/事件/流等资源无效				\~english Device/Event/Stream and so on resource invalid
#define IMV_FG_INVALID_IP				-107		///< \~chinese 设备与主机的IP网段不匹配				\~english Device's and PC's subnet is mismatch
#define IMV_FG_NO_MEMORY				-108		///< \~chinese 内存不足								\~english Malloc memery failed
#define IMV_FG_INSUFFICIENT_MEMORY		-109		///< \~chinese 传入的内存空间不足					\~english Insufficient memory
#define IMV_FG_ERROR_PROPERTY_TYPE		-110		///< \~chinese 属性类型错误							\~english Property type error
#define IMV_FG_INVALID_ACCESS			-111		///< \~chinese 属性不可访问、或不能读/写、或读/写失败	\~english Property not accessible, or not be read/written, or read/written failed
#define IMV_FG_INVALID_RANGE			-112		///< \~chinese 属性值超出范围、或者不是步长整数倍		\~english The property's value is out of range, or is not integer multiple of the step
#define IMV_FG_NOT_SUPPORT				-113		///< \~chinese 设备不支持的功能						\~english Device not supported function
#define IMV_FG_NO_DATA					-114		///< \~chinese 无数据								\~english No data
#define IMV_FG_PARAM_OVERFLOW			-115		///< \~chinese 参数值越界							\~english Param value overflow
#define IMV_FG_NOT_AVAILABLE			-116		///< \~chinese 连接不可达							\~english Connect not available
#define IMV_FG_NOT_GRABBING				-117		///< \~chinese 相机已停止取图						\~english The camera already stop grabbing
#define IMV_FG_NOT_CONNECTED_CARD		-118		///< \~chinese 未连接采集卡							\~english Not connect capture card
#define IMV_FG_TIMEOUT					-119		///< \~chinese 超时									\~english Timeout
#define IMV_FG_IS_CONNECTED				-120		///< \~chinese 设备已连接							\~english Device is connected
#define IMV_FG_IS_GRABBING				-121		///< \~chinese 设备正在取图							\~english The device is grabbing
#define IMV_FG_INVOCATION_ERROR			-122		///< \~chinese 调用时序错误							\~english The timing of the call is incorrect
#define IMV_FG_SYSTEM_ERROR				-123		///< \~chinese 系统接口返回错误						\~english The operating system returns an error
#define IMV_FG_OPENFILE_ERROR           -124        ///< \~chinese 打开文件出现错误						\~english Open file error

#define IMV_FG_MAX_STRING_LENTH			256			///< \~chinese 字符串最大长度						\~english The maximum length of string
#define IMV_FG_MAX_ERROR_LIST_NUM		128			///< \~chinese 失败属性列表最大长度					\~english The maximum size of failed properties list

typedef	void* IMV_FG_IF_HANDLE;						///< \~chinese 采集卡设备句柄							\~english Interface handle
typedef void* IMV_FG_DEV_HANDLE;					///< \~chinese 相机设备句柄								\~english Device handle
typedef void* HANDLE;								///< \~chinese 设备句柄(采集卡设备句柄或者相机设备句柄)	\~english Device handle or Interface handle
typedef void* IMV_FG_FRAME_HANDLE;					///< \~chinese 帧句柄									\~english frame handle

/// \~chinese
///枚举：属性类型
/// \~english
///Enumeration: property type
typedef enum _IMV_FG_EFeatureType
{
	IMV_FG_FEATURE_INT		= 0x10000000, 			///< \~chinese 整型数				\~english Integer
	IMV_FG_FEATURE_FLOAT	= 0x20000000,			///< \~chinese 浮点数				\~english Float
	IMV_FG_FEATURE_ENUM		= 0x30000000,			///< \~chinese 枚举					\~english Enumeration
	IMV_FG_FEATURE_BOOL		= 0x40000000,			///< \~chinese 布尔					\~english Bool
	IMV_FG_FEATURE_STRING	= 0x50000000,			///< \~chinese 字符串				\~english String
	IMV_FG_FEATURE_COMMAND	= 0x60000000,			///< \~chinese 命令					\~english Command
	IMV_FG_FEATURE_GROUP	= 0x70000000,			///< \~chinese 分组节点				\~english Group Node
	IMV_FG_FEATURE_REG		= 0x80000000,			///< \~chinese 寄存器节点			\~english Register Node

	IMV_FG_FEATURE_UNDEFINED = 0x90000000			///< \~chinese 未定义				\~english Undefined
}IMV_FG_EFeatureType;

/// \~chinese
///枚举：接口类型
/// \~english
///Enumeration: interface type
typedef enum _IMV_FG_EInterfaceType
{
	typeGigEInterface	= 0x00000001,			///< \~chinese 网口采集卡		\~english Gev interface type
	typeU3vInterface	= 0x00000002,			///< \~chinese Usb采集			\~english CXP interface type
	typeCLInterface		= 0x00000004,			///< \~chinese CameraLink采集卡	\~english CAMERALINK interface type
	typeCXPInterface	= 0x00000008,			///< \~chinese CXP采集卡		\~english CXP interface type

	typeUndefinedInterface	= 0xFFFFFFFF		///< \~chinese 无效接口类型		\~english Invalid interface type
}IMV_FG_EInterfaceType;

//采集卡模式
/// \~chinese
///信息：采集卡模式
/// \~english
///Information: Capture Card Mode
enum EInterfaceMode
{
	FULL_MODE = 0,			///< \~chinese 单Full模式					\~english full
	DUAL_BASE_MODE = 1,		///< \~chinese 双Base，同时支持2个相机		\~english Dual base
	DUAL_FULL_MODE = 2,		///< \~chinese 双Full，同时支持2个相机		\~english Dual full
	QUAD_BASE_MODE = 3,		///< \~chinese 4Base，同时支持4个相机		\~english Quad full

	TYPE_UNDEFINED = 255    ///< \~chinese 未知类型
};

// CL采集卡信息
/// \~chinese
///信息：CameraLink采集卡信息
/// \~english
///Information: CameraLink Interface Information
typedef struct _IMV_CL_INTERFACE_INFO
{
	EInterfaceMode		nInterfaceMode;			///< \~chinese 采集卡模式				\~english Interface mode
	unsigned int        nPCIEInfo;				///< \~chinese 采集卡的PCIE插槽信息		\~english PCIE Info

	unsigned int        nReserved[64];			///< \~chinese 预留						\~english Reserved field

}IMV_CL_INTERFACE_INFO;

// CXP采集卡信息
/// \~chinese
///信息：CXP采集卡信息
/// \~english
///Information: CXP Interface Information
typedef struct _IMV_CXP_INTERFACE_INFO
{
	EInterfaceMode		nInterfaceMode;			///< \~chinese 采集卡模式				\~english Interface mode
	unsigned int        nPortInfo;				///< \~chinese 采集卡的Port信息			\~english Port Info

	unsigned int        nReserved[64];			///< \~chinese 预留						\~english Reserved field

}IMV_CXP_INTERFACE_INFO;

typedef struct _IMV_FG_INTERFACE_INFO
{
	IMV_FG_EInterfaceType	nInterfaceType;									///< \~chinese 采集卡类型		\~english Interface type
	unsigned int			nInterfaceReserved[8];							///< \~chinese 保留字段			\~english Reserved field

	char					interfaceKey[IMV_FG_MAX_STRING_LENTH];			///< \~chinese 厂商:序列号:端口	\~english Interface key
	char					interfaceName[IMV_FG_MAX_STRING_LENTH];			///< \~chinese 用户自定义名		\~english UserDefinedName
	char					serialNumber[IMV_FG_MAX_STRING_LENTH];			///< \~chinese 设备序列号		\~english Interface SerialNumber
	char					vendorName[IMV_FG_MAX_STRING_LENTH];			///< \~chinese 厂商				\~english Interface Vendor
	char					modelName[IMV_FG_MAX_STRING_LENTH];				///< \~chinese 设备型号			\~english Interface model
	char					manufactureInfo[IMV_FG_MAX_STRING_LENTH];		///< \~chinese 设备制造信息		\~english Interface ManufactureInfo
	char					deviceVersion[IMV_FG_MAX_STRING_LENTH];			///< \~chinese 设备版本			\~english Interface Version
	char					interfaceReserved[5][IMV_FG_MAX_STRING_LENTH];	///< \~chinese 保留				\~english Reserved field

	union
	{
		IMV_CL_INTERFACE_INFO		CLInterfaceInfo;							///< \~chinese CameraLink采集卡信息	\~english CameraLink Capture Card info
		IMV_CXP_INTERFACE_INFO		CxpInterfaceInfo;							///< \~chinese CameraLink采集卡信息	\~english CameraLink Capture Card info

		unsigned int				nReserved[128];								///< \~chinese 限定长度				\~english limit length
	}InterfaceInfo;

}IMV_FG_INTERFACE_INFO;

typedef struct _IMV_FG_INTERFACE_INFO_LIST
{
	unsigned int			nInterfaceNum;	//数量
	IMV_FG_INTERFACE_INFO*	pInterfaceInfoList;
}IMV_FG_INTERFACE_INFO_LIST;

/// \~chinese
///枚举：创建句柄方式
/// \~english
///Enumeration: Create handle mode
typedef enum _IMV_FG_ECreateHandleMode
{
	IMV_FG_MODE_BY_INDEX = 0,				///< \~chinese 通过已枚举设备的索引(从0开始，比如 0, 1, 2...)	\~english By index of enumerated devices (Start from 0, such as 0, 1, 2...)
	IMV_FG_MODE_BY_CAMERAKEY,				///< \~chinese 通过设备键"厂商:序列号"(CL专用)					\~english By device's key "vendor:serial number"(CL Only)	
	IMV_FG_MODE_BY_DEVICE_USERID,			///< \~chinese 通过设备自定义名(CL专用)							\~english By device userID(CL Only)
	IMV_FG_MODE_BY_IPADDRESS,				///< \~chinese 通过设备IP地址(仅适合GigE相机)					\~english By device IP address.(GigE Camera only)
}IMV_FG_ECreateHandleMode;

/// \~chinese
///枚举：设备类型
/// \~english
///Enumeration: device type
typedef enum _IMV_FG_EDeviceType
{
	IMV_FG_TYPE_GIGE_DEVICE	= 0,				///< \~chinese GIGE相机				\~english GigE Vision Device
	IMV_FG_TYPE_U3V_DEVICE	= 1,				///< \~chinese USB3.0相机			\~english USB3.0 Vision Device
	IMV_FG_TYPE_CL_DEVICE	= 2,				///< \~chinese CAMERALINK相机		\~english Cameralink Device
	IMV_FG_TYPE_CXP_DEVICE	= 3,				///< \~chinese PCIe相机				\~english PCIe Device

	IMV_FG_TYPE_UNDEFINED_DEVICE = 255			///< \~chinese 未知类型				\~english Undefined Device
}IMV_FG_EDeviceType;

typedef struct _IMV_FG_DEVICE_INFO
{
	IMV_FG_EDeviceType	nDeviceType;								///< \~chinese 设备类型					\~english Device Type
	unsigned int		nReserved[8];								///< \~chinese 保留字段					\~english Reserved field

	char				cameraKey[IMV_FG_MAX_STRING_LENTH];			///< \~chinese CaptureCard_厂商:序列号	\~english Device key
	char				cameraName[IMV_FG_MAX_STRING_LENTH];		///< \~chinese 用户自定义名				\~english UserDefinedName
	char				serialNumber[IMV_FG_MAX_STRING_LENTH];		///< \~chinese 设备序列号				\~english Device SerialNumber
	char				vendorName[IMV_FG_MAX_STRING_LENTH];		///< \~chinese 厂商						\~english Device Vendor
	char				modelName[IMV_FG_MAX_STRING_LENTH];			///< \~chinese 设备型号					\~english Device model
	char				manufactureInfo[IMV_FG_MAX_STRING_LENTH];	///< \~chinese 设备制造信息				\~english Device ManufactureInfo
	char				deviceVersion[IMV_FG_MAX_STRING_LENTH];		///< \~chinese 设备版本					\~english Device Version
	char				cameraReserved[5][IMV_FG_MAX_STRING_LENTH];	///< \~chinese 保留						\~english Reserved field

	IMV_FG_INTERFACE_INFO	FGInterfaceInfo;						///< \~chinese 采集卡信息				\~english Capture Card info
}IMV_FG_DEVICE_INFO;

typedef struct _IMV_FG_DEVICE_INFO_LIST
{
	unsigned int			nDevNum;								///< \~chinese 设备数量			\~english Device Num
	IMV_FG_DEVICE_INFO*		pDeviceInfoList;						///< \~chinese 设备信息			\~english Device Info
}IMV_FG_DEVICE_INFO_LIST;

#define IMV_FG_PIX_MONO                           0x01000000
#define IMV_FG_PIX_RGB                            0x02000000
#define IMV_FG_PIX_COLOR                          0x02000000
#define IMV_FG_PIX_CUSTOM                         0x80000000
#define IMV_FG_PIX_COLOR_MASK                     0xFF000000

// Indicate effective number of bits occupied by the pixel (including padding).
// This can be used to compute amount of memory required to store an image.
#define IMV_FG_PIX_OCCUPY1BIT                     0x00010000
#define IMV_FG_PIX_OCCUPY2BIT                     0x00020000
#define IMV_FG_PIX_OCCUPY4BIT                     0x00040000
#define IMV_FG_PIX_OCCUPY8BIT                     0x00080000
#define IMV_FG_PIX_OCCUPY12BIT                    0x000C0000
#define IMV_FG_PIX_OCCUPY16BIT                    0x00100000
#define IMV_FG_PIX_OCCUPY24BIT                    0x00180000
#define IMV_FG_PIX_OCCUPY32BIT                    0x00200000
#define IMV_FG_PIX_OCCUPY36BIT                    0x00240000
#define IMV_FG_PIX_OCCUPY48BIT                    0x00300000

/// \~chinese
/// \brief 枚举属性的枚举值信息
/// \~english
/// \brief Enumeration property 's enumeration value information
typedef struct _IMV_FG_EnumEntryInfo
{
	uint64_t				value;							///< \~chinese 枚举值				\~english  Enumeration value 
	char					name[IMV_FG_MAX_STRING_LENTH];	///< \~chinese symbol名				\~english  Symbol name
}IMV_FG_EnumEntryInfo;

/// \~chinese
/// \brief 枚举属性的可设枚举值列表信息
/// \~english
/// \brief Enumeration property 's settable enumeration value list information
typedef struct _IMV_FG_EnumEntryList
{
	unsigned int			nEnumEntryBufferSize;		///< \~chinese 存放枚举值内存大小					\~english The size of saving enumeration value 
	IMV_FG_EnumEntryInfo*	pEnumEntryInfo;				///< \~chinese 存放可设枚举值列表(调用者分配缓存)	\~english Save the list of settable enumeration value(allocated cache by the caller)
}IMV_FG_EnumEntryList;

/// \~chinese
///枚举：图像格式
/// \~english
/// Enumeration:image format
typedef enum _IMV_FG_EPixelType
{
	// Undefined pixel type
	IMV_FG_PIXEL_TYPE_Undefined = -1,

	// Mono Format
	IMV_FG_PIXEL_TYPE_Mono1p = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY1BIT | 0x0037),
	IMV_FG_PIXEL_TYPE_Mono2p = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY2BIT | 0x0038),
	IMV_FG_PIXEL_TYPE_Mono4p = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY4BIT | 0x0039),
	IMV_FG_PIXEL_TYPE_Mono8	= (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY8BIT | 0x0001),
	IMV_FG_PIXEL_TYPE_Mono8S = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY8BIT | 0x0002),
	IMV_FG_PIXEL_TYPE_Mono10 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0003),
	IMV_FG_PIXEL_TYPE_Mono10Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x0004),
	IMV_FG_PIXEL_TYPE_Mono12 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0005),
	IMV_FG_PIXEL_TYPE_Mono12Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x0006),
	IMV_FG_PIXEL_TYPE_Mono14 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0025),
	IMV_FG_PIXEL_TYPE_Mono16 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0007),

	// Bayer Format
	IMV_FG_PIXEL_TYPE_BayGR8 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY8BIT | 0x0008),
	IMV_FG_PIXEL_TYPE_BayRG8 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY8BIT | 0x0009),
	IMV_FG_PIXEL_TYPE_BayGB8 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY8BIT | 0x000A),
	IMV_FG_PIXEL_TYPE_BayBG8 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY8BIT | 0x000B),
	IMV_FG_PIXEL_TYPE_BayGR10 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x000C),
	IMV_FG_PIXEL_TYPE_BayRG10 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x000D),
	IMV_FG_PIXEL_TYPE_BayGB10 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x000E),
	IMV_FG_PIXEL_TYPE_BayBG10 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x000F),
	IMV_FG_PIXEL_TYPE_BayGR12 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0010),
	IMV_FG_PIXEL_TYPE_BayRG12 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0011),
	IMV_FG_PIXEL_TYPE_BayGB12 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0012),
	IMV_FG_PIXEL_TYPE_BayBG12 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0013),
	IMV_FG_PIXEL_TYPE_BayGR10Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x0026),
	IMV_FG_PIXEL_TYPE_BayRG10Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x0027),
	IMV_FG_PIXEL_TYPE_BayGB10Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x0028),
	IMV_FG_PIXEL_TYPE_BayBG10Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x0029),
	IMV_FG_PIXEL_TYPE_BayGR12Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x002A),
	IMV_FG_PIXEL_TYPE_BayRG12Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x002B),
	IMV_FG_PIXEL_TYPE_BayGB12Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x002C),
	IMV_FG_PIXEL_TYPE_BayBG12Packed = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY12BIT | 0x002D),
	IMV_FG_PIXEL_TYPE_BayGR16 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x002E),
	IMV_FG_PIXEL_TYPE_BayRG16 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x002F),
	IMV_FG_PIXEL_TYPE_BayGB16 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0030),
	IMV_FG_PIXEL_TYPE_BayBG16 = (IMV_FG_PIX_MONO | IMV_FG_PIX_OCCUPY16BIT | 0x0031),

	// RGB Format
	IMV_FG_PIXEL_TYPE_RGB8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x0014),
	IMV_FG_PIXEL_TYPE_BGR8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x0015),
	IMV_FG_PIXEL_TYPE_RGBA8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY32BIT | 0x0016),
	IMV_FG_PIXEL_TYPE_BGRA8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY32BIT | 0x0017),
	IMV_FG_PIXEL_TYPE_RGB10 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0018),
	IMV_FG_PIXEL_TYPE_BGR10 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0019),
	IMV_FG_PIXEL_TYPE_RGB12 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x001A),
	IMV_FG_PIXEL_TYPE_BGR12 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x001B),
	IMV_FG_PIXEL_TYPE_RGB16 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0033),
	IMV_FG_PIXEL_TYPE_RGB10V1Packed = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY32BIT | 0x001C),
	IMV_FG_PIXEL_TYPE_RGB10P32 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY32BIT | 0x001D),
	IMV_FG_PIXEL_TYPE_RGB12V1Packed = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY36BIT | 0X0034),
	IMV_FG_PIXEL_TYPE_RGB565P = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x0035),
	IMV_FG_PIXEL_TYPE_BGR565P = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0X0036),

	// YVR Format
	IMV_FG_PIXEL_TYPE_YUV411_8_UYYVYY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY12BIT | 0x001E),
	IMV_FG_PIXEL_TYPE_YUV422_8_UYVY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x001F),
	IMV_FG_PIXEL_TYPE_YUV422_8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x0032),
	IMV_FG_PIXEL_TYPE_YUV8_UYV = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x0020),
	IMV_FG_PIXEL_TYPE_YCbCr8CbYCr = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x003A),
	IMV_FG_PIXEL_TYPE_YCbCr422_8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x003B),
	IMV_FG_PIXEL_TYPE_YCbCr422_8_CbYCrY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x0043),
	IMV_FG_PIXEL_TYPE_YCbCr411_8_CbYYCrYY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY12BIT | 0x003C),
	IMV_FG_PIXEL_TYPE_YCbCr601_8_CbYCr = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x003D),
	IMV_FG_PIXEL_TYPE_YCbCr601_422_8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x003E),
	IMV_FG_PIXEL_TYPE_YCbCr601_422_8_CbYCrY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x0044),
	IMV_FG_PIXEL_TYPE_YCbCr601_411_8_CbYYCrYY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY12BIT | 0x003F),
	IMV_FG_PIXEL_TYPE_YCbCr709_8_CbYCr = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x0040),
	IMV_FG_PIXEL_TYPE_YCbCr709_422_8 = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x0041),
	IMV_FG_PIXEL_TYPE_YCbCr709_422_8_CbYCrY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY16BIT | 0x0045),
	IMV_FG_PIXEL_TYPE_YCbCr709_411_8_CbYYCrYY = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY12BIT | 0x0042),

	// RGB Planar
	IMV_FG_PIXEL_TYPE_RGB8Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x0021),
	IMV_FG_PIXEL_TYPE_RGB10Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0022),
	IMV_FG_PIXEL_TYPE_RGB12Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0023),
	IMV_FG_PIXEL_TYPE_RGB16Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0024),

	// BayerRG10p和BayerRG12p格式，针对特定项目临时添加,请不要使用
	// BayerRG10p and BayerRG12p, currently used for specific project, please do not use them
	IMV_FG_PIXEL_TYPE_BayRG10p = 0x010A0058,
	IMV_FG_PIXEL_TYPE_BayRG12p = 0x010c0059,

	// mono1c格式，自定义格式
	// mono1c, customized image format, used for binary output
	IMV_FG_PIXEL_TYPE_Mono1c = 0x012000FF,

	// mono1e格式，自定义格式，用来显示连通域
	// mono1e, customized image format, used for displaying connected domain
	IMV_FG_PIXEL_TYPE_Mono1e = 0x01080FFF
}IMV_FG_EPixelType;

/// \~chinese
/// \brief 帧图像信息
/// \~english
/// \brief The frame image information
typedef struct _IMV_FG_FrameInfo_
{
	uint64_t				blockId;				///< \~chinese 帧Id(仅对GigE/Usb/PCIe相机有效)					\~english The block ID(GigE/Usb/PCIe Device only)
	unsigned int			status;					///< \~chinese 数据帧状态(0是正常状态)							\~english The status of frame(0 is normal status)
	unsigned int			width;					///< \~chinese 图像宽度											\~english The width of image
	unsigned int			height;					///< \~chinese 图像高度											\~english The height of image
	unsigned int			size;					///< \~chinese 图像大小											\~english The size of image
	IMV_FG_EPixelType		pixelFormat;			///< \~chinese 图像像素格式										\~english The pixel format of image
	uint64_t				timeStamp;				///< \~chinese 图像时间戳(仅对GigE/Usb/PCIe相机有效)			\~english The timestamp of image(GigE/Usb/PCIe Device only)
	unsigned int			chunkCount;				///< \~chinese 帧数据中包含的Chunk个数(仅对GigE/Usb相机有效)	\~english The number of chunk in frame data(GigE/Usb Device Only)
	unsigned int			paddingX;				///< \~chinese 图像paddingX(仅对GigE/Usb/PCIe相机有效)			\~english The paddingX of image(GigE/Usb/PCIe Device only)
	unsigned int			paddingY;				///< \~chinese 图像paddingY(仅对GigE/Usb/PCIe相机有效)			\~english The paddingY of image(GigE/Usb/PCIe Device only)
	unsigned int			recvFrameTime;			///< \~chinese 图像在网络传输所用的时间(单位:微秒,非GigE相机该值为0)	\~english The time taken for the image to be transmitted over the network(unit:us, The value is 0 for non-GigE camera)
	unsigned int			nReserved[19];			///< \~chinese 预留字段											\~english Reserved field
}IMV_FG_FrameInfo;

/// \~chinese
/// \brief 加载失败的属性信息
/// \~english
/// \brief Load failed properties information
typedef struct _IMV_FG_ErrorList
{
	unsigned int		nParamCnt;										///< \~chinese 加载失败的属性个数			\~english The count of load failed properties
	IMV_FG_String		paramNameList[IMV_FG_MAX_ERROR_LIST_NUM];		///< \~chinese 加载失败的属性集合，上限128	\~english Array of load failed properties, up to 128 
}IMV_FG_ErrorList;

/// \~chinese
/// \brief 设置层级，CXP采集卡属性分为多层，每次配置属性时需配置该属性的层级(CXP专用)
/// \~english
/// \brief CXP captrue card featrue are divided into multiple levels. Each time you configure the featrue, you need to configure the level of the featrue
typedef enum _IMV_FG_EFeatureLevel
{
	fg_interface_Level,				///< \~chinese 采集卡接口层				\~english Capture Card interface Level
	fg_device_Level,				///< \~chinese 采集卡设备层				\~english Capture Card device Level
	camera_Level					///< \~chinese 远程设备层(相机)			\~english The camera Level

}IMV_FG_EFeatureLevel;

/// \~chinese
/// \brief Chunk数据信息
/// \~english
/// \brief Chunk data information
typedef struct _IMV_FG_ChunkDataInfo
{
	unsigned int			chunkID;				///< \~chinese ChunkID									\~english ChunkID
	unsigned int			nParamCnt;				///< \~chinese 属性名个数								\~english The number of paramNames
	IMV_FG_String*			pParamNameList;			///< \~chinese Chunk数据对应的属性名集合(SDK内部缓存)\~english ParamNames Corresponding property name of chunk data(cached within the SDK)
}IMV_FG_ChunkDataInfo;

/// \~chinese
/// \brief 帧图像数据信息
/// \~english
/// \brief Frame image data information
typedef struct _IMV_FG_Frame_
{
	IMV_FG_FRAME_HANDLE		frameHandle;			///< \~chinese 帧图像句柄(SDK内部帧管理用)		\~english Frame image handle(used for managing frame within the SDK)
	unsigned char*			pData;					///< \~chinese 帧图像数据的内存首地址			\~english The starting address of memory of image data
	IMV_FG_FrameInfo		frameInfo;				///< \~chinese 帧信息							\~english Frame information
	unsigned int			nReserved[10];			///< \~chinese 预留字段							\~english Reserved field
}IMV_FG_Frame;

/// \~chinese
///枚举：抓图策略
/// \~english
///Enumeration: grab strartegy
typedef enum _IMV_FG_EGrabStrategy
{
	IMV_FG_GRAB_STRARTEGY_SEQUENTIAL = 0,			///< \~chinese 按到达顺序处理图片	\~english The images are processed in the order of their arrival
	IMV_FG_GRAB_STRARTEGY_LATEST_IMAGE = 1,			///< \~chinese 获取最新的图片		\~english Get latest image
	IMV_FG_GRAB_STRARTEGY_UPCOMING_IMAGE = 2,		///< \~chinese 等待获取下一张图片(只针对GigE相机)	\~english Waiting for next image(GigE only)
	IMV_FG_GRAB_STRARTEGY_UNDEFINED   				///< \~chinese 未定义				\~english Undefined
}IMV_FG_EGrabStrategy;

/// \~chinese
/// \brief PCIE设备统计流信息
/// \~english
/// \brief PCIE device stream statistics information
typedef struct _IMV_FG_CLStreamStatsInfo
{
	unsigned int			imageError;				///< \~chinese 图像错误的帧数			\~english  Number of images error frames
	unsigned int			lostPacketBlock;		///< \~chinese 丢包的帧数				\~english  Number of frames lost
	unsigned int			nReserved0[10];			///< \~chinese 预留						\~english  Reserved field

	unsigned int			imageReceived;			///< \~chinese 正常获取的帧数			\~english  Number of frames acquired
	double					fps;                    ///< \~chinese 帧率						\~english  Frame rate
	double					bandwidth;              ///< \~chinese 带宽(Mbps)				\~english  Bandwidth(Mbps)
	unsigned int			nReserved[8];           ///< \~chinese 预留						\~english  Reserved field
}IMV_FG_CLStreamStatsInfo;

/// \~chinese
/// \brief U3V设备统计流信息
/// \~english
/// \brief U3V device stream statistics information
typedef struct _IMV_FG_U3VStreamStatsInfo
{
	unsigned int			imageError;				///< \~chinese 图像错误的帧数			\~english  Number of images error frames
	unsigned int			lostPacketBlock;		///< \~chinese 丢包的帧数				\~english  Number of frames lost
	unsigned int			nReserved0[10];			///< \~chinese 预留						\~english  Reserved field

	unsigned int			imageReceived;			///< \~chinese 正常获取的帧数			\~english  Number of images error frames
	double					fps;                    ///< \~chinese 帧率						\~english  Frame rate
	double					bandwidth;              ///< \~chinese 带宽(Mbps)				\~english  Bandwidth(Mbps)
	unsigned int			nReserved[8];           ///< \~chinese 预留						\~english  Reserved field
}IMV_FG_U3VStreamStatsInfo;

/// \~chinese
/// \brief Gige设备统计流信息
/// \~english
/// \brief Gige device stream statistics information
typedef struct _IMV_FG_GigEStreamStatsInfo
{
	unsigned int			nReserved0[10];			///< \~chinese 预留						\~english  Reserved field

	unsigned int			imageError;				///< \~chinese 图像错误的帧数			\~english  Number of image error frames
	unsigned int			lostPacketBlock;		///< \~chinese 丢包的帧数				\~english  Number of frames lost
	unsigned int			nReserved1[4];			///< \~chinese 预留						\~english  Reserved field
	unsigned int			nReserved2[5];			///< \~chinese 预留						\~english  Reserved field

	unsigned int			imageReceived;			///< \~chinese 正常获取的帧数			\~english  Number of frames acquired
	double					fps;                    ///< \~chinese 帧率						\~english  Frame rate
	double					bandwidth;              ///< \~chinese 带宽(Mbps)				\~english  Bandwidth(Mbps)
	unsigned int			nReserved[4];			///< \~chinese 预留						\~english  Reserved field
}IMV_FG_GigEStreamStatsInfo;

/// \~chinese
/// \brief 统计流信息
/// \~english
/// \brief Stream statistics information
typedef struct _IMV_FG_StreamStatisticsInfo
{
	IMV_FG_EDeviceType			nDeviceType;			///< \~chinese 设备类型				\~english  Device type

	union
	{
		IMV_FG_CLStreamStatsInfo	clStatisticsInfo;	///< \~chinese CL设备统计信息		\~english  CameraLink device statistics information
		IMV_FG_U3VStreamStatsInfo	u3vStatisticsInfo;	///< \~chinese U3V设备统计信息		\~english  U3V device statistics information
		IMV_FG_GigEStreamStatsInfo	gigeStatisticsInfo;	///< \~chinese Gige设备统计信息		\~english  GIGE device statistics information
	};
}IMV_FG_StreamStatisticsInfo;

/// \~chinese
///枚举：图像转换Bayer格式所用的算法
/// \~english
/// Enumeration:alorithm used for Bayer demosaic
typedef enum _IMV_FG_EBayerDemosaic
{
	IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR,					///< \~chinese 最近邻			\~english Nearest neighbor
	IMV_FG_DEMOSAIC_BILINEAR,							///< \~chinese 双线性			\~english Bilinear
	IMV_FG_DEMOSAIC_EDGE_SENSING,						///< \~chinese 边缘检测			\~english Edge sensing
	IMV_FG_DEMOSAIC_NOT_SUPPORT = 255,					///< \~chinese 不支持			\~english Not support
}IMV_FG_EBayerDemosaic;

/// \~chinese
///枚举：视频格式
/// \~english
/// Enumeration:Video format
typedef enum _IMV_FG_EVideoType
{
	IMV_FG_TYPE_VIDEO_FORMAT_AVI = 0,					///< \~chinese AVI格式			\~english AVI format
	IMV_FG_TYPE_VIDEO_FORMAT_NOT_SUPPORT = 255			///< \~chinese 不支持			\~english Not support
}IMV_FG_EVideoType;

/// \~chinese
///枚举：图像翻转类型
/// \~english
/// Enumeration:Image flip type
typedef enum _IMV_FG_EFlipType
{
	IMV_FG_TYPE_FLIP_VERTICAL,							///< \~chinese 垂直(Y轴)翻转	\~english Vertical(Y-axis) flip
	IMV_FG_TYPE_FLIP_HORIZONTAL							///< \~chinese 水平(X轴)翻转	\~english Horizontal(X-axis) flip
}IMV_FG_EFlipType;

/// \~chinese
///枚举：顺时针旋转角度
/// \~english
/// Enumeration:Rotation angle clockwise
typedef enum _IMV_FG_ERotationAngle
{
	IMV_FG_ROTATION_ANGLE90,							///< \~chinese 顺时针旋转90度	\~english Rotate 90 degree clockwise
	IMV_FG_ROTATION_ANGLE180,							///< \~chinese 顺时针旋转180度	\~english Rotate 180 degree clockwise
	IMV_FG_ROTATION_ANGLE270,							///< \~chinese 顺时针旋转270度	\~english Rotate 270 degree clockwise
}IMV_FG_ERotationAngle;

/// \~chinese
/// \brief 像素转换结构体
/// \~english
/// \brief Pixel convert structure
typedef struct _IMV_FG_PixelConvertParam
{
	unsigned int			nWidth;							///< [IN]	\~chinese 图像宽						\~english Width
	unsigned int			nHeight;						///< [IN]	\~chinese 图像高						\~english Height
	IMV_FG_EPixelType		ePixelFormat;					///< [IN]	\~chinese 像素格式						\~english Pixel format
	unsigned char*			pSrcData;						///< [IN]	\~chinese 输入图像数据					\~english Input image data
	unsigned int			nSrcDataLen;					///< [IN]	\~chinese 输入图像长度					\~english Input image length
	unsigned int			nPaddingX;						///< [IN]	\~chinese 图像宽填充					\~english Padding X
	unsigned int			nPaddingY;						///< [IN]	\~chinese 图像高填充					\~english Padding Y
	IMV_FG_EBayerDemosaic	eBayerDemosaic;					///< [IN]	\~chinese 转换Bayer格式算法				\~english Alorithm used for Bayer demosaic
	IMV_FG_EPixelType		eDstPixelFormat;				///< [IN]	\~chinese 目标像素格式					\~english Destination pixel format
	unsigned char*			pDstBuf;						///< [OUT]	\~chinese 输出数据缓存(调用者分配缓存)	\~english Output data buffer(allocated cache by the caller)
	unsigned int			nDstBufSize;					///< [IN]   \~chinese 提供的输出缓冲区大小  		\~english Provided output buffer size
	unsigned int			nDstDataLen;					///< [OUT]	\~chinese 输出数据长度          		\~english Output data length
	unsigned int			nReserved[8];					///<		\~chinese 预留							\~english Reserved field
}IMV_FG_PixelConvertParam;

/// \~chinese
/// \brief 录像结构体
/// \~english
/// \brief Record structure
typedef struct _IMV_FG_RecordParam
{
	unsigned int			nWidth;							///< [IN]	\~chinese 图像宽						\~english Width
	unsigned int			nHeight;						///< [IN]	\~chinese 图像高						\~english Height
	float                   fFameRate;						///< [IN]	\~chinese 帧率(大于0)					\~english Frame rate(greater than 0)
	unsigned int            nQuality;						///< [IN]	\~chinese 视频质量(1-100)				\~english Video quality(1-100)
	IMV_FG_EVideoType		recordFormat;					///< [IN]	\~chinese 视频格式						\~english Video format
	const char*				pRecordFilePath;				///< [IN]	\~chinese 保存视频路径          		\~english Save video path
	unsigned int            nReserved[5];					///<		\~chinese 预留							\~english Reserved field
}IMV_FG_RecordParam;

/// \~chinese
/// \brief 录像用帧信息结构体
/// \~english
/// \brief Frame information for recording structure
typedef struct _IMV_FG_RecordFrameInfoParam
{
	unsigned char*			pData;							///< [IN]	\~chinese 图像数据						\~english Image data
	unsigned int			nDataLen;						///< [IN]	\~chinese 图像数据长度					\~english Image data length
	unsigned int			nPaddingX;						///< [IN]	\~chinese 图像宽填充					\~english Padding X
	unsigned int			nPaddingY;						///< [IN]	\~chinese 图像高填充					\~english Padding Y
	IMV_FG_EPixelType		ePixelFormat;					///< [IN]	\~chinese 像素格式						\~english Pixel format
	unsigned int            nReserved[5];					///<		\~chinese 预留							\~english Reserved field
}IMV_FG_RecordFrameInfoParam;

/// \~chinese
/// \brief 图像翻转结构体
/// \~english
/// \brief Flip image structure
typedef struct _IMV_FG_FlipImageParam
{
	unsigned int			nWidth;							///< [IN]	\~chinese 图像宽						\~english Width
	unsigned int			nHeight;						///< [IN]	\~chinese 图像高						\~english Height
	IMV_FG_EPixelType		ePixelFormat;					///< [IN]	\~chinese 像素格式						\~english Pixel format
	IMV_FG_EFlipType		eFlipType;						///< [IN]	\~chinese 翻转类型						\~english Flip type
	unsigned char*			pSrcData;						///< [IN]	\~chinese 输入图像数据					\~english Input image data
	unsigned int			nSrcDataLen;					///< [IN]	\~chinese 输入图像长度					\~english Input image length
	unsigned char*			pDstBuf;						///< [OUT]	\~chinese 输出数据缓存(调用者分配缓存)	\~english Output data buffer(allocated cache by the caller)
	unsigned int			nDstBufSize;					///< [IN]   \~chinese 提供的输出缓冲区大小  		\~english Provided output buffer size
	unsigned int			nDstDataLen;					///< [OUT]	\~chinese 输出数据长度          		\~english Output data length
	unsigned int            nReserved[8];					///<		\~chinese 预留							\~english Reserved field
}IMV_FG_FlipImageParam;

/// \~chinese
/// \brief 图像旋转结构体
/// \~english
/// \brief Rotate image structure
typedef struct _IMV_FG_RotateImageParam
{
	unsigned int			nWidth;							///< [IN][OUT]	\~chinese 图像宽						\~english Width
	unsigned int			nHeight;						///< [IN][OUT]	\~chinese 图像高						\~english Height
	IMV_FG_EPixelType		ePixelFormat;					///< [IN]		\~chinese 像素格式						\~english Pixel format
	IMV_FG_ERotationAngle	eRotationAngle;					///< [IN]		\~chinese 旋转角度						\~english Rotation angle
	unsigned char*			pSrcData;						///< [IN]		\~chinese 输入图像数据					\~english Input image data
	unsigned int			nSrcDataLen;					///< [IN]		\~chinese 输入图像长度					\~english Input image length
	unsigned char*			pDstBuf;						///< [OUT]		\~chinese 输出数据缓存(调用者分配缓存)	\~english Output data buffer(allocated cache by the caller)
	unsigned int			nDstBufSize;					///< [IN]		\~chinese 提供的输出缓冲区大小  		\~english Provided output buffer size
	unsigned int			nDstDataLen;					///< [OUT]		\~chinese 输出数据长度          		\~english Output data length
	unsigned int			nReserved[8];					///<			\~chinese 预留							\~english Reserved field
}IMV_FG_RotateImageParam;

/// \~chinese
/// \brief 枚举：图像保存格式
/// \~english
/// \brief Enumeration:image save format
typedef enum _IMV_FG_ESaveFileType
{
	IMV_FG_TYPE_SAVE_BMP	= 0,							///< \~chinese BMP图像格式                  \~english BMP Image Type
	IMV_FG_TYPE_SAVE_JPEG	= 1,							///< \~chinese JPEG图像格式                 \~english Jpeg Image Type
	IMV_FG_TYPE_SAVE_PNG	= 2,							///< \~chinese PNG图像格式                  \~english Png  Image Type
	IMV_FG_TYPE_SAVE_TIFF	= 3,							///< \~chinese TIFF图像格式                 \~english TIFF Image Type
	IMV_FG_TYPE_UNDEFINED	= 255,							///< \~chinese 未定义的图像格式             \~english Undefined Image Type
}IMV_FG_ESaveFileType;

/// \~chinese
/// \brief 保存图像结构体
/// \~english
/// \brief Save image structure
typedef struct _IMV_FG_SaveImageToFileParam
{
	unsigned int            nWidth;							///< [IN]   \~chinese 图像宽								\~english Width
	unsigned int            nHeight;						///< [IN]   \~chinese 图像高							    \~english Height
	IMV_FG_EPixelType       ePixelFormat;					///< [IN]   \~chinese 输入数据的像素格式				    \~english Pixel format
	unsigned char*          pSrcData;						///< [IN]	\~chinese 输入图像数据							\~english Input image data
	unsigned int            nSrcDataLen;					///< [IN]	\~chinese 输入数据大小							\~english Input image length

	IMV_FG_ESaveFileType	eImageType;						///< [IN]	\~chinese 输入保存图片格式						\~english Input image save format
	char*                   pImagePath;						///< [IN]	\~chinese 输入文件路径							\~english Input image save path

	unsigned int            nQuality;						///< [IN]	\~chinese JPG编码质量(50-99]，PNG编码质量[0-9]	\~english Encoding quality 
	IMV_FG_EBayerDemosaic   eBayerDemosaic;					///< [IN]	\~chinese 转换Bayer格式算法						\~english Alorithm used for Bayer demosaic
	unsigned int            nReserved[8];					///<		\~chinese 预留									\~english Reserved

}IMV_FG_SaveImageToFileParam;

/// \~chinese
///枚举：事件类型
/// \~english
/// Enumeration:event type
typedef enum _IMV_FG_EVType
{
	IMV_FG_OFFLINE,									///< \~chinese 设备离线通知		\~english device offline notification
	IMV_FG_ONLINE									///< \~chinese 设备在线通知		\~english device online notification
}IMV_FG_EVType;

/// \~chinese
/// \brief 连接事件信息
/// \~english
/// \brief connection event information
typedef struct _IMV_FG_SConnectArg
{
	IMV_FG_EVType		event;						///< \~chinese 事件类型			\~english event type
	unsigned int		nReserve[10];				///< \~chinese 预留字段			\~english Reserved field
}IMV_FG_SConnectArg;

/// \~chinese
/// 消息通道事件ID列表
/// \~english
/// message channel event id list
#define IMV_FG_MSG_EVENT_ID_EXPOSURE_END			0x9001
#define IMV_FG_MSG_EVENT_ID_FRAME_TRIGGER			0x9002
#define IMV_FG_MSG_EVENT_ID_FRAME_START				0x9003
#define IMV_FG_MSG_EVENT_ID_ACQ_START				0x9004
#define IMV_FG_MSG_EVENT_ID_ACQ_TRIGGER				0x9005
#define IMV_FG_MSG_EVENT_ID_DATA_READ_OUT			0x9006
#define IMV_FG_MSG_EVENT_ID_FRAME_END				0x9007
#define IMV_FG_MSG_EVENT_ID_FRAMEACTIVE_START		0x9008
#define IMV_FG_MSG_EVENT_ID_FRAMEACTIVE_END			0x9009
#define IMV_FG_MSG_EVENT_ID_FIRST_LINE				0x900A
#define IMV_FG_MSG_EVENT_ID_LAST_LINE				0x900B

/// \~chinese
/// \brief 消息通道事件信息
/// \~english
/// \brief Message channel event information(Common to equipment)
typedef struct _IMV_FG_SMsgEventArg
{
	unsigned short	eventId;                        ///< \~chinese 事件Id									\~english Event id
	unsigned short	channelId;						///< \~chinese 消息通道号								\~english Channel id
	uint64_t		blockId;						///< \~chinese 流数据BlockID								\~english Block ID of stream data
	uint64_t		timeStamp;						///< \~chinese 时间戳									\~english Event timestamp
	void*			pEventData;						///< \~chinese 事件数据，内部缓存，需要及时进行数据处理	\~english Event data, internal buffer, need to be processed in time
	unsigned int	nEventDataSize;					///< \~chinese 事件数据长度								\~english Event data size
	unsigned int	reserve[8];						///< \~chinese 预留字段									\~english Reserved field
}IMV_FG_SMsgEventArg;

/// \~chinese
/// \brief 设备连接状态事件回调函数声明
/// \param pParamUpdateArg [in] 回调时主动推送的设备连接状态事件信息
/// \param pUser [in] 用户自定义数据
/// \~english
/// \brief Call back function declaration of device connection status event 
/// \param pStreamArg [in] The device connection status event which will be active pushed out during the callback
/// \param pUser [in] User defined data
typedef void(*IMV_FG_ConnectCallBack)(const IMV_FG_SConnectArg* pConnectArg, void* pUser);

/// \~chinese
/// \brief 帧数据信息回调函数声明
/// \param pFrame [in] 回调时主动推送的帧信息
/// \param pUser [in] 用户自定义数据
/// \~english
/// \brief Call back function declaration of frame data information
/// \param pFrame [in] The frame information which will be active pushed out during the callback
/// \param pUser [in] User defined data
typedef void(*IMV_FG_FrameCallBack)(IMV_FG_Frame* pFrame, void* pUser);

/// \~chinese
/// \brief 消息通道事件回调函数声明（设备通用）
/// \param pMsgChannelArg [in] 回调时主动推送的消息通道事件信息
/// \param pUser [in] 用户自定义数据
/// \~english
/// \brief Call back function declaration of message channel event(Common to equipment)
/// \param pMsgChannelArg [in] The message channel event which will be active pushed out during the callback
/// \param pUser [in] User defined data
typedef void(*IMV_FG_MsgChannelCallBack)(const IMV_FG_SMsgEventArg* pMsgChannelArg, void* pUser);

#endif
