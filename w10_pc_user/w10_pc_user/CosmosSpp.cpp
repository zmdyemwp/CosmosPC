#include "stdafx.h"
#include "CosmosSpp.h"
#include "stdio.h"

void CosmosSpp::RequestFrom(CosmosSpp& obj, DWORD i, CosmosReadCommProc * p) {
	//	SN start from 000001
	unsigned char buffer[256] = {0xaa, 0xbb, 0xcc, 0x01, 0x00};
	do
	{
		p->flag = 0;
		sprintf_s((char*)(buffer+4), 252, "%06d", i);
		obj.WriteCom((char*)buffer, 10);
		break;
		Sleep(500);
		while(1 == p->flag) {
			Sleep(50);
		}
		if(2 == p->flag) {
			i++;
		} else {
			break;
		}
	} while(1);
}

