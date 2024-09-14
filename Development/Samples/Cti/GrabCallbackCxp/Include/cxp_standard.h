#pragma once
#include "GenTL.h"

//远程设备（相机）的功能寄存器
//成员不能有std::string等标准库类型，因构造函数中使用memset初始化为0,会破坏std库
typedef struct _cxp_std_features_
{
	//相机只读信息，从genapi接口读取
	char device_vendor_name[128];		//相机公司,如"Adimec" 
	char device_model_name[128];		//相机型号，如"Q-12A180-Fm/CXP-6"
	char device_serial_number[128];		//相机序列号，如"100001"
	//相机可写配置项，从genapi接口读取
	int64_t width;		//宽	
	int64_t height;		//高
	int64_t format;		//图像格式数值
	char format_str[128];	//图像格式，字符串版（兼容性强），如"Mono8" 对应的值，有的相机参照cxp标准，有的相机参照PFNC标准。直接使用名称兼容性强
	int64_t offset_x;	//ROI OffsetX 偏移
	int64_t offset_y;	//ROI OffsetY 偏移

	//...
}cxp_std_features_t;

//命令格式寄存器使能值;
typedef enum _command_gentl_
{
	CMD_GENTL_START = 1
}command_gentl_t;

//自定义相机类型
typedef enum _typedef_camera_
{
	ADIMEC_Q_12A180 = 0,
	DAHUA_AX5A22MX050 = 1,
	HAIKANG_CXP12 = 2,
	BASLER =3
}typedef_camera_t;

typedef enum _flex_io_mask_
{
	IIN11 = 0,
	IIN12 = 1,
	IIN13 = 2,
	IIN14 = 3,
	EXT_IIN11 = 4,
	EXT_IIN12 = 5,
	EXT_IIN13 = 6,
	EXT_IIN14 = 7,
	DIN11 = 8,
	DIN12 = 9,
	TTLIO11 = 10,
	TTLIO12 = 11
}flex_io_mask_t;

//采集卡功能寄存器
typedef struct _card_features_
{
	uint32_t mode;					//触发模式，//1：软触发模式   2：硬触发模式

	//软触发参数
	double pulse_width;				//曝光时间, unit: 1us
	double pulse_period;			//脉冲周期, unit: 1us
	uint32_t pulse_number;			//单次点击使能命令脉冲个数，设置为0，代表一直循环发送;
	command_gentl_t load_desc_en;	//点击1次使能命令，触发pulse_number个脉冲。pulse_number为0时，使能命令默认循环自动触发；

	//硬触发参数
	
	flex_io_mask_t	io_mask;		//输入接线选择;
	double glitch;					//去毛刺，单位us（微秒,精度0.1us）;建议参数1.0us
}card_features_t;



static inline bool8_t gentl_is_success(GC_ERROR status)
{
	return (GC_ERR_SUCCESS == status);
}

////自动锁
class CCapCritical
{
public:
	CCapCritical()
	{
		InitializeCriticalSection(&m_cs);
	}

	virtual ~CCapCritical()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	CRITICAL_SECTION m_cs;
};

class CCapAutoLock//自动锁
{
public:
	CCapCritical* m_csec;
	CCapAutoLock(CCapCritical* critsec)
	{
		m_csec = critsec;
		m_csec->Lock();
	}
	virtual ~CCapAutoLock()
	{
		m_csec->Unlock();
	}
};