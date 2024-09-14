#include "load_gentl_cti.h"
#include <stdio.h>
//static 函数指针初始化
PGCGetInfo					CStdLoadCti::m_GCGetInfo = NULL;
PGCGetLastError				CStdLoadCti::m_GCGetLastError = NULL;
PGCInitLib					CStdLoadCti::m_GCInitLib = NULL;
PGCCloseLib					CStdLoadCti::m_GCCloseLib = NULL;
PGCReadPort					CStdLoadCti::m_GCReadPort = NULL;
PGCWritePort				CStdLoadCti::m_GCWritePort = NULL;
PGCGetPortURL				CStdLoadCti::m_GCGetPortURL = NULL;
PGCGetPortInfo				CStdLoadCti::m_GCGetPortInfo = NULL;
PGCRegisterEvent			CStdLoadCti::m_GCRegisterEvent = NULL;
PGCUnregisterEvent			CStdLoadCti::m_GCUnregisterEvent = NULL;
PEventGetData				CStdLoadCti::m_EventGetData = NULL;
PEventGetDataInfo			CStdLoadCti::m_EventGetDataInfo = NULL;
PEventGetInfo				CStdLoadCti::m_EventGetInfo = NULL;
PEventFlush					CStdLoadCti::m_EventFlush = NULL;
PEventKill					CStdLoadCti::m_EventKill = NULL;
PTLOpen						CStdLoadCti::m_TLOpen = NULL;
PTLClose					CStdLoadCti::m_TLClose = NULL;
PTLGetInfo					CStdLoadCti::m_TLGetInfo = NULL;
PTLGetNumInterfaces			CStdLoadCti::m_TLGetNumInterfaces = NULL;
PTLGetInterfaceID			CStdLoadCti::m_TLGetInterfaceID = NULL;
PTLGetInterfaceInfo			CStdLoadCti::m_TLGetInterfaceInfo = NULL;
PTLOpenInterface			CStdLoadCti::m_TLOpenInterface = NULL;
PTLUpdateInterfaceList		CStdLoadCti::m_TLUpdateInterfaceList = NULL;
PIFClose					CStdLoadCti::m_IFClose = NULL;
PIFGetInfo					CStdLoadCti::m_IFGetInfo = NULL;
PIFGetNumDevices			CStdLoadCti::m_IFGetNumDevices = NULL;
PIFGetDeviceID				CStdLoadCti::m_IFGetDeviceID = NULL;
PIFUpdateDeviceList			CStdLoadCti::m_IFUpdateDeviceList = NULL;
PIFGetDeviceInfo			CStdLoadCti::m_IFGetDeviceInfo = NULL;
PIFOpenDevice				CStdLoadCti::m_IFOpenDevice = NULL;
PDevGetPort					CStdLoadCti::m_DevGetPort = NULL;
PDevGetNumDataStreams		CStdLoadCti::m_DevGetNumDataStreams = NULL;
PDevGetDataStreamID			CStdLoadCti::m_DevGetDataStreamID = NULL;
PDevOpenDataStream			CStdLoadCti::m_DevOpenDataStream = NULL;
PDevGetInfo					CStdLoadCti::m_DevGetInfo = NULL;
PDevClose					CStdLoadCti::m_DevClose = NULL;
PDSAnnounceBuffer			CStdLoadCti::m_DSAnnounceBuffer = NULL;
PDSAllocAndAnnounceBuffer	CStdLoadCti::m_DSAllocAndAnnounceBuffer = NULL;
PDSFlushQueue				CStdLoadCti::m_DSFlushQueue = NULL;
PDSStartAcquisition			CStdLoadCti::m_DSStartAcquisition = NULL;
PDSStopAcquisition			CStdLoadCti::m_DSStopAcquisition = NULL;
PDSGetInfo					CStdLoadCti::m_DSGetInfo = NULL;
PDSGetBufferID				CStdLoadCti::m_DSGetBufferID = NULL;
PDSClose					CStdLoadCti::m_DSClose = NULL;
PDSRevokeBuffer				CStdLoadCti::m_DSRevokeBuffer = NULL;
PDSQueueBuffer				CStdLoadCti::m_DSQueueBuffer = NULL;
PDSGetBufferInfo			CStdLoadCti::m_DSGetBufferInfo = NULL;
PGCGetNumPortURLs			CStdLoadCti::m_GCGetNumPortURLs = NULL;
PGCGetPortURLInfo			CStdLoadCti::m_GCGetPortURLInfo = NULL;
//genapi

