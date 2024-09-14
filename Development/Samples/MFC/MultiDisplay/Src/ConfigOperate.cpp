//#include "stdafx.h"
#include "stdafx.h"
#include "ConfigOperate.h"
#include <string>

typedef enum
{
	MAX_VALUE_LEN = 300,		//< 每个键值最大长度  | maximum length per key
}ENUM_LIMIT;

static CString gl_strConfigPath = _T("");
static CString gl_strLanguagePath = _T("");
static CString gl_currentLanguage = _T("");

CString GetKeyValue(const CString& appName, const CString& keySource)
{
	// 获取值
	// get value
	TCHAR val[MAX_VALUE_LEN] = { 0 };
	GetPrivateProfileString(
		appName, keySource, _T(""),
		val, sizeof(val), gl_strLanguagePath);

	//! 结果
	// result
	CString strVal(_T(""));
	strVal.Format(_T("%s"), val);
	return strVal;
}

CString getParseLanguageString(CString strOrg)
{

	if (gl_strLanguagePath.IsEmpty())
	{	
		//! 获取到模块的全路径，包括模块名称
		// Get the full path to the module, including the module name
		TCHAR szFullPath[MAX_PATH + 1] = { 0 };
		GetModuleFileName(NULL, szFullPath, MAX_PATH);
		gl_strConfigPath = szFullPath;

		//! 过滤掉模块名称
		// Filter out module name
		int nPathIndex = gl_strConfigPath.ReverseFind(_T('\\'));
		if (nPathIndex > 0)
		{
			gl_strConfigPath = gl_strConfigPath.Left(nPathIndex);
		}
		CString lan = getCurrentLanguage();

		gl_strLanguagePath = gl_strConfigPath + _T("\\language\\") + lan + TEXT(".ini");
	}

	CString lan = _T("");
	if (!gl_strLanguagePath.IsEmpty())
	{
		CString strKey(_T("LANGUAGE_TRANSFORM"));
		lan = GetKeyValue(strKey, strOrg);
	}
	if (lan.IsEmpty())
	{
		lan = _T("");
	}

	return lan;

}

// 获取当前配置文件中设置的语言存入gl_currentLanguage
// Get the language set in the current configuration file and save it to gl_currentLanguage
CString getCurrentLanguage()
{
	if (gl_currentLanguage.IsEmpty())
	{

		// "2052" stands for simplified Chinese
		if (2052 == GetUserDefaultUILanguage())
		{

			gl_currentLanguage = "chinese";
		}
		else
		{
			gl_currentLanguage = "english";
		}
		
	
	}

	return gl_currentLanguage;
}


