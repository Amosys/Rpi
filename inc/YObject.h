#ifndef __YOBJECT_H__
#define __YOBJECT_H__
#include "YPublic.h"
#include <time.h>

#ifdef OS_LINUX_ 
#include <sys/time.h>
#endif

class YObject
{
public:
	YObject();
	~YObject();

protected:
	time_t getTime();
	UINT64 getMTime();

};

#endif //__YOBJECT_H__