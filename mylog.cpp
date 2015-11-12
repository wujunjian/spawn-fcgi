#include "mylog.h"

LoggerPtr TaskInfo(Logger::getLogger("TaskInfo"));
LoggerPtr DebugInfoLog(Logger::getLogger("debuginfo"));
    
MyLog::MyLog()
{
}

MyLog::~MyLog()
{
}

void MyLog::DebugLog(std::string debuginfo)
{
    LOG4CXX_DEBUG(DebugInfoLog,debuginfo);
}

void MyLog::ErrLog(std::string errinfo)
{
    LOG4CXX_FATAL(DebugInfoLog,errinfo);
}
void MyLog::InfoLog(std::string str_info)
{
		LOG4CXX_INFO(TaskInfo,str_info);
}
