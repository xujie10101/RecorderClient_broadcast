// RecorderClient_broadcast.cpp : 定义控制台应用程序的入口点。
//

#if WIN32
#include "stdafx.h"
#endif
#include "BroadcastReadMan.h"
#include "jw_log4z.h"
#include "BroadcastRead.h"
#include "strHex.h"

std::string		m_strLocalUid;



int main(int argc, char* argv[])
{
	m_strLocalUid = "";
	while (m_strLocalUid.empty())
	{
		m_strLocalUid = PostGetUid();
		if (m_strLocalUid.empty())
		{
			printf("m_strLocalUid NULL !\n");
#ifdef WIN32
			Sleep(2000);
#else
			sleep(2);
#endif
		}	
	}

	printf("m_strLocalUid:%s\n", m_strLocalUid.c_str());
	ILog4zManager::getInstance()->start();
	CBroadcastReadMan * pCBroadcastReadMan = new CBroadcastReadMan;
	if (pCBroadcastReadMan)
	{
		pCBroadcastReadMan->Start();	//9811
	}else{
		printf("new CBroadcastReadMan ERROR\n");
	}

	while (1)
	{
#if WIN32
		::Sleep(20000);
#else
		usleep(20000 * 1000);
#endif
	}
	if (pCBroadcastReadMan)
	{
		delete pCBroadcastReadMan;
		pCBroadcastReadMan = NULL;
	}
	ILog4zManager::getInstance()->stop();
	return 0;
}