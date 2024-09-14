#pragma once
#include "windows.h"//UINT32等定义配置
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////
//#define DETAIL_DEBUG_ON
//#define IMGUI_LOG_ON

#ifdef DETAIL_DEBUG_ON
    #define PRINT_DETAIL(...) printf(__VA_ARGS__) 
#else
    #define PRINT_DETAIL(...) 
#endif

#ifdef IMGUI_LOG_ON
    #define printf(...) imgui_log(__VA_ARGS__)
    void imgui_log(const char* fmt, ...);
#endif
////////////////////////////////////////////////////////////////////////////////
// Callback Constant
////////////////////////////////////////////////////////////////////////////////
#define CXP_SUCCESS          0
#define CXP_ERR             -1
#define CXP_CMD_ACK_ERR     -2
////////////////////////////////////////////////////////////////////////////////
// CXP Frame Grabber Card Address Allocation
////////////////////////////////////////////////////////////////////////////////
#define OFFSET_RESET                0x00000100
#define OFFSET_DATA_STREAM          0x00002000
#define OFFSET_IO_TRIGGER           0x00000200
#define OFFSET_POCXP                0x00000300
#define OFFSET_EVENT_REC            0x00000400
#define OFFSET_FAN                  0x00000500
#define OFFSET_FPGA_MONITOR         0x00000600
#define OFFSET_CXP_LAMP             0x00000800
#define OFFSET_CXP_0                0x00001000
#define OFFSET_CXP_DELTA            0x00000100
#define OFFSET_SERDES_DRP           0x00000A00
////////////////////////////////////////////////////////////////////////////////
// PCIe Status
////////////////////////////////////////////////////////////////////////////////
#define ADDR_pcie_status            0x00000008
#define ADDR_pcie_ltssm             0x00000009
////////////////////////////////////////////////////////////////////////////////
// CXP Reset
////////////////////////////////////////////////////////////////////////////////
#define ADDR_mcu_rst_gth            (OFFSET_RESET+0x00000000) //WO
#define ADDR_reset_cxp_hs           (OFFSET_RESET+0x00000001) //WO
#define ADDR_reset_cxp_ls           (OFFSET_RESET+0x00000002) //WO
#define ADDR_reset_cxp_ds           (OFFSET_RESET+0x00000003) //WO
#define ADDR_reset_cxp_io           (OFFSET_RESET+0x00000004) //WO
#define ADDR_reset_cxp_cpu          (OFFSET_RESET+0x00000005) //WO

#define ADDR_device_name            (OFFSET_RESET+0x00000000) //RO
#define ADDR_soft_version           (OFFSET_RESET+0x00000007) //RO
#define ADDR_firmware_revision      (OFFSET_RESET+0x00000011) //RO
#define ADDR_firmware_dfx_index     (OFFSET_RESET+0x00000012) //RO
#define ADDR_complie_date           (OFFSET_RESET+0x00000013) //RO
#define ADDR_complie_time           (OFFSET_RESET+0x00000014) //RO
////////////////////////////////////////////////////////////////////////////////
// Stream Data Address
////////////////////////////////////////////////////////////////////////////////
#define ADDR_sw_initial_en          (OFFSET_DATA_STREAM+0x00000000) //WR - stream data包排序模块初始化
#define ADDR_connection_mask        (OFFSET_DATA_STREAM+0x00000001) //WR - 工作通道掩码
#define ADDR_ds_stream_id           (OFFSET_DATA_STREAM+0x00000010) //WR - stream ID
#define ADDR_ds_pixelf_lsb_en       (OFFSET_DATA_STREAM+0x00000011) //WR - '1' lsb, '0' msb (CXP default)
#define ADDR_ds_pixelf_unpacked_en  (OFFSET_DATA_STREAM+0x00000012) //WR - '1' unpacked, '0' packed (CXP default)
#define ADDR_ds_pixelf_padding_low  (OFFSET_DATA_STREAM+0x00000013) //WR - '1' low padding, '0' high padding (Common setting)

