#!/usr/bin/env python
# -*- coding: utf-8 -*-

from ctypes import *


def enum(**enums):
    return type('Enum', (), enums)


IMV_GVSP_PIX_MONO = 0x01000000
IMV_GVSP_PIX_RGB = 0x02000000
IMV_GVSP_PIX_COLOR = 0x02000000
IMV_GVSP_PIX_CUSTOM = 0x80000000
IMV_GVSP_PIX_COLOR_MASK = 0xFF000000

IMV_GVSP_PIX_OCCUPY1BIT = 0x00010000
IMV_GVSP_PIX_OCCUPY2BIT = 0x00020000
IMV_GVSP_PIX_OCCUPY4BIT = 0x00040000
IMV_GVSP_PIX_OCCUPY8BIT = 0x00080000
IMV_GVSP_PIX_OCCUPY12BIT = 0x000C0000
IMV_GVSP_PIX_OCCUPY16BIT = 0x00100000
IMV_GVSP_PIX_OCCUPY24BIT = 0x00180000
IMV_GVSP_PIX_OCCUPY32BIT = 0x00200000
IMV_GVSP_PIX_OCCUPY36BIT = 0x00240000
IMV_GVSP_PIX_OCCUPY48BIT = 0x00300000
IMV_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_MASK = 0x00FF0000
IMV_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_SHIFT = 16

IMV_EPixelType = enum(
    # Undefined pixel type
    gvspPixelTypeUndefined=-1,

    # Mono Format
    gvspPixelMono1p=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY1BIT | 0x0037),
    gvspPixelMono2p=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY2BIT | 0x0038),
    gvspPixelMono4p=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY4BIT | 0x0039),
    gvspPixelMono8=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY8BIT | 0x0001),
    gvspPixelMono8S=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY8BIT | 0x0002),
    gvspPixelMono10=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0003),
    gvspPixelMono10Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x0004),
    gvspPixelMono12=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0005),
    gvspPixelMono12Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x0006),
    gvspPixelMono14=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0025),
    gvspPixelMono16=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0007),

    # Bayer Format
    gvspPixelBayGR8=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY8BIT | 0x0008),
    gvspPixelBayRG8=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY8BIT | 0x0009),
    gvspPixelBayGB8=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY8BIT | 0x000A),
    gvspPixelBayBG8=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY8BIT | 0x000B),
    gvspPixelBayGR10=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x000C),
    gvspPixelBayRG10=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x000D),
    gvspPixelBayGB10=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x000E),
    gvspPixelBayBG10=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x000F),
    gvspPixelBayGR12=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0010),
    gvspPixelBayRG12=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0011),
    gvspPixelBayGB12=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0012),
    gvspPixelBayBG12=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0013),
    gvspPixelBayGR10Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x0026),
    gvspPixelBayRG10Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x0027),
    gvspPixelBayGB10Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x0028),
    gvspPixelBayBG10Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x0029),
    gvspPixelBayGR12Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x002A),
    gvspPixelBayRG12Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x002B),
    gvspPixelBayGB12Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x002C),
    gvspPixelBayBG12Packed=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY12BIT | 0x002D),
    gvspPixelBayGR16=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x002E),
    gvspPixelBayRG16=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x002F),
    gvspPixelBayGB16=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0030),
    gvspPixelBayBG16=(IMV_GVSP_PIX_MONO | IMV_GVSP_PIX_OCCUPY16BIT | 0x0031),

    # RGB Format
    gvspPixelRGB8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x0014),
    gvspPixelBGR8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x0015),
    gvspPixelRGBA8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY32BIT | 0x0016),
    gvspPixelBGRA8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY32BIT | 0x0017),
    gvspPixelRGB10=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x0018),
    gvspPixelBGR10=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x0019),
    gvspPixelRGB12=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x001A),
    gvspPixelBGR12=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x001B),
    gvspPixelRGB16=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x0033),
    gvspPixelRGB10V1Packed=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY32BIT | 0x001C),
    gvspPixelRGB10P32=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY32BIT | 0x001D),
    gvspPixelRGB12V1Packed=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY36BIT | 0X0034),
    gvspPixelRGB565P=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x0035),
    gvspPixelBGR565P=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0X0036),

    # YVR Format
    gvspPixelYUV411_8_UYYVYY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY12BIT | 0x001E),
    gvspPixelYUV422_8_UYVY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x001F),
    gvspPixelYUV422_8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x0032),
    gvspPixelYUV8_UYV=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x0020),
    gvspPixelYCbCr8CbYCr=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x003A),
    gvspPixelYCbCr422_8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x003B),
    gvspPixelYCbCr422_8_CbYCrY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x0043),
    gvspPixelYCbCr411_8_CbYYCrYY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY12BIT | 0x003C),
    gvspPixelYCbCr601_8_CbYCr=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x003D),
    gvspPixelYCbCr601_422_8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x003E),
    gvspPixelYCbCr601_422_8_CbYCrY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x0044),
    gvspPixelYCbCr601_411_8_CbYYCrYY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY12BIT | 0x003F),
    gvspPixelYCbCr709_8_CbYCr=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x0040),
    gvspPixelYCbCr709_422_8=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x0041),
    gvspPixelYCbCr709_422_8_CbYCrY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY16BIT | 0x0045),
    gvspPixelYCbCr709_411_8_CbYYCrYY=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY12BIT | 0x0042),

    # RGB Planar
    gvspPixelRGB8Planar=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY24BIT | 0x0021),
    gvspPixelRGB10Planar=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x0022),
    gvspPixelRGB12Planar=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x0023),
    gvspPixelRGB16Planar=(IMV_GVSP_PIX_COLOR | IMV_GVSP_PIX_OCCUPY48BIT | 0x0024),

    # BayerRG10p和BayerRG12p格式，针对特定项目临时添加请不要使用
    # BayerRG10p and BayerRG12p, currently used for specific project, please do not use them
    gvspPixelBayRG10p=0x010A0058,
    gvspPixelBayRG12p=0x010c0059,

    # mono1c格式，自定义格式
    # mono1c, customized image format, used for binary output
    gvspPixelMono1c=0x012000FF,

    # mono1e格式，自定义格式，用来显示连通域
    # mono1e, customized image format, used for displaying connected domain
    gvspPixelMono1e=0x01080FFF
)

