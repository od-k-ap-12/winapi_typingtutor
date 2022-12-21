#include "LevelSelect.h"
#include "TypingTutor.h"
#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER

MainMenu* MainMenu::ptr = NULL;

MainMenu::MainMenu(void)
{
	ptr = this;
	pNID = new NOTIFYICONDATA;
}

MainMenu::~MainMenu(void)
{
	delete pNID;
}

void MainMenu::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL MainMenu::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE;
}

void MainMenu::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_EASY)
	{
		EndDialog(hwnd, IDC_EASY);
	}
	else if (id == IDC_MEDIUM) {
		EndDialog(hwnd, IDC_MEDIUM);
	}
	else if (id == IDC_HARD) {
		EndDialog(hwnd, IDC_HARD);
	}
}

BOOL CALLBACK MainMenu::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}