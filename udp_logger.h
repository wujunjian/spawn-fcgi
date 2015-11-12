#ifndef _UDP_LOGGER_H
#define _UDP_LOGGER_H
#include <string>

#define UDP_LOG_FATAL "[FATAL]"
#define UDP_LOG_ERROR "[ERROR]"
#define UDP_LOG_WARN  "[WARN]"
#define UDP_LOG_INFO     "[INFO]"
#define UDP_LOGLEN    (1024*128)

bool init_udp_logger(std::string ip,int port,std::string serv,int idx=0);

void write_udp_logger(const char *level, std::string content,int idx=0);

#endif
