
// Mutimedia_project.h : main header file for the Mutimedia_project application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMutimedia_projectApp:
// See Mutimedia_project.cpp for the implementation of this class
//

class CMutimedia_projectApp : public CWinAppEx
{
public:
	CMutimedia_projectApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMutimedia_projectApp theApp;