#define ADDR_ds_xfer_status         (OFFSET_DATA_STREAM+0x00000020) //RO - 传输状态
#define MASK_frame_lost_count       (0xFFFFFF)  //dma丢帧
#define MASK_sw_overflow            (1<<24)     //cxp接收缓冲溢出
#define MASK_ds_overflow            (1<<25)     //pcie发送缓冲溢出
#define MASK_packet_tag_err         (1<<26)     //cxp丢包
#define MASK_frame_image_idx_err    (1<<27)     //cxp丢帧
#define MASK_gth_cdr_unstable       (1<<28)     //gth时钟恢复标志不稳定
#define MASK_link_sym_error         (1<<29)     //link 8b10b 编码错误超门限标志

#define ADDR_ds_frame_stream_id    (OFFSET_DATA_STREAM+0x00000030) //RO - 图像信息
#define ADDR_ds_frame_image_idx    (OFFSET_DATA_STREAM+0x00000031) //RO - 图像信息
#define ADDR_ds_frame_pixel_xsize  (OFFSET_DATA_STREAM+0x00000032) //RO - 图像信息
#define ADDR_ds_frame_pixel_xoffs  (OFFSET_DATA_STREAM+0x00000033) //RO - 图像信息
#define ADDR_ds_frame_pixel_ysize  (OFFSET_DATA_STREAM+0x00000034) //RO - 图像信息
#define ADDR_ds_frame_pixel_yoffs  (OFFSET_DATA_STREAM+0x00000035) //RO - 图像信息
#define ADDR_ds_frame_line_dsize   (OFFSET_DATA_STREAM+0x00000036) //RO - 图像信息
#define ADDR_ds_frame_pixel_format (OFFSET_DATA_STREAM+0x00000037) //RO - 图像信息
#define ADDR_ds_frame_TagG         (OFFSET_DATA_STREAM+0x00000038) //RO - 图像信息
#define ADDR_ds_frame_Flags        (OFFSET_DATA_STREAM+0x00000039) //RO - 图像信息

#define ADDR_tp_dma_select         (OFFSET_DATA_STREAM+0x00000040) //WR - PCIe DMA 测试使能
#define ADDR_tp_full_speed_en      (OFFSET_DATA_STREAM+0x00000041) //WR - 全速率发送使能
#define ADDR_tp_dma_dsize          (OFFSET_DATA_STREAM+0x00000042) //WR - 间歇发送数据时，突发传输数据大小
#define ADDR_tp_data_count_1s      (OFFSET_DATA_STREAM+0x00000043) //RO - 1秒发送数据统计，单位 DWORD

#define ADDR_ds_frame_size_byte    (OFFSET_DATA_STREAM+0x00000050) //WR - PCIe buffer大小，单位 Byte
#define ADDR_ds_buffer_space_used  (OFFSET_DATA_STREAM+0x00000051) //RO - DDR4 缓冲空间占用，单位 Byte
#define ADDR_ds_buffer_status      (OFFSET_DATA_STREAM+0x00000052) //RO - 缓冲区状态，bit:0 frame_buf_empty; bit:1 frame_buf_write_busy; bit:2 frame_buf_almost_full
////////////////////////////////////////////////////////////////////////////////
// IO TRIGGER Address
////////////////////////////////////////////////////////////////////////////////
#define ADDR_trigger_mode           (OFFSET_IO_TRIGGER+0x00000000) //WR -  0 - command(default) 1 - timer 2 - cardio
#define ADDR_soft_trigger_reverse   (OFFSET_IO_TRIGGER+0x00000001) //WR - 电平翻转
#define ADDR_pulse_width            (OFFSET_IO_TRIGGER+0x00000002) //WR - unit: 8 ns
#define ADDR_pulse_period           (OFFSET_IO_TRIGGER+0x00000003) //WR - unit: 1 us
#define ADDR_pulse_number           (OFFSET_IO_TRIGGER+0x00000004) //WR
#define ADDR_load_desc_en           (OFFSET_IO_TRIGGER+0x00000005) //WO
#define ADDR_trigger_timeout_thd    (OFFSET_IO_TRIGGER+0x00000006) //WR - unit: 8 ns，trigger反馈超时时间
#define ADDR_trigger_resend_max     (OFFSET_IO_TRIGGER+0x00000007) //WR - trigger反馈超时后重发次数，默认0
#define ADDR_hard_trigger_reverse   (OFFSET_IO_TRIGGER+0x00000008) //WR - 电平翻转
#define ADDR_trigger_manual         (OFFSET_IO_TRIGGER+0x0000000F) //WO - {trigger_type, trigger_en}

