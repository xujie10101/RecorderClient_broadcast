#include <stdio.h>
#include "BroadcastRead.h"
#include "JSONParse.h"
#include "BroadcastSend.h"
#include "jw_log4z.h"
#include "strHex.h"
#include "HttpRequest.h"
#include "BroadcastReadMan.h"

#if WIN32
#define CONNECTION_IP  "172.16.1.139";
#else
#define CONNECTION_IP  "127.0.0.1";
#endif

CBroadcastRead::CBroadcastRead()
{
	m_strLocalIp = CONNECTION_IP;
	m_strRecorderIp = CONNECTION_IP;
}

CBroadcastRead::~CBroadcastRead()
{

}

bool CBroadcastRead::Init()
{
//	m_strLocalUid = GetLocalUid();
	LOGFMTI("m_strLocalUid : %s", m_strLocalUid.c_str());
	return true;
}

std::string CBroadcastRead::GetLocalUid()
{
	//获取本机UID
	std::string strGuid;
#ifdef	WIN32
	strGuid = "8049D24D-A85C-4BCA-A239-094ADF990004";
#else
	strGuid = GenerateGuidStrHex();
#endif
	return strGuid;
}

int CBroadcastRead::Read(unsigned short uPort)
{
	LOGFMTI("start ip:%s port:%u", m_strLocalIp.c_str(), uPort);
	SOCKET sock;
	int err ;
#if WIN32
	WORD wVersionRequested = MAKEWORD(2,0);
	WSADATA wasData;
	err = WSAStartup(wVersionRequested, &wasData);
	if (err != 0)
	{
		LOGFMTE("WSAStartup error! Error code: %d Error message: %s", errno, strerror(errno)); 
		return -1;
	}
#else
#endif
	sockaddr_in local_addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		LOGFMTE("socket() error! Error code: %d Error message: %s", errno, strerror(errno)); 
		return -1;
	}
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(uPort);
#if WIN32
	local_addr.sin_addr.s_addr = inet_addr(m_strLocalIp.c_str());
#else
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	err = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if (err < 0)
	{
		LOGFMTE("bind() error! Error code: %d Error message: %s", errno, strerror(errno)); 
		return -1;
	}
	bool loop = 1;
#if WIN32
	err = setsockopt(sock,IPPROTO_IP, IP_MULTICAST_LOOP, reinterpret_cast<char FAR *>(&loop), sizeof(loop));
#else
	err = err = setsockopt(sock,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));
#endif
	if (err<0)
	{
		LOGFMTE("setsockopt() error! Error code: %d，Error message: %s", errno, strerror(errno)); 
		return -1;
	}
	struct ip_mreq mreq;
#if WIN32
	mreq.imr_multiaddr.S_un.S_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = inet_addr(m_strLocalIp.c_str());
#else
	//入多播组
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); //多播地址
	mreq.imr_interface.s_addr = htonl(INADDR_ANY); //网络接口为默认
#endif

#if WIN32
	err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<char FAR *>(&mreq), sizeof(mreq));
#else
	err = setsockopt(sock,IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
#endif
	if (err< 0)
	{
		LOGFMTE("setsockopt2() error! Error code: %d Error message: %s", errno, strerror(errno)); 
		return -1;
	}
	socklen_t addr_len = 0;
	char buff[MCAST_SIZE] = {0};
	int nReadLen = 0;
	while (true)
	{
		addr_len = sizeof(local_addr);
		memset(buff, 0, MCAST_SIZE);
		nReadLen = recvfrom(sock, buff, MCAST_SIZE, 0, (struct sockaddr*)&local_addr, &addr_len);
		if (nReadLen == -1)
		{
			LOGFMTE("recvfrom() error! Error code: %d Error message: %s", errno, strerror(errno)); 
			return -1;
		}else{
			OnParse(buff, nReadLen);
		}
#if WIN32
		::Sleep(MCAST_READ_INTERVAL);
#else
		usleep(MCAST_READ_INTERVAL * 1000);
#endif
	}

#if WIN32
	err = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, reinterpret_cast<char FAR *>(&mreq), sizeof(mreq));
	closesocket(sock);
#else
	err = setsockopt(sock,IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));
	close(sock);
#endif
	sock = -1;
	return 1;
}

int CBroadcastRead::SendData(const char* pData, int nLen, unsigned short uPort)
{
	int nRet = -1;
	CBroadcastSend * pCBroadcastSend = new CBroadcastSend;
	if (pCBroadcastSend)
	{
		nRet= pCBroadcastSend->Send(pData, nLen, uPort);
		delete pCBroadcastSend;
		pCBroadcastSend = NULL;
	}
	return nRet;
}

