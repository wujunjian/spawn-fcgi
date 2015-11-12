#include "connpool.h"
#include "mylog.h"

ConnPool::ConnPool(int32_t conn_num,std::string host,int32_t port,std::string user,std::string password,std::string charset)
{
	m_conn_num_max=conn_num;
	
	m_host=host;
	m_port=port;
	m_user=user;
	m_password=password;
	m_charset=charset;
	pthread_mutex_init(&conn_list_lock,NULL);	

  //conn_list.clear();
}

ConnPool::~ConnPool()
{
}

	
MYSQL* ConnPool::popConn()
{
	MYSQL* res=NULL;
	pthread_mutex_lock(&conn_list_lock);
	if(conn_list.size()>0){
		res=conn_list.front();
		conn_list.pop();
	}
	pthread_mutex_unlock(&conn_list_lock);
	return res;
}

void ConnPool::pushConn(MYSQL* conn)
{
	if(!conn)
		return;
	pthread_mutex_lock(&conn_list_lock);
	conn_list.push(conn);
	pthread_mutex_unlock(&conn_list_lock);
	return;
}



int32_t ConnPool::CreatConnPool()
{
	MyLog::ErrLog("CreatConnPool0");
	int32_t res = 0;
	my_bool brecon=1;
  unsigned int conntimeout = 3;
	for(int32_t i=0;i<m_conn_num_max;i++){
		MYSQL* conn = mysql_init((MYSQL*)0);
		if(!conn)
			continue;
		mysql_options(conn,MYSQL_OPT_RECONNECT,&brecon);
    mysql_options(conn,MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&conntimeout);
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, m_charset.c_str());
    if(!mysql_real_connect(conn, m_host.c_str(), m_user.c_str(), m_password.c_str(), NULL, m_port, NULL, 0)){
        MyLog::ErrLog("Failed to connect to database: Error: "+std::string(mysql_error(conn))); 
    		continue;
    }
    MyLog::ErrLog("CreatConnPool");
    pushConn(conn);
    res++;
	}
	return res;
}

int64_t ConnPool::Execute(std::string strsql, int64_t& lastid, std::string& errscript)
{
	MYSQL* tmp_conn = popConn();
	if(tmp_conn==NULL){
		errscript="conn list is null";
		return -1;
	}
	if(mysql_real_query(tmp_conn, strsql.c_str(), strsql.size())!=0){
      errscript.assign(mysql_error(tmp_conn));
      //writeLog(LOG_ERROR,"Execute err: %s.",errscript.c_str());
      pushConn(tmp_conn);
      lastid=-1;
      return -1;
  }
  errscript="";
  lastid=(int64_t)mysql_insert_id(tmp_conn);
  int32_t affect_rows = (int64_t)mysql_affected_rows(tmp_conn);
  pushConn(tmp_conn);
  return affect_rows;
}

bool ConnPool::Query(std::string strsql, MYSQL_RES** myResult, std::string& errscript)
{
	MYSQL* tmp_conn = popConn();
	if(tmp_conn==NULL){
		errscript="conn list is null";
		return -1;
	}
	if(mysql_real_query(tmp_conn, strsql.c_str(), strsql.size()) != 0){
    *myResult=NULL;
    errscript.assign(mysql_error(tmp_conn));
    //writeLog(LOG_ERROR,"Query error: %s.",errscript.c_str()); 
    pushConn(tmp_conn);
    return false;
  }
  errscript="";
  *myResult=mysql_store_result(tmp_conn);
  pushConn(tmp_conn);
  return true;
}
