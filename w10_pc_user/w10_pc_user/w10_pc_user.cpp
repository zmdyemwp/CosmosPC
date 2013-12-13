// w10_pc_user.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "w10_pc_user.h"
#include "MainProcFunc.h"
#include "Commctrl.h"

HWND g_hdlg = NULL;
HWND g_hdlg_bt_init = NULL;
CRITICAL_SECTION main_cs;

INT_PTR CALLBACK	dlgMainProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	dlgBtInit(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	InitializeCriticalSection(&main_cs);
	dmsg(L"IDD_BT_INIT");
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_BT_INIT), NULL, dlgBtInit);
	dmsg(L"IDD_MAIN");
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, dlgMainProc);
	DeleteCriticalSection(&main_cs);

	return 0;
}



INT_PTR CALLBACK dlgMainProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			dmsg(L"dlgMainProc()::WM_INITDIALOG");
			g_hdlg = hDlg;
			for(DWORD i = 0; i < 20; i++) {
				TCHAR temp[8] = {0};
				wsprintf(temp, L"COM%d", i);
				SendDlgItemMessage(hDlg, IDC_COMM, CB_INSERTSTRING, -1, (LPARAM)temp);
			}
			{
				TCHAR temp[8] = {0};
				wsprintf(temp, L"COM%d", checkComm());
				SetDlgItemText(hDlg, IDC_COMM, temp);
			}
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
			case IDOK:
			case IDCANCEL:
				StopSync();
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;

			case IDC_SYNC:
				//doSync();
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)doSync, NULL, 0, NULL);
				return TRUE;

			case IDC_STOP:
				StopSync();
				Refresh();
				return TRUE;

			default:
				break;
		}
		break;
	case WM_UI_REFRESH:
		Refresh();
		return TRUE;

	default:
		break;
	}
	return (INT_PTR)FALSE;
}

void CheckAll(void) {
	for(DWORD i = 0; i < 100; i++) {
		checkComm(i);
		SendDlgItemMessage(g_hdlg_bt_init, IDC_BT_INIT_BAR, PBM_STEPIT, 0, 0);
		Sleep(10);
	}
	SendMessage(g_hdlg_bt_init, WM_CLOSE, 0, 0);
}
INT_PTR CALLBACK dlgBtInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_INITDIALOG:
			{
				dmsg(L"dlgBtInit()::WM_INITDIALOG");
				g_hdlg_bt_init = hDlg;
				SendDlgItemMessage(hDlg, IDC_BT_INIT_BAR, PBM_SETSTEP, 1, 0);
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckAll, NULL, 0, NULL);
			}
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
				default:
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}


