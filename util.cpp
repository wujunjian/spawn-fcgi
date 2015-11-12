#include "util.h"
#include <sstream>
#include "md5.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

static const char enkey[]=  "abcde0123fghij4567klmnopqrstuvwxyzABCDEFG89HIJKLMNOPQRSTUVWXYZ";
static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
char g_ach62Num[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
#define INT_62_LEN	6
#define ULL_62_LEN	9

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

Util::Util()
{
}

Util::~Util()
{
}

int32_t Util::split(std::string str,std::string pattern,std::vector<std::string> &vec_str)
{
     std::string::size_type pos;

     str+=pattern;//扩展字符串以方便操作
     int32_t size=str.size();
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             std::string s=str.substr(i,pos-i);
             vec_str.push_back(s);
             i=pos+pattern.size()-1;
         }
     }
     return vec_str.size();
}

void Util::ReplaceStr(std::string &source,std::string oldstr,std::string newstr)
{
    std::string::size_type pos;
	for(int i=0;i<source.size();++i)
	{
		pos=source.find(oldstr,i);
		if(pos!=std::string::npos)
		{
			source.replace(pos,oldstr.length(),newstr);
			i=pos+newstr.size()-1;
		}
		else
		{
			break;
		}
	}
}

int64_t Util::transStrQueryToI64(std::string strkey,std::string strQuery)
{
		if(strkey.size()<2)
			return 0;
		
		int64_t i64_res=0;	
		int64_t i64_tmp=0;
		const char* tmpkey=strkey.c_str();
		i64_tmp=(int64_t)tmpkey[0];
		i64_tmp=i64_tmp<<56;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmpkey[1];
		i64_tmp=i64_tmp<<48;
		i64_res|=i64_tmp;
		
		std::string str_md5key = MD5(strQuery).toString();
		std::string str_tag=str_md5key.substr(0,3);
		str_tag+=str_md5key.substr(str_md5key.size()-3,3);
		const char* tmptag=str_tag.c_str();
		i64_tmp=(int64_t)tmptag[0];
		i64_tmp=i64_tmp<<40;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmptag[1];
		i64_tmp=i64_tmp<<32;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmptag[2];
		i64_tmp=i64_tmp<<24;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmptag[3];
		i64_tmp=i64_tmp<<16;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmptag[4];
		i64_tmp=i64_tmp<<8;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmptag[5];
		i64_res|=i64_tmp;
		
		return i64_res;
}

int64_t Util::transIntQueryToI64(std::string strkey,int64_t i64Query)
{
    if(strkey.size()<2)
    	return 0;
    if(i64Query>281474976710655)
    	return 0;
    	
    int64_t i64_res=0;	
    int64_t i64_tmp=0;
    const char* tmpkey=strkey.c_str();
    i64_tmp=(int64_t)tmpkey[0];
    i64_tmp=i64_tmp<<56;
    i64_res|=i64_tmp;
    
    i64_tmp=(int64_t)tmpkey[1];
    i64_tmp=i64_tmp<<48;
    i64_res|=i64_tmp;
    
    /*char* tmptest=(char*)&i64Query;
    char* tmpres=(char*)&i64_res;
    tmpres[5]=tmptest[0];
    tmpres[4]=tmptest[1];
    tmpres[3]=tmptest[2];
    tmpres[2]=tmptest[3];
    tmpres[1]=tmptest[4];
    tmpres[0]=tmptest[5];*/
    i64_tmp=i64Query;
    i64_res|=i64_tmp;
	return i64_res;
}

int64_t Util::transUAPAQueryToI64(std::string strkey,int64_t i64Query)
{
	  if(strkey.size()<2)
			return 0;
			
		int64_t i64_res=0;	
		int64_t i64_tmp=0;
		const char* tmpkey=strkey.c_str();
		i64_tmp=(int64_t)tmpkey[0];
		i64_tmp=i64_tmp<<56;
		i64_res|=i64_tmp;
		
		i64_tmp=(int64_t)tmpkey[1];
		i64_tmp=i64_tmp<<48;
		i64_res|=i64_tmp;
		
		i64Query=i64Query>>16;
		i64_res|=i64Query;
		return i64_res;
	
}

int64_t Util::TypeOfTag(int64_t tag){
  return (tag & 0xFFFF000000000000) >> 48;
}

int64_t Util::LabelOfTag(int64_t tag){
  return (tag & 0x0000FFFFFFFFFFFF);
}

std::string Util::GenUniCode()
{
    int i=0;
    char res[64]={0};
    char *tmp=res;
    uuid_t uu;
    uuid_generate(uu);
    for(i=0;i<16;i++)
    {
        sprintf(tmp,"%02x",uu[i]);
        tmp+=2;
    }
    //uint32_t tmpres=getcrc(res,strlen(res));
    
    //std::stringstream ss;
		std::string output(res);
//		ss<<tmpres;
	//	ss>>output;
		return output;
}

