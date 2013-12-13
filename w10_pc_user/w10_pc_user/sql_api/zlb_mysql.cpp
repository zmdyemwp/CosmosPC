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
 *  file:zlb_mysql.cpp
 *
 *  author: @ zlb
 *  
 *  time:2005-12-12  
 *
 *
 *  
 --*/

#include "stdafx.h"
#include "zlb_mysql.h"

namespace zlb_mysql
{
 	//字段操作 
 	Field::Field(){}
 	Field::~Field(){}
 		
 	//是否是數字
 	bool Field::IsNum(int num)
 	{
  		if(IS_NUM(m_type[num]))
   			return true;
  		else
   			return false;
 	}
    //是否是數字
 	bool Field::IsNum(string num)
 	{
  		if(IS_NUM(m_type[GetField_NO(num)]))
   			return true;
  		else
   			return false;
 	}
 
 	//是否是日期 
 	bool Field::IsDate(int num)
 	{
  		if( FIELD_TYPE_DATE == m_type[num] || FIELD_TYPE_DATETIME == m_type[num] ) 
   			return true;
  		else
   			return false;
 	}
 	/* 是否是日期 */
 	bool Field::IsDate(string num)
 	{
  		int temp;
  		temp=GetField_NO(num);
  		if(FIELD_TYPE_DATE == m_type[temp] || FIELD_TYPE_DATETIME == m_type[temp] )
   			return true;
  		else
   			return false;
 	}
 	//是否是字符 
 	bool Field::IsChar(int num)
 	{
  		if(m_type[num]==FIELD_TYPE_STRING || m_type[num]==FIELD_TYPE_VAR_STRING || m_type[num]==FIELD_TYPE_CHAR )
   			return true;
  		else
   			return false; 
 	}
 	//是否是字符 
 	bool Field::IsChar(string num)
 	{
    	int temp;
  		temp=this->GetField_NO (num);
  		if(m_type[temp]==FIELD_TYPE_STRING || m_type[temp]==FIELD_TYPE_VAR_STRING || m_type[temp]==FIELD_TYPE_CHAR )
   			return true;
  		else
   			return false;
 	}
 	//是否為二進制數據
 	bool Field::IsBlob(int num)
 	{
  		if(IS_BLOB(m_type[num]))
   			return true;
  		else
   			return false;
 	}
 	//是否為二進制數據
 	bool Field::IsBlob(string num)
 	{
 		if(IS_BLOB(m_type[GetField_NO(num)]))
   			return true;
  		else
   			return false;
 	}
 	//得到指定字段的序號 
 	int Field::GetField_NO(string field_name)
 	{    
  		for(unsigned int i=0;i<m_name.size ();i++)
  		{
   			if(!m_name[i].compare (field_name))
    			return i;
   
  		}
  		return -1;
 	}
 
 	/*
  	  1. 單條記錄
   	  2. [int ]操作 [""]操作
 	*/
 	Record::Record(Field * m_f)
 	{
  		m_field =m_f;
 	}
 	Record::~Record(){};

 	void Record::SetData(string value)
 	{
  		m_rs.push_back (value);
 	}
 	/* [""]操作 */
 	string Record::operator[](string s)
 	{
  		return m_rs[m_field->GetField_NO(s)];
 	}
 	string Record::operator[](int num)
 	{
  		return m_rs[num];
 	}
 	/* null值判斷 */
 	bool Record::IsNull(int num)
 	{
  		if("" == m_rs[num].c_str ())
   			return true;
  		else
   			return false;
 	}
 	bool Record::IsNull(string s)
 	{
  		if("" == m_rs[m_field->GetField_NO(s)].c_str())
   			return true;
  		else
  			return false;
 	}
 	/* 主要-功能:用 value tab value 的形式 返回結果 */
 	string Record::GetTabText()
 	{
  		string temp;
  		for(unsigned int i=0 ;i<m_rs.size();i++)
  		{
   			temp+=m_rs[i];
   			if(i<m_rs.size ()-1)
    			temp+="\t";
  		}
  		return temp;
 	}
 
