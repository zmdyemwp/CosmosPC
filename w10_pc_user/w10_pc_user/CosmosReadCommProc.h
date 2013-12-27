#ifndef		__COSMOS_READ_COMM_PORC_H__
#define		__COSMOS_READ_COMM_PORC_H__

#include "stdafx.h"
#include "ReadCommProc.h"

#define		SZ_FOLDER_NAME		L"\\cosmos_spp_temp"

extern DWORD RATE;

class CosmosReadCommProc:public ReadCommProc {
	public:
		CosmosReadCommProc();
		~CosmosReadCommProc();
		void InitFile(DWORD);
		void InitFile(unsigned char*);
		void DeinitFile();
		void DeleteLastFile(void);
		void proc(char*,DWORD);
		DWORD flag;		//	{0, 1, 2, 3 | 0: before request; 1: before writing; 2: writing; 3: finish writing}
		DWORD dwName;
	protected:
		TCHAR szLastFile[512];
		HANDLE hfile;
		DWORD file_size;
		DWORD total_recv;
};

#endif		//__COSMOS_READ_COMM_PORC_H__