IMV_OK = 0  # < \~chinese 成功，无错误							\~english Successed, no error

IMV_ERROR = -101  # < \~chinese 通用的错误						\~english Generic error
IMV_INVALID_HANDLE = -102  # < \~chinese 错误或无效的句柄						\~english Error or invalid handle
IMV_INVALID_PARAM = -103  # < \~chinese 错误的参数						\~english Incorrect parameter
IMV_INVALID_FRAME_HANDLE = -104  # < \~chinese 错误或无效的帧句柄				\~english Error or invalid frame handle
IMV_INVALID_FRAME = -105  # < \~chinese 无效的帧								\~english Invalid frame
IMV_INVALID_RESOURCE = -106  # < \~chinese 相机/事件/流等资源无效				\~english Camera/Event/Stream and so on resource invalid
IMV_INVALID_IP = -107  # < \~chinese 设备与主机的IP网段不匹配			\~english Device's and PC's subnet is mismatch
IMV_NO_MEMORY = -108  # < \~chinese 内存不足								\~english Malloc memery failed
IMV_INSUFFICIENT_MEMORY = -109  # < \~chinese 传入的内存空间不足				\~english Insufficient memory
IMV_ERROR_PROPERTY_TYPE = -110  # < \~chinese 属性类型错误						\~english Property type error
IMV_INVALID_ACCESS = -111  # < \~chinese 属性不可访问、或不能读/写、或读/写失败\~english Property not accessible, or not be read/written, or read/written failed
IMV_INVALID_RANGE = -112  # < \~chinese 属性值超出范围、或者不是步长整数倍\~english The property's value is out of range, or is not integer multiple of the step
IMV_NOT_SUPPORT = -113  # < \~chinese 设备不支持的功能						\~english Device not supported function

typeGigeCamera = 0  # < \~chinese GIGE相机				\~english GigE Vision Camera
typeU3vCamera = 1  # < \~chinese USB3.0相机			\~english USB3.0 Vision Camera
typeCLCamera = 2  # < \~chinese CAMERALINK 相机		\~english Cameralink camera
typePCIeCamera = 3  # < \~chinese PCIe相机				\~english PCIe Camera
typeUndefinedCamera = 255  # < \~chinese 未知类型				\~english Undefined Camera

