#pragma once
#include "Header.h"

class TypingTutor
{
public:
	TypingTutor(void);
public:
	~TypingTutor(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static TypingTutor* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
	void OnTrayIcon(WPARAM wp, LPARAM lp);
	HWND hDialog;
	HICON hIcon;
	PNOTIFYICONDATA pNID;
	HANDLE hMutex;
};


