#include "stdafx.h"
#include "MainProcFunc.h"
#include "Resource.h"
#include "Commctrl.h"

#include "CosmosSpp.h"
#include "CosmosReadCommProc.h"

#include "EnvSettings.h"

#include "stdio.h"

CosmosSpp * pcio = NULL;
CosmosReadCommProc rProc;

#ifdef		__COSMOS_MIN_DOWNLOAD_PROC__

	const unsigned char checkcmd[] = {0xaa,0xbb,0xcc,0xdd};
	const unsigned char checkack[] = {0xee,0xff,0xee,0xff,0x00};

#else

	const unsigned char checkcmd[] = "SPP+STOP=\r";
	const unsigned char checkack[] = "STOP,1\r";

#endif		//__COSMOS_MIN_DOWNLOAD_PROC__

TCHAR msg[1024] = {0};

DWORD checkComm(DWORD com) {
	static DWORD result = 0;
	if(max_com > com && 0 == result) {
		TCHAR szDevName[12] = {0};
		//wsprintf(szDevName, L"COM%d:", com);
		wsprintf(szDevName, L"\\\\.\\COM%d", com);
		HANDLE hCom = CreateFile(szDevName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hCom) {
			CloseHandle(hCom);
			return result;
		}
		//	Timeout Settings
		COMMTIMEOUTS timeout;
		timeout.ReadIntervalTimeout = 0;
		timeout.ReadTotalTimeoutConstant = 500;
		timeout.ReadTotalTimeoutMultiplier = 0;
		timeout.WriteTotalTimeoutMultiplier = 0;
		timeout.WriteTotalTimeoutConstant = 50;
		if( ! SetCommTimeouts(hCom, &timeout)) {
			wsprintf(msg, L"%s::SetCommTimeouts()::Fail...(%d)", szDevName, GetLastError());
			dmsg(msg);
			CloseHandle(hCom);
			return result;
		}
		//	Write & Read Com
		{
			DWORD read,write;
			read = write = 0;
			unsigned char buf[32+1] = {0};
			if( ! WriteFile(hCom, checkcmd, dim(checkcmd), &write, NULL)) {
				CloseHandle(hCom);
				return result;
			}
			Sleep(100);
			if( ! ReadFile(hCom, buf, 32, &read, NULL)) {
				CloseHandle(hCom);
				return result;
			}
			wsprintf(msg, L"ReadFile()::%d", read);
			dmsg(msg);
			buf[read] = '\0';
			if(0 == read) {
				dmsg(L"READ SIZE == 0");
			} else {
				msg[0] = '\0';
				for(DWORD f = 0; f < read; f++) {
					wsprintf(msg+wcslen(msg), L"%02x ", buf[f]);
				}	wsprintf(msg+wcslen(msg), L"\r\n");
				dmsg(msg);
			}
			if(NULL != strstr((char*)buf, (char*)checkack)) {
				dmsg(L"THIS IS IT!");
				result = com;
			} else {
				dmsg(L"STR NOT FOUND");
			}
		}
		CloseHandle(hCom);
	}
	return result;
}