p_mw_genapi_call_command    CStdLoadCti::m_mw_genapi_call_command = NULL;
p_mw_genapi_set_string      CStdLoadCti::m_mw_genapi_set_string = NULL;
p_mw_genapi_get_string      CStdLoadCti::m_mw_genapi_get_string = NULL;
p_mw_genapi_set_integer     CStdLoadCti::m_mw_genapi_set_integer = NULL;
p_mw_genapi_get_integer     CStdLoadCti::m_mw_genapi_get_integer = NULL;
p_mw_genapi_set_float       CStdLoadCti::m_mw_genapi_set_float = NULL;
p_mw_genapi_get_float       CStdLoadCti::m_mw_genapi_get_float = NULL;
p_mw_genapi_load_ini		CStdLoadCti::m_mw_genapi_load_ini = NULL;

p_mw_get_xml				CStdLoadCti::m_mw_get_xml			= NULL;
p_mw_get_node_info			CStdLoadCti::m_mw_get_node_info		= NULL;
p_mw_get_enum_array			CStdLoadCti::m_mw_get_enum_array	= NULL;
p_mw_get_node_state			CStdLoadCti::m_mw_get_node_state	= NULL;

p_mw_sim_register_handle	CStdLoadCti::m_mw_sim_register_handle	= NULL;
p_mw_sim_unregister_handle	CStdLoadCti::m_mw_sim_unregister_handle	= NULL;
p_mw_sim_get_xml			CStdLoadCti::m_mw_sim_get_xml			= NULL;
p_mw_sim_find				CStdLoadCti::m_mw_sim_find				= NULL;

p_mw_set_language			CStdLoadCti::m_mw_set_language = NULL;
p_mw_init_log				CStdLoadCti::m_mw_init_log = NULL;
p_mw_uninit_log				CStdLoadCti::m_mw_uninit_log = NULL;

CStdLoadCti::CStdLoadCti()
    :m_dll_handle(NULL)
{

}

CStdLoadCti::~CStdLoadCti()
{
    if (m_dll_handle)
        FreeLibrary(m_dll_handle);
}

