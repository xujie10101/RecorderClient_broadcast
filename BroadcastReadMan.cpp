#include "BroadcastReadMan.h"


CBroadcastReadMan::CBroadcastReadMan()
{
	m_pCBroadcastReadOps = NULL;
	m_pCBroadcastReadRecorder = NULL;
}

CBroadcastReadMan::~CBroadcastReadMan()
{
	if (m_pCBroadcastReadOps)
	{
		delete	m_pCBroadcastReadOps;
		m_pCBroadcastReadOps = NULL;
	}
	if (m_pCBroadcastReadRecorder)
	{
		delete	m_pCBroadcastReadRecorder;
		m_pCBroadcastReadRecorder = NULL;
	}
}

bool CBroadcastReadMan::Start()
{
//	m_thread_ops.Start(ThreadProcOps, this);
	m_thread_recorder.Start(ThreadProcRecorder, this);	
	return true;
}

#ifdef WIN32
DWORD WINAPI CBroadcastReadMan::ThreadProcOps(LPVOID lpParameter)
{
#ifndef USE_LIBUV
	CBroadcastReadMan* pThis = reinterpret_cast<CBroadcastReadMan*>(lpParameter);
	if (pThis)
	{
		pThis->ThreadHandleOps();
	}
#endif

	return 0;
}

DWORD WINAPI CBroadcastReadMan::ThreadProcRecorder(LPVOID lpParameter)
{
#ifndef USE_LIBUV
	CBroadcastReadMan* pThis = reinterpret_cast<CBroadcastReadMan*>(lpParameter);
	if (pThis)
	{
		pThis->ThreadHandleRecorder();
	}
#endif

	return 0;
}
#else
void* CBroadcastReadMan::ThreadProcOps(void* lpParameter)
{
#ifndef USE_LIBUV
	CBroadcastReadMan* pThis = reinterpret_cast<CBroadcastReadMan*>(lpParameter);
	if (pThis)
	{
		pThis->ThreadHandleOps();
	}
#endif

	return 0;
}

void* CBroadcastReadMan::ThreadProcRecorder(void* lpParameter)
{
#ifndef USE_LIBUV
	CBroadcastReadMan* pThis = reinterpret_cast<CBroadcastReadMan*>(lpParameter);
	if (pThis)
	{
		pThis->ThreadHandleRecorder();
	}
#endif

	return 0;
}
#endif

void CBroadcastReadMan::ThreadHandleOps()
{
#if 0
	m_pCBroadcastReadOps = new CBroadcastRead;
	m_pCBroadcastReadOps->Read(OPSPORT);
#endif
}

void CBroadcastReadMan::ThreadHandleRecorder()
{
	m_pCBroadcastReadRecorder = new CBroadcastRead;
	if (m_pCBroadcastReadRecorder)
	{
		if (m_pCBroadcastReadRecorder->Init())
		{
			m_pCBroadcastReadRecorder->Read(BROADCASTPORT);
		}
	}
}