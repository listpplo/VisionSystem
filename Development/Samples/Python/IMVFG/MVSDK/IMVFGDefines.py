#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ctypes import *
from enum import Enum
import string
import struct

int8_t = c_int8
int16_t = c_int16
int32_t = c_int32
int64_t = c_int64
uint8_t = c_uint8
uint16_t = c_uint16
uint32_t = c_uint32
uint64_t = c_uint64
int_least8_t = c_byte
int_least16_t = c_short
int_least32_t = c_int
int_least64_t = c_long
uint_least8_t = c_ubyte
uint_least16_t = c_ushort
uint_least32_t = c_uint
uint_least64_t = c_ulong
int_fast8_t = c_byte
int_fast16_t = c_long
int_fast32_t = c_long
int_fast64_t = c_long
uint_fast8_t = c_ubyte
uint_fast16_t = c_ulong
uint_fast32_t = c_ulong
uint_fast64_t = c_ulong
intptr_t = c_long
uintptr_t = c_ulong
intmax_t = c_long
uintmax_t = c_ulong

MAX_STRING_LENTH     = 256


class _IMV_FG_String_(Structure):
    pass
_IMV_FG_String_._fields_ = [
    ('str',c_char * MAX_STRING_LENTH)
    ]
IMV_FG_String = _IMV_FG_String_




def enum(**enums):
    return type('Enum', (), enums)

IMV_FG_OK                       = 0         # < \~chinese 成功，无错误							\~english Successed, no error

IMV_FG_ERROR					= -101		# < \~chinese 通用的错误							\~english Generic error
IMV_FG_INVALID_HANDLE			= -102		# < \~chinese 错误或无效的句柄						\~english Error or invalid handle
IMV_FG_INVALID_PARAM			= -103		# < \~chinese 错误的参数							\~english Incorrect parameter
IMV_FG_INVALID_FRAME_HANDLE	    = -104		# < \~chinese 错误或无效的帧句柄					\~english Error or invalid frame handle
IMV_FG_INVALID_FRAME			= -105		# < \~chinese 无效的帧								\~english Invalid frame
IMV_FG_INVALID_RESOURCE		    = -106		# < \~chinese 相机/事件/流等资源无效				\~english Camera/Event/Stream and so on resource invalid
IMV_FG_INVALID_IP				= -107		# < \~chinese 设备与主机的IP网段不匹配				\~english Device's and PC's subnet is mismatch
IMV_FG_NO_MEMORY				= -108		# < \~chinese 内存不足								\~english Malloc memery failed
IMV_FG_INSUFFICIENT_MEMORY		= -109		# < \~chinese 传入的内存空间不足					\~english Insufficient memory
IMV_FG_ERROR_PROPERTY_TYPE		= -110		# < \~chinese 属性类型错误							\~english Property type error
IMV_FG_INVALID_ACCESS			= -111		# < \~chinese 属性不可访问、或不能读/写、或读/写失败	\~english Property not accessible, or not be read/written, or read/written failed
IMV_FG_INVALID_RANGE			= -112		# < \~chinese 属性值超出范围、或者不是步长整数倍	\~english The property's value is out of range, or is not integer multiple of the step
IMV_FG_NOT_SUPPORT				= -113		# < \~chinese 设备不支持的功能						\~english Device not supported function
IMV_FG_NO_DATA					= -114		# < \~chinese 无数据								\~english no data
IMV_FG_PARAM_OVERFLOW			= -115		# < \~chinese 参数值越界							\~english param value overflow
IMV_FG_NOT_AVAILABLE			= -116		# < \~chinese 连接不可达							\~english connect not available

IMV_FG_MAX_STRING_LENTH			= 256		# < \~chinese 字符串最大长度		\~english The maximum length of string
IMV_FG_MAX_ERROR_LIST_NUM		= 128		# < \~chinese 失败属性列表最大长度 \~english The maximum size of failed properties list
IMV_FG_MAX_CAM_DEVICE_NUM		= 5			#单卡最大相机数量


