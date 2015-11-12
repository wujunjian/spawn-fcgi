#include "connselecter.h"
#include "util.h"

ConnSelecter::ConnSelecter()
{
	pthread_rwlock_init(&lock_pool_read,NULL); 
	pthread_rwlock_init(&lock_pool_write,NULL); 
	vec_connpool_read.clear();
	vec_connpool_write.clear();
	read_vec_size=0;
	write_vec_size=0;
}

ConnSelecter::~ConnSelecter()
{
}

	
bool ConnSelecter::addConnPool(ConnPool* ptr_connpool,bool isread)
{
	if(!ptr_connpool)
		return false;
	if(isread){
		pthread_rwlock_wrlock(&lock_pool_read);
		vec_connpool_read.push_back(ptr_connpool);
		read_vec_size++;
		pthread_rwlock_unlock(&lock_pool_read);
	}else{
		pthread_rwlock_wrlock(&lock_pool_write);
		vec_connpool_write.push_back(ptr_connpool);
		write_vec_size++;
		pthread_rwlock_unlock(&lock_pool_write);
	}
	return true;
}

ConnPool* ConnSelecter::selectConnPool(std::string hashvalue,bool isread)
{
	if(hashvalue.empty()){
		return NULL;
	}
	int32_t idx = 0;
	ConnPool* res = NULL;
	uint32_t hash_v = Util::getcrc(hashvalue.c_str(), hashvalue.size());
	if(isread){
	  idx = hash_v%read_vec_size;
		res = vec_connpool_read[idx];
	}else{
		idx = hash_v%write_vec_size;
		res = vec_connpool_write[idx];
	}
	return res;
}
