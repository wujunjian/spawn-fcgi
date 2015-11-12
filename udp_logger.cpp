#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "udp_logger.h"

using namespace std;

struct sockaddr_in addr_logger[2];
std::string service_logger[2];
int sockfd_logger[2];

bool init_udp_logger(std::string ip,int port,std::string serv,int idx)
{
     sockfd_logger[idx] = socket(AF_INET,SOCK_DGRAM,0);
     if(sockfd_logger[idx] < 0) {
          return false;
     }
     bzero(&addr_logger[idx],sizeof(struct sockaddr_in));
     addr_logger[idx].sin_family = AF_INET;
     addr_logger[idx].sin_port = htons(port);
     if(inet_aton(ip.c_str(),&addr_logger[idx].sin_addr) < 0) {
          fprintf(stderr,"IP error:%sn",strerror(errno));
          return false;
     }
     service_logger[idx] = serv;
     return true;
}

void write_udp_logger(const char *level, string content,int idx)
{
     char log[UDP_LOGLEN];
     snprintf(log,UDP_LOGLEN,"%s %s %s",service_logger[idx].c_str(),level,content.c_str());
     sendto(sockfd_logger[idx],log,strnlen(log,UDP_LOGLEN),0,(struct sockaddr *)&addr_logger[idx],sizeof(struct sockaddr_in));
     //fflush(sockfd);
}