IMV_FG_IF_HANDLE = c_void_p			        # < \~chinese 采集卡设备句柄							\~english Interface handle
IMV_FG_DEV_HANDLE = c_void_p		        # < \~chinese 相机设备句柄								\~english Device handle
HANDLE = c_void_p					        # < \~chinese 设备句柄(采集卡设备句柄或者相机设备句柄)	\~english Device handle or Interface handle
IMV_FG_FRAME_HANDLE = c_void_p			    # < \~chinese 帧句柄									\~english frame handle



""" /// \~chinese
///枚举：属性类型
/// \~english
///Enumeration: property type """
IMV_FG_EFeatureType =enum(
                            IMV_FG_FEATURE_INT = 0x10000000,				
                            IMV_FG_FEATURE_FLOAT = 0x20000000,			
                            IMV_FG_FEATURE_ENUM = 0x30000000,				
                            IMV_FG_FEATURE_BOOL = 0x40000000,			
                            IMV_FG_FEATURE_STRING = 0x50000000,			
                            IMV_FG_FEATURE_COMMAND = 0x60000000,	
                            IMV_FG_FEATURE_GROUP = 0x70000000,				
                            IMV_FG_FEATURE_REG = 0x80000000,				
                            IMV_FG_FEATURE_UNDEFINED = 0x90000000			
                        )
                            			
                        




""" /// \~chinese
///枚举：接口类型
/// \~english
///Enumeration: interface type"""
IMV_FG_EInterfaceType =enum(
                            typeGigEInterface = 0x00000001,
                            typeU3vInterface = 0x00000002,
                            typeCLInterface  = 0x00000004,
                            typeCXPInterface = 0x00000008,
                            typeUndefinedInterface = 0xFFFFFFFF			
                        )




               	

"""// CL采集卡模式
/// \~chinese
///信息：CameraLink采集卡模式
/// \~english
///Information: CameraLink Interface Mode"""     
EInterfaceMode =enum(
                            FULL_MODE = 0,			# < \~chinese 单Full模式					\~english full
                            DUAL_BASE_MODE = 1,		# < \~chinese 双Base，同时支持2个相机		\~english Dual base
                            DUAL_FULL_MODE = 2,		# < \~chinese 双Full，同时支持2个相机		\~english Dual full
                            QUAD_BASE_MODE = 3,		# < \~chinese 4Base，同时支持4个相机		\~english Quad full
                            TYPE_UNDEFINED = 255    # < \~chinese 未知类型		
                        )


                    


"""// CL采集卡信息
/// \~chinese
///信息：CameraLink采集卡信息
/// \~english
///Information: CameraLink Interface Information"""
class _IMV_CL_INTERFACE_INFO_(Structure):
    pass
_IMV_CL_INTERFACE_INFO_._fields_=[
        ('nInterfaceMode', c_int),             #///< \~chinese 采集卡模式				\~english Interface mode
        ('nPCIEInfo', c_uint),                 #///< \~chinese 采集卡的PCIE插槽信息		\~english PCIE Info
        ('nReserved', c_uint * 64)             #///< \~chinese 预留

]
IMV_CL_INTERFACE_INFO = _IMV_CL_INTERFACE_INFO_



class InterfaceInfo(Union):
    pass
InterfaceInfo._fields_=[
        ('CLInterfaceInfo', IMV_CL_INTERFACE_INFO),         #///< \~chinese CameraLink采集卡信息	\~english CameraLink Capture Card info
        ('nReserved', c_uint * 128),                        #///< \~chinese 限定长度				\~english limit length
]

class _IMV_FG_INTERFACE_INFO_(Structure):
    pass
