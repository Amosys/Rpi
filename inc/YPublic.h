#ifndef __YPUBLIC_H__
#define __YPUBLIC_H__

#ifdef linux
	#define OS_LINUX_
#endif

//定义数据类型
#define VOID 	void

#define BOOL	bool

#define INT8 	signed char
#define UINT8 	unsigned char

#define INT16	signed short
#define UINT16	unsigned short

#define INT32	signed int 
#define UINT32	unsigned int

#define INT64	signed long
#define UINT64 	unsigned long

#define FLOAT 	float
#define DOUBLE	double

#define OK		0
#define FAIL	-1
#define TIMEOUT -2

#endif //__YPUBLIC_H__