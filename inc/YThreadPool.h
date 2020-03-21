#ifndef __YTHREAD_POOL_H__
#define __YTHREAD_POOL_H__
#include <map>
#include <string.h>
#include "YPublic.h"
#include "YThread.h"
#include "YMutexLock.h"
#include "YMultiQueue.hpp"

#define YCUSTOMER_USERATE_TIME 10

#define USE_RATE_LOW_THRESHOLD	15.0
#define USE_RATE_HIGH_THRESHOLD	90.0
#define USE_RATE_ADJUST_MAX_CNT	3

typedef void (*FuncPtr)(void *);

class YCustomerThread;

//线程池任务结构体
typedef struct T_YTHREAD_POOL_TASK
{
	void *pArg;
	FuncPtr pTaskHandel;
	UINT8 uiPrority;
	UINT8 uiPad[3];
}T_YTHREAD_POOL_TASK;

typedef struct T_YTHREAD_POOL_THREAD_INFO
{
	YCustomerThread *tThread;
	UINT64 iStartTime;
	UINT64 iLastTaskTime;
	DOUBLE dUseRate;
}T_YTHREAD_POOL_THREAD_INFO;

//消费者线程，处理任务
class YCustomerThread : public YThread
{
public:
	YCustomerThread(YMultiQueue<T_YTHREAD_POOL_TASK> *m_TaskQueue);
	~YCustomerThread();
	virtual VOID run();
	
	UINT64 getTaskTIme();
private:
	BOOL m_isTask;
	
	YMutexLock *m_TaskLock;
	UINT64 m_TaskStartMTime;
	UINT64 m_TaskStopMTime;
	
	UINT64 m_TaskTime;
	YMultiQueue<T_YTHREAD_POOL_TASK> *m_TaskQueue;
};

//protected 继承，隐藏YThread方法
class YThreadPool : protected YThread
{
public:
	YThreadPool(UINT32 vInitThreadNum = 5, UINT32 vMaxThreadNum = 10);
	~YThreadPool();
	virtual VOID run();
	
	INT32 appendTask(const T_YTHREAD_POOL_TASK &vTask);
private:
	VOID createCustomer(UINT32 num = 1);
	VOID exitCustomer(UINT32 id);
	
	std::map<UINT32, T_YTHREAD_POOL_THREAD_INFO*> m_CustomerList;
	YMultiQueue<T_YTHREAD_POOL_TASK> *m_TaskQueue;
	YMutexLock *m_ThreadPoolLock;
	UINT32 m_uiMaxThreadNum;
};

#endif//__YTHREAD_POOL_H__