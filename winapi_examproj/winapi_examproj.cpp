#include "TypingTutor.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{

	TypingTutor dlg1;
	MSG msg;
	HWND hTypingTutor = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TYPINGTUTOR), NULL, (DLGPROC)TypingTutor::DlgProc);
	ShowWindow(hTypingTutor, nCmdShow); 
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