IMV_MAX_DEVICE_ENUM_NUM = 100  # < \~chinese 支持设备最大个数	\~english The maximum number of supported devices
IMV_MAX_STRING_LENTH = 256  # < \~chinese 字符串最大长度	\~english The maximum length of string
IMV_MAX_ERROR_LIST_NUM = 128  # < \~chinese 失败属性列表最大长度\~english The maximum size of failed properties list

MAX_STRING_LENTH = 256

# STRING = c_char_p

IMV_GVSP_PIX_MONO = 0x01000000
IMV_GVSP_PIX_RGB = 0x02000000
IMV_GVSP_PIX_COLOR = 0x02000000
IMV_GVSP_PIX_CUSTOM = 0x80000000
IMV_GVSP_PIX_COLOR_MASK = 0xFF000000

IMV_GVSP_PIX_OCCUPY1BIT = 0x00010000
IMV_GVSP_PIX_OCCUPY2BIT = 0x00020000
IMV_GVSP_PIX_OCCUPY4BIT = 0x00040000
IMV_GVSP_PIX_OCCUPY8BIT = 0x00080000
IMV_GVSP_PIX_OCCUPY12BIT = 0x000C0000
IMV_GVSP_PIX_OCCUPY16BIT = 0x00100000
IMV_GVSP_PIX_OCCUPY24BIT = 0x00180000
IMV_GVSP_PIX_OCCUPY32BIT = 0x00200000
IMV_GVSP_PIX_OCCUPY36BIT = 0x00240000
IMV_GVSP_PIX_OCCUPY48BIT = 0x00300000

""" /// \~chinese
/// 消息通道事件ID列表
/// \~english
/// message channel event id list """
IMV_MSG_EVENT_ID_EXPOSURE_END = 0x9001
IMV_MSG_EVENT_ID_FRAME_TRIGGER = 0x9002
IMV_MSG_EVENT_ID_FRAME_START = 0x9003
IMV_MSG_EVENT_ID_ACQ_START = 0x9004
IMV_MSG_EVENT_ID_ACQ_TRIGGER = 0x9005
IMV_MSG_EVENT_ID_DATA_READ_OUT = 0x9006


# 定义枚举类型
# define enum type
def enum(**enums):
    return type('Enum', (), enums)


""" /// \~chinese
///枚举：属性类型
/// \~english
///Enumeration: property type """
IMV_EFeatureType = enum(
    featureInt=0x10000000,
    featureFloat=0x20000000,
    featureEnum=0x30000000,
    featureBool=0x40000000,
    featureString=0x50000000,
    featureCommand=0x60000000,
    featureGroup=0x70000000,
    featureReg=0x80000000,
    featureUndefined=0x90000000
)

""" /// \~chinese
///枚举：接口类型
/// \~english
///Enumeration: interface type"""
IMV_EInterfaceType = enum(
    interfaceTypeGige=0x00000001,
    interfaceTypeUsb3=0x00000002,
    interfaceTypeCL=0x00000004,
    interfaceTypePCIe=0x00000008,
    interfaceTypeAll=0x00000000,
    interfaceInvalidType=0xFFFFFFFF
)

""" /// \~chinese
///枚举：设备类型
/// \~english
///Enumeration: device type """
IMV_ECameraType = enum(
    typeGigeCamera=0,
    typeU3vCamera=1,
    typeCLCamera=2,
    typePCIeCamera=3,
    typeUndefinedCamera=255
)

""" /// \~chinese
///枚举：创建句柄方式
/// \~english
///Enumeration: Create handle mode """
IMV_ECreateHandleMode = enum(
    modeByIndex=0,
    modeByCameraKey=1,
    modeByDeviceUserID=2,
    modeByIPAddress=3
)

""" /// \~chinese
///枚举：访问权限
/// \~english
///Enumeration: access permission """
IMV_ECameraAccessPermission = enum(
    accessPermissionOpen=0,
    accessPermissionExclusive=1,
    accessPermissionControl=2,
    accessPermissionControlWithSwitchover=3,
    accessPermissionUnknown=254,
    accessPermissionUndefined=255
)

