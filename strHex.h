#ifndef STRHEX_H
#define STRHEX_H

#include <stdlib.h>
#include <string>
#include <string.h>
#include "vector"
#include "cJSON.h"
#include "HttpRequest.h"

#ifndef WIN32
#include <unistd.h>
#include <iconv.h>
#include <assert.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#endif

#define MAX_BUFFER_SIZE   1024*5

//取出两个字符之间的字符串
void substring( char *s, char ch1, char ch2, char *substr );
//按字符切割
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

//获取UID
//接口
std::string PostGetUid();
std::string HttpClientPost(const char* strUrl, const char* strData);


std::string GenerateGuidStrHex();
#ifdef WIN32
#else
std::string GetDiskUuidStrHex();
#endif

#endif // STRHEX_H
