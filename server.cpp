#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "httphandle.h"
#include "util.h"
#include "mylog.h"
#include "connselecter.h"
#include "mycache.h"
#include "confloader.h"
#include "udp_logger.h"
#include "confloader.h"

ConnSelecter* g_connselecter;
MyCache* g_mycache;
GlobalConf g_global_conf;

bool initConnSelecter()
{
    g_mycache = new MyCache();
    g_connselecter = new ConnSelecter();
    ConnPool* tmpconnpool = new ConnPool(128,"192.168.1.184",3306,"worker","123qwe");
    tmpconnpool->CreatConnPool();
    g_connselecter->addConnPool(tmpconnpool,true);//read
    tmpconnpool = new ConnPool(128,"192.168.1.184",3306,"worker","123qwe");
    tmpconnpool->CreatConnPool();
    g_connselecter->addConnPool(tmpconnpool,false);//write

    return true;
}


void loadProductList()
{

}

void *recover_conn_thread(void *arg)
{
	while(true)
	{
		g_mycache->cleanCache();
		sleep(3600);
	}
}

void *reload_thread(void *arg)
{
	while(true)
	{
		time_t nowtime = time(NULL);
		struct tm *newtime;
		newtime=localtime(&nowtime);
		if(newtime->tm_hour==0&&newtime->tm_min==1){

		}
		loadProductList();
		sleep(60);
	}
}


void init()
{
    ConfLoader confloader;
    confloader.LoadCfg();
    
    //std::cout<<g_global_conf.str_syslog_hp << std::endl;
    
    
    std::vector<std::string> vec_syslog_info;
    Util::split(g_global_conf.str_syslog_hp,":",vec_syslog_info);
    
	srand(time(NULL));
	PropertyConfigurator::configure("./conf/log4cxx.properties");
	FCGX_Init();
    initConnSelecter();
    init_udp_logger(vec_syslog_info[0],atoi(vec_syslog_info[1].c_str()),std::string("moneymonitor"),0);
}

int main()
{
	sigset_t         mask;	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sa.sa_mask = mask;    
	if (sigaction(SIGPIPE, &sa, 0) == -1) {
		printf("sigaction err\n");
		exit(-1);
	}
	if(pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
		return -1;
	}
	init();
	HttpReqHandle* httpreqhandler=new HttpReqHandle[4];
	for(int32_t i=0;i<4;i++){
		httpreqhandler[i].startHttpReqHandle();
		usleep(1000);
	}
	pthread_t recover_tid;
	pthread_t reload_tid;
	pthread_create(&recover_tid, NULL, recover_conn_thread, NULL);
	pthread_create(&reload_tid, NULL, reload_thread, NULL);
	pthread_join(recover_tid,NULL);
	pthread_join(reload_tid,NULL);
	return 0;
}
