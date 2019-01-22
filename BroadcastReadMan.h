#ifndef _BROADCAST_READ_MAN_HH_
#define _BROADCAST_READ_MAN_HH_

#include "jw_thread.h"
#include "BroadcastRead.h"
#include "HttpRequest.h"
#include "cJSON.h"
#include "jw_log4z.h"

#define OPSPORT 9812
#define BROADCASTPORT 9811

extern std::string		m_strLocalUid;

class CBroadcastRead;
class CBroadcastReadMan
{
public:
	CBroadcastReadMan();
	~CBroadcastReadMan();

	bool Start();

#ifdef WIN32
	static DWORD WINAPI ThreadProcOps(LPVOID lpParameter);
	static DWORD WINAPI ThreadProcRecorder(LPVOID lpParameter);
#else
	static void* ThreadProcOps(void* lpParameter);
	static void* ThreadProcRecorder(void* lpParameter);
#endif
	void ThreadHandleOps();
	void ThreadHandleRecorder();
private:
	CThread			m_thread_ops;
	CThread			m_thread_recorder;
	CBroadcastRead	*	m_pCBroadcastReadOps;
	CBroadcastRead	*	m_pCBroadcastReadRecorder;
};
#endif