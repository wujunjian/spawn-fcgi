#include "BloomFilter.h"

const unsigned char masks[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
BloomFilter::BloomFilter()
{
		 pthread_rwlock_init(&_rwlock,NULL);
		 __sync_lock_test_and_set(&item_num,0);
		 isTarget=false;
     n=16377*32;
     size=(n+7)>>3;    //相当于(n+7)/8，求出需要多少个char存储n个bit
     bits=new char[size];
     /*for (int i=0;i<size;i++)    //bits所有位置零
     {
         bits[i] &=0;
     }*/
     memset(bits,0,size);
     vec_items.clear();
}

BloomFilter::BloomFilter(long ln)
{
		 pthread_rwlock_init(&_rwlock,NULL);
		 __sync_lock_test_and_set(&item_num,0);
		 isTarget=false;
     n=3*ln;
     size=(n+7)>>3;    //相当于(n+7)/8，求出需要多少个char存储n个bit
     bits=new char[size];
     for (int i=0;i<size;i++)    //bits所有位置零
     {
         bits[i] &=0;
     }
     vec_items.clear();
}

BloomFilter::~BloomFilter()
{
    vec_items.clear();
    vec_items.reserve(0);
    if(bits){
        delete[] bits;   
        bits=NULL;
    }      
}

unsigned long BloomFilter::hash1(std::string str_s)
{
      const char* s=str_s.c_str();
      unsigned sLength=str_s.length();
      unsigned long hash = 0; 
      unsigned long x    = 0; 
      for(int i = 0; i < sLength; i++) 
      { 
         hash = (hash << 4) + s[i]; 
         if((x = hash & 0xF0000000L) != 0) 
         { 
            hash ^= (x >> 24); 
         } 
         hash &= ~x; 
      } 
      return hash; 

}

unsigned long BloomFilter::hash2(std::string str_s)
{
      const char* s=str_s.c_str();
      unsigned sLength=str_s.length();
      unsigned long hash = 5381; 
      for(int i = 0; i < sLength; i++) 
      { 
         hash = ((hash << 5) + hash) + s[i]; 
      } 
      return hash; 

}

unsigned long BloomFilter::hash3(std::string str_s)
{
      const char* s=str_s.c_str();
      unsigned sLength=str_s.length();
      unsigned long seed = 131; // 31 131 1313 13131 131313 etc.. 
      unsigned long hash = 0; 
      for(int i = 0; i < sLength; i++) 
      { 
         hash = (hash * seed) + s[i]; 
      } 
      return hash; 

}

bool BloomFilter::Filter_Add(std::string str_s,bool is_save)
{
     unsigned long h1=hash1(str_s) % (n);    //在哪一位置1
     unsigned long h2=hash2(str_s) % (n);
     unsigned long h3=hash3(str_s) % (n);
     unsigned long idx1=h1>>3;    //具体到char数组的哪个下标
     unsigned long idx2=h2>>3;
     unsigned long idx3=h3>>3;
     pthread_rwlock_wrlock(&_rwlock);
     bits[idx1] |= masks[h1%8];        //将相应位置1
     bits[idx2] |= masks[h2%8];
     bits[idx3] |= masks[h3%8];
     isTarget=true;
     pthread_rwlock_unlock(&_rwlock);
     __sync_fetch_and_add(&item_num,1);
     if(is_save)
        vec_items.push_back(str_s);
     return true;

}

int BloomFilter::Filter_Check(std::string str_s)
{
     if(!isTarget)
        return -1;//不进行此项定向
     unsigned long h1=hash1(str_s) % (n);
     unsigned long h2=hash2(str_s) % (n);
     unsigned long h3=hash3(str_s) % (n);
     unsigned long idx1=h1>>3;
     unsigned long idx2=h2>>3;
     unsigned long idx3=h3>>3;
 		 pthread_rwlock_rdlock(&_rwlock);
     //只有当所有位都为1时，返回true
     if((bits[idx1] & masks[h1%8]) && (bits[idx2] & masks[h2%8]) && (bits[idx3] & masks[h3%8])){
     	  pthread_rwlock_unlock(&_rwlock);
        return 1;//存在
     }else{
     	  pthread_rwlock_unlock(&_rwlock);
        return 0;//不存在
     }

}

std::string BloomFilter::GetAllItems()
{
    std::string str_res;
    std::vector<std::string>::iterator it=vec_items.begin();
    while(it!=vec_items.end())
    {
        str_res+=(*it)+"\n";
        it++;
    }
    return str_res;
}

long BloomFilter::Init(long ln)
{
		 __sync_lock_test_and_set(&item_num,0);
		 if(n==16377*ln)
		 {
		 		vec_items.clear();
		 		return n;
		 }
     n=16377*ln;
     size=(n+7)>>3;    //相当于(n+7)/8，求出需要多少个char存储n个bit
     if(bits){
        delete[] bits;   
        bits=NULL;
     } 
     bits=new char[size];
     for (int i=0;i<size;i++)    //bits所有位置零
     {
         bits[i] &=0;
     }
     vec_items.clear();
     return n;
}

void BloomFilter::Reset()
{
		__sync_lock_test_and_set(&item_num,0);
     size=(n+7)>>3;    //相当于(n+7)/8，求出需要多少个char存储n个bit
  	isTarget=false;
     /*for (int i=0;i<size;i++)    //bits所有位置零
     {
         bits[i] &=0;
     }*/
     memset(bits,0,size);
     vec_items.clear();
}

int64_t BloomFilter::GetItemNum()
{
		int64_t res = __sync_fetch_and_add(&item_num,0);
		return res;
}