bool8_t CStdLoadCti::load_sdk(const char* dll_name)
{
    if (m_dll_handle)
        return true;

	printf("dll_name is %s\n", dll_name);
    //m_dll_handle = LoadLibraryExA(dll_name, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
	m_dll_handle = LoadLibrary(dll_name);

    if (m_dll_handle == NULL)
        return false;

	m_GCGetInfo = (PGCGetInfo)GetProcAddress(m_dll_handle, "GCGetInfo");
	m_GCGetLastError = (PGCGetLastError)GetProcAddress(m_dll_handle, "GCGetLastError");
	m_GCInitLib = (PGCInitLib)GetProcAddress(m_dll_handle, "GCInitLib");
	m_GCCloseLib = (PGCCloseLib)GetProcAddress(m_dll_handle, "GCCloseLib");
	m_GCReadPort = (PGCReadPort)GetProcAddress(m_dll_handle, "GCReadPort");
	m_GCWritePort = (PGCWritePort)GetProcAddress(m_dll_handle, "GCWritePort");
	m_GCGetPortURL = (PGCGetPortURL)GetProcAddress(m_dll_handle, "GCGetPortURL");
	m_GCGetPortInfo = (PGCGetPortInfo)GetProcAddress(m_dll_handle, "GCGetPortInfo");
	m_GCRegisterEvent = (PGCRegisterEvent)GetProcAddress(m_dll_handle, "GCRegisterEvent");
	m_GCUnregisterEvent = (PGCUnregisterEvent)GetProcAddress(m_dll_handle, "GCUnregisterEvent");
	m_EventGetData = (PEventGetData)GetProcAddress(m_dll_handle, "EventGetData");
	m_EventGetDataInfo = (PEventGetDataInfo)GetProcAddress(m_dll_handle, "EventGetDataInfo");
	m_EventGetInfo = (PEventGetInfo)GetProcAddress(m_dll_handle, "EventGetInfo");
	m_EventFlush = (PEventFlush)GetProcAddress(m_dll_handle, "EventFlush");
	m_EventKill = (PEventKill)GetProcAddress(m_dll_handle, "EventKill");
	m_TLOpen = (PTLOpen)GetProcAddress(m_dll_handle, "TLOpen");
	m_TLClose = (PTLClose)GetProcAddress(m_dll_handle, "TLClose");
	m_TLGetInfo = (PTLGetInfo)GetProcAddress(m_dll_handle, "TLGetInfo");
	m_TLGetNumInterfaces = (PTLGetNumInterfaces)GetProcAddress(m_dll_handle, "TLGetNumInterfaces");
	m_TLGetInterfaceID = (PTLGetInterfaceID)GetProcAddress(m_dll_handle, "TLGetInterfaceID");
	m_TLGetInterfaceInfo = (PTLGetInterfaceInfo)GetProcAddress(m_dll_handle, "TLGetInterfaceInfo");
	m_TLOpenInterface = (PTLOpenInterface)GetProcAddress(m_dll_handle, "TLOpenInterface");
	m_TLUpdateInterfaceList = (PTLUpdateInterfaceList)GetProcAddress(m_dll_handle, "TLUpdateInterfaceList");
	m_IFClose = (PIFClose)GetProcAddress(m_dll_handle, "IFClose");
	m_IFGetInfo = (PIFGetInfo)GetProcAddress(m_dll_handle, "IFGetInfo");
	m_IFGetNumDevices = (PIFGetNumDevices)GetProcAddress(m_dll_handle, "IFGetNumDevices");
	m_IFGetDeviceID = (PIFGetDeviceID)GetProcAddress(m_dll_handle, "IFGetDeviceID");
	m_IFUpdateDeviceList = (PIFUpdateDeviceList)GetProcAddress(m_dll_handle, "IFUpdateDeviceList");
	m_IFGetDeviceInfo = (PIFGetDeviceInfo)GetProcAddress(m_dll_handle, "IFGetDeviceInfo");
	m_IFOpenDevice = (PIFOpenDevice)GetProcAddress(m_dll_handle, "IFOpenDevice");
	m_DevGetPort = (PDevGetPort)GetProcAddress(m_dll_handle, "DevGetPort");
	m_DevGetNumDataStreams = (PDevGetNumDataStreams)GetProcAddress(m_dll_handle, "DevGetNumDataStreams");
	m_DevGetDataStreamID = (PDevGetDataStreamID)GetProcAddress(m_dll_handle, "DevGetDataStreamID");
	m_DevOpenDataStream = (PDevOpenDataStream)GetProcAddress(m_dll_handle, "DevOpenDataStream");
	m_DevGetInfo = (PDevGetInfo)GetProcAddress(m_dll_handle, "DevGetInfo");
	m_DevClose = (PDevClose)GetProcAddress(m_dll_handle, "DevClose");
	m_DSAnnounceBuffer = (PDSAnnounceBuffer)GetProcAddress(m_dll_handle, "DSAnnounceBuffer");
	m_DSAllocAndAnnounceBuffer = (PDSAllocAndAnnounceBuffer)GetProcAddress(m_dll_handle, "DSAllocAndAnnounceBuffer");
	m_DSFlushQueue = (PDSFlushQueue)GetProcAddress(m_dll_handle, "DSFlushQueue");
	m_DSStartAcquisition = (PDSStartAcquisition)GetProcAddress(m_dll_handle, "DSStartAcquisition");
	m_DSStopAcquisition = (PDSStopAcquisition)GetProcAddress(m_dll_handle, "DSStopAcquisition");
	m_DSGetInfo = (PDSGetInfo)GetProcAddress(m_dll_handle, "DSGetInfo");
	m_DSGetBufferID = (PDSGetBufferID)GetProcAddress(m_dll_handle, "DSGetBufferID");
	m_DSClose = (PDSClose)GetProcAddress(m_dll_handle, "DSClose");
	m_DSRevokeBuffer = (PDSRevokeBuffer)GetProcAddress(m_dll_handle, "DSRevokeBuffer");
	m_DSQueueBuffer = (PDSQueueBuffer)GetProcAddress(m_dll_handle, "DSQueueBuffer");
	m_DSGetBufferInfo = (PDSGetBufferInfo)GetProcAddress(m_dll_handle, "DSGetBufferInfo");
	m_GCGetNumPortURLs = (PGCGetNumPortURLs)GetProcAddress(m_dll_handle, "GCGetNumPortURLs");
	m_GCGetPortURLInfo = (PGCGetPortURLInfo)GetProcAddress(m_dll_handle, "GCGetPortURLInfo");

	//genapi
	m_mw_genapi_call_command = (mw_genapi::p_mw_genapi_call_command)GetProcAddress(m_dll_handle, "mw_genapi_call_command");
	m_mw_genapi_set_string = (mw_genapi::p_mw_genapi_set_string)GetProcAddress(m_dll_handle, "mw_genapi_set_string");
	m_mw_genapi_get_string = (mw_genapi::p_mw_genapi_get_string)GetProcAddress(m_dll_handle, "mw_genapi_get_string");
	m_mw_genapi_set_integer = (mw_genapi::p_mw_genapi_set_integer)GetProcAddress(m_dll_handle, "mw_genapi_set_integer");
	m_mw_genapi_get_integer = (mw_genapi::p_mw_genapi_get_integer)GetProcAddress(m_dll_handle, "mw_genapi_get_integer");
	m_mw_genapi_set_float = (mw_genapi::p_mw_genapi_set_float)GetProcAddress(m_dll_handle, "mw_genapi_set_float");
	m_mw_genapi_get_float = (mw_genapi::p_mw_genapi_get_float)GetProcAddress(m_dll_handle, "mw_genapi_get_float");
	m_mw_genapi_load_ini = (mw_genapi::p_mw_genapi_load_ini)GetProcAddress(m_dll_handle, "mw_genapi_load_ini");

	//1.1.3.0
	m_mw_get_xml = (mw_genapi::p_mw_get_xml)GetProcAddress(m_dll_handle, "mw_get_xml");
	m_mw_get_node_info = (mw_genapi::p_mw_get_node_info)GetProcAddress(m_dll_handle, "mw_get_node_info");
	m_mw_get_enum_array = (mw_genapi::p_mw_get_enum_array)GetProcAddress(m_dll_handle, "mw_get_enum_array");
	m_mw_get_node_state = (mw_genapi::p_mw_get_node_state)GetProcAddress(m_dll_handle, "mw_get_node_state");

	m_mw_sim_register_handle	= 	(mw_genapi::p_mw_sim_register_handle)GetProcAddress(  m_dll_handle, "mw_sim_register_handle");
	m_mw_sim_unregister_handle	=	(mw_genapi::p_mw_sim_unregister_handle)GetProcAddress(  m_dll_handle, "mw_sim_unregister_handle");
	m_mw_sim_get_xml			=	(mw_genapi::p_mw_sim_get_xml)GetProcAddress(  m_dll_handle, "mw_sim_get_xml");
	m_mw_sim_find				=	(mw_genapi::p_mw_sim_find)GetProcAddress(  m_dll_handle, "mw_sim_find");

	//1.1.3.22
	m_mw_set_language = (p_mw_set_language)GetProcAddress(m_dll_handle, "mw_set_language");
	m_mw_init_log = (p_mw_init_log)GetProcAddress(m_dll_handle, "mw_init_log");
	m_mw_uninit_log = (p_mw_uninit_log)GetProcAddress(m_dll_handle, "mw_uninit_log");


	if (m_GCGetInfo
		&& m_GCGetLastError
		&& m_GCInitLib
		&& m_GCCloseLib
		&& m_GCReadPort
		&& m_GCWritePort
		&& m_GCGetPortURL
		&& m_GCGetPortInfo
		&& m_GCRegisterEvent
		&& m_GCUnregisterEvent
		&& m_EventGetData
		&& m_EventGetDataInfo
		&& m_EventGetInfo
		&& m_EventFlush
		&& m_EventKill
		&& m_TLOpen
		&& m_TLClose
		&& m_TLGetInfo
		&& m_TLGetNumInterfaces
		&& m_TLGetInterfaceID
		&& m_TLGetInterfaceInfo
		&& m_TLOpenInterface
		&& m_TLUpdateInterfaceList
		&& m_IFClose
		&& m_IFGetInfo
		&& m_IFGetNumDevices
		&& m_IFGetDeviceID
		&& m_IFUpdateDeviceList
		&& m_IFGetDeviceInfo
		&& m_IFOpenDevice
		&& m_DevGetPort
		&& m_DevGetNumDataStreams
		&& m_DevGetDataStreamID
		&& m_DevOpenDataStream
		&& m_DevGetInfo
		&& m_DevClose
		&& m_DSAnnounceBuffer
		&& m_DSAllocAndAnnounceBuffer
		&& m_DSFlushQueue
		&& m_DSStartAcquisition
		&& m_DSStopAcquisition
		&& m_DSGetInfo
		&& m_DSGetBufferID
		&& m_DSClose
		&& m_DSRevokeBuffer
		&& m_DSQueueBuffer
		&& m_DSGetBufferInfo
		&& m_GCGetNumPortURLs
		&& m_GCGetPortURLInfo
		&& m_mw_genapi_call_command
		&& m_mw_genapi_set_string
		&& m_mw_genapi_get_string
		&& m_mw_genapi_set_integer
		&& m_mw_genapi_get_integer
		&& m_mw_genapi_set_float
		&& m_mw_genapi_get_float
		&& m_mw_genapi_load_ini
		&& m_mw_get_xml
		&& m_mw_get_node_info
		&& m_mw_get_enum_array
		&& m_mw_get_node_state
		&& m_mw_sim_register_handle	
		&& m_mw_sim_unregister_handle	
		&& m_mw_sim_get_xml			
		&& m_mw_sim_find		
		//&& m_mw_init_log
		//&& m_mw_uninit_log
		) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}