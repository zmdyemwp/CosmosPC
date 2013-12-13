/*
 *  project: 
 *           通用模組 ( 用 c++ 處理  mysql 資料庫類，像ADO )
 *                  
 *  description:
 *
 *           通過DataBase,RecordSet,Record,Field類,實現對mysql資料庫的操作
 *    包括連接、修改、添加、刪除、查詢等等，像ADO一樣操作資料庫，使用方便
 *
 *          ( the end of this file have one sample, welcom to use... )
 *
 *
 *  file:zlb_mysql.h
 *
 *  author: @ zlb
 *  
 *  time:2005-12-12  
 *
 *
 *  
 --*/


#ifndef ZLB_MYSQL_H
#define ZLB_MYSQL_H

#include <winsock2.h>
#include "mysql.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace zlb_mysql
{
	//字段操作 
 
 	class Field
 	{
 		public :
  			/* 字段名稱 */
  			vector<string> m_name;
  			/* 字段類型 */
  			vector<enum_field_types> m_type;
 		public :
  			Field();
  			~Field();
  
  
  		/* 是否是數字 */
  		bool IsNum(int num);
  		/* 是否是數字 */
  		bool IsNum(string num);
  		/* 是否是日期 */
  		bool IsDate(int num);
  		/* 是否是日期 */
  		bool IsDate(string num);
  		/* 是否是字符 */
 		bool IsChar(int num);
  		/* 是否是字符 */
  		bool IsChar(string num);
  		/* 是否為二進制數據 */
  		bool IsBlob(int num);
  		/* 是否為二進制數據 */
  		bool IsBlob(string num);
  		/* 得到指定字段的序? */
  		int GetField_NO(string field_name);
 	};
 
 
 
 
 	/*
   		1. 單條記錄
   		2. [int ]操作 [""]操作
 	*/
 	class Record
 	{
 		public:
  			/* 結果集 */
  			vector<string> m_rs;
  			/* 字段信息 占用4字節的內存 當記錄數很大時會產生性能問題 */
  			Field *m_field;
 		public :
 			Record(){};
  			Record(Field* m_f);
  			~Record();
  
  
  		void SetData(string value);
  		/* [""]操作 */
  		string operator[](string s);
  		string operator[](int num);
  		/* null值判斷 */
  		bool IsNull(int num);
  		bool IsNull(string s);
  		/* 用 value tab value 的形式 返回結果 */
  		string GetTabText();
 	};
 
 
 
 
 	/*
   		1. 記錄集合
   		2. [int ]操作 [""]操作
   		3. 表結構操作
   		4. 數據的插入修改
 	*/
 	class RecordSet
 	{
 		private :
  			/* 記錄集 */
  			vector<Record> m_s;
  			/* 游標位置*/
  			unsigned long pos;
  			/* 記錄數 */
  			int m_recordcount;
  			/* 字段數 */
  			int m_field_num;
  			/* 字段信息 */
 	 		Field  m_field;
  
  			MYSQL_RES * res ;
  			MYSQL_FIELD * fd ;
  			MYSQL_ROW row;
			MYSQL* m_Data ;
  			
  			
 		public :
			RecordSet();
  			RecordSet(MYSQL *hSQL);
  			~RecordSet();
  
  		/* 處理返回多行的查詢，返回影響的行數 */
  		int ExecuteSQL(const char *SQL, bool flag);
  		/* 得到記錄數目 */
  		int GetRecordCount();
  		/* 得到字段數目 */
  		int GetFieldNum();
  		/* 向下移動游標 */
  		long MoveNext();
  		/* 移動游標 */
  		long Move(long length);
  		/* 移動游標到開始位置 */
  		bool MoveFirst();
  		/* 移動游標到結束位置 */
  		bool MoveLast();
  		/* 獲取當前游標位置 */
  		unsigned long GetCurrentPos()const;
  		/* 獲取當前游標的對應字段數据 */
  		bool GetCurrentFieldValue(const char * sFieldName,char *sValue);
  		bool GetCurrentFieldValue(const int iFieldNum,char *sValue);
  		/* 獲取游標的對應字段數据 */
  		bool GetFieldValue(long index,const char * sFieldName,char *sValue);
  		bool GetFieldValue(long index,int iFieldNum,char *sValue);
  		/* 是否到達游標尾部 */
  		bool IsEof();

  

  		/* 返回字段 */
  		Field* GetField();
  		/* 返回字段名 */
  		const char * GetFieldName(int iNum);
  		/* 返回字段類型 */
  		const int GetFieldType(char * sName);
  		const int GetFieldType(int iNum);
  		/* 返回指定序號的記錄*/
  		Record operator[](int num);
 	};
 
 	/*
  		1. 負責資料庫的連接關閉
   		2. 執行sql 語句(不返回結果)
   		3. 處理事務
 	*/
 	class DataBase
 	{ 
 		public :
  			DataBase(); 
  			~DataBase();
 		private :
  			/* msyql 連接句柄 */
  			MYSQL* m_Data;
 		public :
  			/* 返回句柄 */
  			MYSQL * GetMysql();
  			/* 連接資料庫 */
  			int Connect(string host, string user, 
   			string passwd, string db,
   			unsigned int port, 
   			unsigned long client_flag);
  
  		/* 關閉資料庫連接 */
  		void DisConnect();
  		/* 執行非返回結果查詢 */
  		int ExecQuery(string sql);
  		/* 測試mysql伺服器是否存活 */
  		int Ping();
  		/* 關閉mysql 伺服器 */
  		int ShutDown();
  		/* 主要功能:重新啟動mysql 伺服器 */
  		int ReBoot();
  	
  		//說明:事務支持InnoDB or BDB表類型
  		/* 主要功能:開始事務 */
  		int Start_Transaction();
  		/* 主要功能:提交事務 */
  		int Commit();
  		/* 主要功能:回滾事務 */
  		int Rollback();
  		/* 得到客戶信息 */
  		const char * Get_client_info();
  		/* 主要功能:得到客戶版本信息 */
  		const unsigned long  Get_client_version();
  		/* 主要功能:得到主机信息 */
  		const char * Get_host_info();
  		/* 主要功能:得到伺服器信息 */
  		const char * Get_server_info();
  		/*主要功能:得到伺服器版本信息*/
  		const unsigned long  Get_server_version();
  		/*主要功能:得到當前連接的默認字符集*/
  		const char *  Get_character_set_name();
  
 		/* 主要功能返回單值查詢  */
  		char * ExecQueryGetSingValue(string sql);
  
  		/* 得到系統時間 */
  		const char * GetSysTime();
  		/* 建立新資料庫 */
  		int Create_db(string name);
  		/* 刪除制定的資料庫*/
  		int Drop_db(string name);
	};
    
};

#endif //ZLB_MYSQL_H
