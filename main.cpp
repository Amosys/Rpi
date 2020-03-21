#include <iostream>
#include <stdio.h>
#include <YThreadPool.h>
#include <unistd.h>

void func(void *arg);

INT32 main()
{
	YThreadPool *pool = new YThreadPool;
	T_YTHREAD_POOL_TASK tTask;

	tTask.pTaskHandel = func;
	
	while(1)
	{
		char *cmd = new char();
		*cmd = getchar();
		getchar();
		if(*cmd == 'q')
		{
			delete pool;
			pool = NULL;
			break;
		}
		else
		{
			tTask.pArg = (void*)cmd;
			pool->appendTask(tTask);
		}
	}
}

void func(void *arg)
{
	char *ch = (char*)arg;
	//printf("%c\n", *ch);
	delete ch;
	sleep(1);
}
