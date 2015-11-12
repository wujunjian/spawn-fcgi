#include "confloader.h"
#include "util.h"
#include <dirent.h>

#define MAINCFG "conf/moneycoolmonitor.conf"
extern GlobalConf g_global_conf;

ConfLoader::ConfLoader()
{
}

ConfLoader::~ConfLoader()
{
}

int ConfLoader::transMap2GlobalConf(GlobalConf &global_cfg)
{
    std::map<std::string,std::string>::iterator it = cfg_map.find("BidEventMapNum");
    if(it!=cfg_map.end())
       global_cfg.i32_bidmap_num=atoi(it->second.c_str());
    else
       global_cfg.i32_bidmap_num=8;
       
    it = cfg_map.find("ThreadNum");
    if(it!=cfg_map.end())
       global_cfg.i32_thread_num=atoi(it->second.c_str()); 
    else
       global_cfg.i32_thread_num=2; 
       
    it = cfg_map.find("log4cxx");
    if(it!=cfg_map.end())
       global_cfg.str_log4cxx=it->second; 
    else
       global_cfg.str_log4cxx= Util::transToString(GetCurAbsDir()) + "/conf/log4cxx.properties";  

	it = cfg_map.find("BidTimeOut");
    if(it!=cfg_map.end())
       global_cfg.i32_bid_timeout=atoi(it->second.c_str()); 
    else
       global_cfg.i32_bid_timeout=10 * 1000; // 10 ms
 
	it = cfg_map.find("GetEventLoops");
    if(it!=cfg_map.end())
       global_cfg.u32_get_event_loops=atoi(it->second.c_str()); 
    else
       global_cfg.u32_get_event_loops=9;
    
    // the code followed is added by wLiu
    it = cfg_map.find("EpollSize");
    if(it != cfg_map.end()) {
        global_cfg.i32_epoll_size = atoi(it->second.c_str());
    } else {
        global_cfg.i32_epoll_size = 1024;
    }

   it = cfg_map.find("IndexServer");
    if(it!=cfg_map.end())
       global_cfg.str_index_hp=it->second; 
    else
       global_cfg.str_index_hp="127.0.0.1:6789";

	
	it = cfg_map.find("Iplabe");
    if(it!=cfg_map.end())
       global_cfg.str_iplabe_file=it->second; 
    else
       global_cfg.str_iplabe_file="conf/ip.labe";
       
    
    it = cfg_map.find("syslog_hp");
    if(it!=cfg_map.end())
       global_cfg.str_syslog_hp=it->second; 
    else
       global_cfg.str_syslog_hp="192.168.1.161:12345";
       
    it = cfg_map.find("syslog_hp_bid");
    if(it!=cfg_map.end())
       global_cfg.str_syslog_hp_bid=it->second; 
    else
       global_cfg.str_syslog_hp_bid="192.168.1.185:12345";
       
    
    it = cfg_map.find("BucketTest");
    if(it!=cfg_map.end())
       global_cfg.str_bucket_test=it->second; 
    else
       global_cfg.str_bucket_test="";
    return 0;
   
}

char *ConfLoader::GetCurAbsDir()
{
    static char  buf[1024] = {0};
    static bool  init = false;
    int          dir_len;

    if (!init) {
        dir_len = readlink("/proc/self/exe", buf, 1023);
        if (dir_len <= 0 || dir_len >= 1024) {
            return NULL;
        }
        buf[dir_len] = '\0';
        for ( ; --dir_len; ) {
            if (buf[dir_len] == '/') {
                buf[dir_len] = '\0';
                break;
            }
        }
        bzero(buf + dir_len + 1, 1024 - dir_len - 1);
        init = true;
    }
    return buf;
}
//------------------Load Main conf---------------------------------
int ConfLoader::LoadMainCfg()
{
    char *ptr;
    FILE *cfp=NULL;
    char line[1024]={0};
    char cfg_path[1024] = {0};

    snprintf(cfg_path, 1023, "%s/%s", GetCurAbsDir(), MAINCFG);
    if(cfp=fopen(cfg_path,"r"))
    {
        while(fgets(line, sizeof(line), cfp))
        {
            ptr=line;
            char* tmpEnter=strchr(ptr,'\n');
            if(tmpEnter)
                (*tmpEnter)=0;
            tmpEnter=strchr(ptr,'\r');
            if(tmpEnter)
                (*tmpEnter)=0;
            char* tmppos=strstr(ptr,":");
            if(!tmppos)
                continue;
            (*tmppos)=0;
            std::string cfg_key(ptr);
            tmppos++;
            std::string cfg_value(tmppos);
            cfg_map.insert(std::map<std::string,std::string>::value_type(cfg_key,cfg_value));
        }
        fclose(cfp);
        return transMap2GlobalConf(g_global_conf);
    }
    else
    {
        return -1;
    }
}

std::string ConfLoader::Dump_Global_Conf()
{
    std::string str_res;
    str_res+="BidEventMapNum:"+Util::transToString(g_global_conf.i32_bidmap_num)+"\n";
    str_res+="BidEventThreadNum:"+Util::transToString(g_global_conf.i32_thread_num)+"\n";
    str_res+="EpollSize:"+Util::transToString(g_global_conf.i32_epoll_size)+"\n";
    
    return str_res;
}

int ConfLoader::LoadCfg()
{
    if(LoadMainCfg()==-1)
    {
    	   printf("LoadMainCfg err\n");
        exit(-1);
    }
		return 0;
}