#define Trigger_maunal_rising       ((0x00 << 1) + 0x01)
#define Trigger_maunal_falling      ((0x01 << 1) + 0x01)

#define ADDR_cardio_in_mask         (OFFSET_IO_TRIGGER+0x00000010) //WR - 
#define ADDR_cardio_out_sel         (OFFSET_IO_TRIGGER+0x00000011) //WR - soft_pulse_io\pulse_cardio\o0_out_pulse
#define ADDR_i0_pulse_width         (OFFSET_IO_TRIGGER+0x00000012) //WR - unit: 8 ns
#define ADDR_i0_pulse_period        (OFFSET_IO_TRIGGER+0x00000013) //WR - unit: 1 us
#define ADDR_i0_pulse_number        (OFFSET_IO_TRIGGER+0x00000014) //WR - 
#define ADDR_i0_load_desc_en        (OFFSET_IO_TRIGGER+0x00000015) //WO - 
#define ADDR_ttl_pins_dir           (OFFSET_IO_TRIGGER+0x00000020) //WR -

#define ADDR_trigger_count_sent     (OFFSET_IO_TRIGGER+0x00000030) //RO
#define ADDR_trigger_count_acked    (OFFSET_IO_TRIGGER+0x00000031) //RO
#define ADDR_trigger_timeout_count  (OFFSET_IO_TRIGGER+0x00000032) //RO
/**
* glitch_time0  - FPGA_IIN1
* glitch_time1  - FPGA_IIN2
* glitch_time2  - FPGA_IIN3
* glitch_time3  - FPGA_IIN4
* glitch_time4  - FPGA_EXT_IIN11
* glitch_time5  - FPGA_EXT_IIN12
* glitch_time6  - FPGA_EXT_IIN13
* glitch_time7  - FPGA_EXT_IIN14
* glitch_time8  - FPGA_DIN1
* glitch_time9  - FPGA_DIN2
* glitch_time10 - FPGA_TTLIO11
* glitch_time11 - FPGA_TTLIO12
* glitch_time12~15 - unused
**/
#define ADDR_glitch_time0           (OFFSET_IO_TRIGGER+0x00000040) //WR - unit: 8 ns
#define ADDR_glitch_time1           (OFFSET_IO_TRIGGER+0x00000041) //WR - unit: 8 ns
//...
#define ADDR_glitch_time15          (OFFSET_IO_TRIGGER+0x0000004F) //WR - unit: 8 ns
////////////////////////////////////////////////////////////////////////////////
// PoCXP
////////////////////////////////////////////////////////////////////////////////
#define ADDR_force_power            (OFFSET_POCXP+0x00000000) //WO 2'b10 强制断电，2'b11强制加电
#define ADDR_pocxp_state            (OFFSET_POCXP+0x00000020) //RO s0_PowerOff、s1_Sense、s2_Power24V_INIT、s3_Power24V
#define ADDR_bus_vol_0              (OFFSET_POCXP+0x00000030) //RO 总线电压 - 1.25mV
#define ADDR_bus_vol_1              (OFFSET_POCXP+0x00000031) //RO 总线电压 - 1.25mV
#define ADDR_bus_vol_2              (OFFSET_POCXP+0x00000032) //RO 总线电压 - 1.25mV
#define ADDR_bus_vol_3              (OFFSET_POCXP+0x00000033) //RO 总线电压 - 1.25mV
#define ADDR_current_0              (OFFSET_POCXP+0x00000040) //RO 电流（16bit 有符号数） - 1mA
#define ADDR_current_1              (OFFSET_POCXP+0x00000041) //RO 电流（16bit 有符号数） - 1mA
#define ADDR_current_2              (OFFSET_POCXP+0x00000042) //RO 电流（16bit 有符号数） - 1mA
#define ADDR_current_3              (OFFSET_POCXP+0x00000043) //RO 电流（16bit 有符号数） - 1mA
////////////////////////////////////////////////////////////////////////////////
// Event Recorder
////////////////////////////////////////////////////////////////////////////////
#define ADDR_event_time_L          (OFFSET_EVENT_REC+0x00000000) //RO
#define ADDR_event_time_H          (OFFSET_EVENT_REC+0x00000001) //RO
#define ADDR_event_type            (OFFSET_EVENT_REC+0x00000002) //RO
#define ADDR_event_counter         (OFFSET_EVENT_REC+0x00000003) //RO
#define ADDR_event_sclr            (OFFSET_EVENT_REC+0x00000004) //WO
////////////////////////////////////////////////////////////////////////////////
// FAN
////////////////////////////////////////////////////////////////////////////////
#define ADDR_FAN_VER_CAPS            (OFFSET_FAN+0x00000000)
#define ADDR_FAN_PWM_CONTROL         (OFFSET_FAN+0x00000001) //WO {EN, RESERVED[], PERIOD_MINUS_1}
#define ADDR_FAN_PWM_HIGH            (OFFSET_FAN+0x00000002) //WO {HIGH_TIME}
#define ADDR_FAN_SENSE_STATUS        (OFFSET_FAN+0x00000003) //RO {LOCKED, SENSE_PERIOD}

