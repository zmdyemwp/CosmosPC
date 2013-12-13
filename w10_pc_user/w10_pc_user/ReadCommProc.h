#ifndef		__READ_COMM_PROC_H__
#define		__READ_COMM_PROC_H__

class ReadCommProc {
	public:
		//virtual ~ReadCommProc(){};
		virtual void InitFile(DWORD)=0;
		virtual void InitFile(unsigned char*)=0;
		virtual void DeinitFile()=0;
		virtual void proc(char*,DWORD)=0;
};

#endif		//__READ_COMM_PROC_H__

