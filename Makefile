OBJECT= httphandle.o server.o md5.o util.o mylog.o connpool.o connselecter.o mycache.o BloomFilter.o  udp_logger.o confloader.o
CPP=g++ -g

IFCGI=-I /usr/local/fcgi/include
LFCGI=/usr/local/fcgi/lib/libfcgi.a /usr/local/fcgi/lib/libfcgi++.a

IMYSQL=-I /home/system/mysql/include/
LMYSQL=-L /home/system/mysql/lib/ -lmysqlclient_r -Wl,-rpath=/home/system/mysql/lib/

ILOG4CXX=-I/usr/local/log4cxx/include
#LLOG4CXX=-L/usr/local/log4cxx/lib/ -llog4cxx -Wl,-rpath=/usr/local/log4cxx/lib/
LLOG4CXX=/usr/local/log4cxx/lib/liblog4cxx.a /opt/src/httpd-2.2.17/srclib/apr-util/.libs/libaprutil-1.a /opt/src/httpd-2.2.17/srclib/apr/.libs/libapr-1.a

all: MoneyCoolMonitor
$(OBJECT):%.o:%.cpp
	$(CPP) $(FLAG) -I. -I./proto ${IFCGI} ${ILOG4CXX} ${IMYSQL} -DHAVE_NETINET_IN_H -c $< -o $@
$(PROTO_OBJECT):%.o:%.cc
	$(CPP) $(FLAG) -I. -I./proto ${IPROTO} -c $< -o $@
MoneyCoolMonitor:$(OBJECT) $(PROTO_OBJECT)
	$(CPP) $(FLAG) -o MoneyCoolMonitor ${OBJECT} $(PROTO_OBJECT) ${LPROTO} ${LFCGI} ${LLOG4CXX} ${LMYSQL} ${LHIREDIS} -lpthread -ldl  -lrt -luuid -lssl -lexpat
clean:
	$(RM) ${OBJECT} MoneyCoolMonitor
 
