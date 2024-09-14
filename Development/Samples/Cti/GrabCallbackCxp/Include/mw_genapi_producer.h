#pragma once
#include "GenTL.h"

#if !defined(GC_IMPORT_EXPORT) || !defined(GC_CALLTYPE)
#error Need include GenTL.h before mw_genapi_producer.h
#endif

#if !defined(MW_GENAPI)
#define MW_GENAPI GC_IMPORT_EXPORT GC_ERROR GC_CALLTYPE
#define MW_GENAPI_P(function) typedef GC_ERROR( GC_CALLTYPE *function )
#endif

#ifdef __cplusplus
extern "C" {
    namespace GenTL {
        namespace mw_genapi {
#endif
            enum GC_GENAPI_ERROR_LIST
            {
                GC_GENAPI_ERR_CUSTOM_ID = -10000,// == GenTL::GC_ERR_CUSTOM_ID 

                GC_ERR_NOT_COMMAND = -10001,
                GC_ERR_NOT_INTEGER = -10002,
                GC_ERR_NOT_BOOLEAN = -10003,
                GC_ERR_NOT_ENUM = -10004,
                GC_ERR_NOT_FLOAT = -10005,
                GC_ERR_NOT_STRING = -10006,
                GC_ERR_NOT_READABLE = -10007,
                GC_ERR_NOT_WRITABLE = -10008,
                GC_ERR_ERROR_FILE = -10009,     //
                GC_ERR_MISMATCH = -10010,       //
                GC_ERR_NOT_FOUND_FEATURE = -10011,
                GC_ERR_NO_ACCESS = -10012,
                GC_ERR_EXCEPTION = -10013,
                GC_ERR_FILE_NOT_AVAILABLE = -10014
            };

            enum _node_access_mode
            {
                NI = 0,       //!< Not implemented
                NA = 1,        //!< Not available
                WO = 2,        //!< Write Only
                RO = 3,        //!< Read Only
                RW = 4,        //!< Read and Write
                _UndefinedAccesMode,    //!< Object is not yet initialized
                _CycleDetectAccesMode   //!< used internally for AccessMode cycle detection
            };
            typedef int32_t node_access_mode_t;

            enum _node_type
            {
                T_VALUE = 0,       //!< IValue interface
                T_BASE = 1,
                T_INTEGER = 2,
                T_BOOLEAN = 3,
                T_COMMAND = 4,
                T_FLOAT = 5,
                T_STRING = 6,
                T_REGISTER = 7,
                T_CATEGORY = 8,
                T_ENUMERATION = 9,
                T_ENUMENTRY = 10,
                T_PORT = 11,
                T_NODE_TYPE_ID = 1000
            };
            typedef int32_t node_type_t;

            enum _32b_visibility_
            {
                VB_Beginner = 0,
                VB_Expert = 1,
                VB_Guru = 2,
                VB_Invisible = 3,
                VB_UndefinedVisibility = 99
            };
            typedef int32_t visibility_t;

#   pragma pack (push, 1)
            typedef struct st_load_file_info {
                char name[256];
                node_type_t node_type;
                union
                {
                    char s_value[256];
                    int64_t i_value;
                    double f_value;
                };
                GC_ERROR load_ret;
            }mw_load_file_info_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
            typedef struct _enum_value_
            {
                char*        enum_string;            //enum类型的一个字符串形式内容
                uint32_t    str_maxsize;            //enum类型的字符串需求大小
                int64_t     enum_value;             //enum类型的一个数值型内容
            }enum_value_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
            typedef struct _mw_node_info_
            {
                node_type_t         node_type;
                node_access_mode_t  mode_access;
                union {
                    struct {
                        int64_t i_min;
                        int64_t i_max;
                        int64_t i_inc;
                        int64_t i_value;
                    } integer_info;

                    struct {
                        double f_min;
                        double f_max;
                        double f_inc;
                        double f_value;
                    } float_info;

                    struct {
                        uint32_t        str_need_size;                        
                        uint32_t        str_num;  
                        //enum_value_t* enum_str_list;
                        //enum_value_t    entry_info;//当前值
                    } enum_info;
                }info;

                visibility_t node_vb; //可见值
                uint8_t reserve[60];//预留, 64->60
            } mw_node_info_t;
#   pragma pack (pop)

#   pragma pack (push, 1)
            typedef struct _mw_node_state_
            {
                bool8_t is_streamable;
                bool8_t is_valid;

                uint8_t reserve[16];//预留
            }mw_node_state_t;
#   pragma pack (pop)


            //初始模块
            //Genapi标注接口模块
            MW_GENAPI mw_genapi_call_command(PORT_HANDLE hPort, const char* pFeature);
            
            MW_GENAPI mw_genapi_set_string(PORT_HANDLE hPort, const char* pFeature, const char* pStr);
            MW_GENAPI mw_genapi_get_string(PORT_HANDLE hPort, const char* pFeature, char* pStr, size_t* piSize);
            
            MW_GENAPI mw_genapi_set_integer(PORT_HANDLE hPort, const char* pFeature, int64_t iValue);
            MW_GENAPI mw_genapi_get_integer(PORT_HANDLE hPort, const char* pFeature, int64_t* piValue);
            
            MW_GENAPI mw_genapi_set_float(PORT_HANDLE hPort, const char* pFeature, double fValue);
            MW_GENAPI mw_genapi_get_float(PORT_HANDLE hPort, const char* pFeature, double* pfValue);

            //读取register型参数
            MW_GENAPI mw_genapi_set_register(PORT_HANDLE hPort, const char* pFeature, const char* pStr, size_t* piSize);
            //设置register型参数
            MW_GENAPI mw_genapi_get_register(PORT_HANDLE hPort, const char* pFeature, char* pStr, size_t* piSize);
            

            MW_GENAPI mw_genapi_load_ini(PORT_HANDLE hPort, const wchar_t* file_name, mw_load_file_info_t** p_load_file_info_list, uint32_t* p_list_size);
            MW_GENAPI mw_genapi_save_ini(PORT_HANDLE hPort, const wchar_t* file_name);
           
            
            //参数信息模块
            /*  获取xml信息，并读取
            *   size_t xml_size = 0;
            *   if(GC_ERR_SUCCESS == mw_get_xml(h_port, 0, NULL, &xml_size))
            *   {
            *       char* xml_buf = (char*)malloc(xml_size);
            *       if(xml_buf)
            *           mw_get_xml(h_port, 0, xml_buf, &xml_size);
            *   }
            */
            MW_GENAPI mw_get_xml(PORT_HANDLE h_port, uint32_t iURLIndex, char* xml_buf, size_t* psize);
            //获取节点信息
            MW_GENAPI mw_get_node_info(PORT_HANDLE h_port, const char* node_name, mw_node_info_t* p_node_info);
            //获取enum节点具体信息
            MW_GENAPI mw_get_enum_array(PORT_HANDLE h_port, const char* node_name, enum_value_t* p_entry, enum_value_t** p_enum_array, uint32_t* p_num);
            //获取节点状态
            MW_GENAPI mw_get_node_state(PORT_HANDLE h_port, const char* node_name, mw_node_state_t* p_node_state);

            //归一化数据读取模式
            //只注册已打开的data stream handle，绑定到h_simple上
            MW_GENAPI mw_sim_register_handle(PORT_HANDLE hPort, PORT_HANDLE* h_simple_ptr);
            //释放h_simple
            MW_GENAPI mw_sim_unregister_handle(PORT_HANDLE h_simple);
            //0：采集卡(拼合版，zip格式)
            //1：相机(相机独立zip)
            MW_GENAPI mw_sim_get_xml(PORT_HANDLE h_simple, uint32_t level, char* xml_buf, size_t* psize);
            //根据参数获取到参数所属句柄，具体再调用获取想要的信息
            MW_GENAPI mw_sim_find(PORT_HANDLE h_simple, const char* pFeature, PORT_HANDLE* h_module_ptr);
            
            MW_GENAPI_P(p_mw_genapi_call_command)(PORT_HANDLE hPort, const char* pFeature);
            MW_GENAPI_P(p_mw_genapi_set_string)(PORT_HANDLE hPort, const char* pFeature, const char* pStr);
            MW_GENAPI_P(p_mw_genapi_get_string)(PORT_HANDLE hPort, const char* pFeature, char* pStr, size_t* piSize);
            MW_GENAPI_P(p_mw_genapi_set_integer)(PORT_HANDLE hPort, const char* pFeature, int64_t iValue);
            MW_GENAPI_P(p_mw_genapi_get_integer)(PORT_HANDLE hPort, const char* pFeature, int64_t* piValue);
            MW_GENAPI_P(p_mw_genapi_set_float)(PORT_HANDLE hPort, const char* pFeature, double fValue);
            MW_GENAPI_P(p_mw_genapi_get_float)(PORT_HANDLE hPort, const char* pFeature, double* pfValue);
            MW_GENAPI_P(p_mw_genapi_set_register)(PORT_HANDLE hPort, const char* pFeature, const char* pStr, size_t* piSize);
            MW_GENAPI_P(p_mw_genapi_get_register)(PORT_HANDLE hPort, const char* pFeature, char* pStr, size_t* piSize);

            MW_GENAPI_P(p_mw_genapi_load_ini)(PORT_HANDLE hPort, const wchar_t* file_name, mw_load_file_info_t** p_load_file_info_list, uint32_t* p_list_size);
            MW_GENAPI_P(p_mw_genapi_save_ini)(PORT_HANDLE hPort, const wchar_t* file_name);
            


            MW_GENAPI_P (p_mw_get_xml)(PORT_HANDLE h_port, uint32_t iURLIndex, char* xml_buf, size_t* psize);
            MW_GENAPI_P (p_mw_get_node_info)(PORT_HANDLE h_port, const char* node_name, mw_node_info_t* p_node_info);
            MW_GENAPI_P (p_mw_get_enum_array)(PORT_HANDLE h_port, const char* node_name, enum_value_t* p_entry, enum_value_t** p_enum_array, uint32_t* p_num);
            MW_GENAPI_P (p_mw_get_node_state)(PORT_HANDLE h_port, const char* node_name, mw_node_state_t* p_node_state);

            MW_GENAPI_P (p_mw_sim_register_handle)(PORT_HANDLE hPort, PORT_HANDLE* h_simple_ptr);
            MW_GENAPI_P (p_mw_sim_unregister_handle)(PORT_HANDLE h_simple);
            MW_GENAPI_P (p_mw_sim_get_xml)(PORT_HANDLE h_simple, uint32_t level, char* xml_buf, size_t* psize);
            MW_GENAPI_P (p_mw_sim_find)(PORT_HANDLE h_simple, const char* pFeature, PORT_HANDLE* h_module_ptr);
            

#ifdef __cplusplus
        }
    }
}
#endif
