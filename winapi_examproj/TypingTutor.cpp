#include "TypingTutor.h"
#include <conio.h>
#include <stdio.h>
#include <string>
#include <fstream>
#define STR_LEN 1000
using namespace std;

#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER

HBITMAP hBmp[3];
HWND hPic, hEditText, hEditInput, hLevelSelect, hTimer;

static int CorrectLetter = 0;
static int WrongLetter = 0;
static int CurrentLetter = 0;
TCHAR Text[STR_LEN] = TEXT("Please, select a level.");
static int TextLength;
TCHAR str[STR_LEN];
static bool IsLevelSelected = false;
int Time = 0;
static wstring Input;

TypingTutor* TypingTutor::ptr = NULL;
//void KeyUpHandler(HWND hwnd, WPARAM wParam, LPARAM lParam);
void WmCharHandler(HWND hwnd, WPARAM wParam, LPARAM lParam);



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
	hBackground = CreateSolidBrush(RGB(182, 180, 219));
	hDialog = hwnd;
	hEditText = GetDlgItem(hwnd, IDC_TEXT);
	hEditInput = GetDlgItem(hwnd, IDC_INPUT);
	hTimer = GetDlgItem(hwnd, IDC_EDIT3);

	hPic = GetDlgItem(hwnd, IDC_PICTURECONTROL);
	hBmp[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
	hBmp[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
	hBmp[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3));
	SetWindowText(hEditText, Text);
	SetWindowText(hEditInput, TEXT(" "));

	HINSTANCE hInst = GetModuleHandle(NULL);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SetClassLong(hDialog, -14, LONG(hIcon));
	memset(pNID, 0, sizeof(NOTIFYICONDATA));
	pNID->cbSize = sizeof(NOTIFYICONDATA);
	pNID->hIcon = hIcon;
	pNID->hWnd = hwnd;
	lstrcpy(pNID->szTip, TEXT("Typing Tutor"));
	pNID->uCallbackMessage = WM_ICON;
	pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
	pNID->uID = ID_TRAYICON;

	TCHAR GUID[] = TEXT("{D99CD3E0-670D-4def-9B74-99FD7E793DFB}");
	hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		MessageBox(hwnd, TEXT("App is already running."), TEXT("Warning"), MB_OK | MB_ICONINFORMATION);
		EndDialog(hwnd, 0);
	}



	return TRUE;
}

void TypingTutor::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

HBRUSH TypingTutor::OnCtlColor(HWND hwnd, HDC hdc, HWND hwndCtl, INT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return hBackground;
	}
}


void TypingTutor::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if (state == SIZE_MINIMIZED)
	{
		ShowWindow(hwnd, SW_HIDE);
		Shell_NotifyIcon(NIM_ADD, pNID);
	}
}


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
		HANDLE_MSG(hwnd, WM_CTLCOLORDLG, ptr->OnCtlColor);
		HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
	case WM_TIMER: {
		TCHAR str[STR_LEN];
		wsprintf(str, TEXT("Time: %d s"), Time);
		SetWindowText(hTimer, str);
		if (Time == 60)
		{
			KillTimer(hwnd, 1);
			_stprintf_s(str, TEXT("Correct letters=%d Wrong letters=%d"), CorrectLetter, WrongLetter);
			MessageBox(0, str, TEXT("Results"), MB_OK | MB_ICONINFORMATION);
			SetWindowText(hTimer, str);
			SetWindowText(hEditText, TEXT("Level is over"));
			SetWindowText(hEditInput, TEXT(" "));
			Input.clear();
			CurrentLetter = 0;
			CorrectLetter = 0;
			WrongLetter = 0;
			IsLevelSelected = false;
			SendMessage(hPic, STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[2]));
			Time = 0;
		}
		++Time;
		break;
	}

	case WM_CHAR:
		WmCharHandler(hwnd, wParam, lParam);
		break;
	}
	if (message == WM_ICON)
	{
		ptr->OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	return FALSE;
}


void WmCharHandler(HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	WCHAR symbol = wParam;
	static wstring TextFile;
	static wstring Line;
	if (symbol == '1' && IsLevelSelected == false) {
		TextFile.clear();
		wifstream File;
		File.open("EasyLevel.txt");
		while (getline(File, Line)) {
			TextFile += Line;
			TextFile += TEXT(" ");
		}
		File.close();
		_stprintf_s(Text, TEXT("%s"), TextFile.c_str());
		TextLength = TextFile.length();
		SetWindowText(hEditText, Text);
		IsLevelSelected = true;
		SetTimer(hwnd, 1, 1000, NULL);
	}
	if (symbol == '2' && IsLevelSelected == false) {
		TextFile.clear();
		wifstream File;
		File.open("MediumLevel.txt");
		while (getline(File, Line)) {
			TextFile += Line;
			TextFile += TEXT(" ");
		}
		File.close();
		_stprintf_s(Text, TEXT("%s"), TextFile.c_str());
		TextLength = TextFile.length();
		SetWindowText(hEditText, Text);
		IsLevelSelected = true;
		SetTimer(hwnd, 1, 1000, NULL);
	}
	if (symbol == '3' && IsLevelSelected == false) {
		TextFile.clear();
		wifstream File;
		File.open("HardLevel.txt");
		while (getline(File, Line)) {
			TextFile += Line;
			TextFile += TEXT(" ");
		}
		File.close();
		_stprintf_s(Text, TEXT("%s"), TextFile.c_str());
		TextLength = TextFile.length();
		SetWindowText(hEditText, Text);
		IsLevelSelected = true;
		SetTimer(hwnd, 1, 1000, NULL);
	}
	if (symbol == Text[CurrentLetter] && IsLevelSelected == true) {
		Input += symbol;
		SetWindowText(hEditInput, Input.c_str());
		SendMessage(hPic, STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[0]));
		++CorrectLetter;
		++CurrentLetter;
		if (CurrentLetter == TextLength) {
			KillTimer(hwnd, 1);
			_stprintf_s(str, TEXT("Correct letters=%d Wrong letters=%d"), CorrectLetter, WrongLetter);
			SetWindowText(hTimer, str);
			SetWindowText(hEditText, TEXT("Level is over"));
			SetWindowText(hEditInput, TEXT(" "));
			Input.clear();
			CurrentLetter = 0;
			CorrectLetter = 0;
			WrongLetter = 0;
			IsLevelSelected = false;
			SendMessage(hPic, STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[2]));
			MessageBox(0, str, TEXT("Results"), MB_OK | MB_ICONINFORMATION);
		}
	}
	else if (symbol != Text[CurrentLetter] && IsLevelSelected == true) {
		SendMessage(hPic, STM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(hBmp[1]));
		++WrongLetter;
	}

}
//taskkill /F /IM winapi_examproj.exe