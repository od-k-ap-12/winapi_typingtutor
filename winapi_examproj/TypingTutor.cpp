#include "TypingTutor.h"

#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER

TypingTutor* TypingTutor::ptr = NULL;

TypingTutor::TypingTutor(void)
{
	ptr = this;
	pNID = new NOTIFYICONDATA;
}

TypingTutor::~TypingTutor(void)
{
	delete pNID;
}

void TypingTutor::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL TypingTutor::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;

	HINSTANCE hInst = GetModuleHandle(NULL);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_WINAPIEXAMPROJ));
	SetClassLong(hDialog, -14, LONG(hIcon));
	memset(pNID, 0, sizeof(NOTIFYICONDATA));
	pNID->cbSize = sizeof(NOTIFYICONDATA);
	pNID->hIcon = hIcon;
	pNID->hWnd = hwnd;
	lstrcpy(pNID->szTip, TEXT("Typing Tutor"));
	pNID->uCallbackMessage = WM_ICON;
	pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
	pNID->uID = ID_TRAYICON;
	return TRUE;
}

void TypingTutor::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

void TypingTutor::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if (state == SIZE_MINIMIZED)
	{
		ShowWindow(hwnd, SW_HIDE);
		Shell_NotifyIcon(NIM_ADD, pNID);
	}
}

// обработчик пользовательского сообщения
void TypingTutor::OnTrayIcon(WPARAM wp, LPARAM lp)
{
	if (lp == WM_LBUTTONDBLCLK)
	{
		Shell_NotifyIcon(NIM_DELETE, pNID);
		ShowWindow(hDialog, SW_NORMAL);
		SetForegroundWindow(hDialog);
	}
}

BOOL CALLBACK TypingTutor::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
	}
	// пользовательское сообщение
	if (message == WM_ICON)
	{
		ptr->OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	return FALSE;
}