uint32_t Util::getcrc(const char* buffer, size_t size)
{
	if(size==0)
		return 0;
		
	static const uint32_t crc32tab[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
    };
	//---------------------------------
	uint64_t x;
  	uint32_t crc= 4294967295U;

  	for (x= 0; x < size; x++)
        crc= (crc >> 8) ^ crc32tab[(crc ^ (uint64_t)buffer[x]) & 0xff];

  	return ~crc;
}

std::string Util::UnidEncrypt(int64_t i64_key)
{
    std::string res="";

    int64_t tmp_y=i64_key%62;
    int64_t tmp_k=i64_key/62;
    res.insert(0,1,enkey[tmp_y]);
    while(tmp_k)
    {
        tmp_y=tmp_k%62;
        tmp_k=tmp_k/62;
        res.insert(0,1,enkey[tmp_y]);
    }
    return res;
}

std::string Util::UnidDecrypt(std::string str_mk)
{
        int64_t res=0;
        int32_t iLen=str_mk.size();
        const char* tmp_ch=str_mk.c_str();
        for(int32_t i=0;i<iLen;i++)
        {
                int64_t tmp=0;
                tmp=(strchr(enkey,tmp_ch[i])-enkey);
                tmp=(int64_t)(pow(62,(iLen-i-1))*tmp);
                res+=tmp;
        }
        std::stringstream ss;
        std::string output;
        ss<<res;
        ss>>output;
        return output;
}

