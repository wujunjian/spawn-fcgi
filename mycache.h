#ifndef __MYCACHE_H
#define __MYCACHE_H

#include <map>
#include <vector>
#include <pthread.h>
#include <string>

typedef struct _cachevalue
{
	int32_t time_set;
	int32_t timeout;
	std::string str_value;
}
CacheValue;

typedef struct _productvalue
{
	int32_t id;									//商品id
	std::string descript;						//商品描述
	std::string title;								//商品标题
	double now_price;						//商品当前价格
	double old_price;						//商品原始价格
	std::string pic_url_color;				//商品彩色图片列表
	std::string pic_url_wb;					//商品黑白图片
	int32_t all_slice_num;				//商品被切割的碎片总数的平方根,比如一个商品的碎片总数为9,则该值为3,app需要把图片按3x3的方式切割
	//int32_t now_slice_num;				//用户已获得的商品碎片数量
}
ProductValue;

class MyCache
{
public:
	MyCache(){pthread_rwlock_init(&lock_cache_map,NULL);pthread_rwlock_init(&lock_vec_product,NULL); m_cache_map.clear();};
	~MyCache(){};
	
private:
	pthread_rwlock_t lock_cache_map;
	std::map< std::string, CacheValue > m_cache_map;
		
	pthread_rwlock_t lock_vec_product;
	std::vector<ProductValue*> vec_product;
public:
	//bool startCleanThread();
	void cleanCache();
	void setStrValue(std::string key,std::string str_value,int32_t time_out);
	bool getStrValue(std::string key,std::string& vlaue); 
		
	void addProduct(ProductValue* p_product);
	void cleanProduct();
	
	std::vector<ProductValue*>* getProductListPtr();
	void lockVecProduct(bool is_read);
	void unlockVecProduct();
	
};

#endif