#define FAN_PWM_CLK_FREQ             (1.0e8) //100MHz
#define FAN_PWM_FREQ                 (25000)
#define FAN_DEF_PWM_PERIOD           (FAN_PWM_CLK_FREQ / FAN_PWM_FREQ)
////////////////////////////////////////////////////////////////////////////////
// FPGA Monitor
////////////////////////////////////////////////////////////////////////////////
#define ADDR_MEASURED_TEMP           (OFFSET_FPGA_MONITOR+0x00000000) //RO
#define ADDR_MEASURED_VCCINT         (OFFSET_FPGA_MONITOR+0x00000001) //RO
#define ADDR_MEASURED_VCCAUX         (OFFSET_FPGA_MONITOR+0x00000002) //RO
#define ADDR_MEASURED_VCCBRAM        (OFFSET_FPGA_MONITOR+0x00000003) //RO
////////////////////////////////////////////////////////////////////////////////
// CXP Lamp Indicator
////////////////////////////////////////////////////////////////////////////////
#define ADDR_lamp_connection_state   (OFFSET_CXP_LAMP+0x00000000) //WR
#define ADDR_lamp_force_mode         (OFFSET_CXP_LAMP+0x00000001) //WR

#define LAMP_ST_unused           0 //未使用，上电默认模式
#define LAMP_ST_unconnected      1 //未连接
#define LAMP_ST_in_progress      2 //链路检测中
#define LAMP_ST_connected        3 //链路已连接
#define LAMP_ST_incompatible     4 //链路不兼容
//////////////////////////////////////////////////////////////////////////////// 
// CXP interface
//////////////////////////////////////////////////////////////////////////////// 
#define CXP_ADDR_TX_FREE            0x00000014 //RO - tx buffer 缓冲区空余空间
#define CXP_ADDR_SOP                0x00000015 //control package 包头
#define CXP_ADDR_TYP                0x00000016 //control package 类型
#define CXP_ADDR_TAG                0x00000017 //control package tag
#define CXP_ADDR_DAT                0x00000018 //control package 数据
#define CXP_ADDR_EOP                0x00000019 //control package 包尾

#define CXP_ADDR_RD_COUNT           0x00000010 //i_cpu_rd_count         读缓冲区word个数
#define CXP_ADDR_RD_DATA            0x00000011 //i_cpu_rd_data          读缓冲区数据
#define CXP_ADDR_RD_SCLR            0x00000012 //o_cpu_rst_cmd          读缓冲区复位

