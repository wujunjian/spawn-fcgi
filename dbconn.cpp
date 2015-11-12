#include "dbconn.h"

DBConn::DBConn(std::string host,int32_t port,std::string user,std::string password,std::string charset)
{
	m_host=host;
	m_port=port;
	m_user=user;
	m_password=password;
	m_charset=charset;
	m_conn=NULL;
}

DBConn::~DBConn()
{
	if(m_conn)
		mysql_close(m_conn);
}

bool DBConn::CreatDBConn()
{
		my_bool brecon=1;
    unsigned int conntimeout = 3;
    m_conn=mysql_init((MYSQL*)0);
    mysql_options(m_conn,MYSQL_OPT_RECONNECT,&brecon);
    mysql_options(m_conn,MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&conntimeout);
    mysql_options(m_conn, MYSQL_SET_CHARSET_NAME, m_charset.c_str());
    if(!mysql_real_connect(m_conn, m_host.c_str(), m_user.c_str(), m_password.c_str(), NULL, m_port, NULL, 0))
    {
        //writeLog(LOG_ERROR,"Failed to connect to database: Error: %s\n",mysql_error(nowconn)); 
    		return false;
    }
    return true;
}

int64_t DBConn::Execute(std::string strsql, int64_t& lastid, std::string& errscript)
{
	if(m_conn==NULL){
		errscript="conn is null";
		return -1;
	}
	if(mysql_real_query(m_conn, strsql.c_str(), strsql.size())!=0){
      errscript.assign(mysql_error(m_conn));
      //writeLog(LOG_ERROR,"Execute err: %s.",errscript.c_str());
      lastid=-1;
      return -1;
  }
  errscript="";
  lastid=(int64_t)mysql_insert_id(m_conn);
  return (int64_t)mysql_affected_rows(m_conn);
}

bool DBConn::Query(std::string strsql, MYSQL_RES** myResult, std::string& errscript)
{
	if(m_conn==NULL){
		errscript="conn is null";
		return false;
	}
  if(mysql_real_query(m_conn, strsql.c_str(), strsql.size()) != 0){
    *myResult=NULL;
    errscript.assign(mysql_error(m_conn));
    //writeLog(LOG_ERROR,"Query error: %s.",errscript.c_str()); 
    return false;
  }
  errscript="";
  *myResult=mysql_store_result(m_conn);
  return true;
}
