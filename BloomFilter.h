#ifndef __BLOOMFILTER_H_
#define __BLOOMFILTER_H_
#include <string.h>
#include <string>
#include <vector>
#include <inttypes.h>

class BloomFilter{
public:
    BloomFilter();
    BloomFilter(long ln);
    ~BloomFilter();
private:
    pthread_rwlock_t _rwlock;
    long n;
    char *bits;
    long size;
    std::vector<std::string> vec_items;
    bool isTarget;
    int64_t item_num;
private:
    unsigned long hash1(std::string str_s);
    unsigned long hash2(std::string str_s);
    unsigned long hash3(std::string str_s);
public:
    bool Filter_Add(std::string str_s,bool is_save=true);
    int Filter_Check(std::string str_s);
    std::string GetAllItems();
    long Init(long ln);   
    void Reset();
    int64_t GetItemNum();
};

#endif
