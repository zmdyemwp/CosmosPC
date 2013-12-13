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
 	//�r�q�ާ@ 
 	Field::Field(){}
 	Field::~Field(){}
 		
 	//�O�_�O�Ʀr
 	bool Field::IsNum(int num)
 	{
  		if(IS_NUM(m_type[num]))
   			return true;
  		else
   			return false;
 	}
    //�O�_�O�Ʀr
 	bool Field::IsNum(string num)
 	{
  		if(IS_NUM(m_type[GetField_NO(num)]))
   			return true;
  		else
   			return false;
 	}
 
 	//�O�_�O��� 
 	bool Field::IsDate(int num)
 	{
  		if( FIELD_TYPE_DATE == m_type[num] || FIELD_TYPE_DATETIME == m_type[num] ) 
   			return true;
  		else
   			return false;
 	}
 	/* �O�_�O��� */
 	bool Field::IsDate(string num)
 	{
  		int temp;
  		temp=GetField_NO(num);
  		if(FIELD_TYPE_DATE == m_type[temp] || FIELD_TYPE_DATETIME == m_type[temp] )
   			return true;
  		else
   			return false;
 	}
 	//�O�_�O�r�� 
 	bool Field::IsChar(int num)
 	{
  		if(m_type[num]==FIELD_TYPE_STRING || m_type[num]==FIELD_TYPE_VAR_STRING || m_type[num]==FIELD_TYPE_CHAR )
   			return true;
  		else
   			return false; 
 	}
 	//�O�_�O�r�� 
 	bool Field::IsChar(string num)
 	{
    	int temp;
  		temp=this->GetField_NO (num);
  		if(m_type[temp]==FIELD_TYPE_STRING || m_type[temp]==FIELD_TYPE_VAR_STRING || m_type[temp]==FIELD_TYPE_CHAR )
   			return true;
  		else
   			return false;
 	}
 	//�O�_���G�i��ƾ�
 	bool Field::IsBlob(int num)
 	{
  		if(IS_BLOB(m_type[num]))
   			return true;
  		else
   			return false;
 	}
 	//�O�_���G�i��ƾ�
 	bool Field::IsBlob(string num)
 	{
 		if(IS_BLOB(m_type[GetField_NO(num)]))
   			return true;
  		else
   			return false;
 	}
 	//�o����w�r�q���Ǹ� 
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
  	  1. ����O��
   	  2. [int ]�ާ@ [""]�ާ@
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
 	/* [""]�ާ@ */
 	string Record::operator[](string s)
 	{
  		return m_rs[m_field->GetField_NO(s)];
 	}
 	string Record::operator[](int num)
 	{
  		return m_rs[num];
 	}
 	/* null�ȧP�_ */
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
 	/* �D�n-�\��:�� value tab value ���Φ� ��^���G */
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
  	  1. �O�����X
  	  2. [int ]�ާ@ [""]�ާ@
  	  3. ���c�ާ@
  	  4. �ƾڪ����J�ק�
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
  	  �B�z��^�h�檺�d�ߡA��^�v�T�����
  	  ���\��^��ơA���Ѫ�^-1
 	*/
 	int RecordSet::ExecuteSQL(const char *SQL, bool flag)
 	{
		if(flag)
		{
  			if( !mysql_real_query(m_Data,SQL,strlen(SQL)))
  			//if( !mysql_query(m_Data,SQL))
  			{
   				//�O�s�d�ߵ��G
   				res = mysql_store_result(m_Data);
   				//�o��O���ƶq
   				m_recordcount = (int)mysql_num_rows(res) ; 
   				//�o��r�q�ƶq
				m_field_num = mysql_num_fields(res) ;
   				for (int x = 0 ; fd = mysql_fetch_field(res); x++)
   				{
    				m_field.m_name.push_back(fd->name);
    				m_field.m_type.push_back(fd->type);
   				}
  				//�O�s�Ҧ��ƾ�
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
    				//�K�[�s�O��
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
  	* �V�U���ʴ��
  	* ��^���ʦZ����Ц�m
 	*/
 	long RecordSet::MoveNext()
 	{
  		return (++pos);
 	}
 	/* ���ʴ�� */
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
 	/* ���ʴ�Ш�}�l��m */
 	bool RecordSet::MoveFirst()
 	{
 		pos = 0;
  		return true;
 	}
 	/* ���ʴ�Ш쵲����m */
 	bool RecordSet::MoveLast()
 	{
  		pos = m_s.size()-1;
  		return true;
 	}
 	/* �����e��Ц�m */
 	unsigned long RecordSet::GetCurrentPos()const
 	{
 		return pos;
 	}
 	/* �����e��Ъ������r�q�ƾ� */
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
 	/* �����Ъ������r�ƾ��u */
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
 	/* �O�_��F��Ч��� */
 	bool RecordSet::IsEof()
 	{ 
  		return (pos == m_s.size())?true:false;
 	}
 	/* 
  	* �o��O���ƥ�
 	*/
 	int RecordSet::GetRecordCount()
 	{
  		return m_recordcount;
 	}
 	/* 
 	 * �o��r�q�ƥ�
 	*/
 	int RecordSet::GetFieldNum()
 	{
  		return m_field_num;
 	}
 	/* 
  	* ��^�r�q
 	*/
 	Field * RecordSet::GetField()
 	{
  		return &m_field;
 	}
 	/* ��^�r�q�W */
 	const char * RecordSet::GetFieldName(int iNum)
 	{
  		return m_field.m_name.at(iNum).c_str();
 	}
 	/* ��^�r�q���� */
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
 	 * ��^���w�Ǹ����O��
 	*/
 	Record RecordSet::operator[](int num)
 	{
  		return m_s[num];
 	}

 	/*
 	  1. �t�d��Ʈw���s������
 	  2. ����sql �y�y(����^���G)
 	  3. �B�z�ư�
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
 	/* ��^�y�` */
 	MYSQL * DataBase::GetMysql()
 	{
  		return m_Data;
 	}
 
 	/*
  	* �D�n�\��:�s����Ʈw
  	* �Ѽƻ���:
  	* 1 host �D��ip�a�}�Ϊ̬O�D��W��
  	* 2 user �Τ�W
  	* 3 passwd �K�X
  	* 4 db ���s������Ʈw�W��
  	* 5 port �ݤf�� 
  	* 6 uinx �O�M�r
  	* 7 client_flag �Ȥ�s���Ѽ�
  	* ��^��: 0���\ -1 ����
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
   			//��ܨ�w����Ʈw����
   			if ( mysql_select_db( m_Data, db.c_str () ) < 0 ) 
   			{
    			mysql_close( m_Data) ;
    			return -1 ;
   			}
  		}	
  		else 
  		{
   			//��l��mysql���c����
   			mysql_close( m_Data );
   			return -1 ;
  		}
  		//���\
  		return 0;
 	}
 	/* 
  	 * ������Ʈw�s�� 
 	*/
 	void DataBase::DisConnect( )
 	{
  		mysql_close(m_Data) ;
 	}
 	/*
 	 * �D�n�\��: ����D��^���G�d��
 	 * �Ѽ�:sql �ݰ��檺�d�߻y�y
 	 * ��^��; n�����\ ��ܨ���v�T����� -1 �����楢�� 
 	*/
 	int DataBase::ExecQuery(string sql)
 	{
  		if(!mysql_real_query(m_Data,sql.c_str (),(unsigned long)sql.length()) )
  		{
   			//�o����v�T�����
   			return (int)mysql_affected_rows(m_Data) ;
  		}
  		else
  		{
   			//����d�ߥ���
   			return -1;
  		}
 	}
 	/*
 	 * �D�n�\��:����mysql���A���O�_�s��
 	 * ��^��:0 ��ܦ��\ -1 ����
 	*/
 	int DataBase::Ping()
 	{
  		if(!mysql_ping(m_Data))
   			return 0;
  		else 
   			return -1; 
 	}
 	/*
 	 *  �D�n�\��:����mysql ���A��
 	 * ��^��;0���\ -1 ����
 	*/
 	int DataBase::ShutDown()
 	{
  		if(!mysql_shutdown(m_Data,SHUTDOWN_DEFAULT))
   			return 0;
  		else 
   			return -1;
 	}
 	/*
 	 * �D�n�\��:���s�Ұ�mysql ���A��
 	 * ��^��;0��ܦ��\ -1 ��ܥ���
 	*/
    int DataBase::ReBoot()
 	{
 		if(!mysql_reload(m_Data))
   			return 0;
  		else
   			return -1;
 	}

 	//����:�ưȤ��InnoDB or BDB������
   	
    //�D�n�\��:�}�l�ư�
 	int DataBase::Start_Transaction()
 	{
  		if(!mysql_real_query(m_Data, "START TRANSACTION", (unsigned long)strlen("START TRANSACTION") ))
  		{
   			return 0;
  		}
  		else
   			//����d�ߥ���
   			return -1;
 	}
 	
 	/*
 	 * �D�n�\��:����ư�
 	 * ��^��:0 ��ܦ��\ -1 ��ܥ���
 	*/
 	int DataBase::Commit()
 	{ 
  		if(!mysql_real_query( m_Data, "COMMIT", (unsigned long)strlen("COMMIT") ) )
  		{
   			return 0;
  		}
  		else      
   			//����d�ߥ���
   			return -1;
 	}
 	
 	/*
 	 * �D�n�\��:�^�u�ư�
 	 * ��^��:0 ��ܦ��\ -1 ��ܥ���
 	*/
 	int DataBase::Rollback()
 	{
  		if(!mysql_real_query(m_Data, "ROLLBACK", (unsigned long)strlen("ROLLBACK") ) )
   			return 0;
  		else  
   			//����d�ߥ���
   			return -1; 
 	}
 	
 	/* �o��Ȥ�H�� */
 	const char * DataBase::Get_client_info()
 	{ 
 		return mysql_get_client_info();
 	}
 	/*�D�n�\��:�o��Ȥ᪩���H��*/
 	const unsigned long DataBase::Get_client_version()
 	{
  		return mysql_get_client_version();
 	}
 	/* �D�n�\��:�o��D��H�� */
 	const char * DataBase::Get_host_info()
 	{
  		return mysql_get_host_info(m_Data);
 	}
 	/* �D�n�\��:�o����A���H�� */
 	const char * DataBase::Get_server_info()
 	{
  		return mysql_get_server_info( m_Data ); 
 	}
 	/* �D�n�\��:�o����A�������H�� */
 	const unsigned long   DataBase::Get_server_version()
 	{
  		return mysql_get_server_version(m_Data);
 	}
 	/*�D�n�\��:�o�� ��e�s�����q�{�r�Ŷ�*/
 	const char *  DataBase::Get_character_set_name()
 	{
  		return mysql_character_set_name(m_Data);
 	}
 	/*
 	 * �D�n�\���^��Ȭd�� 
 	*/
 	char * DataBase::ExecQueryGetSingValue(string sql)
 	{
  		MYSQL_RES * res;
  		MYSQL_ROW row ;
  		char *p = NULL;
  		if(!mysql_real_query( m_Data, sql.c_str(),(unsigned long)sql.length()))
  		{
   			//�O�s�d�ߵ��G
   			res = mysql_store_result( m_Data ) ;
   			row = mysql_fetch_row( res ) ; 
   			p = ((row[0]==NULL)||(!strlen(row[0])))?"-1":row[0];
   			mysql_free_result( res ) ; 
  		}
  		else
   			//����d�ߥ���
   			p = "-1";
  		return p;
 	}
 	/*
 	 * �o��t�ήɶ�
 	*/
 	const char * DataBase::GetSysTime()
 	{
 		return ExecQueryGetSingValue("select now()");
 	}
 	/*
 	 * �D�n�\��:�إ߷s��Ʈw
 	 * �Ѽ�:name ���s��Ʈw���W��
 	 * ��^:0���\ -1 ����
 	*/
 	int DataBase::Create_db(string name)
 	{
  		string temp ;
  		temp="CREATE DATABASE ";
  		temp+=name;
  		if(!mysql_real_query( m_Data,temp.c_str () ,(unsigned long)temp.length ()) )
   			return 0;
  		else
   			//����d�ߥ���
   			return -1;
 	}
 	
 	/*
 	 * �D�n�\��:�R����w����Ʈw
 	 * �Ѽ�:name �����R����Ʈw���W��
 	 * ��^:0���\ -1 ����
 	*/
 	int DataBase::Drop_db(string name)
 	{
  		string temp ;
  		temp="DROP DATABASE ";
  		temp+=name;
  		if(!mysql_real_query( m_Data,temp.c_str (), (unsigned long)temp.length ()) )
   			return 0;
  		else
   			//����d�ߥ���
   			return -1;
 	}

};
