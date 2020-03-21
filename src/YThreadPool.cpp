#include <YThreadPool.h>
#include <stdio.h>

YCustomerThread::YCustomerThread(YMultiQueue<T_YTHREAD_POOL_TASK> *vTaskQueue) :
	m_TaskQueue(vTaskQueue), m_TaskTime(0), 
	m_isTask(false)
{
	m_TaskLock = new YMutexLock();
}
YCustomerThread::~YCustomerThread()
{
	delete m_TaskLock;
	m_TaskLock = NULL;
}

VOID YCustomerThread::run()
{
	INT32 iRet = 0;
	printf("thread 0x%x run...\n", getId());
	while(!getExitFlag())
	{
		T_YTHREAD_POOL_TASK vTask;

		//等待2s超时
		iRet = m_TaskQueue->pop(vTask, 2000);
		if(iRet == OK)
		{
			m_TaskStartMTime = getMTime();
			printf("thread 0x%x get task\n", getId());
			m_isTask = true;
			vTask.pTaskHandel(vTask.pArg);
			m_TaskStopMTime = getMTime();
			printf("thread 0x%x task over\n", getId());
			
			m_isTask = false;
			m_TaskTime += (m_TaskStopMTime - m_TaskStartMTime);
		}
		else if(iRet != TIMEOUT)
		{
			printf("task queue pop error iRet:[%d]!\n", iRet);
		}
	}
	printf("0x%x Thread exit\n", getId());
}
UINT64 YCustomerThread::getTaskTIme()
{
	UINT64 take_time;
	
	if(m_isTask)
	{
		UINT64 now_time = getMTime();
		take_time =  m_TaskTime + now_time - m_TaskStartMTime;
	}
	else
	{
		take_time =  m_TaskTime;
	}
	
	return take_time;
}

YThreadPool::YThreadPool(UINT32 vInitThreadNum, UINT32 vMaxThreadNum) :
	m_uiMaxThreadNum(vMaxThreadNum)
{
	m_TaskQueue = new YMultiQueue<T_YTHREAD_POOL_TASK>();
	m_ThreadPoolLock = new YMutexLock();
	
	createCustomer(vInitThreadNum);
	start();
}
YThreadPool::~YThreadPool()
{
	m_ThreadPoolLock->lock();
	std::map<UINT32, T_YTHREAD_POOL_THREAD_INFO*>::iterator it;
	for(it = m_CustomerList.begin(); it != m_CustomerList.end(); it++)
	{
		T_YTHREAD_POOL_THREAD_INFO *ptInfo = it->second;
		ptInfo->tThread->exitThread();
		ptInfo->tThread->join();
		delete ptInfo->tThread;
	}
	m_ThreadPoolLock->unlock();

	exitThread();
	join();
	
	delete m_TaskQueue;
	m_TaskQueue = NULL;
}
VOID YThreadPool::run()
{
	UINT32 uiHighUseRateCnt = 0, uiLowUseRateCnt = 0;
	while(!getExitFlag())
	{
		DOUBLE dUseRateSum = 0.0, dAvrUseRate = 0.0, dLowestUseRate = 100.0;
		UINT32 uiUseRateLowestId;

		m_ThreadPoolLock->lock();
		std::map<UINT32, T_YTHREAD_POOL_THREAD_INFO*>::iterator it;
		printf("\t\t\t*******************************\n");
		for(it = m_CustomerList.begin(); it != m_CustomerList.end(); it++)
		{
			T_YTHREAD_POOL_THREAD_INFO *ptInfo = it->second;
			YCustomerThread *thread_tmp = ptInfo->tThread;
			UINT64 ulTaskTime = thread_tmp->getTaskTIme();

			ptInfo->dUseRate = (DOUBLE)(ulTaskTime - ptInfo->iLastTaskTime) / (DOUBLE)(5000) * (DOUBLE)100;
			ptInfo->iLastTaskTime = ulTaskTime;

			printf("\t\t\tt:[%d] task use rate:[%f\%]\n", thread_tmp->getId(), ptInfo->dUseRate);
			dUseRateSum += ptInfo->dUseRate;
			if(ptInfo->dUseRate <= dLowestUseRate)
			{
				uiUseRateLowestId = thread_tmp->getId();
				dLowestUseRate = ptInfo->dUseRate;
			}
		}
		printf("\t\t\t*****LOW:[%d]******HIGH:[%d]*****\n", uiLowUseRateCnt, uiHighUseRateCnt);
		m_ThreadPoolLock->unlock();

		dAvrUseRate = dUseRateSum / m_CustomerList.size();
		if(dAvrUseRate > USE_RATE_HIGH_THRESHOLD)
		{
			uiHighUseRateCnt++;
			uiLowUseRateCnt = 0;
			if(USE_RATE_ADJUST_MAX_CNT <= uiHighUseRateCnt)
			{
				uiHighUseRateCnt = 0;
				createCustomer(1);
			}
		}
		else if(dAvrUseRate < USE_RATE_LOW_THRESHOLD)
		{

			uiLowUseRateCnt++;
			uiHighUseRateCnt = 0;
			if(USE_RATE_ADJUST_MAX_CNT <= uiLowUseRateCnt)
			{
				uiLowUseRateCnt = 0;
				exitCustomer(uiUseRateLowestId);
			}
		}
		else
		{
			uiLowUseRateCnt = 0;
			uiHighUseRateCnt = 0;
		}
		sleep(5);
	}
	printf("thread pool exit!\n");
}

INT32 YThreadPool::appendTask(const T_YTHREAD_POOL_TASK &vTask)
{
	return m_TaskQueue->push(vTask);
}

VOID YThreadPool::createCustomer(UINT32 num)
{
	m_ThreadPoolLock->lock();
	for(INT32 i = 0; i < num && m_CustomerList.size() < m_uiMaxThreadNum; i++)
	{
		T_YTHREAD_POOL_THREAD_INFO *ptInfo = new T_YTHREAD_POOL_THREAD_INFO;
		ptInfo->tThread = new YCustomerThread(m_TaskQueue);
		ptInfo->iStartTime = getMTime();
		ptInfo->iLastTaskTime = 0;
		ptInfo->dUseRate = 0.0;

		m_CustomerList.insert(std::pair<UINT32, T_YTHREAD_POOL_THREAD_INFO*>(ptInfo->tThread->getId(), ptInfo));
		ptInfo->tThread->start();
	}
	m_ThreadPoolLock->unlock();
}
VOID YThreadPool::exitCustomer(UINT32 id)
{
	m_ThreadPoolLock->lock();
	if(m_CustomerList.size() > 1)
	{
		std::map<UINT32, T_YTHREAD_POOL_THREAD_INFO*>::iterator it = m_CustomerList.find(id);
		if(it != m_CustomerList.end())
		{
			T_YTHREAD_POOL_THREAD_INFO *ptInfo = it->second;
			ptInfo->tThread->exitThread();
			ptInfo->tThread->join();
			delete ptInfo->tThread;
			m_CustomerList.erase(it);
		}
	}
	m_ThreadPoolLock->unlock();
}