std::string Util::base64_encode(char const* from, unsigned int in_len) {
	unsigned char const * bytes_to_encode = (unsigned char const *)from; 
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string Util::base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

std::string Util::urlencode(const char *text)
{
	static const char NUM2HEX[] = "0123456789ABCDEF";

	size_t size = 0;
	for (const char *p = text; *p; p ++)
		if (*p >= 'A' && *p <= 'Z' || *p >= 'a' && *p <= 'z'
				|| *p >= '0' && *p <= '9' || *p == '-' || *p == '_')
			size ++;
		else
			size += 3;
//			size += 2;

	std::string result;
	result.reserve(size);

	for (const char *p = text; *p; p ++)
		if (*p >= 'A' && *p <= 'Z' || *p >= 'a' && *p <= 'z'
				|| *p >= '0' && *p <= '9' || *p == '-' || *p == '_')
			result.push_back(*p);
		else if (*p == ' ')
			result.push_back('+');
		else
		{
			result.push_back('%');
			result.push_back(NUM2HEX[uint8_t(*p) / 16]);
			result.push_back(NUM2HEX[uint8_t(*p) % 16]);
		}

	return result;
}

std::string Util::urldecode(const char *text)
{
	size_t size = 0;
	for (const char *p = text; *p; p ++, size ++)
		if (p[0] == '%'
				&& (p[1] >= '0' && p[1] <= '9'
						|| p[1] >= 'A' && p[1] <= 'Z' || p[1] >= 'a' && p[1] <= 'z')
				&& (p[2] >= '0' && p[2] <= '9'
						|| p[2] >= 'A' && p[2] <= 'Z' || p[2] >= 'a' && p[2] <= 'z'))
			p += 2;

	std::string result;
	result.reserve(size);

	for (const char *p = text; *p; p ++)
		if (*p == '%')
		{
			uint8_t c = 0;
			if      (p[1] >= '0' && p[1] <= '9') c += p[1] - '0';
			else if (p[1] >= 'A' && p[1] <= 'Z') c += p[1] - 'A' + 10;
			else if (p[1] >= 'a' && p[1] <= 'z') c += p[1] - 'a' + 10;
			else
			{
				result.push_back(*p);
				continue;
			}

			c *= 16;
			if      (p[2] >= '0' && p[2] <= '9') c += p[2] - '0';
			else if (p[2] >= 'A' && p[2] <= 'Z') c += p[2] - 'A' + 10;
			else if (p[2] >= 'a' && p[2] <= 'z') c += p[2] - 'a' + 10;
			else
			{
				result.push_back(*p);
				continue;
			}

			result.push_back((char)c);
			p += 2;
		}
		else if (*p == '+')
			result.push_back(' ');
		else
			result.push_back(*p);

	return result;
}



int fn_i_FormatIn62Scale( char* pszDestBuf , int iBufLen , unsigned long long ul64 )
{
	int ic , imod  ;
	char* pch ;

	if( iBufLen <= 1 )
		return 0 ;

	if( ul64 == 0 )
	{
		*pszDestBuf = '0' ;
		*( pszDestBuf + 1 ) = 0 ;
		
		return 2 ;
	}

	pch = pszDestBuf ;
	ic = 0 ;

	iBufLen-- ;

	for( ; ic < iBufLen && ul64 > 0 ; ic++ )
	{
		imod = ul64 % 62 ;
		*pch = g_ach62Num[ imod ] ;
		pch++ ;

		ul64 = ul64 / 62 ;
	}

	*pch = 0 ;

	return ic + 1 ;
}

const char* Util::genUsrID( char* achbuf, int iBufLen, char* chrIP)
{
	const char *pcszUsrID = NULL ;
	char *pch ;
	struct timezone tz =
	{
		0, 0
	};

	struct timeval tv;
	char *client_ip = NULL;
	unsigned long long ul64Time ;
	unsigned int uirand = rand() ;
	unsigned long ul_converted_ip = 0;
	int ic , ilen ;

	gettimeofday (&tv, &tz);

	ul64Time = tv.tv_sec ;
	ul64Time *= 1000000 ;
	ul64Time += tv.tv_usec ;

	
	
	ul_converted_ip = ( unsigned int )inet_network(chrIP);

	ilen = 0 ;
	ic = fn_i_FormatIn62Scale( achbuf , iBufLen , uirand ) ;

	if( ic < INT_62_LEN + 1 )
	{
		pch = achbuf + ilen + ic - 1 ;

		for( ; ic <= INT_62_LEN ; ic++ )
		{
			*pch = '0' ;
			pch++ ;
		}
	}

	ic = INT_62_LEN ;

	iBufLen -= INT_62_LEN ;
	ilen += ic ;

	ic = fn_i_FormatIn62Scale( achbuf + ilen , iBufLen , ul_converted_ip ) ;
	if( ic < INT_62_LEN + 1 )
	{
		pch = achbuf + ilen + ic - 1 ;

		for( ; ic <= INT_62_LEN ; ic++ )
		{
			*pch = '0' ;
			pch++ ;
		}
	}

	ic = INT_62_LEN ;

	iBufLen -= INT_62_LEN ;
	ilen += ic ;

	ic = fn_i_FormatIn62Scale( achbuf + ilen , iBufLen , ul64Time ) ;
	ilen += ic ;

	achbuf[ ilen ] = 0;

	return achbuf ;
}

double Util::GetUseTime(struct timeval time_begin,struct timeval time_end)
{
  int64_t res_ms=0;
  int64_t tmptime=1000000 * ( time_end.tv_sec - time_begin.tv_sec ) + (time_end.tv_usec - time_begin.tv_usec);
	double dl=(double)tmptime;
	//res_ms=((int64_t)(dl/1000)?(int64_t)(dl/1000):1);
  return (dl/1000);
}

std::string Util::getDomain(const std::string& str_url)
{
    if(str_url.size()<=7)
        return "";
    std::string str_res;
    std::string::size_type pos_b;
    std::string::size_type pos_e;    
    pos_b=str_url.find("http://");
    pos_e=str_url.find("/",7);
    if(pos_b!=std::string::npos)
    {
        pos_b+=7;
        if(pos_e!=std::string::npos)
        {
            str_res=str_url.substr(pos_b,pos_e-pos_b);
        }
        else
        {
            str_res=str_url.substr(pos_b,str_url.size()-pos_b);
        }
    }
    else
    {
        if(str_url.size()>1024)
        {
          str_res="err_url_too_long";
        }
    }
    return str_res;
}

void Util::GetWandH(std::string& strsize,int& width,int& height)
{
    int wpos=0;
    int xpos=0;
    int hpos=0;
    xpos=strsize.find("x",0);
    if(xpos==std::string::npos)
    {
        width=0;
        height=0;
    }
    else
    {
        std::string tmpw=strsize.substr(wpos,(xpos-wpos));
        std::string tmph=strsize.substr(xpos+1,(strsize.size()-xpos-1));
        width=atoi(tmpw.c_str());
        height=atoi(tmph.c_str());
    }
}

int32_t Util::GetNowHour()
{
    int32_t i_now_hour=0;

    time_t t_nowtime=time(NULL);
	struct tm *tblock;
	tblock = localtime(&t_nowtime);
	if(tblock){
		i_now_hour = tblock->tm_hour;
	}else{
		i_now_hour = 0;
	}
    return i_now_hour;
}

bool Util::GetIPandPort(std::string ip_port,std::string &ip,int &port)
{
    bool res=false;
    std::string::size_type pos=ip_port.find(":");
    if(pos!=std::string::npos)
    {
        ip=ip_port.substr(0,pos);
        port=atoi(ip_port.substr(pos+1,ip_port.size()-pos-1).c_str());
        res=true;
    }
    else
    {
        res=false;
    }
    return res;
}

double Util::GetDistance(double lon1,double lat1,double lon2,double lat2)
{
	double c = sin(lat1/57.2958)*sin(lat2/57.2958) + cos(lat1/57.2958)*cos(lat2/57.2958)*cos((lon1-lon2)/57.2958);
	return 6371.004*acos(c);
}
