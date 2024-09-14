#ifndef CAMMERWIDGET_H
#define CAMMERWIDGET_H

#include <windows.h>
#include <process.h>
#include <QWidget>
#include "Media/VideoRender.h"
#include "MessageQue.h"
#include "IMVAPI/IMVApi.h"
#include <QElapsedTimer>
#include <QMutex>
#include <QThread>

// 状态栏统计信息 
// Status bar statistics
struct FrameStatInfo
{
	quint32     m_nFrameSize;       // 帧大小, 单位: 字节 | frame size ,length :byte
	qint64      m_nPassTime;         // 接收到该帧时经过的纳秒数 |  The number of nanoseconds passed when the frame was received
    FrameStatInfo(int nSize, qint64 nTime) : m_nFrameSize(nSize), m_nPassTime(nTime)
	{
	}
};

// 帧信息 
// frame imformation
class CFrameInfo
{
public:
	CFrameInfo()
	{
		m_pImageBuf = NULL;
		m_nBufferSize = 0;
		m_nWidth = 0;
		m_nHeight = 0;
		m_ePixelType = gvspPixelMono8;
		m_nPaddingX = 0;
		m_nPaddingY = 0;
		m_nTimeStamp = 0;
	}

	~CFrameInfo()
	{
	}

public:
	unsigned char*	m_pImageBuf;
	int				m_nBufferSize;
	int				m_nWidth;
	int				m_nHeight;
	IMV_EPixelType	m_ePixelType;
	int				m_nPaddingX;
	int				m_nPaddingY;
	uint64_t		m_nTimeStamp;
};

namespace Ui {
class CammerWidget;
}

class CammerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CammerWidget(QWidget *parent = 0);
    ~CammerWidget();

	// 枚举触发方式
	// Enumeration trigger mode
	enum ETrigType
	{
		trigContinous = 0,	// 连续拉流 | continue grabbing
		trigSoftware = 1,	// 软件触发 | software trigger
		trigLine = 2,		// 外部触发	| external trigger
	};

    // 打开相机
	// open cmaera
    bool CameraOpen(void);
    // 关闭相机
	// close camera
    bool CameraClose(void);
    // 开始采集
	// start grabbing
    bool CameraStart(void);
    // 停止采集
	// stop grabbing
    bool CameraStop(void);
    // 切换采集方式、触发方式 （连续采集、外部触发、软件触发）
	// Switch acquisition mode and triggering mode (continuous acquisition, external triggering and software triggering)
    bool CameraChangeTrig(ETrigType trigType = trigContinous);
    // 执行一次软触发        
	// Execute a soft trigger
    bool ExecuteSoftTrig(void);
	// 设置曝光
	// set exposuse time
	bool SetExposeTime(double dExposureTime);
	// 设置增益
	// set gain
	bool SetAdjustPlus(double dGainRaw);
	// 设置当前相机
	// set current camera
	void SetCamera(const QString& strKey);

	// 状态栏统计信息
	// Status bar statistics
	void recvNewFrame(quint32 frameSize);
    void resetStatistic();
	QString getStatistic();
	void updateStatistic();

	void display();

private:

	// 设置显示频率，默认一秒钟显示30帧
	// Set the display frequency to display 30 frames in one second by default
	void setDisplayFPS(int nFPS);

	// 计算该帧是否显示
	// Calculate whether the frame is displayed
	bool isTimeToDisplay();
	
	// 窗口关闭响应函数
	// Window close response function
	void closeEvent(QCloseEvent * event);
	
    // 显示一帧图像
	// display a frame image 
	bool ShowImage(unsigned char* pRgbFrameBuf, int nWidth, int nHeight, IMV_EPixelType nPixelFormat);

	// VedioRender相关函数 
	// VedioRender relative function
	bool openRender(int width, int height);
    bool closeRender();

public:
	TMessageQue<CFrameInfo>				m_qDisplayFrameQueue;		// 显示队列      | diaplay queue

private:
    Ui::CammerWidget *ui;
	
	QString								m_currentCameraKey;			// 当前相机key | current camera key
	IMV_HANDLE							m_devHandle;				// 相机句柄 | camera handle

    // 控制显示帧率   | Control display frame rate
	QMutex								m_mxTime;
	int									m_nDisplayInterval;         // 显示间隔 | diaplay time internal
	uint64_t							m_nFirstFrameTime;          // 第一帧的时间戳 | Time stamp of the first frame
	uint64_t							m_nLastFrameTime;           // 上一帧的时间戳 | Timestamp of previous frame
	QElapsedTimer						m_elapsedTimer;				// 用来计时，控制显示帧率 | Used to time and control the display frame rate

	// 状态栏统计信息 
	// Status bar statistics	
	typedef std::list<FrameStatInfo> FrameList;
	FrameList   m_listFrameStatInfo;
	QMutex      m_mxStatistic;
	quint64     m_nTotalFrameCount;		// 收到的总帧数 | recieve all frames
    QString     m_strStatistic;			// 统计信息, 不包括错误  | Statistics, excluding errors
	bool		m_bNeedUpdate;
	
	// VedioRender相关 
	VR_HANDLE          m_handler;           // 绘图句柄 | draw inco handle
	VR_OPEN_PARAM_S    m_params;            // 显示参数 | display parmeters
	VR_HWND			m_hWnd;             // 显示窗口句柄 | display window handle

	bool			m_isExitDisplayThread;
	HANDLE			m_threadHandle;
};

#endif // CAMMERWIDGET_H
