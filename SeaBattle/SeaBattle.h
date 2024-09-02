
// SeaBattle.h: основной файл заголовка для приложения SeaBattle
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CSeaBattleApp:
// Сведения о реализации этого класса: SeaBattle.cpp
//

class CSeaBattleApp : public CWinApp
{
public:
	CSeaBattleApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSeaBattleApp theApp;
