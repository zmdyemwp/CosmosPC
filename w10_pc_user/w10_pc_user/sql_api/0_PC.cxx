// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zlb_mysql.h"
#include <time.h> 
#include <windows.h> 
#include <stdio.h> 
#include <winnetwk.h> 
#include <iostream>
#include <string>
#pragma comment(lib, "mpr") 
#pragma comment(lib, "libmysql.lib")
using namespace std; 

void WriteLogFile(string s);
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime); 

HWND hWnd;
DWORD dwIndex;
int g_index = 600;
int TimerIndex = 1;


void _tmain(int argc, _TCHAR* argv[])
{	
	string log="";
	WriteLogFile(log);

	//Get system time
	SYSTEMTIME iSysTime;
    GetLocalTime(&iSysTime);
	char szData[20] = {0};
	sprintf(szData, "Date: %d %02d %02d", iSysTime.wYear, iSysTime.wMonth, iSysTime.wDay); 
	WriteLogFile(szData);
	sprintf(szData, "Time: %d %02d %02d", iSysTime.wHour, iSysTime.wMinute, iSysTime.wSecond);   
	WriteLogFile(szData);
	if(iSysTime.wHour>9 && iSysTime.wHour<18)
	{
		cout<<"Now is work time"<<endl;
		log="Now is work time";
		WriteLogFile(log);
		return;
	}
	else
	{
		cout<<"Execute autoupdate"<<endl;
		log="Execute autoupdate";
		WriteLogFile(log);
	}

	
	hWnd=GetForegroundWindow();
	dwIndex = SetTimer(NULL, NULL, 1000, TimerProc);
	int value = MessageBox(hWnd, L"是否執行自動更新", L"自動更新程式", MB_YESNO | MB_ICONEXCLAMATION | MB_ICONWARNING);
	if(value == IDNO)
		return;
	else if(value == IDYES)
		KillTimer(NULL, dwIndex);

	//connect to server
	NETRESOURCE   nr; 
	DWORD res; 
	LPWSTR szUserName = L"henry_lin",//MyUserName 
		   szPassword = L"henry_lin",//MyPassword 
		   szLocalName = L"Q:", 
           szRemoteName = L"\\\\192.168.1.240"; 

	//Assign values to the NETRESOURCE structure.  
	nr.dwType = RESOURCETYPE_ANY; 
	nr.lpLocalName = NULL;//szLocalName; 
	nr.lpRemoteName = szRemoteName; 
	nr.lpProvider = NULL; 
	
	//Call the WNetAddConnection2 function to assign a drive letter to the share.  
	res = WNetAddConnection2(&nr, szPassword, szUserName, FALSE); 

	//If the call succeeds, inform the user; otherwise, print the error. 
	switch(res) 
	{ 
		case NO_ERROR: 
			cout<<"NO_ERROR"<<endl;;
			printf("Connection added %s\n", szRemoteName);
			log="Server connect success";
			WriteLogFile(log);
			break; 
		case ERROR_ACCESS_DENIED: 
			printf("Connection added %s ERROR_ACCESS_DENIED\n", szRemoteName); 
			log="Connection ERROR_ACCESS_DENIED";
			WriteLogFile(log);
			break; 
		case ERROR_ALREADY_ASSIGNED: 
			cout<<"Connection added ";
			cout<<szRemoteName;
			cout<<" ERROR_ALREADY_ASSIGNED"<<endl;
			printf("Connection added %s ERROR_ALREADY_ASSIGNED\n", szRemoteName); 
			log="Connection ERROR_ALREADY_ASSIGNED";
			WriteLogFile(log);
			break; 
		case ERROR_BAD_DEV_TYPE: 
			printf("Connection added %s ERROR_BAD_DEV_TYPE\n", szRemoteName); 
			log="Connection ERROR_BAD_DEV_TYPE";
			WriteLogFile(log);
			break; 
		case ERROR_BAD_DEVICE: 
			cout<<"Connection added ";
			cout<<szRemoteName;
			cout<<" ERROR_BAD_DEVICE"<<endl;
			printf("Connection added %s ERROR_BAD_DEVICE\n", szRemoteName); 
			log="Connection ERROR_BAD_DEVICE";
			WriteLogFile(log);
			break; 
		case ERROR_BAD_NET_NAME: 
			printf("Connection added %s ERROR_BAD_NET_NAME\n", szRemoteName); 
			log="Connection ERROR_BAD_NET_NAME";
			WriteLogFile(log);
			break; 
		case ERROR_BAD_PROFILE: 
			printf("Connection added %s ERROR_BAD_PROFILE\n", szRemoteName); 
			log="Connection ERROR_BAD_PROFILE";
			WriteLogFile(log);
			break; 
		case ERROR_BAD_PROVIDER: 
			printf("Connection added %s ERROR_BAD_PROVIDER\n", szRemoteName); 
			log="Connection ERROR_BAD_PROVIDER";
			WriteLogFile(log);
			break; 
		case ERROR_BUSY: 
			printf("Connection added %s ERROR_BUSY\n", szRemoteName); 
			log="Connection ERROR_BUSY";
			WriteLogFile(log);
			break; 
		case ERROR_CANCELLED: 
			printf("Connection added %s ERROR_CANCELLED\n", szRemoteName); 
			log="Connection ERROR_CANCELLED";
			WriteLogFile(log);
			break; 
		case ERROR_CANNOT_OPEN_PROFILE: 
			printf("Connection added %s ERROR_CANNOT_OPEN_PROFILE\n", szRemoteName); 
			log="Connection ERROR_CANNOT_OPEN_PROFILE";
			WriteLogFile(log);
			break; 
		case ERROR_DEVICE_ALREADY_REMEMBERED: 
			printf("Connection added %s ERROR_DEVICE_ALREADY_REMEMBERED\n", szRemoteName); 
			log="Connection ERROR_DEVICE_ALREADY_REMEMBERED";
			WriteLogFile(log);
			break; 
		case ERROR_EXTENDED_ERROR: 
		{ 
			printf("Connection added %s ERROR_EXTENDED_ERROR\n", szRemoteName); 
			DWORD dwWNetResult, dwLastError;   
			CHAR szDescription[256];   
			CHAR szProvider[256]; 
            dwWNetResult = WNetGetLastError(&dwLastError, //error code 
								   (LPTSTR) szDescription, //buffer for error description   
								   sizeof(szDescription), //size of error buffer 
									  (LPTSTR) szProvider, //buffer for provider name   
									 sizeof(szProvider)); //size of name buffer 
  
            //Process errors. 
            if(dwWNetResult != NO_ERROR)
			{   
				printf("WNetGetLastError failed; error %ld", dwWNetResult);   
				log="Connection WnetGetLastError failed";
				WriteLogFile(log);
                return;   
            }     
            
            //Otherwise, print the additional error information. 
            printf("%s failed with code %ld;\n%s", (LPSTR) szProvider, dwLastError, (LPSTR) szDescription); 
			log="Connection failed with code";
			WriteLogFile(log);
		} 
		break; 

		case ERROR_INVALID_PASSWORD: 
			printf("Connection added %s ERROR_INVALID_PASSWORD\n", szRemoteName); 
			log="Connection ERROR_INVALID_PASSWORD";
			WriteLogFile(log);
			break; 
		case ERROR_NO_NET_OR_BAD_PATH: 
			printf("Connection added %s ERROR_NO_NET_OR_BAD_PATH\n", szRemoteName); 
			log="Connection ERROR_NOT_NET_OR_BAD_PATH";
			WriteLogFile(log);
			break; 
		case ERROR_NO_NETWORK: 
			printf("Connection added %s ERROR_NO_NETWORK\n", szRemoteName); 
			log="Connection ERROR_NO_NETWORK";
			WriteLogFile(log);
			break; 
		default:
			cout<<"Other"<<endl;
			log="Connection other error";
			WriteLogFile(log);
			break;
	}

	//connect to database
	zlb_mysql::DataBase zlb;
 	if(-1 == zlb.Connect("192.168.1.21", "root","henry_lin", "autoupdate", 0, 0))
 	{
  		std::cout<<"Database connect failed "<<std::endl;
		log="Database connet failed";
		WriteLogFile(log);
 	}
 	else
 	{
  		std::cout<<"Database connect success"<<std::endl;
		log="Database connect success";
		WriteLogFile(log);
 	}
	
 	zlb_mysql::RecordSet rs(zlb.GetMysql());
	rs.ExecuteSQL("select machine from userinfo where name='Schumann'", 1);  //get machine
 	//display the record
	string str="";
	for(int i=0;i<rs.GetRecordCount();++i)
 	{
   		for(int j =0;j<rs.GetFieldNum();++j)
		{
			str+=rs[i][j];
			//cout<<rs[i][j];
		}
		cout<<"update the "<<str<<endl;
		//str="";
 	}
	zlb_mysql::RecordSet rs1(zlb.GetMysql());
	rs1.ExecuteSQL("select build from userinfo where name='Schumann'", 1);  //get build
	string str1="";
	for(int i=0;i<rs1.GetRecordCount();++i)
 	{
   		for(int j =0;j<rs1.GetFieldNum();++j)
		{
			str1+=rs1[i][j];
			//cout<<rs[i][j];
		}
		cout<<str1<<endl;
		//str1="";
 	}

	//copy file from server to PC
	string s1,s2,s3;
	s1="\\\\192.168.1.240\\UserSpaces\\Schumann\\";
	if(str1 == "Retail build")
		s2 = "\\ImageInstall_Retail.exe";
	else if(str1 == "Ship build")
		s2 = "\\ImageInstall_Ship.exe";
	else 
		s2 = "\\ImageInstall_Debug.exe";
	s3=s1+str+s2;
	cout<<s3<<endl;
	WriteLogFile(s3);
	//convert string to LPCWSTR
	int len;
	int slength = (int)s3.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s3.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s3.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	#ifdef UNICODE
	wstring stemp = r; // Temporary buffer is required
	LPCWSTR result = stemp.c_str();
	#else
	LPCWSTR result = s3.c_str();
	#endif

	//copy file from server
	bool flag = CopyFile(result, L"d:\\ImageInstall.exe ",false);  //(source path, target path, false)
	if(flag)
	{
		cout<<"Copy success"<<endl;
		log="Copy file success";
		WriteLogFile(log);
	}
	else
	{
		cout<<"Copy failed"<<endl;
		log="copy file failed";
		WriteLogFile(log);
	}

	//delete data from database 
	rs.ExecuteSQL("delete from userinfo where name='Schumann' && yesno='no'", 0);  //username shuold be modified
	
	/*int a;
g2:
	cout<<"Enter 0 to exit"<<endl;
	cin>>a;
	if(a==0)
		return;
	else
	    goto g2;*/

	//run .exe
	WinExec("D:\\ImageInstall.exe", SW_SHOW);

	//shutdown the computer
	system( "shutdown -s -t 900" );
	value = MessageBox(hWnd, L"是否於更新後自動關機", L"自動更新程式", MB_TOPMOST | MB_YESNO | MB_ICONEXCLAMATION | MB_ICONWARNING);
	if(value == IDNO)
		system("shutdown -a");
   
	return; 
}

void WriteLogFile(string s)
{
	FILE * pFile1; 
	pFile1 = fopen ( "D:\\log.txt" , "a" );
	if (pFile1 != NULL)
	{  
		char *log;
		log=new char[s.length()+1];
		strcpy(log, s.c_str());
		fprintf(pFile1, "%s\n", log);   
		fclose (pFile1);
	}
	return;
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	switch (uMsg) 
    {
		case WM_TIMER:
			{
				//wsprintf(szTime, L"%d", g_index);
				cout<<g_index<<endl;
				if (g_index > 0)
					g_index --;
				else						
				{
					KillTimer(NULL, dwIndex);
					DestroyWindow(FindWindow(NULL, L"自動更新程式"));
				}
			}
			break;
	}
}