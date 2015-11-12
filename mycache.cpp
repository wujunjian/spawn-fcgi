#include "mycache.h"

void MyCache::cleanCache()
{
    pthread_rwlock_wrlock(&(lock_cache_map));
    m_cache_map.clear();
    pthread_rwlock_unlock(&(lock_cache_map));
}

void MyCache::setStrValue(std::string key,std::string str_value,int32_t time_out)
{
	CacheValue cvalue;
	cvalue.str_value=str_value;
	cvalue.time_set=time(NULL);
	cvalue.timeout=time_out;
	
	pthread_rwlock_wrlock(&(lock_cache_map));
	m_cache_map[key]=cvalue;
	pthread_rwlock_unlock(&(lock_cache_map));
	return;
}

bool MyCache::getStrValue(std::string key,std::string& vlaue)
{
	bool res = false;
	vlaue="";
	pthread_rwlock_rdlock(&(lock_cache_map));
	std::map< std::string, CacheValue >::iterator it = m_cache_map.find(key);
	if(it!=m_cache_map.end()){
		if((it->second.time_set+it->second.timeout)>time(NULL)){
			res=true;
			vlaue=it->second.str_value;
		}
	}
	pthread_rwlock_unlock(&(lock_cache_map));
	return res;
}

void MyCache::addProduct(ProductValue* p_product)
{
	if(!p_product)
		return;
	pthread_rwlock_wrlock(&(lock_vec_product));
	vec_product.push_back(p_product);
	pthread_rwlock_unlock(&(lock_vec_product));
}

void MyCache::cleanProduct()
{
	pthread_rwlock_wrlock(&(lock_vec_product));
	std::vector<ProductValue*>::iterator it = vec_product.begin();
	while(it!=vec_product.end()){
		ProductValue* tmp = (*it);
		delete tmp;
		it++;
	}
	vec_product.clear();
	pthread_rwlock_unlock(&(lock_vec_product));
}

std::vector<ProductValue*>* MyCache::getProductListPtr()
{
	return &vec_product;
}

void MyCache::lockVecProduct(bool is_read)
{
	if(is_read)
		pthread_rwlock_rdlock(&(lock_vec_product));
	else
		pthread_rwlock_wrlock(&(lock_vec_product));
}

void MyCache::unlockVecProduct()
{
	pthread_rwlock_unlock(&(lock_vec_product));
}
