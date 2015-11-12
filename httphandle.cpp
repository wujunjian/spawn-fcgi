#include "httphandle.h"
#include "util.h"
#include "mylog.h"
#include "udp_logger.h"

HttpReqHandle::HttpReqHandle():is_work_thread(true)
{
}

HttpReqHandle::~HttpReqHandle()
{
	is_work_thread=false;
	sleep(1);
}
    
void HttpReqHandle::getHttpReq(FCGX_Request *fcgi_req, httpRequest& tmp_http)
{
	char *arg_para;
	arg_para = FCGX_GetParam("REQUEST_URI", fcgi_req->envp);
	if((arg_para != NULL)&&strlen(arg_para) > 0){
	    tmp_http.str_uri_all.assign(arg_para);
	    char* tmppos=strchr(arg_para,'?');
	    if(tmppos)
	        (*tmppos)=0;
			tmp_http.str_uri.assign(arg_para);
	}
	
	arg_para = FCGX_GetParam("CONTENT_LENGTH", fcgi_req->envp);
	if((arg_para != NULL)&&strlen(arg_para) > 0){
	    tmp_http.ibody_len=atoi(arg_para);
	}else{
			tmp_http.ibody_len=0;
	}
	
	arg_para = FCGX_GetParam("REQUEST_BODY", fcgi_req->envp);
	if((arg_para != NULL)&&strlen(arg_para) > 0)
	{   
		tmp_http.str_body.assign(arg_para,tmp_http.ibody_len);
	}
	
	arg_para = FCGX_GetParam("QUERY_STRING", fcgi_req->envp);
	if((arg_para != NULL)&&strlen(arg_para) > 0)
	{   
		tmp_http.str_query.assign(arg_para);
	}
}

void HttpReqHandle::initHttpReq(httpRequest& tmp_http)
{
	tmp_http.str_uri_all="";
	tmp_http.str_uri="";
	tmp_http.ibody_len=0;
	tmp_http.str_body="";
	tmp_http.str_query="";
}

void *HttpReqHandle::http_req_thread(void *arg)
{
	HttpReqHandle* httpreq = (HttpReqHandle*)arg;
	httpRequest tmp_http;
	FCGX_Request fcgi_request;
 	FCGX_InitRequest(&fcgi_request, 0, 0);
 	while(httpreq->is_work_thread)
	{
		std::string __return("Status: 200 OK\r\nContent-type: text/plain\r\nContent-Length: 20\r\n\r\nParseFromString err!\r");
		httpreq->initHttpReq(tmp_http);
		#ifdef DEBUG_MOD
			MyLog::DebugLog("====================HttpReqHandle::http_req_thread initHttpReq!================");
		#endif
		int32_t rc = FCGX_Accept_r(&fcgi_request);
		#ifdef DEBUG_MOD
			MyLog::DebugLog("====================HttpReqHandle::http_req_thread FCGX_Accept_r!================");
		#endif
		if(rc>=0)
        {
			httpreq->getHttpReq(&fcgi_request, tmp_http);
			#ifdef DEBUG_MOD
				MyLog::DebugLog("====================HttpReqHandle::http_req_thread genTask start!================");
			#endif
			if(tmp_http.str_uri=="/task")
            {
				if(httpreq->process(tmp_http, __return))
                {
					__return=std::string("Status: 200 OK\r\nContent-type: text/plain\r\nContent-Length: "+Util::transToString(__return.size())+"\r\n\r\n"+__return);
				}
			}
            else
            {
                MyLog::DebugLog("wrong uri : " + tmp_http.str_uri);
            }
			//MyLog::ErrLog("Response:\n"+__return);
			FCGX_PutStr(__return.c_str(),__return.size(), fcgi_request.out);
			#ifdef DEBUG_MOD
				MyLog::DebugLog("====================HttpReqHandle::http_req_thread return END!================");
			#endif
			FCGX_Finish_r(&fcgi_request);
		}
	}
	#ifdef DEBUG_MOD
		MyLog::DebugLog("====================HttpReqHandle::http_req_thread EXIT!!!!!!================");
	#endif
}


int32_t HttpReqHandle::startHttpReqHandle()
{
	pthread_create(&m_tid, NULL, http_req_thread, static_cast<void *>(this));
	pthread_detach(m_tid);
	return 0;
}

void HttpReqHandle::parseArgs(std::string& req, std::map<std::string, std::string>& args)
{
  args.clear();
  std::vector<std::string> v;
  Util::split(req,"&",v);
  for(size_t i = 0; i < v.size(); ++i)
  {
    size_t nPos = v[i].find('=');
    if(nPos == 0 || nPos == std::string::npos)
      continue;
    std::string key = v[i].substr(0,nPos);
    std::string value = v[i].substr(nPos+1);
    for (size_t j = 0; j != value.size(); j ++)
      if (value[j] == '+') value[j] = ' ';
    value = Util::urldecode(value.c_str());
    args.insert(make_pair(key, value));
  }
}

bool HttpReqHandle::process(httpRequest& tmp_http, std::string& __return)
{
    MyLog::ErrLog("====================HttpReqHandle::process:  \n");
    __return = "";
    
    std::map<std::string, std::string> args;
    parseArgs(tmp_http.str_query, args);

    // TODO
    std::string LOG = "|" + args["imei"] + "|" + args["taskid"] + "|" + args["subtaskid"];
    MyLog::InfoLog(LOG);
    write_udp_logger(UDP_LOG_INFO, LOG, 0);
    return true;
    
}
