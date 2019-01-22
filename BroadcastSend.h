#ifndef _BROADCAST_SEND_HH_
#define _BROADCAST_SEND_HH_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
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

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#define MCAST_PORT 9811
#define MCAST_ADDR "234.5.6.7"  // 多播地址
#define MCAST_DATA "BROADCAST TEST DATA WINDOWS\n"  // 多播内容
#define MCAST_SEND_INTERVAL 1000  //多播时间间隔
#define MCAST_SIZE 1024

class CBroadcastSend
{
public:
	CBroadcastSend();
	~CBroadcastSend();
	int Send(const char *szBuffer, int nLen, unsigned short uPort);
private:
};
#endif