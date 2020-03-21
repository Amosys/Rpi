#ifndef __YMULTI_QUEUE_H__
#define __YMULTI_QUEUE_H__
#include <queue>
#include "YObject.h"
#include "YMutexLock.h"

template<typename T>
class YMultiQueue : public YObject
{
public:
	YMultiQueue();
	~YMultiQueue();

	INT32 push(const T &vTask);
	//é˜Ÿåˆ—å¼¹å‡ºï¼Œå‚æ•°2è¶…æ—¶æ—¶é—´ï¼Œ<0ä¸€ç›´ç­‰å¾…
	INT32 pop(T &vTask, INT32 vTimewait = -1);
private:
	YMutexCond *m_QueueCond;
	std::queue<T*> m_TaskQueue;
};

template<typename T>
YMultiQueue<T>::YMultiQueue()
{
	m_QueueCond = new YMutexCond();
}

template<typename T>
YMultiQueue<T>::~YMultiQueue()
{
	delete m_QueueCond;
	m_QueueCond = NULL;
}

template<typename T>
INT32 YMultiQueue<T>::push(const T &vTask)
{
	m_QueueCond->lock();
	
	T *tmp_task = new T;
	memcpy(tmp_task, &vTask, sizeof(T));
	m_TaskQueue.push(tmp_task);
	
	m_QueueCond->unlock();
	m_QueueCond->signal();
	return OK;
}
template<typename T>
INT32 YMultiQueue<T>::pop(T &vTask, INT32 vTimewait)
{
	INT32 iRet = 0;

	iRet = m_QueueCond->lock();
	if(OK != iRet)
	{
		return iRet;
	}

	if(m_TaskQueue.size() == 0)
	{
		iRet = m_QueueCond->wait(vTimewait);
		if(OK != iRet)
		{
			m_QueueCond->unlock();
			return iRet;
		}
		
		if(m_TaskQueue.empty())
		{
			m_QueueCond->unlock();
			return FAIL;
		}
	}
	
	T *tmp_task = m_TaskQueue.front();
	memcpy(&vTask, tmp_task, sizeof(T));
	
	//pop会自动释放内存
	m_TaskQueue.pop();
	//delete tmp_task;
	
	m_QueueCond->unlock();
	return OK;
}

#endif //__YMULTI_QUEUE_H__
