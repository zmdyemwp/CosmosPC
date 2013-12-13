#include "stdafx.h"
#include "CosmosReadCommProc.h"

CosmosReadCommProc::CosmosReadCommProc() {
	hfile = NULL;
	file_size = 0;
	flag = 0;
	szLastFile[0] = '\0';
}

CosmosReadCommProc::~CosmosReadCommProc() {
	DeinitFile();
}

void CosmosReadCommProc::InitFile(DWORD name) {
	if(hfile) {
		CloseHandle(hfile);
		hfile = NULL;
	}
	TCHAR szName[512] = {0};
	GetCurrentDirectory(512, szName);
	wcscat_s(szName, 512, SZ_FOLDER_NAME);
	//dmsg(szName, 1);
	CreateDirectory(szName, NULL);
	wsprintf(szName+wcslen(szName), L"\\%06d.tmp", name);
	wcscpy_s(szLastFile, 512, szName);
	hfile = CreateFile(szName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	dmsg(szName, 1);
}
void CosmosReadCommProc::InitFile(unsigned char * name) {
	char n[7];
	memset(n, 0, sizeof(n));
	memcpy(n, name, 6);
	char * p = n;
	while(p < (n+6) && '0' == *p) {
		p++;
	}
	DWORD i = strtol(p, NULL, 10);
	InitFile(i);
}

void CosmosReadCommProc::DeinitFile() {
	if(NULL != hfile) {
		CloseHandle(hfile);
		hfile = NULL;
	}
}

void CosmosReadCommProc::DeleteLastFile(void) {
	dmsg(L"Delete the Last File");
	dmsg(szLastFile);
	DeinitFile();
	DeleteFile(szLastFile);
}

#define		dim(x)	sizeof(x)/sizeof(x[0])
static unsigned char cmdTag[] = {0xaa, 0xbb, 0xcc, 0x02, 0x00};
void CosmosReadCommProc::proc(char* buf,DWORD len) {
	if(0) {
		TCHAR buffer[1024] = {0};
		for(DWORD i = 0; i < len; i++) {
			wsprintf(buffer+wcslen(buffer), L"%02x, ", (unsigned char)buf[i]);
		}
		dmsg(buffer, 1);
	}
	DWORD w = 0;
	char * ptag = strstr(buf, (char*)cmdTag);										//	use str api is OK?
	if(ptag) {
		flag = 2;
		InitFile((unsigned char*)(ptag+4));
		memcpy(&file_size, ptag + 4 + 6, sizeof(int));
		dmsg(L"strstr(ptag)::FOUND", 1);
		total_recv = 0;
	} else {
		dmsg(L"strstr(ptag)::NOT found", 1);
		if(WriteFile(hfile, buf, len, &w, NULL)) {
			total_recv += w;
			RATE = 100 * total_recv / file_size;
			if(total_recv >= file_size) {
				DeinitFile();
				flag = 3;
			}
		}
	}
}