 	/*
  	  1. 記錄集合
  	  2. [int ]操作 [""]操作
  	  3. 表結構操作
  	  4. 數據的插入修改
 	*/
 	RecordSet::RecordSet()
 	{
  		res = NULL;
  		row = NULL;
  		pos = 0;
 	}
 	RecordSet::RecordSet(MYSQL *hSQL)
 	{
  		res = NULL;
  		row = NULL;
  		m_Data = hSQL;
  		pos = 0;
 	}
 	RecordSet::~RecordSet()
 	{
 	}
 	/*
  	  處理返回多行的查詢，返回影響的行數
  	  成功返回行數，失敗返回-1
 	*/
 	int RecordSet::ExecuteSQL(const char *SQL, bool flag)
 	{
		if(flag)
		{
  			if( !mysql_real_query(m_Data,SQL,strlen(SQL)))
  			//if( !mysql_query(m_Data,SQL))
  			{
   				//保存查詢結果
   				res = mysql_store_result(m_Data);
   				//得到記錄數量
   				m_recordcount = (int)mysql_num_rows(res) ; 
   				//得到字段數量
				m_field_num = mysql_num_fields(res) ;
   				for (int x = 0 ; fd = mysql_fetch_field(res); x++)
   				{
    				m_field.m_name.push_back(fd->name);
    				m_field.m_type.push_back(fd->type);
   				}
  				//保存所有數據
  				while (row = mysql_fetch_row(res))
   				{
    				Record temp(&m_field);
    				for (int k = 0 ; k < m_field_num ; k++ )
    				{   
     					if(row[k]==NULL||(!strlen(row[k])))
     					{
      						temp.SetData ("");
     					}
     					else
     					{
     						temp.SetData(row[k]);
     					} 
   	 				}
    				//添加新記錄
    				m_s.push_back (temp); 
   				}
   				mysql_free_result(res );
				return m_s.size();
  			}
		}
		else
		{
			mysql_real_query(m_Data,SQL,strlen(SQL));
			return 0; 
		}
		return -1;
	}
 	/*
  	* 向下移動游標
  	* 返回移動后的游標位置
 	*/
 	long RecordSet::MoveNext()
 	{
  		return (++pos);
 	}
 	/* 移動游標 */
 	long  RecordSet::Move(long length)
 	{
  		DWORD l = pos + length;
  
  		if(l<0)
  		{
   			pos = 0;
   			return 0;
  		}
  		else 
  		{ 
   			if(l >= m_s.size())
   			{
    			pos = m_s.size()-1;
    			return pos;
   			}
   			else
   			{
    			pos = l;
    			return pos;
   			}
  		}
  
 	}
 	/* 移動游標到開始位置 */
 	bool RecordSet::MoveFirst()
 	{
 		pos = 0;
  		return true;
 	}
 	/* 移動游標到結束位置 */
 	bool RecordSet::MoveLast()
 	{
  		pos = m_s.size()-1;
  		return true;
 	}
 	/* 獲取當前游標位置 */
 	unsigned long RecordSet::GetCurrentPos()const
 	{
 		return pos;
 	}
 	/* 獲取當前游標的對應字段數據 */
 	bool RecordSet::GetCurrentFieldValue(const char * sFieldName, char *sValue)
 	{
  		strcpy(sValue,m_s[pos][sFieldName].c_str());
  		return true;
 	}
 	bool RecordSet::GetCurrentFieldValue(const int iFieldNum,char *sValue)
 	{
  		strcpy(sValue,m_s[pos][iFieldNum].c_str());
  		return true;
 	}
 	/* 獲取游標的對應字數據据 */
 	bool RecordSet::GetFieldValue(long index,const char * sFieldName, char *sValue)
 	{
 		strcpy(sValue,m_s[index][sFieldName].c_str());
  		return true;
 	}
 	bool RecordSet::GetFieldValue(long index,int iFieldNum,char *sValue)
 	{
  		strcpy(sValue,m_s[index][iFieldNum].c_str());
  		return true;
 	}
 	/* 是否到達游標尾部 */
 	bool RecordSet::IsEof()
 	{ 
  		return (pos == m_s.size())?true:false;
 	}
 	/* 
  	* 得到記錄數目
 	*/
 	int RecordSet::GetRecordCount()
 	{
  		return m_recordcount;
 	}
 	/* 
 	 * 得到字段數目
 	*/
 	int RecordSet::GetFieldNum()
 	{
  		return m_field_num;
 	}
 	/* 
  	* 返回字段
 	*/
 	Field * RecordSet::GetField()
 	{
  		return &m_field;
 	}
 	/* 返回字段名 */
 	const char * RecordSet::GetFieldName(int iNum)
 	{
  		return m_field.m_name.at(iNum).c_str();
 	}
 	/* 返回字段類型 */
 	const int RecordSet::GetFieldType(char * sName)
 	{
  		int i = m_field.GetField_NO(sName);
  		return m_field.m_type.at(i);
 	}
 	const int RecordSet::GetFieldType(int iNum)
 	{
  		return m_field.m_type.at(iNum);
 	}
 	/* 
 	 * 返回指定序號的記錄
 	*/
 	Record RecordSet::operator[](int num)
 	{
  		return m_s[num];
 	}

