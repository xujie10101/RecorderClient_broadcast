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
		sem_wait(&m_hEvent);//�ȴ�����
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
		windows��Ϊ����û��waitʱ��notify n�� ֮�����ж��wait��ֻ��ͨ��һ��
		linux��Ϊ����û��waitʱ��notify n�� ֮�����ж��wait����ͨ��n������n+1����ʼ����
		��˵����windows�ϵ�2~n��notify���źŶ�ʧ��

		Ϊ�˺�windows��Ϊһ�£����ȴ��߳�Ϊ0ʱ����������źŶ���
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