""" /// \~chinese
///枚举：抓图策略
/// \~english
///Enumeration: grab strartegy """
IMV_EGrabStrategy = enum(
    grabStrartegySequential=0,
    grabStrartegyLatestImage=1,
    grabStrartegyUpcomingImage=2,
    grabStrartegyUndefined=3
)

""" /// \~chinese
///枚举：流事件状态
/// \~english
/// Enumeration:stream event status """
IMV_EEventStatus = enum(
    streamEventNormal=1,
    streamEventLostFrame=2,
    streamEventLostPacket=3,
    streamEventImageError=4,
    streamEventStreamChannelError=5,
    streamEventTooManyConsecutiveResends=6,
    streamEventTooManyLostPacket=7
)

""" /// \~chinese
///枚举：图像转换Bayer格式所用的算法
/// \~english
/// Enumeration:alorithm used for Bayer demosaic """
IMV_EBayerDemosaic = enum(
    demosaicNearestNeighbor=0,
    demosaicBilinear=1,
    demosaicEdgeSensing=2,
    demosaicNotSupport=255,
)

""" /// \~chinese
///枚举：事件类型
/// \~english
/// Enumeration:event type """
IMV_EVType = enum(
    offLine=0,
    onLine=1
)

""" /// \~chinese
///枚举：视频格式
/// \~english
/// Enumeration:Video format """
IMV_EVideoType = enum(
    typeVideoFormatAVI=0,
    typeVideoFormatNotSupport=255
)

""" /// \~chinese
///枚举：图像翻转类型
/// \~english
/// Enumeration:Image flip type """
IMV_EFlipType = enum(
    typeFlipVertical=0,  # ///< \~chinese 垂直(Y轴)翻转	\~english Vertical(Y-axis) flip
    typeFlipHorizontal=1  # ///< \~chinese 水平(X轴)翻转	\~english Horizontal(X-axis) flip
)

""" /// \~chinese
///枚举：顺时针旋转角度
/// \~english
/// Enumeration:Rotation angle clockwise """
IMV_ERotationAngle = enum(
    rotationAngle90=0,  # ///< \~chinese 顺时针旋转90°\~english Rotate 90 degree clockwise
    rotationAngle180=1,  # ///< \~chinese 顺时针旋转180°\~english Rotate 180 degree clockwise
    rotationAngle270=2  # ///< \~chinese 顺时针旋转270°\~english Rotate 270 degree clockwise
)

# """"/// \~chinese
# /// \brief 枚举：图像保存格式
# /// \~english
# /// \brief Enumeration:image save format"""
IMV_ESaveType = enum(
    typeImageBmp=0,  # ///< \~chinese BMP图像格式                  \~english BMP Image Type
    typeImageJpeg=1,  # ///< \~chinese JPEG图像格式                 \~english Jpeg Image Type
    typeImagePng=2,  # ///< \~chinese PNG图像格式                  \~english Png  Image Type
    typeImageTif=3,  # ///< \~chinese TIFF图像格式                 \~english TIFF Image Type
    typeImageUndefined=255  # ///< \~chinese 未定义的图像格式             \~english Undefined Image Type
)

# RecordVideo.h => enum tagRECORD_EErr
RECORD_EErr = enum(
    RECORD_SUCCESS=0,
    RECORD_ILLEGAL_PARAM=1,
    RECORD_ERR_ORDER=2,
    RECORD_NO_MEMORY=3,
    RECORD_NOT_SUPPORT=255
)

RECORD_EVideoFormatType = enum(
    RECORD_VIDEO_FMT_AVI=0,
    RECORD_VIDEO_FMT_NOT_SUPPORT=255
)


# RecordVideo.h => struct tagRECORD_SRecordParam
class _RECORD_SRecordParam_(Structure):
    pass


_RECORD_SRecordParam_._fields_ = [
    ('width', c_uint),
    ('height', c_uint),
    ('frameRate', c_float),
    ('quality', c_uint),
    ('recordFmtType', c_uint),
    ('recordFilePath', c_char_p),
    ('reserved', c_uint * 26)
]
RECORD_SRecordParam = _RECORD_SRecordParam_


class _RECORD_SFrameInfo_(Structure):
    pass


