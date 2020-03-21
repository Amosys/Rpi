#ifndef __YTHREAD_H__
#define __YTHREAD_H__
#include "YPublic.h"
#include "YObject.h"

#ifdef OS_LINUX_
#include <pthread.h>
#include <unistd.h>
#endif

class YThread : public YObject
{
public:
	YThread();
	~YThread();

    enum THREAD_STATUS{RUNNING,EXIT,NEW,};
	
	VOID start();
	VOID* join();

	VOID sleepThread(UINT32 sec);
	VOID usleepThread(UINT32 usec);
	
	UINT32 getRunTime();
	inline UINT32 getId(){
		return m_YThreadId;
	}
	THREAD_STATUS getThreadStatus(){
		return m_ThreadStatus;
	}

	//只是置标志位，不会强制退出线程，需要线程执行函数自行退出。
	inline VOID exitThread(){
		if(m_ThreadStatus == RUNNING){
			m_bExitFlag = true;
		}
	}
	inline BOOL getExitFlag(){
		return m_bExitFlag;
	}

	virtual VOID run()=0;
private:
	VOID createThread();
	VOID destoryThread();
	VOID* joinThread();

	VOID _start();
    //��ȡִ�з�����ָ��
    static VOID* thread_proxy_func(VOID *args);
	//��̬������ȡYThreadId��YThread��Ψһ��ݱ�ʶ��
	static UINT32 getYThreadId();	

	//线程退出标志位
	BOOL m_bExitFlag;
	//�̵߳�ID
	UINT32 m_YThreadId;
	//�߳̿�ʼ����ʱ��,��λs
	time_t m_StartTime;
    //�̵߳�״̬
    THREAD_STATUS m_ThreadStatus;

#ifdef OS_LINUX_
    pthread_t m_PTHread_t;
#endif
};

#endif //__YTHREAD_H__