_IMV_FG_INTERFACE_INFO_._fields_=[
        ('nInterfaceType', c_int),                                          #///< \~chinese 采集卡类型		\~english Interface type
        ('nInterfaceReserved',c_uint * 8),                                  #///< \~chinese 保留字段
        ('interfaceKey', c_char * IMV_FG_MAX_STRING_LENTH),                 #///< \~chinese 厂商:序列号:端口	\~english Interface key
        ('interfaceName',c_char * IMV_FG_MAX_STRING_LENTH),                 #///< \~chinese 用户自定义名		\~english UserDefinedName
        ('serialNumber', c_char * IMV_FG_MAX_STRING_LENTH),                 #///< \~chinese 设备序列号		\~english Interface SerialNumber
        ('vendorName', c_char * IMV_FG_MAX_STRING_LENTH),                   #///< \~chinese 厂商				\~english Interface Vendor

        ('modelName', c_char * IMV_FG_MAX_STRING_LENTH),                    #///< \~chinese 设备型号			\~english Interface model
        ('manufactureInfo', c_char * IMV_FG_MAX_STRING_LENTH),              #///< \~chinese 设备制造信息		\~english Interface ManufactureInfo
        ('deviceVersion', c_char * IMV_FG_MAX_STRING_LENTH),                #///< \~chinese 设备版本			\~english Interface Version
        ('interfaceReserved', c_char * IMV_FG_MAX_STRING_LENTH * 5),        #///< \~chinese 保留				\~english Reserved field
        ('InterfaceInfo', InterfaceInfo)
]
IMV_FG_INTERFACE_INFO = _IMV_FG_INTERFACE_INFO_





class _IMV_FG_INTERFACE_INFO_LIST_(Structure):
    pass
_IMV_FG_INTERFACE_INFO_LIST_._fields_=[
        ('nInterfaceNum', c_uint),
        ('pInterfaceInfoList', POINTER(IMV_FG_INTERFACE_INFO))
]
IMV_FG_INTERFACE_INFO_LIST = _IMV_FG_INTERFACE_INFO_LIST_




""" /// \~chinese
///枚举：创建句柄方式
/// \~english
///Enumeration: Create handle mode """
IMV_FG_ECreateHandleMode =enum(
                            IMV_FG_MODE_BY_INDEX = 0,						
                            IMV_FG_MODE_BY_CAMERAKEY=1,						
                            IMV_FG_MODE_BY_DEVICE_USERID=2,						
                            IMV_FG_MODE_BY_IPADDRESS=3			
                        )
                            
                        

""" /// \~chinese
///枚举：设备类型
/// \~english
///Enumeration: device type """
IMV_FG_EDeviceType =enum(
	IMV_FG_TYPE_GIGE_DEVICE = 0,					# < \~chinese GIGE相机				\~english GigE Vision Camera
	IMV_FG_TYPE_U3V_DEVICE = 1,						# < \~chinese USB3.0相机			\~english USB3.0 Vision Camera
	IMV_FG_TYPE_CL_DEVICE = 2,						# < \~chinese CAMERALINK 相机		\~english Cameralink camera
	IMV_FG_TYPE_CXP_DEVICE = 3,						# < \~chinese PCIe相机				\~english PCIe Camera
	IMV_FG_TYPE_UNDEFINED_DEVICE = 255				# < \~chinese 未知类型				\~english Undefined Camera
                        )



class _IMV_FG_DEVICE_INFO_(Structure):
    pass
_IMV_FG_DEVICE_INFO_._fields_=[
        ('nDeviceType',c_int),                                                  #///< \~chinese 设备类型
        ('nReserved', c_uint * 8),                                              #///< \~chinese 保留字段
        ('cameraKey',c_char * IMV_FG_MAX_STRING_LENTH),                         #///< \~chinese 厂商:序列号		\~english Device key
        ('cameraName', c_char * IMV_FG_MAX_STRING_LENTH),                       #///< \~chinese 用户自定义名		\~english UserDefinedName
        ('serialNumber', c_char * IMV_FG_MAX_STRING_LENTH),                     #///< \~chinese 设备序列号		\~english Device SerialNumber
        ('vendorName', c_char * IMV_FG_MAX_STRING_LENTH),                       #///< \~chinese 厂商				\~english Device Vendor
        ('modelName', c_char * IMV_FG_MAX_STRING_LENTH),                        #///< \~chinese 设备型号			\~english Device model
        ('manufactureInfo', c_char * IMV_FG_MAX_STRING_LENTH),                  #///< \~chinese 设备制造信息		\~english Device ManufactureInfo
        ('deviceVersion', c_char * IMV_FG_MAX_STRING_LENTH),                    #///< \~chinese 设备版本			\~english Device Version
        ('cameraReserved', c_char * IMV_FG_MAX_STRING_LENTH * 5),               #///< \~chinese 保留				\~english Reserved field
        ('FGInterfaceInfo',IMV_FG_INTERFACE_INFO)                               #///< \~chinese 采集卡信息		\~english Capture Card info
]
IMV_FG_DEVICE_INFO = _IMV_FG_DEVICE_INFO_