 	/*
 	  1. 負責資料庫的連接關閉
 	  2. 執行sql 語句(不返回結果)
 	  3. 處理事務
 	*/
 	DataBase::DataBase()
 	{
  		m_Data = NULL;
 	}
 	DataBase::~DataBase()
 	{
  		if(NULL != m_Data)
  		{
   			DisConnect();
  		}
 	}
 	/* 返回句柄 */
 	MYSQL * DataBase::GetMysql()
 	{
  		return m_Data;
 	}
 
 	/*
  	* 主要功能:連接資料庫
  	* 參數說明:
  	* 1 host 主机ip地址或者是主机名稱
  	* 2 user 用戶名
  	* 3 passwd 密碼
  	* 4 db 欲連接的資料庫名稱
  	* 5 port 端口號 
  	* 6 uinx 嵌套字
  	* 7 client_flag 客戶連接參數
  	* 返回值: 0成功 -1 失敗
 	*/
 	int DataBase::Connect(string host, string user, 
      	string passwd, string db,
        unsigned int port, 
        unsigned long client_flag)
 	{
 		if((m_Data = mysql_init(NULL)) &&  
   			mysql_real_connect( m_Data, host.c_str(), 
        	user.c_str(), passwd.c_str(),
        	db.c_str(),port , NULL, 
        	client_flag))
  		{
   			//選擇制定的資料庫失敗
   			if ( mysql_select_db( m_Data, db.c_str () ) < 0 ) 
   			{
    			mysql_close( m_Data) ;
    			return -1 ;
   			}
  		}	
  		else 
  		{
   			//初始化mysql結構失敗
   			mysql_close( m_Data );
   			return -1 ;
  		}
  		//成功
  		return 0;
 	}
 	/* 
  	 * 關閉資料庫連接 
 	*/
 	void DataBase::DisConnect( )
 	{
  		mysql_close(m_Data) ;
 	}
 	/*
 	 * 主要功能: 執行非返回結果查詢
 	 * 參數:sql 待執行的查詢語句
 	 * 返回值; n為成功 表示受到影響的行數 -1 為執行失敗 
 	*/
 	int DataBase::ExecQuery(string sql)
 	{
  		if(!mysql_real_query(m_Data,sql.c_str (),(unsigned long)sql.length()) )
  		{
   			//得到受影響的行數
   			return (int)mysql_affected_rows(m_Data) ;
  		}
  		else
  		{
   			//執行查詢失敗
   			return -1;
  		}
 	}
 	/*
 	 * 主要功能:測試mysql伺服器是否存活
 	 * 返回值:0 表示成功 -1 失敗
 	*/
 	int DataBase::Ping()
 	{
  		if(!mysql_ping(m_Data))
   			return 0;
  		else 
   			return -1; 
 	}
 	/*
 	 *  主要功能:關閉mysql 伺服器
 	 * 返回值;0成功 -1 失敗
 	*/
 	int DataBase::ShutDown()
 	{
  		if(!mysql_shutdown(m_Data,SHUTDOWN_DEFAULT))
   			return 0;
  		else 
   			return -1;
 	}
 	/*
 	 * 主要功能:重新啟動mysql 伺服器
 	 * 返回值;0表示成功 -1 表示失敗
 	*/
    int DataBase::ReBoot()
 	{
 		if(!mysql_reload(m_Data))
   			return 0;
  		else
   			return -1;
 	}

