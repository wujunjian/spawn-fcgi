#ifndef __CONFLOADER_H_
#define __CONFLOADER_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>

typedef struct _GlobalConf
{
	int32_t i32_bidmap_num;
	int32_t i32_thread_num;//ÿ��bideventmap�е����Ķ����߳�����
	std::string str_log4cxx;
	int32_t i32_bid_timeout; 	      //us,ÿ��ѭ���ȴ�ʱ��
    uint32_t u32_get_event_loops;     // �����ľ��۳�ʱʱ��Ϊ u32_get_event_loops * i32_bid_timeout
	int32_t i32_epoll_size;           // epoll�ܹ������������
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
    static char *GetCurAbsDir();   //��ȡ��ǰ��������Ŀ¼�ľ���·��
};

#endif