class _IMV_FG_DEVICE_INFO_LIST_(Structure):
    pass
_IMV_FG_DEVICE_INFO_LIST_._fields_=[
        ('nDevNum', c_uint),
        ('pDeviceInfoList', POINTER(IMV_FG_DEVICE_INFO))
]
IMV_FG_DEVICE_INFO_LIST = _IMV_FG_DEVICE_INFO_LIST_



IMV_FG_PIX_MONO                           =0x01000000
IMV_FG_PIX_RGB                            =0x02000000
IMV_FG_PIX_COLOR                          =0x02000000
IMV_FG_PIX_CUSTOM                         =0x80000000
IMV_FG_PIX_COLOR_MASK                     =0xFF000000

"""Indicate effective number of bits occupied by the pixel (including padding).
// This can be used to compute amount of memory required to store an image."""
IMV_FG_PIX_OCCUPY1BIT                     =0x00010000
IMV_FG_PIX_OCCUPY2BIT                     =0x00020000
IMV_FG_PIX_OCCUPY4BIT                     =0x00040000
IMV_FG_PIX_OCCUPY8BIT                     =0x00080000
IMV_FG_PIX_OCCUPY12BIT                    =0x000C0000
IMV_FG_PIX_OCCUPY16BIT                    =0x00100000
IMV_FG_PIX_OCCUPY24BIT                    =0x00180000
IMV_FG_PIX_OCCUPY32BIT                    =0x00200000
IMV_FG_PIX_OCCUPY36BIT                    =0x00240000
IMV_FG_PIX_OCCUPY48BIT                    =0x00300000


""" /// \~chinese
/// \brief 枚举属性的枚举值信息
/// \~english
/// \brief Enumeration property 's enumeration value information """
class _IMV_FG_EnumEntryInfo_(Structure):
    pass
_IMV_FG_EnumEntryInfo_._fields_=[
        ('value', c_uint64),
        ('name',c_char*IMV_FG_MAX_STRING_LENTH),
]
IMV_FG_EnumEntryInfo=_IMV_FG_EnumEntryInfo_

""" /// \~chinese
/// \brief 枚举属性的可设枚举值列表信息
/// \~english
/// \brief Enumeration property 's settable enumeration value list information """
class _IMV_FG_EnumEntryList_(Structure):
    pass
_IMV_FG_EnumEntryList_._fields_=[
        ('nEnumEntryBufferSize', c_uint),
        ('pEnumEntryInfo',POINTER(IMV_FG_EnumEntryInfo))
]
IMV_FG_EnumEntryList=_IMV_FG_EnumEntryList_


IMV_FG_EPixelType = enum(
    # Undefined pixel type
	IMV_FG_PIXEL_TYPE_Undefined = -1,

	# Mono Format
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

	# Bayer Format
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

	# RGB Format
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

	# YVR Format
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

	# RGB Planar
	IMV_FG_PIXEL_TYPE_RGB8Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY24BIT | 0x0021),
	IMV_FG_PIXEL_TYPE_RGB10Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0022),
	IMV_FG_PIXEL_TYPE_RGB12Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0023),
	IMV_FG_PIXEL_TYPE_RGB16Planar = (IMV_FG_PIX_COLOR | IMV_FG_PIX_OCCUPY48BIT | 0x0024),

	# BayerRG10p和BayerRG12p格式，针对特定项目临时添加,请不要使用
	# BayerRG10p and BayerRG12p, currently used for specific project, please do not use them
	IMV_FG_PIXEL_TYPE_BayRG10p = 0x010A0058,
	IMV_FG_PIXEL_TYPE_BayRG12p = 0x010c0059,

	# mono1c格式，自定义格式
	# mono1c, customized image format, used for binary output
	IMV_FG_PIXEL_TYPE_Mono1c = 0x012000FF,

	# mono1e格式，自定义格式，用来显示连通域
	# mono1e, customized image format, used for displaying connected domain
	IMV_FG_PIXEL_TYPE_Mono1e = 0x01080FFF
)


