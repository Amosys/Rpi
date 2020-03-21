#ifndef __YMUTEX_LOCK_H__
#define __YMUTEX_LOCK_H__
#include "YPublic.h"
#include "YObject.h"

#ifdef OS_LINUX_ 
#include <errno.h>  
#include <pthread.h>
#include <sys/time.h>
#endif

class YMutexLock : public YObject
{
public:
	YMutexLock();
	~YMutexLock();

	INT32 lock();
	INT32 trylock();
	INT32 unlock();

	inline bool isLocked(){
		return m_bLock;
	}
	
protected:
#ifdef OS_LINUX_
	inline pthread_mutex_t* getMutex(){
		return &m_Mutex;
	}
#endif

private:
	INT32 mutexCreate();
	INT32 mutexDestroy();
	INT32 mutexLock();
	INT32 mutexTrylock();
	INT32 mutexUnlock();

	bool m_bLock;
#ifdef OS_LINUX_
	pthread_mutex_t m_Mutex;
#endif
};

class YMutexSem : public YObject
{
public:
	YMutexSem(INT32 max_value = 100);
	~YMutexSem();
	
	typedef enum{SEM_OK, SEM_FULL, SEM_EMPTY, SEM_ERROR,}SEM_RESULT;
	
	SEM_RESULT semTake();
	SEM_RESULT semGive();
	
	VOID setMaxValue(UINT32 value);
private:
	YMutexLock *m_MutexLock;
	UINT32 		m_SemMaxValue;
	UINT32 		m_SemValue;
};

class YMutexCond : public YMutexLock
{
public:
	YMutexCond();
	~YMutexCond();

	INT32 wait(INT32 timewait = -1);
	INT32 signal();
private:
	INT32 waitCond(INT32 timewait = -1);
	INT32 signalCond();
	
	INT32 condCreate();
	INT32 condDestroy();
	
#ifdef OS_LINUX_
	pthread_cond_t m_Cond;
#endif
};

#endif //__YMUTEX_LOCK_H__