_RECORD_SFrameInfo_._fields_ = [
    ('data', c_char_p),
    ('size', c_uint),
    ('paddingX', c_uint),
    ('paddingY', c_uint),
    ('pixelformat', c_int),
    ('reserved', c_uint * 27),
]

""" /// \~chinese
/// 消息通道事件ID列表
/// \~english
/// message channel event id list """
IMV_MSG_EVENT_ID_EXPOSURE_END = 0x9001
IMV_MSG_EVENT_ID_FRAME_TRIGGER = 0x9002
IMV_MSG_EVENT_ID_FRAME_START = 0x9003
IMV_MSG_EVENT_ID_ACQ_START = 0x9004
IMV_MSG_EVENT_ID_ACQ_TRIGGER = 0x9005
IMV_MSG_EVENT_ID_DATA_READ_OUT = 0x9006

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


class _IMV_String_(Structure):
    pass


_IMV_String_._fields_ = [
    ('str', c_char * MAX_STRING_LENTH),
]
IMV_String = _IMV_String_


# gige接口信息
class _IMV_GigEInterfaceInfo_(Structure):
    pass


_IMV_GigEInterfaceInfo_._fields_ = [
    ('description', c_char * MAX_STRING_LENTH),
    ('macAddress', c_char * MAX_STRING_LENTH),
    ('ipAddress', c_char * MAX_STRING_LENTH),
    ('subnetMask', c_char * MAX_STRING_LENTH),
    ('defaultGateWay', c_char * MAX_STRING_LENTH),
    ('chReserved', c_char * MAX_STRING_LENTH * 5),
]
IMV_GigEInterfaceInfo = _IMV_GigEInterfaceInfo_


# usb接口信息
class _IMV_UsbInterfaceInfo_(Structure):
    pass


_IMV_UsbInterfaceInfo_._fields_ = [
    ('description', c_char * MAX_STRING_LENTH),
    ('vendorID', c_char * MAX_STRING_LENTH),
    ('deviceID', c_char * MAX_STRING_LENTH),
    ('subsystemID', c_char * MAX_STRING_LENTH),
    ('revision', c_char * MAX_STRING_LENTH),
    ('speed', c_char * MAX_STRING_LENTH),
    ('chReserved', c_char * MAX_STRING_LENTH * 4),
]
IMV_UsbInterfaceInfo = _IMV_UsbInterfaceInfo_


# GigE设备信息    \~english GigE device info
class _IMV_GigEDeviceInfo_(Structure):
    pass


_IMV_GigEDeviceInfo_._fields_ = [
    ('nIpConfigOptions', c_uint),
    ('nIpConfigCurrent', c_uint),
    ('nReserved', c_uint * 3),
    ('macAddress', c_char * MAX_STRING_LENTH),
    ('ipAddress', c_char * MAX_STRING_LENTH),
    ('subnetMask', c_char * MAX_STRING_LENTH),
    ('defaultGateWay', c_char * MAX_STRING_LENTH),
    ('protocolVersion', c_char * MAX_STRING_LENTH),
    ('ipConfiguration', c_char * MAX_STRING_LENTH),
    ('strReserved', c_char * MAX_STRING_LENTH * 6),
]
IMV_GigEDeviceInfo = _IMV_GigEDeviceInfo_


# USB设备信息    \~english USB device info
class _IMV_UsbDeviceInfo_(Structure):
    pass


_IMV_UsbDeviceInfo_._fields_ = [
    ('bLowSpeedSupported', c_bool),
    ('bFullSpeedSupported', c_bool),
    ('bHighSpeedSupported', c_bool),
    ('bSuperSpeedSupported', c_bool),
    ('bDriverInstalled', c_bool),
    ('boolReserved', c_bool * 3),
    ('Reserved', c_uint * 4),
    ('configurationValid', c_char * MAX_STRING_LENTH),
    ('genCPVersion', c_char * MAX_STRING_LENTH),
    ('u3vVersion', c_char * MAX_STRING_LENTH),
    ('deviceGUID', c_char * MAX_STRING_LENTH),
    ('familyName', c_char * MAX_STRING_LENTH),
    ('u3vSerialNumber', c_char * MAX_STRING_LENTH),
    ('speed', c_char * MAX_STRING_LENTH),
    ('maxPower', c_char * MAX_STRING_LENTH),
    ('chReserved', c_char * MAX_STRING_LENTH * 4)
]
IMV_UsbDeviceInfo = _IMV_UsbDeviceInfo_