#define CXP_ADDR_LINK_IDLE_ERR      0x00000030 //RO - 空闲码错误统计
#define CXP_ADDR_SDP_TAILER_VLD     0x00000031 //RO - stream data 包计数
#define CXP_ADDR_SDP_CRC_ERR        0x00000032 //RO - stream data CRC 错误包计数
#define CXP_ADDR_SDP_DSIZEP_ERR     0x00000033 //RO - 包长错误包统计
#define CXP_ADDR_DPKT_VLD           0x00000034 //i_cpu_rx_dpkt_vld      数据包计数（回读后自动清空）
#define CXP_ADDR_CRC_ERR            0x00000035 //i_cpu_rx_crc_err_ind   control message CRC校验错误计数（回读后自动清空）
#define CXP_ADDR_TRIGGER_VLD        0x00000036 //接收trigger 计数
#define CXP_ADDR_TRIGGER_ERR        0x00000037 //接收trigger 错误计数
#define CXP_ADDR_TRIGGER_ACK_VLD    0x00000038 //接收trigger ack 计数
#define CXP_ADDR_TRIGGER_ACK_ERR    0x00000039 //接收trigger ack 错误计数
#define CXP_ADDR_HEARTBEAT_VLD      0x0000003A //心跳包计数
#define CXP_ADDR_HEARTBEAT_ERR      0x0000003B //心跳包错误计数
#define CXP_ADDR_TEST_COUNT         0x0000003C //TEST 
#define CXP_ADDR_TEST_ERROR         0x0000003D //TEST
#define CXP_ADDR_LINK_LOSS          0x0000003E //IDLE空闲码超时计数
#define CXP_ADDR_LINK_SYM_ERR       0x0000003F //8b10b编码错误超门限计数
#define CXP_ADDR_EVENT_DPKT_VLD     0x00000040 //event 数据包计数（回读后自动清空）
#define CXP_ADDR_EVENT_CRC_ERR      0x00000041 //event message CRC校验错误计数（回读后自动清空）

#define CXP_ADDR_LINK_ALIGN         0x00000020 // 0 链接未建立，else 链接建立
#define CXP_ADDR_CONNECTION_ID      0x00000000 //设定当前connetcion的ID（从devcie中回读获得）
#define CXP_ADDR_DATA_ENABLE        0x00000001 //WO - RX DATA 至解析模块传输开关，默认开
#define CXP_ADDR_BIT_SHIFT_TIME     0x00000002 //WO - DWORD边界锁定时，bit位位移时间，默认128
#define CXP_ADDR_IDLE_DET_THR       0x00000003 //WO - LINK Detecting 时，有连续 N 个IDLE码出现，即认为锁定，默认N=1000
#define CXP_ADDR_ls_speed_mode      0x00000004 //WO - up connection 的速率切换，默认低速率
#define CXP_ADDR_CXP_TEST_EN        0x00000005 //WO - 进入test模式，up connection 上传测试数据包，默认关闭
#define CXP_ADDR_STAT_SYM_PERIOD    0x00000006 //WO - detected模式下，8b10b编码错误统计周期，默认10E8
#define CXP_ADDR_STAT_SYM_THD       0x00000007 //WO - detected模式下，8b10b编码错门限，默认10E6(1%)

#define CXP_ADDR_EVENT_RD_COUNT     0x00000060 //i_event_rd_count         读缓冲区word个数
#define CXP_ADDR_EVENT_RD_DATA      0x00000061 //i_event_rd_data          读缓冲区数据
#define CXP_ADDR_EVENT_RD_SCLR      0x00000062 //o_event_rst_cmd          读缓冲区复位

#define FLAG_LOW_SPEED              0x00
#define FLAG_HIGH_SPEED             0x01 //CXP-10\CXP-12
////////////////////////////////////////////////////////////////////////////////
// SERDES DRP
////////////////////////////////////////////////////////////////////////////////
#define ADDR_SERDES_BIT_RATE       (OFFSET_SERDES_DRP+0x00000000) //WR - 配置bit rate

//////////////////////////////////////////////////////////////////////////////// 
// CXP Standard
//////////////////////////////////////////////////////////////////////////////// 
#define CXP_CTRL_DPKT_HEAD          0xfbfbfbfb  //control packet包头
#define CXP_CTRL_DPKT_TAIL          0xfdfdfdfd  //control packet包尾