"""/// \~chinese
/// \brief 帧图像信息
/// \~english
/// \brief The frame image information"""
class _IMV_FG_FrameInfo_(Structure):
    pass
_IMV_FG_FrameInfo_._fields_=[
        ('blockId', c_uint64),
        ('status', c_uint),
        ('width',c_uint),
        ('height', c_uint),
        ('size', c_uint),
        ('pixelFormat', c_int),
        ('timeStamp', c_uint64),
        ('chunkCount', c_uint),
        ('paddingX', c_uint),
        ('paddingY', c_uint),
        ('recvFrameTime', c_uint),
        ('nReserved', c_uint*19)

]
IMV_FG_FrameInfo = _IMV_FG_FrameInfo_

""" /// \~chinese
/// \brief 加载失败的属性信息
/// \~english
/// \brief Load failed properties information """
class _IMV_FG_ErrorList_ (Structure):
    pass
_IMV_FG_ErrorList_._fields_ = [
        ('nParamCnt', c_uint),
        ('paramNameList', IMV_FG_String * IMV_FG_MAX_ERROR_LIST_NUM),
    ]
IMV_FG_ErrorList=_IMV_FG_ErrorList_


""" /// \~chines
/// \brief Chunk数据信息
/// \~english
/// \brief Chunk data information """
class _IMV_FG_ChunkDataInfo_(Structure):
    pass
_IMV_FG_ChunkDataInfo_._fields_=[
        ('chunkID', c_uint),
        ('nParamCnt', c_uint),
        ('pParamNameList', POINTER(IMV_FG_String))
]
IMV_FG_ChunkDataInfo=_IMV_FG_ChunkDataInfo_


""" /// \~chinese
/// \brief 帧图像数据信息
/// \~english
/// \brief Frame image data information """
class _IMV_FG_Frame_(Structure):
    pass
_IMV_FG_Frame_._fields_=[
        ('frameHandle', c_void_p),
        ('pData', POINTER(c_ubyte)),
        ('frameInfo', IMV_FG_FrameInfo),
        ('nReserved', c_uint*10),
]
IMV_FG_Frame=_IMV_FG_Frame_




""" /// \~chinese
///枚举：抓图策略
/// \~english
///Enumeration: grab strartegy """
IMV_FG_EGrabStrategy=enum(
    IMV_FG_GRAB_STRARTEGY_SEQUENTIAL = 0,			
	IMV_FG_GRAB_STRARTEGY_LATEST_IMAGE = 1,			
	IMV_FG_GRAB_STRARTEGY_UPCOMING_IMAGE = 2,			
	IMV_FG_GRAB_STRARTEGY_UNDEFINED = 3
)	




""" /// \~chinese
/// \brief PCIE设备统计流信息
/// \~english
/// \brief PCIE device stream statistics information """
class _IMV_FG_CLStreamStatsInfo_(Structure):
    pass
_IMV_FG_CLStreamStatsInfo_._fields_=[
        ('imageError', c_uint),
        ('lostPacketBlock', c_uint),
        ('nReserved0', c_uint*10),
        ('imageReceived', c_uint),
        ('fps', c_double),
        ('bandwidth', c_double),
        ('nReserved', c_uint*8)
]
IMV_FG_CLStreamStatsInfo=_IMV_FG_CLStreamStatsInfo_

""" /// \~chinese
/// \brief U3V设备统计流信息
/// \~english
/// \brief U3V device stream statistics information """
class _IMV_FG_U3VStreamStatsInfo_(Structure):
    pass
_IMV_FG_U3VStreamStatsInfo_._fields_=[
        ('imageError', c_uint),
        ('lostPacketBlock', c_uint),
        ('nReserved0', c_uint*10),
        ('imageReceived', c_uint),
        ('fps', c_double),
        ('bandwidth', c_double),
        ('nReserved', c_uint*8)
]
IMV_FG_U3VStreamStatsInfo=_IMV_FG_U3VStreamStatsInfo_