int CBroadcastRead::OnParse(const char* pData, int nLen)
{
	if (!pData || nLen <= 0)
	{
		LOGFMTE("pData error");
		return -1;
	}

	LOGFMTI("pData:%s", pData);
	cJSON* AnalyJson = cJSON_Parse(pData);
	if(!AnalyJson)
	{
		LOGFMTE("AnalyJson error");
		return -1;
	}

	std::string strCmd;
	std::string strSourceUID;
	std::string strGet;
	std::string strDestinationUID;
	CJSONParse parse(pData);
	if(parse.GetValueStr(parse.GetRootItem(), "cmd", strCmd) == false)
		return -1;
	parse.GetValueStr(parse.GetRootItem(), "source", strSourceUID);
	parse.GetValueStr(parse.GetRootItem(), "get", strGet);
	parse.GetValueStr(parse.GetRootItem(), "destination", strDestinationUID);
	if (0 == strcmp(strCmd.c_str(), "NETCMD_UDP_BROADCAST_OPS"))
	{
		if (0 == strcmp(strGet.c_str(), "NETCMD_UDP_BROADCAST_OPS_DEVINFO"))
		{
			if ((0 == strcmp(strDestinationUID.c_str(), m_strLocalUid.c_str())) || strDestinationUID.empty())
			{
				std::string strData;
				CreateJson("NETCMD_UDP_BROADCAST_OPS_RET", strSourceUID, strGet, strData);
				SendData(strData.c_str(), strData.length(), MCAST_OPS_SEND_PORT);
			}
		}

	}else if (0 == strcmp(strCmd.c_str(), "NETCMD_UDP_BROADCAST_RECORDER"))
	{
		if (0 == strcmp(strGet.c_str(), "NETCMD_UDP_BROADCAST_RECORDER_DEVINFO"))
		{
			if ((0 == strcmp(strDestinationUID.c_str(), m_strLocalUid.c_str())) || strDestinationUID.empty())
			{
				std::string strData;
				CreateJson("NETCMD_UDP_BROADCAST_RECORDER_RET", strSourceUID, strGet, strData);
				SendData(strData.c_str(), strData.length(), MCAST_RECORDER_SEND_PORT);
			}
		}
	}else if (0 == strcmp(strCmd.c_str(), "NETCMD_UDP_BROADCAST_OPS_RET"))
	{
		LOGFMTI("%s", strCmd.c_str());
	}else if (0 == strcmp(strCmd.c_str(), "NETCMD_UDP_BROADCAST_RECORDER_RET"))
	{
		LOGFMTI("%s", strCmd.c_str());
	}
	return 0;
}

int CBroadcastRead::CreateJson(const char* szCmd, std::string strSourceUID, std::string strGetCmd, std::string &strOutData)
{
	//获取设备名称
	std::string strValue;
	std::string strDevName;
	GetDevName(strValue);
	cJSON* AnalyJson = cJSON_Parse(strValue.c_str());
	if(!AnalyJson)
	{
//		LOGFMTE("AnalyJson error");
	}else{
		CJSONParse parse(strValue.c_str());
		parse.GetValueStr(parse.GetRootItem(), "devName", strDevName);
	}

	std::map<std::string, std::string> mapDevInfo;
	CreateDevInfo(mapDevInfo);
	if (!mapDevInfo.size())
	{
		LOGFMTE("mapDevInfo.size():%d", mapDevInfo.size());
		return -1;
	}
	std::string serverIp = MapFind(mapDevInfo, "serverIp");
	std::string serverVersion = MapFind(mapDevInfo, "serverVersion");
	std::string serverModel = MapFind(mapDevInfo, "serverModel");
	std::string serverMac = MapFind(mapDevInfo, "serverMac");
	std::string serverMcuVersion = MapFind(mapDevInfo, "serverMcuVersion");
	std::string devMode = MapFind(mapDevInfo, "devMode");

	if (strDevName.empty())
	{
		strDevName = serverIp;
	}

	cJSON *JsonObject = cJSON_CreateObject();
	cJSON *JsonObject1 = cJSON_CreateObject();
	cJSON *JsonObject2 = cJSON_CreateObject();
	cJSON *JsonObject3 = cJSON_CreateObject();
	cJSON_AddItemToObject(JsonObject,"cmd",			cJSON_CreateString(szCmd));
	cJSON_AddItemToObject(JsonObject,"destination",		cJSON_CreateString(strSourceUID.c_str()));
	cJSON_AddItemToObject(JsonObject,"source",		cJSON_CreateString(m_strLocalUid.c_str()));
	cJSON_AddItemToObject(JsonObject,"get",		cJSON_CreateString(strGetCmd.c_str()));
	cJSON_AddItemToObject(JsonObject,"data",		JsonObject1);
//	cJSON_AddItemToObject(JsonObject,"data",		JsonObject3);

	cJSON_AddItemToObject(JsonObject1,"ip",			cJSON_CreateString(serverIp.c_str()));
	cJSON_AddItemToObject(JsonObject1,"name",			cJSON_CreateString(strDevName.c_str()));
	cJSON_AddItemToObject(JsonObject1,"mac",			cJSON_CreateString(serverMac.c_str()));
	cJSON_AddItemToObject(JsonObject1,"serverModel",			cJSON_CreateString(serverModel.c_str()));
	cJSON_AddItemToObject(JsonObject1,"devMode",			cJSON_CreateString(devMode.c_str()));
	cJSON_AddItemToObject(JsonObject1,"version",		JsonObject2);

	cJSON_AddItemToObject(JsonObject2,"serverVersion",			cJSON_CreateString(serverVersion.c_str()));
	cJSON_AddItemToObject(JsonObject2,"serverMcuVersion",			cJSON_CreateString(serverMcuVersion.c_str()));

	char *SendJson = cJSON_Print(JsonObject);
	strOutData = SendJson;
	os_free(JsonObject);
	os_free(SendJson);

	return 0;
}

