#include "strHex.h"
#include <stdio.h>
#include <errno.h>
#include "jw_log4z.h"

#ifndef WIN32
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#endif

std::string GenerateGuidStrHex()
{  
#ifdef WIN32
	char szGuidBuf[64] = {0};
	return std::string(szGuidBuf);  
#else
	return GetDiskUuidStrHex();
#endif
}

#ifdef WIN32
#else
std::string GetDiskUuidStrHex()
{
	std::string path1, path2 = "";
	size_t index1, index2;
	FILE *pf;
	char szbuffer[128] = {0};
	pf = popen("blkid /dev/sda*", "r");
	if(NULL != pf)
	{
		fread(szbuffer, sizeof(szbuffer), 1, pf);
	}
	pclose(pf);
	std::string strTemp = szbuffer;
	index1=strTemp.find("UUID=\"");
	if (index1==std::string::npos) 
		return "";
	path1 = strTemp.substr(index1 + 6);
	index2 = path1.find("\"");
	if (index2==std::string::npos) 
		return "";
	path2 = path1.substr(0, index2);
	return path2;	
}
#endif

void substring( char *s, char ch1, char ch2, char *substr )
{
    while( *s && *s++!=ch1 ) ;
    while( *s && *s!=ch2 ) *substr++=*s++ ;
    *substr='\0';
}

//按字符切割
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1 = 0, pos2 = 0;
	pos2 = s.find(c);
	if (pos2==std::string::npos) 
		return;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length())
		v.push_back(s.substr(pos1));
}

//获取UID
std::string PostGetUid()
{
	std::string strValue;
	std::string strTemp;
	cJSON *JsonObject = cJSON_CreateObject();
	if (!JsonObject)
	{
		LOGFMTE("error");
		return "";
	}
	cJSON_AddItemToObject(JsonObject,"cmd",		cJSON_CreateString("getUUID"));
	char *SendJson = cJSON_Print(JsonObject);

	char szUrl[256] = {0};
#if WIN32
	sprintf(szUrl, "%s/action/INSetInterface?userName=admin&password=admin&fingerId=123456", "172.16.1.139");
#else
	sprintf(szUrl, "%s/action/INSetInterface?userName=admin&password=admin&fingerId=123456", "127.0.0.1");
#endif
	strTemp = HttpClientPost(szUrl, SendJson);
	if (SendJson){free(SendJson);SendJson = NULL;}

	std::string strTemp1;
	std::string path1, path2 = "";
	size_t index1, index2;
	index1=strTemp.find("\"uuid\":");
	if (index1==std::string::npos) 
		return "";
	path1 = strTemp.substr(index1 + 8);
	index2 = path1.find("}");
	if (index2==std::string::npos) 
		return "";
	strValue = path1.substr(0, index2 - 1);
	return strValue;
}

//HTTP POST请求
std::string HttpClientPost(const char* strUrl, const char* strData)
{
	std::string strValue;
	HttpRequest* Http = new HttpRequest;
	if (Http)
	{
		char* str = (char*)malloc(MAX_BUFFER_SIZE);
		if (str)
		{
			memset(str, 0, MAX_BUFFER_SIZE);
			if(Http->HttpPost(strUrl, strData, str)) {
				//				LOGFMTD("%s", str);
				strValue = str;
			} else {
				LOGFMTE(" HttpPost请求失败!");
			}	
			free(str);str = NULL;
		}
		delete Http;Http = NULL;
	}
	return strValue;
}