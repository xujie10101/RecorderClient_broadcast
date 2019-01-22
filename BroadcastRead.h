#ifndef _BROADCAST_READ_HH_
#define _BROADCAST_READ_HH_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "Common.h"
#include <map>
#if WIN32
#include "stdafx.h"
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
#include<ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define SOCKET	unsigned int
#endif

#define MCAST_OPS_SEND_PORT 9812
#define MCAST_RECORDER_SEND_PORT 9811
//#define MCAST_PORT 9811
#define MCAST_ADDR "234.5.6.7"
#define LOCAL_ADDR "172.16.1.90"
#define MCAST_READ_INTERVAL 100
#define MCAST_SIZE 1024

#define MAX_BUFFER_SIZE   1024*5

class CBroadcastRead
{
public:
	CBroadcastRead();
	~CBroadcastRead();

	bool Init();
	int Read(unsigned short uPort);
	int OnParse(const char* pData, int nLen);
	int SendData(const char* pData, int nLen, unsigned short uPort);
	int CreateJson(const char* szCmd, std::string strSourceUID, std::string strGetCmd, std::string &strOutData);


	std::string GetLocalUid();
	void GetDevName(std::string &strDevName);
	void DevNameFactory(const char* szIn, std::string &strOut);
	void CreateDevInfo(std::map<std::string, std::string>& m);
	void CreateDevInfoMap(char * szStr, std::map<std::string, std::string>& m);
	std::string MapFind(std::map<std::string, std::string>& mapDevInfo, const char * szStr);
private:
	std::string		m_strLocalIp;
	std::string		m_strRecorderIp;
};
#endif