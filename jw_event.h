// Signal.h: interface for the Signal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef OS_EVENT_HH
#define OS_EVENT_HH
#include "os_define.h"
#define INFINITE 0xFFFFFFFF

class os_event  
{
public:
	os_event();
	virtual ~os_event();

	virtual bool WaitEvent(unsigned long nMillSecond = INFINITE);
	virtual void PostEvent();
	
private:
#ifdef WIN32
	void * m_hEvent;
#else
	int m_waitCount;
	sem_t m_hEvent;			
#endif
};

#endif // MDK_SINGLE_H