class InterfaceInfo(Union):
    pass


InterfaceInfo._fields_ = [
    ('gigeInterfaceInfo', IMV_GigEInterfaceInfo),
    ('usbInterfaceInfo', IMV_UsbInterfaceInfo),
]


class DeviceSpecificInfo(Union):
    pass


DeviceSpecificInfo._fields_ = [
    ('gigeDeviceInfo', IMV_GigEDeviceInfo),
    ('usbDeviceInfo', IMV_UsbDeviceInfo),
]


class _IMV_DeviceInfo_(Structure):
    pass


_IMV_DeviceInfo_._fields_ = [
    ('nCameraType', c_int),
    ('nCameraReserved', c_int * 5),
    ('cameraKey', c_char * MAX_STRING_LENTH),
    ('cameraName', c_char * MAX_STRING_LENTH),
    ('serialNumber', c_char * MAX_STRING_LENTH),
    ('vendorName', c_char * MAX_STRING_LENTH),
    ('modelName', c_char * MAX_STRING_LENTH),
    ('manufactureInfo', c_char * MAX_STRING_LENTH),
    ('deviceVersion', c_char * MAX_STRING_LENTH),
    ('cameraReserved', c_char * MAX_STRING_LENTH * 5),
    ('DeviceSpecificInfo', DeviceSpecificInfo),
    ('nInterfaceType', c_int),
    ('nInterfaceReserved', c_int * 5),
    ('interfaceName', c_char * MAX_STRING_LENTH),
    ('interfaceReserved', c_char * MAX_STRING_LENTH * 5),
    ('InterfaceInfo', InterfaceInfo),
]
IMV_DeviceInfo = _IMV_DeviceInfo_

""" /// \~chinese
/// \brief 加载失败的属性信息
/// \~english
/// \brief Load failed properties information """


class _IMV_ErrorList_(Structure):
    pass


_IMV_ErrorList_._fields_ = [
    ('nParamCnt', c_uint),
    ('paramNameList', IMV_String),
]
IMV_ErrorList = _IMV_ErrorList_

""" /// \~chinese
/// \brief 设备信息列表
/// \~english
/// \brief Device information list """


class _IMV_DeviceList_(Structure):
    pass


_IMV_DeviceList_._fields_ = [
    ('nDevNum', c_uint),
    ('pDevInfo', POINTER(IMV_DeviceInfo)),
]
IMV_DeviceList = _IMV_DeviceList_

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
IMV_SConnectArg = _IMV_SConnectArg_

""" /// \~chinese
/// \brief 参数更新事件信息
/// \~english
/// \brief Updating parameters event information """


class _IMV_SParamUpdateArg(Structure):
    pass


_IMV_SParamUpdateArg._fields_ = [
    ('isPoll', c_bool),
    ('nReserve', c_uint * 10),
    ('nParamCnt', c_uint),
    ('pParamNameList', POINTER(IMV_String)),
]
IMV_SParamUpdateArg = _IMV_SParamUpdateArg

""" /// \~chinese
/// \brief 流事件信息
/// \~english
/// \brief Stream event information """


class _IMV_SStreamArg_(Structure):
    pass


_IMV_SStreamArg_._fields_ = [
    ('channel', c_uint),
    ('blockId', c_uint64),
    ('timeStamp', c_uint64),
    ('eStreamEventStatus', c_int),
    ('status', c_uint),
    ('nReserve', c_uint * 9)
]
IMV_SStreamArg = _IMV_SStreamArg_

""" /// \~chinese
/// \brief 消息通道事件信息
/// \~english
/// \brief Message channel event information """


class _IMV_SMsgChannelArg_(Structure):
    pass


_IMV_SMsgChannelArg_._fields_ = [
    ('eventId', c_ushort),
    ('channelId', c_ushort),
    ('blockId', c_uint64),
    ('timeStamp', c_uint64),
    ('nReserve', c_uint * 8),
    ('nParamCnt', c_uint),
    ('pParamNameList', POINTER(IMV_String))
]
IMV_SMsgChannelArg = _IMV_SMsgChannelArg_

