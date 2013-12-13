/*
 *  project: 
 *           �q�μҲ� ( �� c++ �B�z  mysql ��Ʈw���A��ADO )
 *                  
 *  description:
 *
 *           �q�LDataBase,RecordSet,Record,Field��,��{��mysql��Ʈw���ާ@
 *    �]�A�s���B�ק�B�K�[�B�R���B�d�ߵ����A��ADO�@�˾ާ@��Ʈw�A�ϥΤ�K
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
	//�r�q�ާ@ 
 
 	class Field
 	{
 		public :
  			/* �r�q�W�� */
  			vector<string> m_name;
  			/* �r�q���� */
  			vector<enum_field_types> m_type;
 		public :
  			Field();
  			~Field();
  
  
  		/* �O�_�O�Ʀr */
  		bool IsNum(int num);
  		/* �O�_�O�Ʀr */
  		bool IsNum(string num);
  		/* �O�_�O��� */
  		bool IsDate(int num);
  		/* �O�_�O��� */
  		bool IsDate(string num);
  		/* �O�_�O�r�� */
 		bool IsChar(int num);
  		/* �O�_�O�r�� */
  		bool IsChar(string num);
  		/* �O�_���G�i��ƾ� */
  		bool IsBlob(int num);
  		/* �O�_���G�i��ƾ� */
  		bool IsBlob(string num);
  		/* �o����w�r�q����? */
  		int GetField_NO(string field_name);
 	};
 
 
 
 
 	/*
   		1. ����O��
   		2. [int ]�ާ@ [""]�ާ@
 	*/
 	class Record
 	{
 		public:
  			/* ���G�� */
  			vector<string> m_rs;
  			/* �r�q�H�� �e��4�r�`�����s ��O���ƫܤj�ɷ|���ͩʯ���D */
  			Field *m_field;
 		public :
 			Record(){};
  			Record(Field* m_f);
  			~Record();
  
  
  		void SetData(string value);
  		/* [""]�ާ@ */
  		string operator[](string s);
  		string operator[](int num);
  		/* null�ȧP�_ */
  		bool IsNull(int num);
  		bool IsNull(string s);
  		/* �� value tab value ���Φ� ��^���G */
  		string GetTabText();
 	};
 
 
 
 
 	/*
   		1. �O�����X
   		2. [int ]�ާ@ [""]�ާ@
   		3. ���c�ާ@
   		4. �ƾڪ����J�ק�
 	*/
 	class RecordSet
 	{
 		private :
  			/* �O���� */
  			vector<Record> m_s;
  			/* ��Ц�m*/
  			unsigned long pos;
  			/* �O���� */
  			int m_recordcount;
  			/* �r�q�� */
  			int m_field_num;
  			/* �r�q�H�� */
 	 		Field  m_field;
  
  			MYSQL_RES * res ;
  			MYSQL_FIELD * fd ;
  			MYSQL_ROW row;
			MYSQL* m_Data ;
  			
  			
 		public :
			RecordSet();
  			RecordSet(MYSQL *hSQL);
  			~RecordSet();
  
  		/* �B�z��^�h�檺�d�ߡA��^�v�T����� */
  		int ExecuteSQL(const char *SQL, bool flag);
  		/* �o��O���ƥ� */
  		int GetRecordCount();
  		/* �o��r�q�ƥ� */
  		int GetFieldNum();
  		/* �V�U���ʴ�� */
  		long MoveNext();
  		/* ���ʴ�� */
  		long Move(long length);
  		/* ���ʴ�Ш�}�l��m */
  		bool MoveFirst();
  		/* ���ʴ�Ш쵲����m */
  		bool MoveLast();
  		/* �����e��Ц�m */
  		unsigned long GetCurrentPos()const;
  		/* �����e��Ъ������r�q���u */
  		bool GetCurrentFieldValue(const char * sFieldName,char *sValue);
  		bool GetCurrentFieldValue(const int iFieldNum,char *sValue);
  		/* �����Ъ������r�q���u */
  		bool GetFieldValue(long index,const char * sFieldName,char *sValue);
  		bool GetFieldValue(long index,int iFieldNum,char *sValue);
  		/* �O�_��F��Ч��� */
  		bool IsEof();

  

  		/* ��^�r�q */
  		Field* GetField();
  		/* ��^�r�q�W */
  		const char * GetFieldName(int iNum);
  		/* ��^�r�q���� */
  		const int GetFieldType(char * sName);
  		const int GetFieldType(int iNum);
  		/* ��^���w�Ǹ����O��*/
  		Record operator[](int num);
 	};
 
 	/*
  		1. �t�d��Ʈw���s������
   		2. ����sql �y�y(����^���G)
   		3. �B�z�ư�
 	*/
 	class DataBase
 	{ 
 		public :
  			DataBase(); 
  			~DataBase();
 		private :
  			/* msyql �s���y�` */
  			MYSQL* m_Data;
 		public :
  			/* ��^�y�` */
  			MYSQL * GetMysql();
  			/* �s����Ʈw */
  			int Connect(string host, string user, 
   			string passwd, string db,
   			unsigned int port, 
   			unsigned long client_flag);
  
  		/* ������Ʈw�s�� */
  		void DisConnect();
  		/* ����D��^���G�d�� */
  		int ExecQuery(string sql);
  		/* ����mysql���A���O�_�s�� */
  		int Ping();
  		/* ����mysql ���A�� */
  		int ShutDown();
  		/* �D�n�\��:���s�Ұ�mysql ���A�� */
  		int ReBoot();
  	
  		//����:�ưȤ��InnoDB or BDB������
  		/* �D�n�\��:�}�l�ư� */
  		int Start_Transaction();
  		/* �D�n�\��:����ư� */
  		int Commit();
  		/* �D�n�\��:�^�u�ư� */
  		int Rollback();
  		/* �o��Ȥ�H�� */
  		const char * Get_client_info();
  		/* �D�n�\��:�o��Ȥ᪩���H�� */
  		const unsigned long  Get_client_version();
  		/* �D�n�\��:�o��D��H�� */
  		const char * Get_host_info();
  		/* �D�n�\��:�o����A���H�� */
  		const char * Get_server_info();
  		/*�D�n�\��:�o����A�������H��*/
  		const unsigned long  Get_server_version();
  		/*�D�n�\��:�o���e�s�����q�{�r�Ŷ�*/
  		const char *  Get_character_set_name();
  
 		/* �D�n�\���^��Ȭd��  */
  		char * ExecQueryGetSingValue(string sql);
  
  		/* �o��t�ήɶ� */
  		const char * GetSysTime();
  		/* �إ߷s��Ʈw */
  		int Create_db(string name);
  		/* �R����w����Ʈw*/
  		int Drop_db(string name);
	};
    
};

#endif //ZLB_MYSQL_H
