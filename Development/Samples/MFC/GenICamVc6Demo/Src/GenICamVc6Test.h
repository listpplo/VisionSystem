// GenICamVc6Test.h : main header file for the GENICAMVC6TEST application
//

#if !defined(AFX_GENICAMVC6TEST_H__91A66704_75B0_4846_BBD2_2E8CFB7CB833__INCLUDED_)
#define AFX_GENICAMVC6TEST_H__91A66704_75B0_4846_BBD2_2E8CFB7CB833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGenICamVc6TestApp:
// See GenICamVc6Test.cpp for the implementation of this class
//

class CGenICamVc6TestApp : public CWinApp
{
public:
	CGenICamVc6TestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenICamVc6TestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGenICamVc6TestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENICAMVC6TEST_H__91A66704_75B0_4846_BBD2_2E8CFB7CB833__INCLUDED_)