""" /// \~chinese
/// \brief Gige设备统计流信息
/// \~english
/// \brief Gige device stream statistics information """
class _IMV_FG_GigEStreamStatsInfo_(Structure):
    pass
_IMV_FG_GigEStreamStatsInfo_._fields_=[
        ('nReserved0', c_uint*10),
        ('imageError', c_uint),
        ('lostPacketBlock', c_uint),
        ('nReserved1', c_uint*4),
        ('nReserved2', c_uint*5),
        ('imageReceived', c_uint),
        ('fps', c_double),
        ('bandwidth', c_double),
        ('nReserved', c_uint*4)
]
IMV_FG_GigEStreamStatsInfo=_IMV_FG_GigEStreamStatsInfo_


class _IMV_FG_StreamStatistics_(Union):
    pass
_IMV_FG_StreamStatistics_._fields_=[
        ('pcieStatisticsInfo',IMV_FG_CLStreamStatsInfo),
        ('gigeStatisticsInfo',IMV_FG_GigEStreamStatsInfo),
        ('u3vStatisticsInfo',IMV_FG_U3VStreamStatsInfo),
]
IMV_FG_StreamStatistics = _IMV_FG_StreamStatistics_

""" /// \~chinese
/// \brief 统计流信息
/// \~english
/// \brief Stream statistics information """
class _IMV_FG_StreamStatisticsInfo_(Structure):
    pass
_IMV_FG_StreamStatisticsInfo_._fields_=[
        ('nDeviceType',c_int),
        ('streamStatistics',IMV_FG_StreamStatistics),
]
IMV_FG_StreamStatisticsInfo = _IMV_FG_StreamStatisticsInfo_


""" /// \~chinese
///枚举：图像转换Bayer格式所用的算法
/// \~english
/// Enumeration:alorithm used for Bayer demosaic """
IMV_FG_EBayerDemosaic=enum(
    IMV_FG_DEMOSAIC_NEAREST_NEIGHBOR=0,					
    IMV_FG_DEMOSAIC_BILINEAR=1,							
    IMV_FG_DEMOSAIC_EDGE_SENSING=2,						
    IMV_FG_DEMOSAIC_NOT_SUPPORT = 255,					
)


""" /// \~chinese
///枚举：视频格式
/// \~english
/// Enumeration:Video format """
IMV_FG_EVideoType=enum(
    IMV_FG_TYPE_VIDEO_FORMAT_AVI = 0,						
    IMV_FG_TYPE_VIDEO_FORMAT_NOT_SUPPORT = 255,
)

""" /// \~chinese
///枚举：图像翻转类型
/// \~english
/// Enumeration:Image flip type """
IMV_FG_EFlipType = enum(
    IMV_FG_TYPE_FLIP_VERTICAL=0,							#///< \~chinese 垂直(Y轴)翻转	\~english Vertical(Y-axis) flip
    IMV_FG_TYPE_FLIP_HORIZONTAL=1							#///< \~chinese 水平(X轴)翻转	\~english Horizontal(X-axis) flip
)

""" /// \~chinese
///枚举：顺时针旋转角度
/// \~english
/// Enumeration:Rotation angle clockwise """
IMV_FG_ERotationAngle= enum(
    IMV_FG_ROTATION_ANGLE90=0,							#///< \~chinese 顺时针旋转90度	\~english Rotate 90 degree clockwise
    IMV_FG_ROTATION_ANGLE180=1,							#///< \~chinese 顺时针旋转180度	\~english Rotate 180 degree clockwise
    IMV_FG_ROTATION_ANGLE270=2							#///< \~chinese 顺时针旋转270度	\~english Rotate 270 degree clockwise
)


""" /// \~chinese
/// \brief 像素转换结构体
/// \~english
/// \brief Pixel convert structure """
class _IMV_FG_PixelConvertParam_(Structure):
    pass