""" /// \~chines
/// \brief Chunk数据信息
/// \~english
/// \brief Chunk data information """


class _IMV_ChunkDataInfo_(Structure):
    pass


_IMV_ChunkDataInfo_._fields_ = [
    ('chunkID', c_uint),
    ('nParamCnt', c_uint),
    ('pParamNameList', POINTER(IMV_String))
]
IMV_ChunkDataInfo = _IMV_ChunkDataInfo_

""" /// \~chinese
/// \brief 帧图像信息
/// \~english
/// \brief The frame image information """


class _IMV_FrameInfo_(Structure):
    pass


_IMV_FrameInfo_._fields_ = [
    ('blockId', c_uint64),
    ('status', c_uint),
    ('width', c_uint),
    ('height', c_uint),
    ('size', c_uint),
    ('pixelFormat', c_int),
    ('timeStamp', c_uint64),
    ('chunkCount', c_uint),
    ('paddingX', c_uint),
    ('paddingY', c_uint),
    ('recvFrameTime', c_uint),
    ('nReserved', c_uint * 19),
]
IMV_FrameInfo = _IMV_FrameInfo_

""" /// \~chinese
/// \brief 帧图像数据信息
/// \~english
/// \brief Frame image data information """


class _IMV_Frame_(Structure):
    pass


_IMV_Frame_._fields_ = [
    ('frameHandle', c_void_p),
    ('pData', POINTER(c_ubyte)),
    ('frameInfo', IMV_FrameInfo),
    ('nReserved', c_uint * 10),
]
IMV_Frame = _IMV_Frame_

""" /// \~chinese
/// \brief PCIE设备统计流信息
/// \~english
/// \brief PCIE device stream statistics information """


class _IMV_PCIEStreamStatsInfo_(Structure):
    pass


_IMV_PCIEStreamStatsInfo_._fields_ = [
    ('imageError', c_uint),
    ('lostPacketBlock', c_uint),
    ('nReserved0', c_uint * 10),
    ('imageReceived', c_uint),
    ('fps', c_double),
    ('bandwidth', c_double),
    ('nReserved', c_uint * 8)
]
IMV_PCIEStreamStatsInfo = _IMV_PCIEStreamStatsInfo_

""" /// \~chinese
/// \brief U3V设备统计流信息
/// \~english
/// \brief U3V device stream statistics information """


class _IMV_U3VStreamStatsInfo_(Structure):
    pass


_IMV_U3VStreamStatsInfo_._fields_ = [
    ('imageError', c_uint),
    ('lostPacketBlock', c_uint),
    ('nReserved0', c_uint * 10),
    ('imageReceived', c_uint),
    ('fps', c_double),
    ('bandwidth', c_double),
    ('nReserved', c_uint * 8)
]
IMV_U3VStreamStatsInfo = _IMV_U3VStreamStatsInfo_

""" /// \~chinese
/// \brief Gige设备统计流信息
/// \~english
/// \brief Gige device stream statistics information """


class _IMV_GigEStreamStatsInfo_(Structure):
    pass


_IMV_GigEStreamStatsInfo_._fields_ = [
    ('nReserved0', c_uint * 10),
    ('imageError', c_uint),
    ('lostPacketBlock', c_uint),
    ('nReserved1', c_uint * 4),
    ('nReserved2', c_uint * 5),
    ('imageReceived', c_uint),
    ('fps', c_double),
    ('bandwidth', c_double),
    ('nReserved', c_uint * 4)
]
IMV_GigEStreamStatsInfo = _IMV_GigEStreamStatsInfo_


class IMV_StreamStatsInfo(Union):
    pass


IMV_StreamStatsInfo._fields_ = [
    ('pcieStatisticsInfo', IMV_PCIEStreamStatsInfo),
    ('u3vStatisticsInfo', IMV_UsbInterfaceInfo),
    ('gigeStatisticsInfo', IMV_GigEStreamStatsInfo)
]

""" /// \~chinese
/// \brief 统计流信息
/// \~english
/// \brief Stream statistics information """


class _IMV_StreamStatisticsInfo_(Structure):
    pass


