#pragma once
#include "GenTL.h"//GenTL接口
#include "windows.h"
#include "mw_genapi_producer.h"//GenApi接口
#include "mw_gentl_log.h"

using namespace GenTL;
using namespace mw_genapi;
using namespace mw_gentl_log;

//动态加载gentl cti库
class CStdLoadCti
{
public:
	CStdLoadCti();
	virtual ~CStdLoadCti();

	bool8_t load_sdk(const char* dll_name);
public:	
	HINSTANCE							m_dll_handle;

	//动态库函数指针对象成员
	static PGCGetInfo					m_GCGetInfo  ;
	static PGCGetLastError				m_GCGetLastError  ;
	static PGCInitLib					m_GCInitLib  ;
	static PGCCloseLib					m_GCCloseLib  ;
	static PGCReadPort					m_GCReadPort  ;
	static PGCWritePort					m_GCWritePort  ;
	static PGCGetPortURL				m_GCGetPortURL  ;
	static PGCGetPortInfo				m_GCGetPortInfo  ;
	static PGCRegisterEvent				m_GCRegisterEvent  ;
	static PGCUnregisterEvent			m_GCUnregisterEvent  ;
	static PEventGetData				m_EventGetData  ;
	static PEventGetDataInfo			m_EventGetDataInfo  ;
	static PEventGetInfo				m_EventGetInfo  ;
	static PEventFlush					m_EventFlush  ;
	static PEventKill					m_EventKill  ;
	static PTLOpen						m_TLOpen  ;
	static PTLClose						m_TLClose  ;
	static PTLGetInfo					m_TLGetInfo  ;
	static PTLGetNumInterfaces			m_TLGetNumInterfaces  ;
	static PTLGetInterfaceID			m_TLGetInterfaceID  ;
	static PTLGetInterfaceInfo			m_TLGetInterfaceInfo  ;
	static PTLOpenInterface				m_TLOpenInterface  ;
	static PTLUpdateInterfaceList		m_TLUpdateInterfaceList  ;
	static PIFClose						m_IFClose  ;
	static PIFGetInfo					m_IFGetInfo  ;
	static PIFGetNumDevices				m_IFGetNumDevices  ;
	static PIFGetDeviceID				m_IFGetDeviceID  ;
	static PIFUpdateDeviceList			m_IFUpdateDeviceList  ;
	static PIFGetDeviceInfo				m_IFGetDeviceInfo  ;
	static PIFOpenDevice				m_IFOpenDevice  ;
	static PDevGetPort					m_DevGetPort  ;
	static PDevGetNumDataStreams		m_DevGetNumDataStreams  ;
	static PDevGetDataStreamID			m_DevGetDataStreamID  ;
	static PDevOpenDataStream			m_DevOpenDataStream  ;
	static PDevGetInfo					m_DevGetInfo  ;
	static PDevClose					m_DevClose  ;
	static PDSAnnounceBuffer			m_DSAnnounceBuffer  ;
	static PDSAllocAndAnnounceBuffer	m_DSAllocAndAnnounceBuffer  ;
	static PDSFlushQueue				m_DSFlushQueue  ;
	static PDSStartAcquisition			m_DSStartAcquisition  ;
	static PDSStopAcquisition			m_DSStopAcquisition  ;
	static PDSGetInfo					m_DSGetInfo  ;
	static PDSGetBufferID				m_DSGetBufferID  ;
	static PDSClose						m_DSClose  ;
	static PDSRevokeBuffer				m_DSRevokeBuffer  ;
	static PDSQueueBuffer				m_DSQueueBuffer  ;
	static PDSGetBufferInfo				m_DSGetBufferInfo  ;
	static PGCGetNumPortURLs			m_GCGetNumPortURLs  ;
	static PGCGetPortURLInfo			m_GCGetPortURLInfo  ;

	//genapi
	static p_mw_genapi_call_command     m_mw_genapi_call_command	;
	static p_mw_genapi_set_string       m_mw_genapi_set_string		;
	static p_mw_genapi_get_string       m_mw_genapi_get_string		;
	static p_mw_genapi_set_integer      m_mw_genapi_set_integer		;
	static p_mw_genapi_get_integer      m_mw_genapi_get_integer		;
	static p_mw_genapi_set_float        m_mw_genapi_set_float		;
	static p_mw_genapi_get_float        m_mw_genapi_get_float		;
	static p_mw_genapi_load_ini			m_mw_genapi_load_ini		;

	//1.1.3.0
	static p_mw_get_xml					m_mw_get_xml				;	
	static p_mw_get_node_info			m_mw_get_node_info			;
	static p_mw_get_enum_array			m_mw_get_enum_array			;
	static p_mw_get_node_state			m_mw_get_node_state			;	

	static p_mw_sim_register_handle		m_mw_sim_register_handle	;
	static p_mw_sim_unregister_handle	m_mw_sim_unregister_handle	;
	static p_mw_sim_get_xml				m_mw_sim_get_xml			;
	static p_mw_sim_find				m_mw_sim_find				;

	//1.1.3.22
	static p_mw_set_language			m_mw_set_language;
	static p_mw_init_log				m_mw_init_log;
	static p_mw_uninit_log				m_mw_uninit_log;
};