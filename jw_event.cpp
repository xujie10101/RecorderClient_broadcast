#include "jw_atom.h"
#include "jw_event.h"


os_event::os_event()
{
#ifdef WIN32
	m_hEvent = CreateEvent(NULL, false, false, NULL);
#else
	m_waitCount = 0;
	sem_init(&m_hEvent, 1, 0);
#endif	
}

os_event::~os_event()
{
#ifdef WIN32
	SAFE_CLSHANDLE(m_hEvent)
#else
	sem_destroy(&m_hEvent);
#endif
}

bool os_event::WaitEvent(unsigned long lMillSecond)
{
	bool bHasSingle = true;
#ifdef WIN32
	int nObjIndex = WaitForSingleObject(m_hEvent, lMillSecond );
	if ( WAIT_TIMEOUT == nObjIndex ||(WAIT_ABANDONED_0 <= nObjIndex && nObjIndex <= WAIT_ABANDONED_0 + 1)){
		bHasSingle = false;
	}
#else
	AtomAdd(&m_waitCount, 1);
	if (INFINITE == lMillSecond){
		sem_wait(&m_hEvent);//等待任务
	}else{
		timespec timeout;
		int nSecond = lMillSecond / 1000;
		int nNSecond = (lMillSecond - nSecond * 1000) * 1000;
		
		timeout.tv_nsec=nNSecond;
		timeout.tv_sec=time(NULL) + nSecond;         
		if (0 != sem_timedwait(&m_hEvent, &timeout)){
			bHasSingle = false;
		}
	}
	/*
		windows行为，在没有wait时，notify n次 之后再有多个wait，只能通过一个
		linux行为，在没有wait时，notify n次 之后再有多个wait，会通过n个，第n+1个开始阻塞
		简单说就是windows上第2~n个notify的信号丢失了

		为了和windows行为一致，当等待线程为0时，将多余的信号丢弃
	 */
	if (1 == AtomDec(&m_waitCount, 1)){
		sem_init( &m_hEvent, 1, 0 );
	}
#endif
	return bHasSingle;
}

void os_event::PostEvent()
{
#ifdef WIN32
	SetEvent(m_hEvent );
#else
	sem_post(&m_hEvent);	
#endif
}
