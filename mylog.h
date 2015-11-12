#ifndef __MYLOG_H_
#define __MYLOG_H_

#include <log4cxx/logger.h>
#include <log4cxx/logstring.h>
#include <log4cxx/propertyconfigurator.h>

#define LOG_FIELD_SEP1			( ( char )0x02 )
using namespace log4cxx;

class MyLog{
public:
    MyLog();
    ~MyLog();

public:
    static void DebugLog(std::string debuginfo); 
    static void ErrLog(std::string errinfo); 
    static void InfoLog(std::string str_info);
};

#endif
