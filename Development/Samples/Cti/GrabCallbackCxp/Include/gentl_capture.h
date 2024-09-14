#pragma once
#include "load_gentl_cti.h"
#include "cxp_standard.h"

#define DMA_BUF_NUM 256         //指定采集卡可绑定缓存最大个数(当前最新版实际可不超过500)



typedef void (*mw_ezgentl_frame_callback_t)(void* p_cb_contex, void* p_base, uint32_t size);

class CGentlCap
{
public:
    CGentlCap();
    virtual ~CGentlCap();

    //-------------0,初始化sdk接口-----------------------------------------------------------------------------
    bool8_t init_sdk_ex();                  //方法1：自动化加载cti文件（根据GenICam标准位置找到库）
    bool8_t init_sdk(const char* cti_url);  //方法2：直接加载cti库文件的url

    //-------------1,连接、关闭gentl设备-----------------------------------------------------------------------
    //连接并初始化GenTL system层、interface层、device层、remote device层、data stream层;
    bool8_t connect_gentl();
    //关闭SDK
    bool8_t close_gentl();

    //-------------2,配置采集卡、相机--------------------------------------------------------------------------
        //设置相机的拉流模式是
        bool8_t set_camera_params();
        //配置、读取采集卡信息（device）
        //设置采集卡配置
        bool8_t set_card_params(card_features_t* in_card_features_ptr);
		//获取宽
		int64_t getWidth();
		//获取高
		int64_t getHeight();

    //-------------3,开始、停止采集-----------------------------------------------------------------------------
	/*
	* 开启采集,
	* @param[in] frame_cb           capture callback function
	* @param[in] p_cb_context       callback parameter
	* @param[in] buf_count          alloc buffer num
	* @param[in] cap_count          采集图片数量， -1则无数量限制
	*/
	bool8_t start_gentl_capture(mw_ezgentl_frame_callback_t frame_cb, void* p_cb_context, uint32_t buf_count, uint64_t cap_count = -1);

	bool8_t capture_wait(uint64_t i_time);

    //停止采集
    bool8_t stop_gentl_capture();
    //获取数据的线程
    int play_get_threader(void* p_cb_context=NULL);
 
private:

    bool8_t create_get_thread();
    bool8_t create_get_thread(void* p_cb_context);

    void    clear_data();

public:
    cxp_std_features_t  m_std_features;//相机标准功能
    TL_HANDLE	m_h_system;         //GenTL系统层句柄
    IF_HANDLE	m_h_if;             //Interface层句柄
    DEV_HANDLE  m_h_device;         //Device层句柄
    PORT_HANDLE m_h_rmt_device;     //remote device(相机层)句柄
    DS_HANDLE	m_h_ds;             //视频流层句柄

    mw_ezgentl_frame_callback_t     m_cb_frame;		//回调函数
    uint64_t                        m_cap_count;	//获取数量

    CStdLoadCti m_gentl_api; //接口类初始化
private:  
    bool8_t     m_api_is_init;

    card_features_t     m_card_features;
    cxp_std_features_t  in_data;

    EVENT_HANDLE            m_new_buffer_event;             //图像采集事件
    EVENT_NEW_BUFFER_DATA   m_event_buffer;                 //采集事件信息
    //BUFFER_HANDLE           m_buffer_array[DMA_BUF_NUM];    //采集图像缓存句柄数组(旧版，已弃用)
    BUFFER_HANDLE*          m_p_hbuf_array;                 //采集图像缓存句柄数组指针，堆内存版
    char**                  m_pp_user_alloc;                //用户申请内存，
    uint32_t                m_p_buf_num;         //申请的缓存个数,必须小于DMA_BUF_NUM；
    size_t                  m_buffer_len;        //每帧图像大小

    HANDLE	m_play_thread_id;
    bool8_t m_run_status;
    CCapCritical		m_locker;		//自动锁
};