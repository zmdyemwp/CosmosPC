#ifndef		__COM_IO_H__
#define		__COM_IO_H__

#include "CosmosReadCommProc.h"

typedef void (*READPROC)(char*,DWORD);

class ComIO {
	public:
		~ComIO();
		void InitCS();
		ComIO(DWORD com);
		ComIO(TCHAR* com);
		BOOL InitComIO(BOOL bSingle = FALSE);
		BOOL DeinitComIO(void);
		DWORD ReadCom(char*, DWORD);
		DWORD WriteCom(char*, DWORD);
		void cmdEngine(TCHAR*, DWORD);		//	send event according to the command received.
		HANDLE getHandle(void) {
			return hCom;
		}
		void RegReadProc(READPROC p) {
			proc = p;
		}
		void RegReadClass(ReadCommProc * p) {
			objproc = p;
		}
	private:
		static CRITICAL_SECTION cs;
	protected:
		void SendAck(void);
		static void ReadThread(ComIO* pobj);
	protected:
		TCHAR szDevName[256];
		HANDLE hCom;
		HANDLE hReadThread;
		READPROC proc;
		ReadCommProc * objproc;
	protected:
		static const int eventCount = 1;
		static TCHAR szEvent[][64];
		unsigned char ack[64];
};

#endif		//__COM_IO_H__