 	//說明:事務支持InnoDB or BDB表類型
   	
    //主要功能:開始事務
 	int DataBase::Start_Transaction()
 	{
  		if(!mysql_real_query(m_Data, "START TRANSACTION", (unsigned long)strlen("START TRANSACTION") ))
  		{
   			return 0;
  		}
  		else
   			//執行查詢失敗
   			return -1;
 	}
 	
 	/*
 	 * 主要功能:提交事務
 	 * 返回值:0 表示成功 -1 表示失敗
 	*/
 	int DataBase::Commit()
 	{ 
  		if(!mysql_real_query( m_Data, "COMMIT", (unsigned long)strlen("COMMIT") ) )
  		{
   			return 0;
  		}
  		else      
   			//執行查詢失敗
   			return -1;
 	}
 	
 	/*
 	 * 主要功能:回滾事務
 	 * 返回值:0 表示成功 -1 表示失敗
 	*/
 	int DataBase::Rollback()
 	{
  		if(!mysql_real_query(m_Data, "ROLLBACK", (unsigned long)strlen("ROLLBACK") ) )
   			return 0;
  		else  
   			//執行查詢失敗
   			return -1; 
 	}
 	
 	/* 得到客戶信息 */
 	const char * DataBase::Get_client_info()
 	{ 
 		return mysql_get_client_info();
 	}
 	/*主要功能:得到客戶版本信息*/
 	const unsigned long DataBase::Get_client_version()
 	{
  		return mysql_get_client_version();
 	}
 	/* 主要功能:得到主机信息 */
 	const char * DataBase::Get_host_info()
 	{
  		return mysql_get_host_info(m_Data);
 	}
 	/* 主要功能:得到伺服器信息 */
 	const char * DataBase::Get_server_info()
 	{
  		return mysql_get_server_info( m_Data ); 
 	}
 	/* 主要功能:得到伺服器版本信息 */
 	const unsigned long   DataBase::Get_server_version()
 	{
  		return mysql_get_server_version(m_Data);
 	}
 	/*主要功能:得到 當前連接的默認字符集*/
 	const char *  DataBase::Get_character_set_name()
 	{
  		return mysql_character_set_name(m_Data);
 	}
 	/*
 	 * 主要功能返回單值查詢 
 	*/
 	char * DataBase::ExecQueryGetSingValue(string sql)
 	{
  		MYSQL_RES * res;
  		MYSQL_ROW row ;
  		char *p = NULL;
  		if(!mysql_real_query( m_Data, sql.c_str(),(unsigned long)sql.length()))
  		{
   			//保存查詢結果
   			res = mysql_store_result( m_Data ) ;
   			row = mysql_fetch_row( res ) ; 
   			p = ((row[0]==NULL)||(!strlen(row[0])))?"-1":row[0];
   			mysql_free_result( res ) ; 
  		}
  		else
   			//執行查詢失敗
   			p = "-1";
  		return p;
 	}
 	/*
 	 * 得到系統時間
 	*/
 	const char * DataBase::GetSysTime()
 	{
 		return ExecQueryGetSingValue("select now()");
 	}
 	/*
 	 * 主要功能:建立新資料庫
 	 * 參數:name 為新資料庫的名稱
 	 * 返回:0成功 -1 失敗
 	*/
 	int DataBase::Create_db(string name)
 	{
  		string temp ;
  		temp="CREATE DATABASE ";
  		temp+=name;
  		if(!mysql_real_query( m_Data,temp.c_str () ,(unsigned long)temp.length ()) )
   			return 0;
  		else
   			//執行查詢失敗
   			return -1;
 	}
 	
 	/*
 	 * 主要功能:刪除制定的資料庫
 	 * 參數:name 為欲刪除資料庫的名稱
 	 * 返回:0成功 -1 失敗
 	*/
 	int DataBase::Drop_db(string name)
 	{
  		string temp ;
  		temp="DROP DATABASE ";
  		temp+=name;
  		if(!mysql_real_query( m_Data,temp.c_str (), (unsigned long)temp.length ()) )
   			return 0;
  		else
   			//執行查詢失敗
   			return -1;
 	}

};