_IMV_FG_PixelConvertParam_._fields_=[
        ('nWidth', c_uint),
        ('nHeight',c_uint),
        ('ePixelFormat',c_int),
        ('pSrcData',POINTER(c_ubyte)),
        ('nSrcDataLen', c_uint),
        ('nPaddingX',c_uint),
        ('nPaddingY',c_uint),
        ('eBayerDemosaic',c_int),
        ('eDstPixelFormat', c_int),
        ('pDstBuf',POINTER(c_ubyte)),
        ('nDstBufSize',c_uint),
        ('nDstDataLen',c_uint),
        ('nReserved',c_uint*8)
]
IMV_FG_PixelConvertParam=_IMV_FG_PixelConvertParam_

""" /// \~chinese
/// \brief 图像翻转结构体
/// \~english
/// \brief Flip image structure """
class  _IMV_FG_FlipImageParam_(Structure):
    pass
_IMV_FG_FlipImageParam_._fields_=[
        ('nWidth', c_uint),
        ('nHeight',c_uint),
        ('ePixelFormat',c_int),
        ('eFlipType',c_int),
        ('pSrcData', POINTER(c_ubyte)),
        ('nSrcDataLen', c_uint),
        ('pDstBuf',POINTER(c_ubyte)),
        ('nDstBufSize',c_uint),
        ('nDstDataLen',c_uint),
        ('nReserved', c_uint*8),
]
IMV_FG_FlipImageParam=_IMV_FG_FlipImageParam_

""" /// \~chinese
/// \brief 图像旋转结构体
/// \~english
/// \brief Rotate image structure """
class _IMV_FG_RotateImageParam_(Structure):
    pass
_IMV_FG_RotateImageParam_._fields_=[
        ('nWidth', c_uint),
        ('nHeight',c_uint),
        ('ePixelFormat',c_int),
        ('eRotationAngle',c_int),
        ('pSrcData', POINTER(c_ubyte)),
        ('nSrcDataLen', c_uint),
        ('pDstBuf', POINTER(c_ubyte)),
        ('nDstBufSize',c_uint),
        ('nDstDataLen',c_uint),
        ('nReserved',c_uint*8),
]
IMV_FG_RotateImageParam=_IMV_FG_RotateImageParam_

""" /// \~chinese
///枚举：事件类型
/// \~english
/// Enumeration:event type """
IMV_FG_EVType=enum(
    IMV_FG_OFFLINE=0,
    IMV_FG_ONLINE=1
)

""" /// \~chinese
/// \brief 连接事件信息
/// \~english
/// \brief connection event information """
class _IMV_SConnectArg_(Structure):
    pass
_IMV_SConnectArg_._fields_ = [
    ('event', c_int),
    ('nReserve', c_uint * 10)
]
IMV_SConnectArg=_IMV_SConnectArg_

# """"/// \~chinese
# /// \brief 枚举：图像保存格�?
# /// \~english
# /// \brief Enumeration:image save format"""
IMV_FG_ESaveType = enum(
    typeImageBmp=0,  # ///< \~chinese BMP图像格式                  \~english BMP Image Type
    typeImageJpeg=1,  # ///< \~chinese JPEG图像格式                 \~english Jpeg Image Type
    typeImagePng=2,  # ///< \~chinese PNG图像格式                  \~english Png  Image Type
    typeImageTif=3,  # ///< \~chinese TIFF图像格式                 \~english TIFF Image Type
    typeImageUndefined=255  # ///< \~chinese 未定义的图像格式             \~english Undefined Image Type
)

class _IMV_FG_SaveImageToFileParam_(Structure):
    pass
_IMV_FG_SaveImageToFileParam_._fields_ = [
    ('nWidth', c_uint),
    ('nHeight', c_uint),
    ('nPixelFormat', c_uint),
    ('pSrcData', POINTER(c_ubyte)),
    ('nSrcDataLen', c_uint),
    ('eImageType', c_int),
    ('pImagePath', c_char_p),
    ('nQuality', c_uint),
    ('nBayerDemosaic', c_int),
    ('nReserved', c_uint * 8)
]
IMV_FG_SaveImageToFileParam = _IMV_FG_SaveImageToFileParam_