_IMV_StreamStatisticsInfo_._fields_ = [
    ('nCameraType', c_uint),
    ('pcieStatisticsInfo', IMV_PCIEStreamStatsInfo),
    ('gigeStatisticsInfo', IMV_GigEStreamStatsInfo),
    ('u3vStatisticsInfo', IMV_U3VStreamStatsInfo)
]

IMV_StreamStatisticsInfo = _IMV_StreamStatisticsInfo_

""" /// \~chinese
/// \brief 枚举属性的枚举值信息
/// \~english
/// \brief Enumeration property 's enumeration value information """


class _IMV_EnumEntryInfo_(Structure):
    pass


_IMV_EnumEntryInfo_._fields_ = [
    ('value', c_uint64),
    ('name', c_char * IMV_MAX_STRING_LENTH),
]
IMV_EnumEntryInfo = _IMV_EnumEntryInfo_

""" /// \~chinese
/// \brief 枚举属性的可设枚举值列表信息
/// \~english
/// \brief Enumeration property 's settable enumeration value list information """


class _IMV_EnumEntryList_(Structure):
    pass


_IMV_EnumEntryList_._fields_ = [
    ('nEnumEntryBufferSize', c_uint),
    ('pEnumEntryInfo', POINTER(IMV_EnumEntryInfo))
]
IMV_EnumEntryList = _IMV_EnumEntryList_

""" /// \~chinese
/// \brief 像素转换结构体
/// \~english
/// \brief Pixel convert structure """


class _IMV_PixelConvertParam_(Structure):
    pass


_IMV_PixelConvertParam_._fields_ = [
    ('nWidth', c_uint),
    ('nHeight', c_uint),
    ('ePixelFormat', c_int),
    ('pSrcData', POINTER(c_ubyte)),
    ('nSrcDataLen', c_uint),
    ('nPaddingX', c_uint),
    ('nPaddingY', c_uint),
    ('eBayerDemosaic', c_int),
    ('eDstPixelFormat', c_int),
    ('pDstBuf', POINTER(c_ubyte)),
    ('nDstBufSize', c_uint),
    ('nDstDataLen', c_uint),
    ('nReserved', c_uint * 8)
]
IMV_PixelConvertParam = _IMV_PixelConvertParam_

""" /// \~chinese
/// \brief 图像翻转结构体
/// \~english
/// \brief Flip image structure """


class _IMV_FlipImageParam_(Structure):
    pass


_IMV_FlipImageParam_._fields_ = [
    ('nWidth', c_uint),
    ('nHeight', c_uint),
    ('ePixelFormat', c_int),
    ('eFlipType', c_int),
    ('pSrcData', POINTER(c_ubyte)),
    ('nSrcDataLen', c_uint),
    ('pDstBuf', POINTER(c_ubyte)),
    ('nDstBufSize', c_uint),
    ('nDstDataLen', c_uint),
    ('nReserved', c_uint * 8),
]
IMV_FlipImageParam = _IMV_FlipImageParam_

""" /// \~chinese
/// \brief 图像旋转结构体
/// \~english
/// \brief Rotate image structure """


class _IMV_RotateImageParam_(Structure):
    pass
_IMV_RotateImageParam_._fields_ = [
    ('nWidth', c_uint),
    ('nHeight', c_uint),
    ('ePixelFormat', c_int),
    ('eRotationAngle', c_int),
    ('pSrcData', POINTER(c_ubyte)),
    ('nSrcDataLen', c_uint),
    ('pDstBuf', POINTER(c_ubyte)),
    ('nDstBufSize', c_uint),
    ('nDstDataLen', c_uint),
    ('nReserved', c_uint * 8),
]
IMV_RotateImageParam = _IMV_RotateImageParam_


class _BitmapRGBQuad_(Structure):
    pass
_BitmapRGBQuad_._fields_ = [
    ('rgbBlue', c_char),
    ('rgbGreen', c_char),
    ('rgbRed', c_char),
    ('rgbReserved', c_char)
]
BitmapRGBQuad = _BitmapRGBQuad_

class _IMV_SaveImageToFileParam_(Structure):
    pass
_IMV_SaveImageToFileParam_._fields_ = [
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
IMV_SaveImageToFileParam = _IMV_SaveImageToFileParam_