#define Bootstrap_Standard                              0x00000000 // Support M R 4 Integer
#define Bootstrap_Revision                              0x00000004 // Support M, X R 4 Integer
#define Bootstrap_XmlManifestSize                       0x00000008 // Support M R 4 Integer
#define Bootstrap_XmlManifestSelector                   0x0000000C // Support M R/W 4 Integer
#define Bootstrap_XmlVersion                            0x00000010 // Support M, X R 4 Integer
#define Bootstrap_XmlSchemaVersion                      0x00000014 // Support M R 4 Integer
#define Bootstrap_XmlUrlAddress                         0x00000018 // Support M R 4 Integer
#define Bootstrap_Iidc2Address                          0x0000001C // Support M R 4 Integer
#define Bootstrap_DeviceVendorName                      0x00002000 // GenICam M, X R 32 String
#define Bootstrap_DeviceModelName                       0x00002020 // GenICam M, X R 32 String
#define Bootstrap_DeviceManufacturerInfo                0x00002040 // GenICam M, X R 48 String
#define Bootstrap_DeviceVersion                         0x00002070 // GenICam M, X R 32 String
#define Bootstrap_DeviceSerialNumber                    0x000020B0 // GenICam M, X R 16 String
#define Bootstrap_DeviceUserID                          0x000020C0 // GenICam M, X R/W 16 String
#define Bootstrap_WidthAddress                          0x00003000 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_HeightAddress                         0x00003004 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_AcquisitionModeAddress                0x00003008 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_AcquistionStartAddress                0x0000300C // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_AcquistionStopAddress                 0x00003010 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_PixelFormatAddress                    0x00003014 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_DeviceTapGeometryAddress              0x00003018 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_Image1StreamIDAddress                 0x0000301C // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_Image_n_StreamIDAddress               0x00003018 // CXP M R 4 Integer (allows non-GenICam applications) - Support CXP1.1
#define Bootstrap_ConnectionReset                       0x00004000 // CXP M, E W/(R) 4 Integer
#define Bootstrap_DeviceConnectionID                    0x00004004 // CXP M, E R 4 Integer
#define Bootstrap_MasterHostConnectionID                0x00004008 // CXP M, E R/W 4 Integer
#define Bootstrap_ControlPacketSizeMax                  0x0000400C // CXP M R 4 Integer
#define Bootstrap_StreamPacketSizeMax                   0x00004010 // CXP M R/W 4 Integer
#define Bootstrap_ConnectionConfig                      0x00004014 // CXP M, X R/W 4 Integer
#define Bootstrap_ConnectionConfigDefault               0x00004018 // CXP M, X R 4 Integer
#define Bootstrap_TestMode                              0x0000401C // CXP M, X R/W 4 Integer
#define Bootstrap_TestErrorCountSelector                0x00004020 // CXP M, X R/W 4 Integer
#define Bootstrap_TestErrorCount                        0x00004024 // CXP M, X R/W 4 Integer
#define Bootstrap_TestPacketCountTx                     0x00004028 // CXP M, X R/W 8 Integer   - Support CXP1.1
#define Bootstrap_TestPacketCountRx                     0x00004030 // CXP M, X R/W 8 Integer   - Support CXP1.1
#define Bootstrap_ElectricalComplianceTest              0x00004038 // CXP O R/W 4 Integer      - Support CXP1.1
#define Bootstrap_CapabilityRegister                    0x0000403C // CXP M R 4 Integer        - Support CXP1.1
#define Bootstrap_FeatureControlRegister                0x00004040 // CXP M R/W 4 Integer      - Support CXP1.1
#define Bootstrap_VersionsSupported                     0x00004044 // CXP M, X R 4 Integer     - Support CXP1.1
#define Bootstrap_VersionUsed                           0x00004048 // CXP M, X R/W 4 Integer   - Support CXP1.1
#define Bootstrap_LinkSharingStatus                     0x0000404C // CXP O R 4 Integer        - Support CXP1.1
#define Bootstrap_LinkSharingHorizontalStripeCount      0x00004050 // CXP O, X R/(W) 4 Integer - Support CXP1.1
#define Bootstrap_LinkSharingVerticalStripeCount        0x00004054 // CXP O, X R/(W) 4 Integer - Support CXP1.1
#define Bootstrap_LinkSharingHorizontalOverlap          0x00004058 // CXP O, X R/(W) 4 Integer - Support CXP1.1
#define Bootstrap_LinkSharingVerticalOverlap            0x0000405C // CXP O, X R/(W) 4 Integer - Support CXP1.1
#define Bootstrap_LinkSharingDuplicateStripe            0x00004060 // CXP O, X R/(W) 4 Integer - Support CXP1.1
#define Bootstrap_ManufacturerRegStartSpace             0x00006000 // ----

