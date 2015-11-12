#ifndef __HTTPREQHANDLE_H_
#define __HTTPREQHANDLE_H_

#include <string>
#include <pthread.h>
#include <queue>
#include <map>
#include "fcgiapp.h"
#include "fcgi_config.h"
#include "fcgio.h"



typedef struct _httpRequest
{
    std::string str_uri_all;
    std::string str_uri;
    std::string str_body;
    int32_t ibody_len;
    std::string str_query;
}
httpRequest;

class HttpReqHandle
{
public:
    HttpReqHandle();
    ~HttpReqHandle();
public:
	  pthread_t m_tid;
	  volatile bool is_work_thread;

public:
		void getHttpReq(FCGX_Request *fcgi_req, httpRequest& tmp_http);
		void initHttpReq(httpRequest& tmp_http);
		static void *http_req_thread(void *arg);
		void parseArgs(std::string& req, std::map<std::string, std::string>& args);
		bool process(httpRequest& tmp_http, std::string& __return);
public:
	    int32_t startHttpReqHandle();
};

#endif
