#ifndef __DBCONN_H
#define __DBCONN_H

#include <stdio.h>
#include <unistd.h>
#include <mysql.h>

class DBConn
{
public:
	DBConn(std::string host,int32_t port,std::string user,std::string password,std::string charset = "utf8");
	~DBConn();
	
private:
	MYSQL* m_conn;
	std::string m_host;
	int32_t m_port;
	std::string m_user;
	std::string m_password;
	std::string m_charset;
		
public:
	bool CreatDBConn();
	int64_t Execute(std::string strsql, int64_t& lastid, std::string& errscript);
  bool Query(std::string strsql, MYSQL_RES** myResult, std::string& errscript);
};

#endif
