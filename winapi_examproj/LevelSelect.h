#pragma once
#include "Header.h"

class MainMenu
{
public:
	MainMenu(void);
public:
	~MainMenu(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static MainMenu* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hDialog;
	HICON hIcon;
	PNOTIFYICONDATA pNID;
	HANDLE hMutex;
};


