#ifndef __CONNPOOL_H
#define __CONNPOOL_H

#include <queue>
#include <stdio.h>
#include <unistd.h>
#include <mysql.h>
#include <pthread.h>
#include <string>

class ConnPool
{
public:
	ConnPool(int32_t conn_num,std::string host,int32_t port,std::string user,std::string password,std::string charset = "utf8");
	~ConnPool();
	
private:
	int32_t m_conn_num_max;//连接池设定容量
	
	std::string m_host;
	int32_t m_port;
	std::string m_user;
	std::string m_password;
	std::string m_charset;
		
	pthread_mutex_t conn_list_lock;
  std::queue<MYSQL*> conn_list;
		
private:
	MYSQL* popConn();
	void pushConn(MYSQL* conn);
public:
	int32_t CreatConnPool();
	int64_t Execute(std::string strsql, int64_t& lastid, std::string& errscript);
  bool Query(std::string strsql, MYSQL_RES** myResult, std::string& errscript);
};

#endif
