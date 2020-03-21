#include "YMutexLock.h"
#include <stdio.h>

YMutexLock::YMutexLock()
{
	mutexCreate();

	m_bLock = false;
}

YMutexLock::~YMutexLock()
{
	mutexDestroy();
}

INT32 YMutexLock::lock()
{
	INT32 iRet = 0;

	iRet = mutexLock();
	if(iRet == OK)
	{
		m_bLock = true;
		return OK;
	}
	else
	{
		return iRet;
	}
}

INT32 YMutexLock::trylock()
{
	INT32 iRet = 0;

	iRet = mutexTrylock();
	if(iRet == OK)
	{
		m_bLock = true;
		return OK;
	}
	else
	{
		return iRet;
	}
}
INT32 YMutexLock::unlock()
{
	INT32 iRet = 0;

	iRet = mutexUnlock();
	if(iRet == OK)
	{
		m_bLock = true;
		return OK;
	}
	else
	{
		return iRet;
	}
}

INT32 YMutexLock::mutexCreate()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_mutex_init(&m_Mutex, NULL);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}

INT32 YMutexLock::mutexDestroy()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_mutex_destroy(&m_Mutex);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}

INT32 YMutexLock::mutexLock()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_mutex_lock(&m_Mutex);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}

INT32 YMutexLock::mutexTrylock()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_mutex_trylock(&m_Mutex);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}

INT32 YMutexLock::mutexUnlock()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_mutex_unlock(&m_Mutex);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}

YMutexSem::YMutexSem(INT32 max_value) : 
	m_SemMaxValue(max_value), m_SemValue(0)
{
	m_MutexLock = new YMutexLock();
}
YMutexSem::~YMutexSem()
{
	delete m_MutexLock;
	m_MutexLock = NULL;
}

YMutexSem::SEM_RESULT YMutexSem::semTake()
{
	SEM_RESULT tResult;
	
	if(OK == m_MutexLock->lock())
	{
		if(m_SemValue == 0)
		{
			tResult = SEM_EMPTY;
		}
		else
		{
			m_SemValue--;
			tResult = SEM_OK;
		}
		m_MutexLock->unlock();
	}
	else
	{
		tResult = SEM_ERROR;
	}
	return tResult;
}
YMutexSem::SEM_RESULT YMutexSem::semGive()
{
	SEM_RESULT tResult;
	
	if(OK == m_MutexLock->lock())
	{
		if(m_SemValue == m_SemMaxValue)
		{
			tResult = SEM_FULL;
		}
		else
		{
			m_SemValue++;
			tResult = SEM_OK;
		}
		m_MutexLock->unlock();
	}
	else
	{
		tResult = SEM_ERROR;
	}
	return tResult;
}

YMutexCond::YMutexCond()
{
	condCreate();
}
YMutexCond::~YMutexCond()
{
	condDestroy();
}

INT32 YMutexCond::wait(INT32 timewait)
{
	return waitCond(timewait);
}
INT32 YMutexCond::signal()
{
	return signalCond();
}

INT32 YMutexCond::waitCond(INT32 timewait)
{
	INT32 iRet;
#ifdef OS_LINUX_
	if(timewait < 0)
	{
		iRet = pthread_cond_wait(&m_Cond, getMutex());
		if(OK != iRet)
		{
			return iRet;
		}
	}
	else
	{
		struct timespec abstime;
		struct timeval now;

		gettimeofday(&now, NULL);
		UINT64 nsec = now.tv_usec * 1000 + ((UINT64)timewait % 1000) * 1000000;
		abstime.tv_sec=now.tv_sec + nsec / 1000000000 + (UINT64)timewait / 1000;
		abstime.tv_nsec=nsec % 1000000000;
		iRet = pthread_cond_timedwait(&m_Cond, getMutex(), &abstime);
		if(ETIMEDOUT == iRet)
		{
			return TIMEOUT;
		}
		else if(OK != iRet)
		{
			return iRet;
		}
	}
#endif
	return OK;
}
INT32 YMutexCond::signalCond()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_cond_signal(&m_Cond);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}
INT32 YMutexCond::condCreate()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_cond_init(&m_Cond, NULL);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}
INT32 YMutexCond::condDestroy()
{
	INT32 iRet = 0;
#ifdef OS_LINUX_
	iRet = pthread_cond_destroy(&m_Cond);
	if(0 == iRet)
	{
		return OK;
	}
	else
	{
		return iRet;
	}
#endif
}