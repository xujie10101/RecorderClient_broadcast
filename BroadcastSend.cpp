#include "BroadcastSend.h"
#include "jw_log4z.h"

CBroadcastSend::CBroadcastSend()
{

}

CBroadcastSend::~CBroadcastSend()
{

}

int CBroadcastSend::Send(const char *szBuffer, int nLen, unsigned short uPort)
{
	SOCKET sock;
	struct sockaddr_in mcast_addr;
#if WIN32
	WORD wVersionRequested = MAKEWORD(2,0);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		perror("WSAStartup");
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)

	{
		WSACleanup();
		return -1;
	}
#else
#endif
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
//		perror("socket");
		LOGFMTE("socket error! Error code: %d Error message: %s", errno, strerror(errno)); 
		return -1;
	}
	memset(&mcast_addr, 0, sizeof(mcast_addr));
	bool opt = 1;
#if WIN32
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char FAR *>(&opt), sizeof(opt));
#else
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
#endif
	mcast_addr.sin_family = AF_INET;
	mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);  //inet_npton
	mcast_addr.sin_port = htons(uPort);
	 //向局部多播地址发送多播内容
	int n = sendto(sock, szBuffer, nLen, 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
	if (n<0)
	{
//		perror("sendto");
		LOGFMTE("sendto error! Error code: %d Error message: %s", errno, strerror(errno)); 
		return -1;
	}
	else
	{
		LOGFMTI("sendto:port:%d nLen:%d %s", uPort, nLen, szBuffer);
	}
#if WIN32
	if (closesocket(sock)) //关闭套接字
#else
	if (-1 == close(sock)) //关闭套接字
#endif
	{
		perror("closesocket()");
		return -1;
	}
#if WIN32
	if (!WSACleanup())       //关闭Socket库
	{
		WSAGetLastError();
		return -1;
	}
#else
#endif
	return 1;
}