void CBroadcastRead::CreateDevInfo(std::map<std::string, std::string>& m)
{
	if (m_strRecorderIp.empty())
	{
		LOGFMTE("m_strRecorderIp NULL!");
		return;
	}
	HttpRequest* Http = new HttpRequest;

	int nLen = 256;
	char* strValue = (char*)malloc(BUFSIZE);
	char* szUrl = (char*)malloc(nLen);
	memset(strValue, 0, BUFSIZE);
	memset(szUrl, 0, nLen);

	sprintf(szUrl, "http://%s/GetServerBaseInfo", m_strRecorderIp.c_str());
	if(Http->HttpGet(szUrl, strValue)) {
		CreateDevInfoMap(strValue, m);
	} else {
		LOGFMTE("HttpGet Request failed!");
	}	
	os_free(szUrl);
	os_free(strValue);
	os_delete(Http);
}

void CBroadcastRead::GetDevName(std::string &strDevName)
{
	if (m_strRecorderIp.empty())
	{
		LOGFMTE("m_strRecorderIp NULL!");
		return;
	}
	HttpRequest* Http = new HttpRequest;

	int nLen = 256;
	char* strValue = (char*)malloc(BUFSIZE);
	char* szUrl = (char*)malloc(nLen);
	memset(strValue, 0, BUFSIZE);
	memset(szUrl, 0, nLen);

	sprintf(szUrl, "http://%s/GetCustomInfo?name=baseInfo", m_strRecorderIp.c_str());
	if(Http->HttpGet(szUrl, strValue)) {
		std::string strOut;
		DevNameFactory(strValue, strOut);
		strDevName = strOut;
	} else {
		LOGFMTE("HttpGet Request failed!");
	}	
	os_free(szUrl);
	os_free(strValue);
	os_delete(Http);
}

void CBroadcastRead::DevNameFactory(const char* szIn, std::string &strOut)
{
	std::string strTemp;
	strTemp = szIn;
	size_t fi = 0;
	fi = strTemp.find("{");
	if (fi==std::string::npos) 
		return;
	std::string strTemp1 = strTemp.substr(fi);
	size_t rfi = 0;
	rfi = strTemp1.find_last_of("}");
	if (rfi==std::string::npos) 
		return;
	strOut = strTemp1.substr(0, rfi+1);
}

void CBroadcastRead:: CreateDevInfoMap(char * szStr, std::map<std::string, std::string>& m)
{
	std::string strTemp;
	strTemp = szStr;
	size_t fi = 0;
	fi = strTemp.find("[");
	if (fi==std::string::npos) 
		return;

	std::string strTemp1 = strTemp.substr(fi);
	size_t rfi = 0;
	rfi = strTemp1.find_last_of("]>");
	if (rfi==std::string::npos) 
		return;
	strTemp1 = strTemp1.substr(1, rfi-3);

	std::vector<std::string> v;
	SplitString(strTemp1, v,"],"); //可按多个字符来分隔;
	for(std::vector<std::string>::size_type i = 0; i != v.size(); ++i)
	{
		size_t pos = 0;
		pos = v[i].find("[");
		if (pos==std::string::npos) 
			continue;;

		std::string s1 = v[i].substr(0, pos);
		std::string s2 = v[i].substr(pos+1,v[i].length());
		m[s1] = s2;
	}
}

std::string CBroadcastRead:: MapFind(std::map<std::string, std::string>& mapDevInfo, const char * szStr)
{
	std::map<std::string, std::string>::iterator iter1;
	iter1 = mapDevInfo.find(std::string(szStr));
	if(iter1 != mapDevInfo.end())
	{
		return iter1->second;
	}
	return "";
}