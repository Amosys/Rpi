#include "YThread.h"
#include <iostream>

YThread::YThread() :
	m_ThreadStatus(NEW), m_bExitFlag(false)
{
	m_YThreadId = getYThreadId();
}

YThread::~YThread()
{
	joinThread();
}

VOID YThread::start()
{
	if(m_ThreadStatus != RUNNING)
	{
		createThread();
		m_StartTime = getTime();
	}
}
VOID* YThread::join()
{
	if(m_ThreadStatus == RUNNING)
	{
		return joinThread();
	}
	else
	{
		return NULL;
	}
}

VOID YThread::sleepThread(UINT32 sec)
{
#ifdef OS_LINUX_
	sleep(sec);
#endif
}
VOID YThread::usleepThread(UINT32 usec)
{
#ifdef OS_LINUX_
	usleep(usec);
#endif
}
UINT32 YThread::getRunTime()
{
	if(m_ThreadStatus == RUNNING)
	{
		return getTime() - m_StartTime;
	}
	else
	{
		return 0;
	}
}

VOID YThread::_start()
{
	m_ThreadStatus = RUNNING;
	run();
	m_ThreadStatus = EXIT;
}
VOID YThread::createThread()
{
#ifdef OS_LINUX_
	pthread_create(&m_PTHread_t, NULL, thread_proxy_func, (void*)this);
#endif
}
VOID YThread::destoryThread()
{
#ifdef OS_LINUX_
	
#endif
}
VOID* YThread::joinThread()
{
	VOID *ret;
#ifdef OS_LINUX_
	pthread_join(m_PTHread_t, &ret);
#endif	
	return ret;
}

//获取执行方法的指针
VOID* YThread::thread_proxy_func(VOID *args)
{
    YThread *thread = static_cast<YThread*>(args);
    thread->_start();
    return NULL;
}
//线程初始化时获取当前线程的ID
UINT32 YThread::getYThreadId()
{
	static UINT32 s_uiCurrentId = 0x00;
	
	return ++s_uiCurrentId;
}

