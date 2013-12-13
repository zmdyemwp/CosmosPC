#ifndef		__MAIN_PROC_FUNCTIONS_H__
#define		__MAIN_PROC_FUNCTIONS_H__

#include "Windows.h"

#define		WM_UI_REFRESH		WM_USER+1111

#define dim(x) sizeof(x)/sizeof(x[0])

extern HWND g_hdlg;
extern CRITICAL_SECTION main_cs;

void Refresh(void);
void doSync(void);
void StopSync(void);
const DWORD max_com = 100;
DWORD checkComm(DWORD com = max_com);
void doWriteMsg(TCHAR* msg);
void doParseFile(DWORD i);

#endif		//__MAIN_PROC_FUNCTIONS_H__