BOOL checkFileExist(DWORD sn) {
	DWORD result = FALSE;
	TCHAR szName[512] = {0};
	GetCurrentDirectory(512, szName);
	wcscat_s(szName, 512, SZ_FOLDER_NAME);
	//dmsg(szName, 1);
	CreateDirectory(szName, NULL);
	wsprintf(szName+wcslen(szName), L"\\%06d.tmp", sn);
	HANDLE hfile = CreateFile(szName, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(INVALID_HANDLE_VALUE == hfile)
		result = FALSE;
	else
		result = TRUE;
	CloseHandle(hfile);
	return result;
}

TCHAR* getCommPort() {
	static TCHAR buffer[32] = L"\\\\.\\";
	GetDlgItemText(g_hdlg, IDC_COMM, buffer+wcslen(buffer), 28);
	TCHAR * ptr = wcsstr(buffer, L":");
	if(ptr) {
		*ptr = '\0';
	}
	return buffer;
}
void doSync(void) {
	//	this function check the local files and download the others from W10
	//	RUN AS THREAD for nonblocked UI
	//		connect
	static DWORD dotCount = 0;
	if(NULL == pcio) {
		/*
		TCHAR buffer[32] = L"\\\\.\\";
		TCHAR * ptr = wcsstr(buffer, L":");
		if(ptr) *ptr = '\0';
		GetDlgItemText(g_hdlg, IDC_COMM, buffer+wcslen(buffer), 28);
		*/
		TCHAR buffer[32] = {0};
		wsprintf(buffer, L"%s", getCommPort());
		dmsg(buffer);
		EnterCriticalSection(&main_cs);
		pcio = new CosmosSpp(buffer);
		if( ! pcio->InitComIO()) {
			doWriteMsg(L"Open Bluetooth com port fail...\r\n");
			delete pcio;
			pcio = NULL;
			LeaveCriticalSection(&main_cs);
			return;
		}
		doWriteMsg(L"Bluetooth com port opened\r\nand initialized...\r\n");
		pcio->RegReadClass((ReadCommProc*)&rProc);
		LeaveCriticalSection(&main_cs);
	}
	PostMessage(g_hdlg, WM_UI_REFRESH, 0, 0);
	//		start to sync with W10
	DWORD i = 1;
	dotCount = 0;

#ifdef		__COSMOS_MIN_DOWNLOAD_PROC__
	unsigned char buffer[256] = {0xaa, 0xbb, 0xcc, 0x01, 0x00};
#else
	unsigned char buffer[256] = "SPP+FREAD=";
#endif		//__COSMOS_MIN_DOWNLOAD_PROC__

	do
	{
		EnterCriticalSection(&main_cs);
		dmsg(L"doSync()::EnterCriticalSection");
		if(NULL == pcio) {
			LeaveCriticalSection(&main_cs);
			break;
		}
		if(0 == rProc.flag) {
			if( ! checkFileExist(i)) {
				rProc.flag = 1;
#ifdef		__COSMOS_MIN_DOWNLOAD_PROC__
				sprintf_s((char*)(buffer + 4), 252, "%06d\r\0", i);
#else
				sprintf_s((char*)(buffer + 10), 246, "%06d\r\0", i);
#endif		//__COSMOS_MIN_DOWNLOAD_PROC__

				wsprintf(msg, L"Downloading file: %06d\r\n", i);
				doWriteMsg(msg);
#ifdef		__COSMOS_MIN_DOWNLOAD_PROC__
				pcio->WriteCom((char*)buffer, 18);
#else
				rProc.dwName = i;
				Sleep(100);
				pcio->WriteCom((char*)buffer, 19);
#endif		//__COSMOS_MIN_DOWNLOAD_PROC__
				Sleep(500);
			} else {
				i++;
			}
		} else if(2 == rProc.flag) {
			Sleep(500);
		} else if(3 == rProc.flag) {
			//	parse the file just downloaded
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)doParseFile, (LPVOID)i, 0, NULL);
			wsprintf(msg, L"File downloading finished: %06d\r\n\r\n", i);
			i++;
			rProc.flag = 0;
			//wsprintf(msg, L"Get file: %S\r\n", buffer+4);
			doWriteMsg(msg);
		} else if(1 == rProc.flag) {
			doWriteMsg(L"File NOT Found\r\n");
			LeaveCriticalSection(&main_cs);
			break;
		} else {
			dmsg(L"Unknown Flag");
			LeaveCriticalSection(&main_cs);
			break;
		}
		SendDlgItemMessage(g_hdlg, IDC_DOWNLOAD, PBM_SETPOS, RATE, 0);
		LeaveCriticalSection(&main_cs);
		dmsg(L"doSync()::LeaveCriticalSection");
		Sleep(100);
	} while(i < 1000000);

	if(2 == rProc.flag) {
		//	file downloaded NOT completely
		rProc.DeleteLastFile();
	}
	//StopSync();
	rProc.flag = 0;
	dmsg(L"End of Sync");
}

void StopSync(void) {
	SetDlgItemText(g_hdlg, IDC_MSG, L"");
	EnterCriticalSection(&main_cs);
	dmsg(L"StopSync()::EnterCriticalSection");
	//rProc.flag = 0;
	if(NULL != pcio) {
		delete pcio;
		pcio = NULL;
	}
	LeaveCriticalSection(&main_cs);
	dmsg(L"StopSync()::LeaveCriticalSection");
}

void Refresh(void) {
	//EnterCriticalSection(&main_cs);
	if(NULL == pcio) {
		ShowWindow(GetDlgItem(g_hdlg, IDC_SYNC), SW_SHOW);
		ShowWindow(GetDlgItem(g_hdlg, IDC_STOP_SYNC), SW_HIDE);
	} else {
		ShowWindow(GetDlgItem(g_hdlg, IDC_SYNC), SW_HIDE);
		ShowWindow(GetDlgItem(g_hdlg, IDC_STOP_SYNC), SW_SHOW);
	}
	//LeaveCriticalSection(&main_cs);
}

/****************************************************************************************************************/
void doWriteMsg(TCHAR* msg) {
	SendDlgItemMessage(g_hdlg, IDC_MSG, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)msg);
}

void dmsg(TCHAR * msg, DWORD x)
{
	if(0 == x)
		return;

	TCHAR buffer[1024] = {0};
	wsprintf(buffer, L"[Cosmos]\t%s\r\n", msg);
	HWND hDlg = FindWindow(NULL, L"DebugMsg");
	if(IsWindow(hDlg))
	{
		SendDlgItemMessage(hDlg, 10101, EM_REPLACESEL, 0, (LPARAM)buffer);
	}
	else {
		//MessageBox(NULL, L"Window NOT Found", NULL, 0);
	}
}

DWORD RATE = 0;

