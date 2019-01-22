#ifndef _SOCKET_DATA_OB_READ_HH_
#define _SOCKET_DATA_OB_READ_HH_


class CSocketDataObserver
{
public:
	virtual ~CSocketDataObserver(){};
	virtual int OnSocketRecvData(const char* pBuffer, int len);
};
#endif