#include "stdafx.h"
#include "MainProcFunc.h"
#include "stdio.h"

#include "Storage.h"


//#define		__DIRECTLY_SQL__
#ifdef		__DIRECTLY_SQL__
#include "zlb_mysql.h"
#pragma comment(lib, "libmysql.lib")
void writeSQL(char * sql) {
	zlb_mysql::DataBase zlb;
	if(-1 == zlb.Connect("192.168.1.189", "root","henry", "watch", 0, 0)) {
		dmsg(L"Connect to DB FAIL!", 1);
		return;
	}// zlb_mysql::RecordSet rs(zlb.GetMysql());
	zlb.ExecQuery(sql);
}
#endif		//__DIRECTLY_SQL__


static TCHAR msg[512] = {0};

void doParseFile(DWORD i) {
	wsprintf(msg, L"\tdoParseFile(%d)", i);
	dmsg(msg);
	unsigned char buffer[512];
	char wbuffer[2048+1] = {0};
	char mode;
	TCHAR szName[512] = {0};
	GetCurrentDirectory(512, szName);
	wcscat_s(szName, 512, L"\\cosmos_spp_temp");
	TCHAR * pszName = szName+wcslen(szName);
	HANDLE hfile = NULL;
	HANDLE hdestfile = NULL;
	DWORD w = 0;
	
	wsprintf(pszName, L"\\%06d.tmp", i);
	hfile = CreateFile(szName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(INVALID_HANDLE_VALUE == hfile) {
		CloseHandle(hfile);
		return;
	}

	//		Read Head
	if(ReadFile(hfile, buffer, sizeof(STORAGE_HEAD), &w, NULL)) {
		if(w < sizeof(STORAGE_HEAD)) {
			CloseHandle(hfile);
			return;
		}
		STORAGE_HEAD & data = *(STORAGE_HEAD*)buffer;
		//	Create dest file
		wsprintf(pszName, L"\\%d_%d.cosmos", (data.Year*10000+data.Month*100+data.Day), i);
		hdestfile = CreateFile(szName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hdestfile) {
			CloseHandle(hfile);
			return;
		}
		//	SQL insert summary
		if(1 == data.Mode) mode = 'R';
		else if(2 == data.Mode) mode = 'B';
		else if(3 == data.Mode) mode = 'S';
		else if(4 == data.Mode) mode = 'P';
		else mode = 'O';
		//	select record to check data existance
		sprintf_s(wbuffer, 2048, "sn = '%d' and date = '%d' and mode = '%c' and time = '%d' and distance = '%f'\r\n",
					i, (data.Year*10000+data.Month*100+data.Day), mode, (data.TotalTick/1000), data.Distance);
		WriteFile(hdestfile, wbuffer, strlen(wbuffer), &w, NULL);
		//	insert summary
		sprintf_s(wbuffer, 2048, "'%d','%d','%c','%d','%d'\r\n",
			i, (data.Year*10000+data.Month*100+data.Day), mode, (data.TotalTick/1000), data.Distance);
		WriteFile(hdestfile, wbuffer, strlen(wbuffer), &w, NULL);


		#ifdef		__DIRECTLY_SQL__
		//	insert summary
		sprintf_s(wbuffer, 2048, "insert into summary values('min','%d','%d','%c','%d','%d','100')\0",
				i, (data.Year*10000+data.Month*100+data.Day), mode, (data.TotalTick/1000), data.Distance);
		writeSQL(wbuffer);
		#endif		//__DIRECTLY_SQL__
		
	} else {
		CloseHandle(hfile);
		CloseHandle(hdestfile);
		return;
	}
	//		Read Body
	while(ReadFile(hfile, buffer, 1, &w, NULL) && w > 0) {
		{/*********************************************************************/
			TCHAR body[12] = {0};
			wsprintf(body, L"body:%02x", buffer[0]);
			dmsg(body);
		}/*********************************************************************/
		switch(buffer[0]) {
			case 0x10:
				ReadFile(hfile, buffer, sizeof(STORAGE_GPS), &w, NULL);
				//	GPS
				if(w < sizeof(STORAGE_HEARTBEAT)) {
					w = 0;
				} else {
					STORAGE_GPS& data = *(STORAGE_GPS*)buffer;
					if(0 != data.Lat || 0 != data.Long) {
						sprintf_s(wbuffer, 2048, "'%d','%c','%f','%f'\r\n",
							i, mode, data.Lat, data.Long);
						WriteFile(hdestfile, wbuffer, strlen(wbuffer), &w, NULL);
						#ifdef		__DIRECTLY_SQL__
						//	insert GPS
						sprintf_s(wbuffer, 1024, "insert into gps values('min','%d','%c','%f','%f')",
								i, mode, data.Lat, data.Long);
						writeSQL(wbuffer);
						#endif		//__DIRECTLY_SQL__
					}
				}
				break;
			case 0x20:
				ReadFile(hfile, buffer, sizeof(STORAGE_HEARTBEAT), &w, NULL);
				if(w < sizeof(STORAGE_HEARTBEAT))
					w = 0;
				break;
			case 0x21:
				ReadFile(hfile, buffer, sizeof(STORAGE_PEDOMETER), &w, NULL);
				if(w < sizeof(STORAGE_PEDOMETER))
					w = 0;
				break;
			case 0x22:
				ReadFile(hfile, buffer, sizeof(STORAGE_BIKE), &w, NULL);
				if(w < sizeof(STORAGE_BIKE))
					w = 0;
				break;
			case 0x30:
				ReadFile(hfile, buffer, sizeof(STORAGE_BAROMETER), &w, NULL);
				if(w < sizeof(STORAGE_BAROMETER))
					w = 0;
				break;
			case 0x31:
				ReadFile(hfile, buffer, sizeof(STORAGE_TEMPERATURE), &w, NULL);
				if(w < sizeof(STORAGE_TEMPERATURE))
					w = 0;
				break;
			case 0x82:
				dmsg(L"lap");
				ReadFile(hfile, buffer, sizeof(STORAGE_LAP), &w, NULL);
				/*
				if(w < sizeof(STORAGE_LAP))
					w = 0;
				{
					char sql[128] = {0};
					sprintf(sql, "insert into member values('min','min','%d','X','25.123456','128.654321')", i); 
					writeMember(sql);
				}
				*/
				break;
			default:
				break;
			}
		if(0 == w)
			break;
	}

	CloseHandle(hfile);
	CloseHandle(hdestfile);
}