#ifndef __UTIL_H_
#define __UTIL_H_

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <uuid/uuid.h>
#include <math.h>
#include<iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#define CHARSEP     ","
class Util 
{
public:
	Util();
	~Util();
public:
  template<typename T>
  static std::string transToString(const T &input)
  {
	std::stringstream ss;
	std::string output;
	ss<<input;
	ss>>output;
	return output;
  }
  static int32_t split(std::string str,std::string pattern,std::vector<std::string> &vec_str);
  static void ReplaceStr(std::string &source,std::string oldstr,std::string newstr);
  static int64_t transStrQueryToI64(std::string strkey,std::string strQuery);
  static int64_t transIntQueryToI64(std::string strkey,int64_t i64Query);
  static int64_t transUAPAQueryToI64(std::string strkey,int64_t i64Query);

  static int64_t TypeOfTag(int64_t);
  static int64_t LabelOfTag(int64_t);
  
  static std::string GenUniCode();
  static uint32_t getcrc(const char* buffer, size_t size);
  static std::string UnidEncrypt(int64_t i64_key);
  static std::string UnidDecrypt(std::string str_mk);
  	
  static std::string base64_encode(char const* , unsigned int len);
	static std::string base64_decode(std::string const& s);
		
	static std::string urlencode(const char *text);
	static std::string urldecode(const char *text);
	static const char* genUsrID( char* achbuf, int iBufLen, char* chrIP);
	static double GetUseTime(struct timeval time_begin,struct timeval time_end);
	static std::string getDomain(const std::string& str_url);
	static void GetWandH(std::string& strsize,int& width,int& height);
	static int32_t GetNowHour();
	static bool GetIPandPort(std::string ip_port,std::string &ip,int &port);
	static double GetDistance(double lon1,double lat1,double lon2,double lat2);
  
};
#endif 
