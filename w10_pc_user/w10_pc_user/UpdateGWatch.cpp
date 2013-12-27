#include "stdafx.h"
#include "stdio.h"
#include "UpdateGWatch.h"
#include "modem/ymodem.h"

class BCB2WIN;

extern TCHAR* getCommPort();
extern BCB2WIN * Form1;
char Image_data[0x200000];

void doUpdate(TCHAR* szFileName) {
	//	Read Bin File into MetaBuffer
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {
		return;
	}
	DWORD ImageLen = GetFileSize(hFile, NULL);
	if(ImageLen > USER_FLASH_SIZE) {
		CloseHandle(hFile);
		return;
	}
	DWORD len = 0;
	DWORD readlen = 0;
	for(len = 0; len < ImageLen;) {
		readlen = ImageLen - len;
		if( ! ReadFile(hFile, Image_data+len, readlen, &readlen, NULL) || 0 == readlen) {
			CloseHandle(hFile);
			return;
		}
		len += readlen;
	}
	CloseHandle(hFile);
	//	Update
	Form1->Init(getCommPort());
	char		buffer[256];
	DWORD		 in_length;
	DWORD		out_length;
	DWORD				 i;
	DWORD		total_length;
	buffer[0] = 'a';
	buffer[1] = '0';
	in_length = 2;
	Form1->ShowTestMessage("OPEN %s .....\r\n", getCommPort());
	
	if(!Form1->UART_WRITE(buffer,in_length,&out_length)){
		Form1->ShowTestMessage("WRITE %s ERROR\r\n", getCommPort());
		Form1->UART_CLOSE();
		return;
	}
	total_length = 0;
	for(i=0;i<20;i++){
		memset(buffer,0,256);
		in_length = 128;
		if(!Form1->UART_READ(buffer,in_length,&out_length)){
			Form1->ShowTestMessage("READ %s ERROR\r\n", getCommPort());
			Form1->UART_CLOSE();
			return;
		}
		Form1->ShowTestMessage("%s",buffer);
		total_length+=out_length;
		Sleep(1);
	}
	if(!total_length){
		Form1->ShowTestMessage("DEVICE NO RESPONDSE\r\n");
		Form1->UART_CLOSE();
		return;
	}
	buffer[0] = '1';
	in_length = 1;
	if(!Form1->UART_WRITE(buffer,in_length,&out_length)){
		Form1->ShowTestMessage("WRITE %s ERROR\r\n", getCommPort());
		Form1->UART_CLOSE();
		return;
	}
	total_length = 0;
	for(i=0;i<20;i++){
		memset(buffer,0,256);
		in_length = 128;
		if(!Form1->UART_READ(buffer,in_length,&out_length)){
			Form1->ShowTestMessage("READ %s ERROR\r\n", getCommPort());
			Form1->UART_CLOSE();
			return;
		}
		Form1->ShowTestMessage("%s",buffer);
		total_length+=out_length;
		Sleep(1);
	}
	if(!total_length){
		Form1->ShowTestMessage("DEVICE NO RESPONDSE\r\n");
		Form1->UART_CLOSE();
		return;
	}
	char filename[MAX_PATH+1] = {0};
	sprintf(filename, "%S", szFileName);
	if(!Ymodem_Transmit((uint8_t*)Image_data,(const uint8_t*)filename,ImageLen)){
		for(i=0;i<20;i++){
			memset(buffer,0,256);
			in_length = 128;
			if(!Form1->UART_READ(buffer,in_length,&out_length)){
				Form1->ShowTestMessage("READ %s ERROR\r\n", getCommPort());
				Form1->UART_CLOSE();
				return;
			}
			Form1->ShowTestMessage("%s",buffer);
			total_length+=out_length;
			Sleep(1);
		}
	}
	buffer[0] = '3';
	in_length = 1;
	if(!Form1->UART_WRITE(buffer,in_length,&out_length)){
		Form1->ShowTestMessage("WRITE %s ERROR\r\n", getCommPort());
		Form1->UART_CLOSE();
		return;
	}
	for(i=0;i<30;i++){
		memset(buffer,0,256);
		in_length = 128;
		if(!Form1->UART_READ(buffer,in_length,&out_length)){
			Form1->ShowTestMessage("READ %s ERROR\r\n", getCommPort());
			Form1->UART_CLOSE();
			return;
		}
		Form1->ShowTestMessage("%s",buffer);
		total_length+=out_length;
		Sleep(1);
	}
	Form1->UART_CLOSE();
}