/**
* CXP Control Message Acknowledgment code
* //Success:
*  0x00    // Final, command executed OK, reply data is appended (i.e.acknowledgment of read command).
*  0x01    // Final, command executed OK, No reply data is appended (i.e.acknowledgment of write command).
*  0x04    // Wait.The time for the Host to wait shall be sent as a 4 byte integer
*  //Logical Errors(final acknowledgments) :
*  0x40    // Invalid address.
*  0x41    // Invalid data for the address.
*  0x42    // Invalid control operation code.
*  0x43    // Write attempted to a read - only address.
*  0x44    // Read attempted from a write - only address.
*  0x45    // Size field too large – command message(write) or acknowledgment message(read) would exceed packet size limit.
*  0x46    // Incorrect size received, message size is inconsistent with message size indication.
*  0x47    // Malformed packet.
*  // Physical Errors(final acknowledgments) :
*  0x80    // Failed CRC test in last received command.
**/

/**
* 从device中读取的GenICam部分地址(CXP协议强制要求的)
**/
#define GenICam_Addr_Width             0x00008118
#define GenICam_Addr_Height            0x0000811c
#define GenICam_Addr_AcquisitionMode   0x00008200
#define GenICam_Addr_AcquistionStart   0x00008204
#define GenICam_Addr_AcquistionStop    0x00008208
#define GenICam_Addr_PixelFormat       0x00008144
#define GenICam_Addr_DeviceTapGeometry 0x00008160
#define GenICam_Addr_Image1StreamID    0x00008164

//get from the register
#define GenICam_XmlUrlAddress          0x0f000028
#define GenICam_XmlAddress             0x0F000128
#define GenICam_XmlLength              0x611D //byte

/**
* 从XML中解析的地址
**/
#define XML_Addr_TestImageSelector          0x815c //RW - Selects the type of test image that is sent by the camera.
#define XML_Addr_TriggerSource              0x8238
#define XML_Addr_TriggerActivation          0x823C
#define XML_Addr_TLParamsLocked             0x823C
#define XML_Addr_AcquisitionFramePeriodRaw  0x8220     
#define XML_Addr_AcquisitionMaxFrameRate    0x8224 //WO
#define XML_Addr_InterfaceUtilization       0x8260
#define XML_Addr_DeviceIndicatorMode        0x8388 //RW - Controls the behavior of the indicators (such as LEDs) showing the status of the Device.
#define XML_Addr_PayloadSize                0xa800 //RO - Provides the number of bytes transferred for each image or chunk on the stream channel.
#define XML_Addr_BuiltInTest                0xa02c //RO - Returns the Built In Test status of the camera.
//AcquisitionControl
#define XML_Addr_AcquisitionFramePeriodRaw  0x8220 //     Controls the acquisition rate (in 1us steps) at which the frames are captured.
#define XML_Addr_AcquisitionMaxFrameRate    0x8224 //WO - Sets the camera to the maximum frame rate, whit respect to the given settings
#define XML_Addr_TriggerSource              0x8238 //     Specifies the internal signal or physical input Line to use as the trigger source.
#define XML_Addr_TriggerActivation          0x823C //     0 - FallingEdge, 1 - RisingEdge
#define XML_Addr_ExposureMode               0x8250 //     Sets the operation mode of the Exposure (or shutter).
#define XML_Addr_ExposureTime               0x8258 //     Sets the Exposure time (in 1us steps)
#define XML_Addr_GainRaw                    0x8908 //     Selects which Gain is controlled by the various Gain features.
//**************************************************************************** typedef && enum
#define CTRL_PKT_MAX_DWORD  512   //实际与ControlPacketSizeMax相关
#define ACK_OVERTIME_MS     200   //control指令应答超时时间
#define ACK_MAX_OVERTIME_MS 10200 //control指令应答超时时间 + wait应答延迟时间



#ifdef __cplusplus
}
#endif