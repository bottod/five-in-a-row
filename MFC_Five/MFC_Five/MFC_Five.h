
// MFC_Five.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_FiveApp:
// �йش����ʵ�֣������ MFC_Five.cpp
//

class CMFC_FiveApp : public CWinApp
{
public:
	CMFC_FiveApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_FiveApp theApp;