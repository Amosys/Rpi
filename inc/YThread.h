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

	//åªæ˜¯ç½®æ ‡å¿—ä½ï¼Œä¸ä¼šå¼ºåˆ¶é€€å‡ºçº¿ç¨‹ï¼Œéœ€è¦çº¿ç¨‹æ‰§è¡Œå‡½æ•°è‡ªè¡Œé€€å‡ºã€‚
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
    //»ñÈ¡Ö´ĞĞ·½·¨µÄÖ¸Õë
    static VOID* thread_proxy_func(VOID *args);
	//¾²Ì¬·½·¨»ñÈ¡YThreadId£¬YThreadµÄÎ¨Ò»Éí·İ±êÊ¶·û
	static UINT32 getYThreadId();	

	//çº¿ç¨‹é€€å‡ºæ ‡å¿—ä½
	BOOL m_bExitFlag;
	//Ïß³ÌµÄID
	UINT32 m_YThreadId;
	//Ïß³Ì¿ªÊ¼ÔËĞĞÊ±¼ä,µ¥Î»s
	time_t m_StartTime;
    //Ïß³ÌµÄ×´Ì¬
    THREAD_STATUS m_ThreadStatus;

#ifdef OS_LINUX_
    pthread_t m_PTHread_t;
#endif
};

#endif //__YTHREAD_H__