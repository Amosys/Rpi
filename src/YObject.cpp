#include "YObject.h"

YObject::YObject()
{

}

YObject::~YObject()
{
	
}

time_t YObject::getTime()
{
	return time(NULL);
}

UINT64 YObject::getMTime()
{
	UINT64 mTime;
#ifdef OS_LINUX_ 
	struct timeval now;

	gettimeofday(&now, NULL);
	mTime = now.tv_usec / 1000 + ((UINT64)now.tv_sec * 1000);
#endif

	return mTime;
}

