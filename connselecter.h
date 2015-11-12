#ifndef __CONNSELECTER_H
#define __CONNSELECTER_H

#include <vector>
#include "connpool.h"

class ConnSelecter
{
public:
	ConnSelecter();
	~ConnSelecter();
	
private:
	pthread_rwlock_t lock_pool_read;
	pthread_rwlock_t lock_pool_write;
	std::vector<ConnPool*> vec_connpool_read;
	std::vector<ConnPool*> vec_connpool_write;
	size_t read_vec_size;
	size_t write_vec_size;
		
public:
	bool addConnPool(ConnPool* ptr_connpool,bool isread);
	ConnPool* selectConnPool(std::string hashvalue,bool isread);
};

#endif
