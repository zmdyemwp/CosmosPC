#include "stdafx.h"
#include "CosmosReadCommProc.h"

#include "EnvSettings.h"

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

#ifdef		__COSMOS_MIN_DOWNLOAD_PROC__
	static unsigned char cmdTag[] = {0xaa, 0xbb, 0xcc, 0x02, 0x00};
#else
	static unsigned char cmdTag[][16] = {
		"CHECK",
		"FCHECK",
		"FSIZE",
		"FREAD",
	};
	DWORD getTagIndex(char* buf) {
		DWORD index = 0;
		for(index = 0; index < dim(cmdTag); index++) {
			if(NULL != strstr(buf, (char*)cmdTag[index])) {
				break;
			}
		}
		return index;
	}
#endif		//__COSMOS_MIN_DOWNLOAD_PROC__


void CosmosReadCommProc::proc(char* buf,DWORD len) {

	DWORD w = 0;
	buf[len] = '\0';


#ifdef		__COSMOS_MIN_DOWNLOAD_PROC__
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
#else


	char * p = strstr(buf, ",");
	DWORD index = getTagIndex(buf);
	wsprintf(msg, L"[%d/%d] -- (%d)", index, dim(cmdTag), len);
	dmsg(msg);
	static DWORD errorCount = 0;
	/*if(0 == len) {
		errorCount++;
		if(10 < errorCount) {
			errorCount = 0;
			flag = 1;
		}
	} else */if(dim(cmdTag) > index && NULL != p) {
		errorCount = 0;
		switch(index) {
			case 0:
				//	Connection ACK
				//		NEVER happen HERE!
				break;
	
			case 1:
				//	File Existence
				dmsg(L"CosmosReadCommProc::proc::FCHECK");
				break;
	
			case 2:
				//	File Size
				dmsg(L"CosmosReadCommProc::proc::FSIZE");
				file_size = strtol(p+1, NULL, 10);
				break;
	
			case 3:
				//	File Read
				dmsg(L"CosmosReadCommProc::proc::FREAD");
				wsprintf(msg, L"%S", buf);
				dmsg(msg);
				total_recv = 0;
				file_size = strtol(p+1, NULL, 10);
				if(0 == file_size) {
					flag = 1;
				} else {
					flag = 2;
					InitFile(dwName);
				}
				break;
	
			default:
				dmsg(L"CosmosReadCommProc::proc::default");
				break;

		}
	} else {
		errorCount = 0;
		//	NO Tag Found: File Content
		if(2 == flag && WriteFile(hfile, buf, len, &w, NULL)) {
			total_recv += w;
			RATE = 100 * total_recv / file_size;
			if(total_recv >= file_size) {
				DeinitFile();
				flag = 3;
			}
			wsprintf(msg, L"%d", total_recv);
			dmsg(msg);
		}
		wsprintf(msg, L"ReadProc::flag::[%d]%d/%d", flag, total_recv, file_size);
		dmsg(msg);
	}


#endif		//__COSMOS_MIN_DOWNLOAD_PROC__


}

