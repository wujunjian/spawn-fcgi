#ifndef __CONFLOADER_H_
#define __CONFLOADER_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>

typedef struct _GlobalConf
{
	int32_t i32_bidmap_num;
	int32_t i32_thread_num;//每个bideventmap中的消耗队列线程数量
	std::string str_log4cxx;
	int32_t i32_bid_timeout; 	      //us,每次循环等待时间
    uint32_t u32_get_event_loops;     // 真正的竞价超时时间为 u32_get_event_loops * i32_bid_timeout
	int32_t i32_epoll_size;           // epoll能管理的链接数量
	std::string str_index_hp;
	std::string str_iplabe_file;
	std::string str_syslog_hp;
	std::string str_syslog_hp_bid;
	std::string str_bucket_test;
}
GlobalConf;

//

class ConfLoader
{
public:
    ConfLoader();
    ~ConfLoader();
private:
    std::map<std::string,std::string> cfg_map;
private:
    int transMap2GlobalConf(GlobalConf &global_cfg);
    int LoadMainCfg();
    
public:
    int LoadCfg();
    std::string Dump_Global_Conf();
    static char *GetCurAbsDir();   //获取当前进程所在目录的绝对路径
};

#endif
